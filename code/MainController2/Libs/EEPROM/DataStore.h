#ifndef __DATA_STORE_H__
#define __DATA_STORE_H__
#include "main.h"

#define EEPROM_DATA_VERSION  0x02  // 当前结构体版本（你可以升级为 0x02、0x03）
// #define INIT_EEPROM
#define EEPROM_DATA_ADDR 0x20

#define EEP_VER_OFFSET  0
#define EEP_VER_LEN  1
#define EEP_RBC_OFFSET  1   // 2 3 4
#define EEP_RBC_LEN  4
#define EEP_STC_OFFSET  5   // 6 7 8
#define EEP_STC_LEN  4
#define EEP_MTL_OFFSET  9 // 10 11 12 13 14 15 16
#define EEP_MTL_LEN 4

typedef struct __attribute__((packed)) {
    uint8_t version;        // 数据结构版本
    uint32_t reboot_cnt;
    uint32_t shut_cnt;
    uint32_t motor_life;
} EepromData_t;

void init_data_eeprom();
void data_update_rbc();
uint32_t data_acq_rbc();
uint32_t data_acq_stc();
void data_set_stc(uint32_t val);
uint32_t data_acq_mtl();
void data_set_mtl(uint32_t val);


#endif