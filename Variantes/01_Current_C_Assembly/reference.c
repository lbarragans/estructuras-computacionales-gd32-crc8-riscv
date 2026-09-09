#include <stdint.h>

#define CRC8_POLY 0x07U

uint8_t crc8_c_reference(const uint8_t *data, uint32_t length)
{
    uint8_t crc = 0U;

    while (length-- > 0U) {
        crc ^= *data++;

        for (uint32_t bit = 0U; bit < 8U; ++bit) {
            if ((crc & 0x80U) != 0U) {
                crc = (uint8_t)((crc << 1U) ^ CRC8_POLY);
            } else {
                crc = (uint8_t)(crc << 1U);
            }
        }
    }

    return crc;
}
