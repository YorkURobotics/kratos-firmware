/*
 * can_handler.c
 *
 *  Created on: Mar 6, 2026
 *      Author: Tirth Patel
 */

#include <stdint.h>

#include "can_handler.h"

HAL_StatusTypeDef CAN_CONFIG(CAN_HandleTypeDef *hcan) {
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;

	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO1;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;

	return HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
}

HAL_StatusTypeDef CAN_TRANSMIT(CAN_HandleTypeDef *hcan, uint32_t id, uint8_t *data, uint8_t size) {
	CAN_TxHeaderTypeDef txHeader;
	uint32_t mailbox;

	txHeader.ExtId = id;
	txHeader.IDE = CAN_ID_EXT;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.DLC = size;
	txHeader.TransmitGlobalTime = DISABLE;

	return HAL_CAN_AddTxMessage(hcan, &txHeader, data, &mailbox);
}

void CAN_Process_Incoming(uint32_t id, uint8_t *data, uint8_t len) {
	switch (id) {
		case CAN_DEFAULT_ID:
		// Handle specific logic for CAN_DEFAULT_ID here
			break;
//		case CAN_SERVO1:
//		// Handle specific logic for CAN_SERVO1 here
//			break;
		default:
			// Log unknown IDs for debugging
			break;
	}
}
