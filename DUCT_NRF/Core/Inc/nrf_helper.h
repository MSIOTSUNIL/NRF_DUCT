/*
 * nrf_helper.h
 *
 *  Created on: Aug 8, 2023
 *      Author: sunil
 *
 *
 *      USAGE: UART2 & SPI1
 */

#ifndef INC_NRF_HELPER_H_
#define INC_NRF_HELPER_H_

#include <string.h>
#include <stdio.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "MY_NRF24.h"

void nrf_init(void);
void Switch_to_Transmitt_mode();
void Switch_to_Receiver_mode(void);
bool Data_Available_For_Reading_NRF(void);
void TRANSMITT_UPDATE_OVER_NRF(char my_tx_data[]);
bool READ_NRF_AVAILABLE_DATA( void* buf, uint8_t len );

#endif /* INC_NRF_HELPER_H_ */
