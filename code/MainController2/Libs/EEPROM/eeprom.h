#ifndef __EEPROM_H__
#define __EEPROM_H__
#include "i2c.h"

#define EEPROM_PAGE_SIZE 16
#define AT24C08C_ADDR  0x50  // A2=0,A1=0,A0=0
#define EEPROM_I2C hi2c2

#define EEPROM_TEST_ADDR 0x03F0
#define EEPROM_TEST_LEN 0x1F

HAL_StatusTypeDef AT24C08C_Write2KB(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t *data, uint16_t len);
HAL_StatusTypeDef AT24C08C_Read2KB(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t *data, uint16_t len);
void Test_AT24C08C_WriteAndRead();
#endif