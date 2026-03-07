/*
 * can_handler.h
 *
 *  Created on: Mar 6, 2026
 *      Author: Tirth Patel
 */

#include <stdint.h>
#include "main.h"

#ifndef CAN_HANDLER_H
#define CAN_HANDLER_H

// -----------------------------------------------------------------------
// Configuration (Define all CAN_IDs here)
// -----------------------------------------------------------------------

#define CAN_DEFAULT_ID 0x0E110C00
//#define CAN_SERVO1

// -----------------------------------------------------------------------
// Function Prototypes
// -----------------------------------------------------------------------

HAL_StatusTypeDef CAN_CONFIG(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef CAN_TRANSMIT(CAN_HandleTypeDef * hcan, uint32_t id, uint8_t *data, uint8_t size);

void CAN_Process_Incoming(uint32_t id, uint8_t *data, uint8_t len);

#endif /* CAN_HANDLER_H */
