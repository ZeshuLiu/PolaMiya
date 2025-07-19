#include "eeprom.h"
#include "main.h"


HAL_StatusTypeDef AT24C08C_Write2KB(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t *data, uint16_t len) {
    HAL_StatusTypeDef ret;
    uint16_t offset = 0;

    while (offset < len) {
        uint16_t curr_addr = addr + offset;

        uint8_t base_addr = (curr_addr >= 0x400) ? 0x54 : 0x50;
        uint8_t dev_addr  = base_addr | ((curr_addr >> 8) & 0x07);
        uint8_t mem_byte  = curr_addr & 0xFF;

        uint8_t page_left = EEPROM_PAGE_SIZE - (curr_addr % EEPROM_PAGE_SIZE);
        uint8_t chunk_len = (len - offset > page_left) ? page_left : (len - offset);

        ret = HAL_I2C_Mem_Write(hi2c, dev_addr << 1, mem_byte, I2C_MEMADD_SIZE_8BIT,
                                &data[offset], chunk_len, 2000);
        if (ret != HAL_OK) return ret;

        // HAL_Delay(5);  // 写入完成时间（典型为 5ms）
        offset += chunk_len;
    }

    return HAL_OK;
}

HAL_StatusTypeDef AT24C08C_Read2KB(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t *data, uint16_t len) {
    HAL_StatusTypeDef ret;
    uint16_t offset = 0;

    while (offset < len) {
        uint16_t curr_addr = addr + offset;

        // 判断属于哪颗芯片
        uint8_t base_addr = (curr_addr >= 0x400) ? 0x54 : 0x50;
        uint8_t dev_addr  = base_addr | ((curr_addr >> 8) & 0x07);
        uint8_t mem_byte  = curr_addr & 0xFF;

        // 按 EEPROM 页对齐（最多读16字节连续页）
        uint8_t page_left = 16 - (curr_addr % 16);
        uint8_t chunk_len = (len - offset > page_left) ? page_left : (len - offset);

        ret = HAL_I2C_Mem_Read(hi2c, dev_addr << 1, mem_byte, I2C_MEMADD_SIZE_8BIT, &data[offset], chunk_len, 2000);
        if (ret != HAL_OK) return ret;

        offset += chunk_len;
    }

    return HAL_OK;
}

void Test_AT24C08C_WriteAndRead() {
    uint8_t write_buf[EEPROM_TEST_LEN] = "Across EEPROM 0x3FF->0x400 OK!";
    uint8_t read_buf[EEPROM_TEST_LEN] = {0};

    volatile uint8_t stat = 0;

    uint16_t test_addr = EEPROM_TEST_ADDR; // 故意跨越 0x3FF 和 0x400

    // 写入数据
    if (AT24C08C_Write2KB(&EEPROM_I2C, test_addr, write_buf, EEPROM_TEST_LEN) == HAL_OK) {
        // printf("写入成功！\r\n");
        stat = 1;
    } else {
        // printf("写入失败！\r\n");
        stat = 2;
        return;
    }

    HAL_Delay(10); // 等待 EEPROM 写入完成

    // 读取数据
    if (AT24C08C_Read2KB(&EEPROM_I2C, test_addr, read_buf, EEPROM_TEST_LEN) == HAL_OK) {
        // printf("读取成功: %s\r\n", read_buf);
        stat = 3;
    } else {
        // printf("读取失败！\r\n");
        stat = 4;
    }
}