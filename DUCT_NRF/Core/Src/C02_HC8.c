/*
 * C02_HC8.c
 *
 *  Created on: Apr 29, 2024
 *      Author: sunil
 */

#include "C02_HC8.h"
//C02 SENSOR_CURRENT_STATE
uint8_t current_c02_state = 0;
uint8_t C02_HIGH_BYTE = 0;
uint8_t C02_LOW_BYTE = 0;

uint32_t PPM_VALUE;

//PROCESS CO2 SENSOR DATA
void PROCESS_C02(uint8_t c) {
	switch (current_c02_state) {
	case CO2_STATE_BYTE_0:
		if (c == 0x42) {
			current_c02_state = CO2_STATE_BYTE_1;
		} else {
			current_c02_state = CO2_STATE_BYTE_0;
		}
		break;
	case CO2_STATE_BYTE_1:
		if (c == 0x4D) {
			current_c02_state = CO2_STATE_BYTE_2;
		} else {
			current_c02_state = CO2_STATE_BYTE_0;
		}
		break;
	case CO2_STATE_BYTE_2:
		current_c02_state = CO2_STATE_BYTE_3;
		break;
	case CO2_STATE_BYTE_3:
		current_c02_state = CO2_STATE_BYTE_4;
		break;
	case CO2_STATE_BYTE_4:
		current_c02_state = CO2_STATE_BYTE_5;
		break;
	case CO2_STATE_BYTE_5:
		current_c02_state = CO2_STATE_BYTE_6;
		break;
	case CO2_STATE_BYTE_6:
		current_c02_state = CO2_STATE_BYTE_7;
		C02_HIGH_BYTE = c;
		break;
	case CO2_STATE_BYTE_7:
		current_c02_state = CO2_STATE_BYTE_8;
		C02_LOW_BYTE = c;
		PPM_VALUE = (C02_HIGH_BYTE*256) + C02_LOW_BYTE;
		break;
	case CO2_STATE_BYTE_8:
		current_c02_state = CO2_STATE_BYTE_9;
		break;
	case CO2_STATE_BYTE_9:
		current_c02_state = CO2_STATE_BYTE_10;
		break;
	case CO2_STATE_BYTE_10:
		current_c02_state = CO2_STATE_BYTE_11;
		break;
	case CO2_STATE_BYTE_11:
		current_c02_state = CO2_STATE_BYTE_12;
		break;
	case CO2_STATE_BYTE_12:
		current_c02_state = CO2_STATE_BYTE_13;
		break;
	case CO2_STATE_BYTE_13:
		current_c02_state = CO2_STATE_BYTE_14;
		break;
	case CO2_STATE_BYTE_14:
		current_c02_state = CO2_STATE_BYTE_0;
		break;
	}
}
