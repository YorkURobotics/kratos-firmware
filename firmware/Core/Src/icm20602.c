/*
 * icm20602.c
 *
 *  Created on: May 18, 2026
 *      Author: Tirth Patel
 */

#include "icm20602.h"
#include "string.h"

// -----------------------------------------------------------------------
// I2C Helpers (Private)
// -----------------------------------------------------------------------

// Write single byte to a device register
static HAL_StatusTypeDef _write(ICM20602_Handle *dev, uint8_t reg, uint8_t val) {
	uint8_t buf[2] = {reg, val};

	return HAL_I2C_Master_Transmit(dev->cfg.hi2c,
								   dev->cfg.addr << 1,
								   buf , 2, 10);
}

// Read one or more bytes starting at a register address
static HAL_StatusTypeDef _read(ICM20602_Handle *dev, uint8_t reg, uint8_t *buf, uint16_t len) {
	return HAL_I2C_Mem_Read(dev->cfg.hi2c,
							dev->cfg.addr << 1,
							reg, I2C_MEMADD_SIZE_8BIT,
							buf, len, 20);
}

// Read a single byte from a register and return its value
static uint8_t _read1(ICM20602_Handle *dev, uint8_t reg) {
	uint8_t val = 0;
	_read(dev, reg, &val, 1);
	return val;
}

// -----------------------------------------------------------------------
// Scale Factors (Private)
// -----------------------------------------------------------------------

// Returns the conversion factor from raw ADC counts to degrees per second.
// Values from Table 1 of the ICM-20602 datasheet.
static float _gyro_scale(ICM_GyroFSR fsr) {
	switch (fsr) {
		case ICM_GYRO_FSR_250:  return 1.0f / 131.0f;
		case ICM_GYRO_FSR_500:  return 1.0f / 65.5f;
		case ICM_GYRO_FSR_1000: return 1.0f / 32.8f;
		case ICM_GYRO_FSR_2000: return 1.0f / 16.4f;
		default:			    return 1.0f / 131.0f;
	}
}

// Returns the conversion factor from raw ADC counts to g.
// Values from Table 2 of the ICM-20602 datasheet.
static float _accel_scale(ICM_AccelFSR fsr) {
	switch (fsr) {
		case ICM_ACCEL_FSR_2G:  return 1.0f / 16384.0f;
		case ICM_ACCEL_FSR_4G:  return 1.0f / 8192.0f;
		case ICM_ACCEL_FSR_8G:  return 1.0f / 4096.0f;
		case ICM_ACCEL_FSR_16G: return 1.0f / 2048.0f;
		default:			    return 1.0f / 16384.0f;
	}
}

// -----------------------------------------------------------------------
// Initialization
// -----------------------------------------------------------------------

HAL_StatusTypeDef ICM20602_Init(ICM20602_Handle *dev, ICM20602_Config *cfg) {
	memcpy(&dev->cfg, cfg, sizeof(ICM20602_Config));

	_write(dev, ICM_REG_PWR_MGMT_1, 0x80);						// Full device reset
	HAL_Delay(100);

	_write(dev, ICM_REG_PWR_MGMT_1, 0x01);						// Wake from sleep, autpselect clock
	HAL_Delay(10);

	if (_read1(dev, ICM_REG_WHO_AM_I) != ICM_WHO_AM_I_VAL) {	// Confirm we are talking to correct device
		return HAL_ERROR;
	}

	_write(dev, ICM_REG_ACCEL_INTEL_CTRL, 0x02);				// Prevent output saturation (Section 9.35)

	// Gyro: FSR + DLPF
	_write(dev, ICM_REG_GYRO_CONFIG, (uint8_t)cfg->gyro_fsr);
	_write(dev, ICM_REG_CONFIG,		 (uint8_t)cfg->dlpf);

	// Accel: FSR + DLPF Bandwidth
	_write(dev, ICM_REG_ACCEL_CONFIG,  (uint8_t)cfg->accel_fsr);
	_write(dev, ICM_REG_ACCEL_CONFIG2, 0x03);

	// Sample Rate Divider: rate = 1000 / (1 + div)
	uint16_t rate = (cfg->sample_rate_hz > 0 && cfg->sample_rate_hz <= 1000)
				  ? cfg->sample_rate_hz : 200;
	_write(dev, ICM_REG_SMPLRT_DIV, (uint8_t)((1000U / rate) - 1U));

	// Data ready interrupt
	_write(dev, ICM_REG_INT_PIN_CFG, 0x10);  					// INT_RD_CLEAR
	_write(dev, ICM_REG_INT_ENABLE,  0x01);  					// DATA_RDY_INT_EN


	dev->gyro_scale = _gyro_scale(cfg->gyro_fsr);				// Cache scale factors so ReadAll doesn't
	dev->accel_scale = _accel_scale(cfg->accel_fsr);			// recompute them every call

	return HAL_OK;
}

