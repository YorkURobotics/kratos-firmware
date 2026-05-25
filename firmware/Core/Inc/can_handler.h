/*
 * can_handler.h
 *
 *  Created on: Mar 6, 2026
 *      Author: Tirth Patel
 */

#ifndef CAN_HANDLER_H
#define CAN_HANDLER_H

#include <stdint.h>
#include "main.h"

// -----------------------------------------------------------------------
// Configuration (Define all CAN_IDs here)
// -----------------------------------------------------------------------

#define CAN_DEFAULT_ID 0x0E110C00        // Kratos Default ID

// IMU CAN IDs
#define CAN_IMU_GYRO_ID     0x0E110000   // Gyro  x, y, z
#define CAN_IMU_ACCEL_ID    0x0E110400   // Accel x, y, z
#define CAN_IMU_TEMP_ID     0x0E110800   // Temperature

// -----------------------------------------------------------------------
// Function Prototypes
// -----------------------------------------------------------------------

HAL_StatusTypeDef CAN_CONFIG(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef CAN_TRANSMIT(CAN_HandleTypeDef * hcan, uint32_t id, uint8_t *data, uint8_t size);

void CAN_Process_Incoming(uint32_t id, uint8_t *data, uint8_t len);

#endif /* CAN_HANDLER_H */
