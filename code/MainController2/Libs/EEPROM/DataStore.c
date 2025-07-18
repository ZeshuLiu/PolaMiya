#include "DataStore.h"
#include "eeprom.h"
#include "string.h"
#include "zui_usr.h"

static EepromData_t EpData;

void init_data_eeprom()
{

#ifdef INIT_EEPROM
    if ( EEPROM_DATA_VERSION  == 0x01){
        memset(&EpData, 0x00, sizeof(EpData));
        EpData.version = EEPROM_DATA_VERSION;
        AT24C08C_Write2KB(&EEPROM_I2C, EEPROM_DATA_ADDR,(uint8_t *) &EpData, sizeof(EpData));
    }
    if ( EEPROM_DATA_VERSION  == 0x02){
        // memset(&EpData, 0x00, sizeof(EpData));
        AT24C08C_Read2KB(&EEPROM_I2C, EEPROM_DATA_ADDR,(uint8_t *)  &EpData, sizeof(EpData));
        EpData.version = EEPROM_DATA_VERSION;
        EpData.motor_life = EpData.motor_life&0xffff;
        // AT24C08C_Read2KB(&EEPROM_I2C, EEPROM_DATA_ADDR,(uint8_t *)  &EpData, sizeof(EpData));
        AT24C08C_Write2KB(&EEPROM_I2C, EEPROM_DATA_ADDR,(uint8_t *) &EpData, sizeof(EpData));
    }
#endif

    AT24C08C_Read2KB(&EEPROM_I2C, EEPROM_DATA_ADDR,(uint8_t *)  &EpData, sizeof(EpData));

    data_set_mtl(0);
    data_set_stc(0);
}

void data_update_rbc()
{
    extern char RBC_vals[6];
    extern UI_Element RBCVal_elm_char16;
    uint32_t tmp;
    EpData.reboot_cnt += 1;
    AT24C08C_Write2KB(&EEPROM_I2C, EEPROM_DATA_ADDR+EEP_RBC_OFFSET,(uint8_t *) &EpData.reboot_cnt, EEP_RBC_LEN);
    tmp = EpData.reboot_cnt;

    RBC_vals[0] = (tmp)/10000 + '0';
    RBC_vals[1] = (tmp% 10000)/1000 + '0';
    RBC_vals[2] = (tmp% 1000)/100 + '0';
    RBC_vals[3] = (tmp% 100)/10 + '0';
    RBC_vals[4] = (tmp% 10)/1 + '0';
    RBCVal_elm_char16.dirty = 1;
}

uint32_t data_acq_rbc()
{
    return EpData.reboot_cnt;
}

uint32_t data_acq_stc()
{
    return EpData.shut_cnt;
}

void data_set_stc(uint32_t val)
{
    extern char STC_vals[6];
    extern UI_Element STCVal_elm_char16;
    EpData.shut_cnt += val;
    AT24C08C_Write2KB(&EEPROM_I2C, EEPROM_DATA_ADDR+EEP_STC_OFFSET,(uint8_t *) &EpData.shut_cnt, EEP_STC_LEN);
    STC_vals[0] = (EpData.shut_cnt)/10000 + '0';
    STC_vals[1] = (EpData.shut_cnt% 10000)/1000 + '0';
    STC_vals[2] = (EpData.shut_cnt% 1000)/100 + '0';
    STC_vals[3] = (EpData.shut_cnt% 100)/10 + '0';
    STC_vals[4] = (EpData.shut_cnt% 10)/1 + '0';
    STCVal_elm_char16.dirty = 1;
}

uint32_t data_acq_mtl()
{
    return EpData.motor_life;
}

void data_set_mtl(uint32_t val)
{
    uint32_t tmp;
    extern char MTL_vals[6];
    extern UI_Element MTLVal_elm_char16;
    EpData.motor_life += val;
    AT24C08C_Write2KB(&EEPROM_I2C, EEPROM_DATA_ADDR+EEP_MTL_OFFSET,(uint8_t *) &EpData.motor_life, EEP_MTL_LEN);

    tmp = EpData.motor_life/1000;
    MTL_vals[0] = (tmp)/10000 + '0';
    MTL_vals[1] = (tmp% 10000)/1000 + '0';
    MTL_vals[2] = (tmp% 1000)/100 + '0';
    MTL_vals[3] = (tmp% 100)/10 + '0';
    MTL_vals[4] = (tmp% 10)/1 + '0';
    MTLVal_elm_char16.dirty = 1;
}
