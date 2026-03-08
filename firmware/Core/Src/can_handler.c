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

	sFilterConfig.FilterIdHigh = 0x0000; 	 // Since we do not want to filter, we set these to 0.
	sFilterConfig.FilterIdLow = 0x0000; 	 // Since we do not want to filter, we set these to 0.
	sFilterConfig.FilterMaskIdHigh = 0x0000; // Setting Mask to 0x0000 means "Don't Care" for every single bit.
	sFilterConfig.FilterMaskIdLow = 0x0000;  // This disables filtering and accepts every message on the bus.

	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO1;  // Route all accepted messages to Hardware FIFO 1
	sFilterConfig.FilterBank = 0;					    // Select filter bank 0 (STM32 usually has 14 or 28 banks)
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;   // Use Identifier Mask mode (instead of List mode which matches exact IDs)
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;  // Use a single 32 bit filter
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE; // Enable this filter bank

	return HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
}

HAL_StatusTypeDef CAN_TRANSMIT(CAN_HandleTypeDef *hcan, uint32_t id, uint8_t *data, uint8_t size) {
	CAN_TxHeaderTypeDef txHeader;
	uint32_t mailbox;

	txHeader.ExtId = id;					// Set the 29-bit Extended Identifier
	txHeader.IDE = CAN_ID_EXT;				// We are using Extended IDs rather than Standard 11 bit IDs
	txHeader.RTR = CAN_RTR_DATA;			// Set Remote Transmission Request to DATA rather than a REMOTE frame
	txHeader.DLC = size;					// Data Length Code: Tells the receiver how many bytes are in this packet (0-8)
	txHeader.TransmitGlobalTime = DISABLE;  // Disable time triggering features

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
