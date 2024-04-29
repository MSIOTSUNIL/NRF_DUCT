#include "ee.h"
#include <string.h>

/*
 * @brief write data into the EEPROM
 * @args UINT16_T ADDRESS TO WRITE , UINT64_T DATA TO WRITE
 * */
bool ee_write_16(uint32_t Address_To_Write, uint16_t Data_To_Write) {
	HAL_FLASH_Unlock();
	HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,
			Address_To_Write, Data_To_Write);
	if (status != HAL_OK) {
		HAL_FLASH_Lock();
		return false;
	} else {
		HAL_FLASH_Lock();
		return true;
	}
	return false;
}
/*
 * Read Data into the EEPROM
 * @brief read uin16_t data type from the ee-prom
 * @args Address to read from the ee-prom
 * */
uint16_t ee_read_16(uint32_t Address_To_Read) {
	uint16_t RData;
	HAL_FLASH_Unlock();
	uint32_t *RDAddr = (uint32_t*) Address_To_Read;
	RData = *RDAddr;
	HAL_FLASH_Lock();
	return RData;
}
/*
 * @brief write data into the EEPROM
 * @args UINT32_T ADDRESS TO WRITE , UINT64_T DATA TO WRITE
 * */
bool ee_write_32(uint32_t Address_To_Write, uint32_t Data_To_Write) {
	HAL_FLASH_Unlock();
	HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
			Address_To_Write, Data_To_Write);
	if (status != HAL_OK) {
		HAL_FLASH_Lock();
		return false;
	} else {
		HAL_FLASH_Lock();
		return true;
	}
	return false;
}
/*
 * Read Data into the EEPROM
 * @brief read uin32_t data type from the ee-prom
 * @args Address to read from the ee-prom
 * */
uint32_t ee_read_32(uint32_t Address_To_Read) {
	uint32_t RData;
	HAL_FLASH_Unlock();
	uint32_t *RDAddr = (uint32_t*) Address_To_Read;
	RData = *RDAddr;
	HAL_FLASH_Lock();
	return RData;
}

/*
 * @brief write data into the EEPROM
 * @args UINT32_T ADDRESS TO WRITE , UINT64_T DATA TO WRITE
 * */
bool ee_write_64(uint32_t Address_To_Write, uint64_t Data_To_Write) {
	HAL_FLASH_Unlock();
	HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,
			Address_To_Write, Data_To_Write);
	if (status != HAL_OK) {
		HAL_FLASH_Lock();
		return false;
	} else {
		HAL_FLASH_Lock();
		return true;
	}
	return false;
}

/*
 * Read Data into the EEPROM
 * @brief read uin64_t data type from the ee-prom
 * @args Address to read from the ee-prom
 * */
uint64_t ee_read_64(uint32_t Address_To_Read) {
	uint64_t RData;
	HAL_FLASH_Unlock();
	uint64_t *RDAddr = (uint64_t*) Address_To_Read;
	RData = *RDAddr;
	HAL_FLASH_Lock();
	return RData;
}
/*
 * @brief Erase the page before writing data (if using repeated memory location) into the specific page
 * @args Starting adress of the page to eras and nmber of pages to erase
 */
bool ee_format_page(uint32_t Starting_Address, uint8_t Number_Of_Pages) {
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef flashErase;
	uint32_t page_error;
	flashErase.NbPages = Number_Of_Pages;
	flashErase.PageAddress = Starting_Address;
	flashErase.TypeErase = FLASH_TYPEERASE_PAGES;
	flashErase.Banks = FLASH_BANK_1; //Later if u will be using high density then consider using bank 2 also
	if (HAL_FLASHEx_Erase(&flashErase, &page_error) == HAL_OK) {
		HAL_FLASH_Lock();
		return true;
	} else {
		return false;
	}
	return false;
}

