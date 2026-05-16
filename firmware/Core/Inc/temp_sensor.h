/*
 * temp_sensor.h
 *
 *  Created on: May 14, 2026
 *      Author: antonio
 */


#include <stdint.h>
#include "stm32f3xx_hal.h"


#ifndef INC_TEMP_SENSOR_H_
#define INC_TEMP_SENSOR_H_

// definitions




// structs

typedef struct {

	uint32_t ADC_Value;
	float temperature;

} TempStruct;

typedef struct {

	float resistance;
	float temperature;


} ResistorTable;

const ResistorTable TempResValues[] = {
		{333560.0f, -40.0f},
		{241070.0f, -35.0f},
		{176080.0f, -30.0f},
		{129930.0f, -25.0f},
		{96807.0f, -20.0f},
		{72809.0f, -15.0f},
		{55253.0f, -10.0f},
		{42292.0f, -5.0f},
		{32639.0f, 0.0f},
		{25391.0f, 5.0f},   //10
		{19902.0f, 10.0f},
		{15713.0f, 15.0f},
		{12493.0f, 20.0f},
		{10000.0f, 25.0f},
		{8055.9f, 30.0f},
		{6530.0f, 35.0f},
		{5324.6f, 40.0f},
		{4366.5f, 45.0f},
		{3600.5f, 50.0f},
		{2984.6f, 55.0f},
		{2486.6f, 60.0f}, //21
		{2081.8f, 65.0f},
		{1751.1f, 70.0f},
		{1479.6f, 75.0f},
		{1255.6f, 80.0f},
		{1070.0f, 85.0f},
		{915.55f, 90.0f},
		{786.43f, 95.0f},
		{678.07f, 100.0f},
		{586.75f, 105.0f},
		{509.52f, 110.0f},
		{443.94f, 115.0f} //32
};



// functions

HAL_StatusTypeDef getTempVoltage(ADC_HandleTypeDef *ADCpointer, TempStruct *valuesMeasured);

float ResistanceToTempConversion(uint32_t adc_to_convert);

float getTemperature(TempStruct *valuesMeasured);


#endif /* INC_TEMP_SENSOR_H_ */
