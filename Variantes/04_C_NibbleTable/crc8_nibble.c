#include <stdint.h>

static const uint8_t nibble_table[16] = {
    0x00U, 0x70U, 0xE0U, 0x90U, 0xC7U, 0xB7U, 0x27U, 0x57U, 0x89U, 0xF9U, 0x69U, 0x19U, 0x4EU, 0x3EU, 0xAEU, 0xDEU
};

static uint8_t crc8_update_nibble(uint8_t crc, uint8_t nibble)
{
    const uint8_t index = (uint8_t)(((crc >> 4U) ^ nibble) & 0x0FU);
    return (uint8_t)((crc << 4U) ^ nibble_table[index]);
}

uint8_t crc8_nibble(const uint8_t *data, uint32_t length)
{
    uint8_t crc = 0U;

    while (length-- > 0U) {
        const uint8_t value = *data++;
        crc = crc8_update_nibble(crc, (uint8_t)(value >> 4U));
        crc = crc8_update_nibble(crc, (uint8_t)(value & 0x0FU));
    }

    return crc;
}
