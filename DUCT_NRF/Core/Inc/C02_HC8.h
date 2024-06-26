/*
 * C02_HC8.h
 *
 *  Created on: Apr 29, 2024
 *      Author: sunil
 */

#ifndef INC_C02_HC8_H_
#define INC_C02_HC8_H_

#include "main.h"

enum {
	CO2_STATE_BYTE_0 = 0,
	CO2_STATE_BYTE_1 = 1,
	CO2_STATE_BYTE_2 = 2,
	CO2_STATE_BYTE_3 = 3,
	CO2_STATE_BYTE_4 = 4,
	CO2_STATE_BYTE_5 = 5,
	CO2_STATE_BYTE_6 = 6,
	CO2_STATE_BYTE_7 = 7,
	CO2_STATE_BYTE_8 = 8,
	CO2_STATE_BYTE_9 = 9,
	CO2_STATE_BYTE_10 = 10,
	CO2_STATE_BYTE_11 = 11,
	CO2_STATE_BYTE_12 = 12,
	CO2_STATE_BYTE_13 = 13,
	CO2_STATE_BYTE_14 = 14,
	CO2_STATE_BYTE_15 = 15
};

void PROCESS_C02(uint8_t c);

#endif /* INC_C02_HC8_H_ */
