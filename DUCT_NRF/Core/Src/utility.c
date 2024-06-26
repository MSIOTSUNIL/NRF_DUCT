/*
 * utility.c
 *
 *  Created on: Apr 29, 2024
 *      Author: sunil
 */

#include "utility.h"

char my_node_id[2];
extern UART_HandleTypeDef huart1;
extern float tCelsius;
extern float RH;
extern uint32_t PPM_VALUE;

void Transmitt_reply_to_gateway(char *msg);

void Manage_NRF_Data() {
	uint8_t temp;
	char received_data_from_gateway[50] = "";
	NRF24_read(received_data_from_gateway, 5); //store received data

	//copy the 3rd byte, it is the command
	char rcvd_command = 0;
	rcvd_command = received_data_from_gateway[2];

	//commands
	char get_temperature = '0';
	char get_humidity = '1';
	char get_CO2 = '2';

	char return_data[10];
	//==================if received command is to GET TEMPERATURE ===========//
	if (rcvd_command == get_temperature) {  //Read Temp From Sensor
		DHT11_READ_TEMP_AND_HUM();
		// Convert float to string
		sprintf(return_data, "%f", tCelsius);
	} else if (rcvd_command == get_humidity) {  //Read Temp From Sensor
		DHT11_READ_TEMP_AND_HUM();
		// Convert float to string
		sprintf(return_data, "%f", RH);
	} else if (rcvd_command == get_CO2) {  //Read Temp From Sensor
		DHT11_READ_TEMP_AND_HUM();
		// Convert float to string
		sprintf(return_data, "%lu", PPM_VALUE);
	} else {
		Print_Debug_Data((char*) "INVALID COMMAND RECEIVED \n");
		return;
	}
	char return_string1[32];
	memcpy(return_string1, my_node_id, 2);
	return_string1[2] = rcvd_command;
	return_string1[3] = return_data[0];
	return_string1[4] = return_data[1];
	return_string1[5] = return_data[2];
	return_string1[6] = return_data[3];

//If it is getting PPM, data will be 4 digit
	if (rcvd_command == get_CO2) {
		temp = 7;
	} else {
		temp = 5;
	}

	for (uint8_t i = temp; i < 32; i++) {
		return_string1[i] = '\0';
	}

	HAL_UART_Transmit(&huart1, (uint8_t*) "\nReturning data ::> ",
			strlen("\nReturning data ::> "), 10);
	HAL_UART_Transmit(&huart1, (uint8_t*) return_string1, 7, 10);
	Switch_to_Transmitt_mode();
	Transmitt_reply_to_gateway(return_string1);
	Switch_to_Receiver_mode();
}
void Transmitt_reply_to_gateway(char *msg) {
	HAL_UART_Transmit(&huart1,
			(uint8_t*) "\nTransmitting reply to gateway .. waiting for ack",
			strlen("\nTransmitting reply to gateway .. waiting for ack"), 10);
	char received_data_from_srvr1[40];
	memcpy(received_data_from_srvr1, msg, 32);
	for (int i = 0; i <= 10; i++) {
		if (!NRF24_write(received_data_from_srvr1, 32)) {
			NRF24_write(received_data_from_srvr1, 32);
			HAL_UART_Transmit(&huart1, (uint8_t*) ".", strlen("."), 10);
			HAL_Delay(400);
		} else {
			HAL_UART_Transmit(&huart1,
					(uint8_t*) "\nReply Transmitted to gateway successfully",
					strlen("\nReply Transmitted to gateway successfully"), 10);
			break;
		}
	}
	HAL_UART_Transmit(&huart1,
			(uint8_t*) "\nTransmission Failed, No acknowledgement received",
			strlen("\nTransmission Failed, No acknowledgement received"), 10);

}

