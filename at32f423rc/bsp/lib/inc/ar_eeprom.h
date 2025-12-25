#ifndef AR_EEPROM_H
#define AR_EEPROM_H

#include "main.h"

#define EEPROM_DEVICE_ADDR 0xA0 // AT24C02 8位地址
#define EEPROM_PAGE_SIZE 8      // AT24C02页大小为8字节
#define AT24CXX 255             // AT24C02的最后一个地址来存储标志字

uint8_t eeprom_write_byte(uint16_t addr, uint8_t data);
uint8_t eeprom_write_bytes(uint8_t addr, uint8_t *pbuf, uint8_t size);
uint8_t eeprom_read_bytes(uint8_t addr, uint8_t *pbuf, uint8_t size);
uint8_t eeprom_check_id(void);

#endif /* AR_EEPROM_H */
