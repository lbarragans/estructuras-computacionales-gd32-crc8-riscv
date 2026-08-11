#ifndef CRC8_RISCV_H
#define CRC8_RISCV_H

#include <stdint.h>

/* CRC-8/ATM: poly=0x07, init=0x00, refin=false, refout=false, xorout=0x00. */
uint8_t riscv_crc8(const uint8_t *data, uint32_t length);

#endif /* CRC8_RISCV_H */
