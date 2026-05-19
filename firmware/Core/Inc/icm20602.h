/*
 * icm20602.h
 *
 *  Created on: May 18, 2026
 *      Author: Tirth Patel
 *
 *  Driver for the Invensense ICM-20602 6-axis IMU (3 axis gyroscope + accelerometer) over I2C.
 *  Includes CAN transmission support for sending sensor data.
 */

#ifndef INC_ICM20602_H_
#define INC_ICM20602_H_

#include "stm32f3xx_hal.h"
#include "can_handler.h"

// -----------------------------------------------------------------------
// Register Map
// -----------------------------------------------------------------------

#define ICM_REG_SMPLRT_DIV 			0x19		// Sample rate divider
#define ICM_REG_CONFIG	   			0x1A		// DLPF and FSYNC configuration
#define ICM_REG_GYRO_CONFIG			0x1B		// Gyroscope FSR and self-test
#define ICM_REG_ACCEL_CONFIG		0x1C		// Accelerometer FSR and self-test
#define ICM_REG_ACCEL_CONFIG2		0x1D		// Accelerometer DLPF configuration
#define ICM_REG_INT_PIN_CFG			0x37		// INT pin and interrupt configuration
#define ICM_REG_INT_ENABLE			0x38		// Interrupt enable flags
#define ICM_REG_INT_STATUS			0x3A		// Interrupt status flags (read to clear)
#define ICM_REG_ACCEL_XOUT_H		0x3B		// Accelerometer X high byte (burst read start)
#define ICM_REG_TEMP_OUT_H 			0x41		// Temperature high byte
#define ICM_REG_GYRO_XOUT_H			0x43		// Gyroscope X high byte
#define ICM_REG_ACCEL_INTEL_CTRL	0x69		// Accelerometer intelligence / wake-on-motion control
#define ICM_REG_PWR_MGMT_1			0x6B		// Power management: reset, sleep, clock select
#define ICM_REG_PWR_MGMT_2 			0x6C		// Power management: per-axis enable/disable
#define ICM_REG_WHO_AM_I			0x75		// Device identity register (should return 0x12)

#define ICM_WHO_AM_I_VAL			0x12		// Expected WHO_AM_I response

#define ICM_I2C_ADDR_LOW			0x68		// I2C address determined by SA0 pin (tied to GND on Kratos)

// -----------------------------------------------------------------------
// CAN Transmission
// -----------------------------------------------------------------------

/*
 * Scale factors for packing float sensor values into int16_t for CAN.
 * Packed value = float * scale. Receiver divides by the same scale.
 *
 * ACCEL: 1000 LSB/g    → 0.001g resolution,  ±32.7g range
 * GYRO:  10 LSB/dps    → 0.1 dps resolution, ±3276 dps range
 * TEMP:  100 LSB/°C    → 0.01°C resolution,  ±327°C range
 */

#define IMU_ACCEL_SCALE 1000.0f
#define IMU_GYRO_SCALE	10.0f
#define IMU_TEMP_SCALE	100.0f

// -----------------------------------------------------------------------
// Enumerations
// -----------------------------------------------------------------------

typedef enum {
	ICM_GYRO_FSR_250 = 0x00,		// ±250  dps, 131.0 LSB/dps
	ICM_GYRO_FSR_500 = 0x08,		// ±500  dps,  65.5 LSB/dps
	ICM_GYRO_FSR_1000 = 0x10,		// ±1000 dps,  32.8 LSB/dps
	ICM_GYRO_FSR_2000 = 0x18		// ±2000 dps,  16.4 LSB/dps
} ICM_GyroFSR;						// Gyroscope full-scale range

typedef enum {
	ICM_ACCEL_FSR_2G = 0x00,		// ±2g,  16384 LSB/g
	ICM_ACCEL_FSR_4G = 0x08,		// ±4g,   8192 LSB/g
	ICM_ACCEL_FSR_8G = 0x10,		// ±8g,   4096 LSB/g
	ICM_ACCEL_FSR_16G = 0x18		// ±16g,  2048 LSB/g
} ICM_AccelFSR;						// Accelerometer full-scale range

typedef enum {
	ICM_DLPF_250HZ = 0,
	ICM_DLPF_176HZ = 1,
	ICM_DLPF_92HZ = 2,
	ICM_DLPF_41HZ = 3,
	ICM_DLPF_20HZ = 4,
	ICM_DLPF_10HZ = 5,
	ICM_DLPF_5HZ = 6
} ICM_DLPF;							// Gyroscope digital low-pass filter bandwidth

// -----------------------------------------------------------------------
// Structs
// -----------------------------------------------------------------------

typedef struct {
	I2C_HandleTypeDef *hi2c;				// HAL I2C handle
	uint8_t			   addr;				// I2C device address (ICM_I2C_ADDR_LOW)
	ICM_GyroFSR		   gyro_fsr;			// Gyroscope full-scale range
	ICM_AccelFSR	   accel_fsr;			// Accelerometer full-scale range
	ICM_DLPF		   dlpf;				// Digital low-pass filter bandwidth
	uint16_t		   sample_rate_hz;		// Output data rate in Hz (1–1000)
} ICM20602_Config;							// User supplied config passed to IMC20602_Init

typedef struct {
	float accel_x, accel_y, accel_z;		// Acceleration in g
	float gyro_x, gyro_y, gyro_z;			// Angular rate in dps
	float temp_c;							// Die temperature in C
} ICM20602_Data;

typedef struct {
	ICM20602_Config 	cfg;				// Copy of user config
	float				accel_scale;		// Precomputed: 1 / LSB_per_g
	float				gyro_scale;			// Precomputed: 1 / LSB_per_dps
} ICM20602_Handle;

// -----------------------------------------------------------------------
// Function Prototypes
// -----------------------------------------------------------------------

HAL_StatusTypeDef ICM20602_Init(ICM20602_Handle *dev, ICM20602_Config *cfg);	// Initialise device
HAL_StatusTypeDef ICM20602_ReadAll(ICM20602_Handle *dev, ICM20602_Data *out);	// Reads all 14 sensor bytes
HAL_StatusTypeDef ICM20602_ReadRaw(ICM20602_Handle *dev,						// 16 bit value without conversion
								   int16_t *ax, int16_t *ay, int16_t *az,
								   int16_t *gx, int16_t *gy, int16_t *gz,
								   int16_t *temp);

HAL_StatusTypeDef ICM20602_SendCAN(CAN_HandleTypeDef *hcan, const ICM20602_Data *data); // Transmit packed IMU CAN Data

#endif /* INC_ICM20602_H_ */
