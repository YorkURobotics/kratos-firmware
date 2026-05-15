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
		{333000.0f, -40.0f},
		{176000.0f, -30.0f},
		{130000.0f, -25.0f},
		{96800.0f, -20.0f},
		{72800.0f, -15.0f},
		{55000.0f, -10.0f},
		{42200.0f, -5.0f},
		{32600.0f, 0.0f},
		{25300.0f, 5.0f},   //9
		{19900.0f, 10.0f},
		{15700.0f, 15.0f},
		{12500.0f, 20.0f},
		{10000.0f, 25.0f},
		{8000.0f, 30.0f},
		{6530.0f, 35.0f},
		{5320.0f, 40.0f},
		{4360.0f, 45.0f},
		{3620.0f, 50.0f},
		{2980.0f, 55.0f},
		{2480.0f, 60.0f}, //20
		{2080.0f, 65.0f},
		{1750.0f, 70.0f},
		{1480.0f, 75.0f},
		{1250.0f, 80.0f},
		{1070.0f, 85.0f},
		{915.0f, 90.0f},
		{786.0f, 95.0f},
		{678.0f, 100.0f},
		{586.0f, 105.0f},
		{509.0f, 110.0f},
		{443.0f, 115.0f} //31
};



// functions

HAL_StatusTypeDef getTempVoltage(ADC_HandleTypeDef *ADCpointer, TempStruct *valuesMeasured);

float ResistanceToTempConversion(uint32_t adc_to_convert);

float getTemperature();


#endif /* INC_TEMP_SENSOR_H_ */
