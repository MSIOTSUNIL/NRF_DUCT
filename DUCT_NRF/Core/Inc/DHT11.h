/*
 * DHT11.h
 *
 *  Created on: Apr 29, 2024
 *      Author: sunil
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "main.h"
#include <stdbool.h>

void microDelay(uint16_t delay);
uint8_t DHT11_Start(void);
uint8_t DHT11_Read(void);

bool DHT11_READ_TEMP_AND_HUM();

#endif /* INC_DHT11_H_ */
