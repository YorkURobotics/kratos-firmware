/*
 * temp_sensor.c
 *
 *  Created on: May 14, 2026
 *      Author: antonio
 */

#include "temp_sensor.h"


#define Rfixed 15000.0f


HAL_StatusTypeDef getTempVoltage(ADC_HandleTypeDef *ADCStructpointer, TempStruct *valuesMeasured) {

	HAL_ADC_Start(ADCStructpointer);

	//check if there is a measurement
	if (HAL_ADC_PollForConversion(ADCStructpointer, 12) == HAL_OK) {

		valuesMeasured->ADC_Value = HAL_ADC_GetValue(ADCStructpointer);
		valuesMeasured->temperature = ResistanceToTempConversion(valuesMeasured->ADC_Value);
		return HAL_OK;
	}

	return HAL_ERROR;
}

	/*
	 * 12 bit ADC with 3.3 v so a value of 4095 is 3.3v
	 */
	float ResistanceToTempConversion(uint32_t adc_to_convert) {


		// resistance calculation of the temperature sensor using ADC value and voltage divider setup
		float thermistor = Rfixed * ((float)adc_to_convert / (4095.0f - (float)adc_to_convert));


		//guards
		if (thermistor > 333560.0f) {
			return -40.0f;
		}
		if (thermistor < 443.94f) {
					return 115.0f;
				}


		for (int i = 0; i < 32; i++) {

			if (thermistor <= TempResValues[i].resistance && thermistor >= TempResValues[i + 1].resistance) {

				//interpolate values in between
				float y1 = TempResValues[i].temperature;
				float x1 = TempResValues[i].resistance;
				float y2 = TempResValues[i + 1].temperature;
				float x2 = TempResValues[i + 1].resistance;

				float temp = y1 + ((thermistor - x1) * (y2-y1) / (x2 - x1));

				return temp;

			}
		}
		return 200.0f; // error
	}


	float getTemperature(TempStruct *valuesMeasured) {

		return valuesMeasured->temperature;
	}