void Print_Debug_Data(char *Debug_Msg) {
	HAL_UART_Transmit(&huart1, (const uint8_t*) Debug_Msg, strlen(Debug_Msg),
			200);
}
void Manage_Received_Data_From_UART(uint8_t UART_DATA[], size_t data_length) {
// Check if the first element is '*'
	if (data_length > 0 && UART_DATA[0] == '*') {

//IF THE COMAMND IS TO ERASE FDLASH [It will erase the next three page, which contain the data ]
		if (strncmp((char*) &UART_DATA[1], "ERASE_NRF_DATA", 14) == 0) {
			//Erase FLASH_ADDR_4_No_Of_Polling_Node
			bool status = ee_format_page(FLASH_ADDR_START_PG_29_USER_VAR, 4);
			if (!status) {
				Print_Debug_Data(
						(char*) "[ERASE FLASH ERROR] UNABLE TO ERASE \n");
			} else {
				Print_Debug_Data(
						(char*) "[ERASE FLASH ] FLASH ERASED SUCCESFULLY \n");
			}
		}

//IF THE COMAMND IS TO STORE NRF CHANNEL
		if (strncmp((char*) &UART_DATA[1], "NRF_CH:", 7) == 0) {
			//if the condition meet, then the next two byte is the channel
			uint8_t nrfChannel[2];
			nrfChannel[0] = UART_DATA[8];
			nrfChannel[1] = UART_DATA[9];
			// Convert ASCII characters to numeric values
			uint8_t highValue = nrfChannel[0] - '0';
			uint8_t lowValue = nrfChannel[1] - '0';
			// Combine the digits into a single uint8_t
			uint8_t nrfChannel_8_t = (highValue * 10) + lowValue;
			if (!ee_write_16(FLASH_ADDR_4_Self_Channel, nrfChannel_8_t)) {
				Print_Debug_Data(
						(char*) "[ERROR WRITING FLASH] UNABLE TO WRITE CHANNEL Variable\n");
			} else {
				Print_Debug_Data(
						(char*) "[Writing Channel] Channel Written SUCCESFULLY \n");
			}
		}
//IF THE COMAMND IS TO STORE NRF PIPE ADDRESS
		if (strncmp((char*) &UART_DATA[1], "NRF_PIPE_ADDR:", 14) == 0) {
			// If the condition is met, copy the next two bytes to another array
			uint8_t PIPE_Addr[2];
			PIPE_Addr[0] = UART_DATA[15];
			PIPE_Addr[1] = UART_DATA[16];
			//process the command

			//STORE THE NODE ID, like 01 or 02, which will be required for REPLY
			uint16_t DEVICE_ADDRESS = (PIPE_Addr[1] << 8) | PIPE_Addr[0];
			if (!ee_write_16(FLASH_ADDR_4_DEVICE_ID, DEVICE_ADDRESS)) {
				Print_Debug_Data(
						(char*) "[ERROR WRITING FLASH] UNABLE TO WRITE DEVICE ID Variable\n");
			} else {
				Print_Debug_Data(
						(char*) "[Writing DEVICE ID] DEVICE ID Written SUCCESFULLY \n");
			}

			HAL_Delay(100);

			// Convert ASCII characters to numeric values
			uint8_t highValue = PIPE_Addr[0] - '0';
			uint8_t lowValue = PIPE_Addr[1] - '0';
			// Combine the digits into a single uint8_t
			uint8_t nrf_8_t = (highValue * 10) + lowValue;
			//copy the gateway code of finding address, and save the address
			uint64_t NRF_Pipe_Addr = Get_Pipe_Address(nrf_8_t);
			//WRITE IT EEPROM
			if (!ee_write_64(FLASH_ADDR_4_PIPE_ADDR_FOR_NRF, NRF_Pipe_Addr)) {
				Print_Debug_Data(
						(char*) "[UART CB] UNABLE TO WRITE NRF PIPE ADDRES\n");
			} else {
				Print_Debug_Data(
						(char*) "[UART CB] VALID ADDRESS FOUND AND IS SAVED INTO THE EMULATED EEPROM\n");
			}
		}
//IF THE COMMAND IS TO RESTART STM32
		if (strncmp((char*) &UART_DATA[1], "RESTART", 7) == 0) {
			NVIC_SystemReset();
		}
	}
}

uint64_t Get_Pipe_Address(uint8_t nrf_8_t) {
	switch (nrf_8_t) {
	case 1:
		return 0x11223344AA;
	case 2:
		return 0x11223344AB;
	case 3:
		return 0x11223344AC;
	case 4:
		return 0x11223344AD;
	case 5:
		return 0x11223344AE;
	case 6:
		return 0x11223344AF;
	case 7:
		return 0x11223344B0;
	case 8:
		return 0x11223344B1;
	case 9:
		return 0x11223344B2;
	case 10:
		return 0x11223344B3;
	case 11:
		return 0x11223344B4;
	case 12:
		return 0x11223344B5;
	case 13:
		return 0x11223344B6;
	case 14:
		return 0x11223344B7;
	case 15:
		return 0x11223344B8;
	case 16:
		return 0x11223344B9;
	case 17:
		return 0x11223344BA;
	case 18:
		return 0x11223344BB;
	case 19:
		return 0x11223344BC;
	case 20:
		return 0x11223344BD;
	case 21:
		return 0x11223344BE;
	case 22:
		return 0x11223344BF;
	case 23:
		return 0x11223344C0;
	case 24:
		return 0x11223344C1;
	case 25:
		return 0x11223344C2;
	case 26:
		return 0x11223344C3;
	case 27:
		return 0x11223344C4;
	case 28:
		return 0x11223344C5;
	case 29:
		return 0x11223344C6;
	case 30:
		return 0x11223344C7;
	default:
		return 0;
	}
	return 0;
}
