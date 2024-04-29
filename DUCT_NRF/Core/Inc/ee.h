#ifndef __EEPROM_H
#define __EEPROM_H

/* This library is solely written for working with F103C8T6*/
#include <stdbool.h>
#include "main.h"

bool ee_write_16(uint32_t Address_To_Write, uint16_t Data_To_Write);
uint16_t ee_read_16(uint32_t Address_To_Read);
bool ee_write_32(uint32_t Address_To_Write, uint32_t Data_To_Write);
uint32_t ee_read_32(uint32_t Address_To_Read);
bool ee_write_64(uint32_t Address_To_Write, uint64_t Data_To_Write);
uint64_t ee_read_64(uint32_t Address_To_Read);
bool ee_format_page(uint32_t Starting_Address , uint8_t Number_Of_Pages);
#endif
