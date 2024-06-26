/*
 * utility.h
 *
 *  Created on: Apr 29, 2024
 *      Author: sunil
 */

#ifndef INC_UTILITY_H_
#define INC_UTILITY_H_

#include "main.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "MY_NRF24.h"
#include "ee.h"
#include "nrf_helper.h"
#include "DHT11.h"


#define Debug_Port &huart1

#define MY_NODE_ID 1
#define My_Channel_ID 55
#define Server_Channel 52
#define PayLoad_Size_In_Byte 32
#define Reading_Writing_Pipe_Num 1

#define FLASH_ADDR_START_PG_29_USER_VAR 0X800E800
#define FLASH_ADDR_4_Self_Channel 0X800E804 //should be of 2 bytes
#define FLASH_ADDR_4_PIPE_ADDR_FOR_NRF 0X800E808 //it is of 8 byte
#define FLASH_ADDR_4_DEVICE_ID 0X800E818

void Manage_NRF_Data();
void Print_Debug_Data(char *Debug_Msg);
void Manage_Received_Data_From_UART(uint8_t UART_DATA[], size_t data_length);
uint64_t Get_Pipe_Address(uint8_t nrf_8_t);

#endif /* INC_UTILITY_H_ */