// -----------------------------------------------------------------------
// Read
// -----------------------------------------------------------------------

// Burst read 14 bytes starting at ACCEL_XOUT_H (0x3B).
// Register order: ax_h ax_l  ay_h ay_l  az_h az_l
//                 temp_h temp_l
//                 gx_h gx_l  gy_h gy_l  gz_h gz_l
HAL_StatusTypeDef ICM20602_ReadRaw(ICM20602_Handle *dev,
								   int16_t *ax, int16_t *ay, int16_t *az,
								   int16_t *gx, int16_t *gy, int16_t *gz,
								   int16_t *temp) {

	uint8_t buf[14];

	HAL_StatusTypeDef s = _read(dev, ICM_REG_ACCEL_XOUT_H, buf, 14); // Burst Read
	if(s != HAL_OK) return s;

	*ax   = (int16_t)((buf[0]  << 8) | buf[1]);
	*ay   = (int16_t)((buf[2]  << 8) | buf[3]);
	*az   = (int16_t)((buf[4]  << 8) | buf[5]);
	*temp = (int16_t)((buf[6]  << 8) | buf[7]);
    *gx   = (int16_t)((buf[8]  << 8) | buf[9]);
	*gy   = (int16_t)((buf[10] << 8) | buf[11]);
	*gz   = (int16_t)((buf[12] << 8) | buf[13]);

	return HAL_OK;
}

// Read raw sensor values and convert to physical units.
// Accel: raw * accel_scale → g
// Gyro:  raw * gyro_scale  → dps
// Temp:  raw / 326.8 + 25  → C  (formula from Section 9.30 of the datasheet)
HAL_StatusTypeDef ICM20602_ReadAll(ICM20602_Handle *dev, ICM20602_Data *out) {
	int16_t ax, ay, az, gx, gy, gz, temp;

	HAL_StatusTypeDef s = ICM20602_ReadRaw(dev, &ax, &ay, &az, &gx, &gy, &gz, &temp);
	if(s != HAL_OK) return s;

	out->accel_x = (float)ax * dev->accel_scale;
	out->accel_y = (float)ay * dev->accel_scale;
	out->accel_z = (float)az * dev->accel_scale;

	out->gyro_x  = (float)gx * dev->gyro_scale;
	out->gyro_y  = (float)gy * dev->gyro_scale;
	out->gyro_z  = (float)gz * dev->gyro_scale;

	out->temp_c  = (float)temp / 326.8f + 25.0f;

	return HAL_OK;
}

// -----------------------------------------------------------------------
// CAN Transmission
// -----------------------------------------------------------------------

// Pack a float into two bytes (big-endian int16) at dst.
// Clamped to int16 range before casting to avoid undefined behaviour.
static void pack_int16(uint8_t *dst, float val, float scale) {
	float scaled = val * scale;

	if (scaled > 32767.0f) scaled = 32767.0f;
	if (scaled < -32768.0f) scaled = -32768.0f;

	int16_t i = (int16_t)scaled;
	dst[0] = (uint8_t)(i >> 8);						// High byte
	dst[1] = (uint8_t)(i & 0xFF);					// Low byte
}

// Transmit sensor data across three CAN frames.
// The Jetson receives these and divides by the matching scale factor to recover the original float value.
HAL_StatusTypeDef ICM20602_SendCAN(CAN_HandleTypeDef *hcan,
                                   const ICM20602_Data *data) {
    uint8_t buf[6];
    HAL_StatusTypeDef s;

    // Accel frame: 6 bytes — x, y, z each as big-endian int16
    pack_int16(&buf[0], data->accel_x, IMU_ACCEL_SCALE);
    pack_int16(&buf[2], data->accel_y, IMU_ACCEL_SCALE);
    pack_int16(&buf[4], data->accel_z, IMU_ACCEL_SCALE);
    s = CAN_TRANSMIT(hcan, CAN_IMU_ACCEL_ID, buf, 6);
    if (s != HAL_OK) return s;

    // Gyro frame: 6 bytes — x, y, z each as big-endian int16
    pack_int16(&buf[0], data->gyro_x, IMU_GYRO_SCALE);
    pack_int16(&buf[2], data->gyro_y, IMU_GYRO_SCALE);
    pack_int16(&buf[4], data->gyro_z, IMU_GYRO_SCALE);
    s = CAN_TRANSMIT(hcan, CAN_IMU_GYRO_ID, buf, 6);
    if (s != HAL_OK) return s;

    // Temp frame: 2 bytes - temperature as big-endian int16
    pack_int16(&buf[0], data->temp_c, IMU_TEMP_SCALE);
    return CAN_TRANSMIT(hcan, CAN_IMU_TEMP_ID, buf, 2);
}
