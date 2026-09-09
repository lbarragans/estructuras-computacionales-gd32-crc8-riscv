#include <stdint.h>

#define CRC8_POLY 0x07U

uint8_t crc8_bitwise(const uint8_t *data, uint32_t length)
{
    uint8_t crc = 0U;

    for (uint32_t i = 0U; i < length; ++i) {
        crc ^= data[i];

        for (uint32_t bit = 0U; bit < 8U; ++bit) {
            const uint8_t msb = (uint8_t)(crc & 0x80U);
            crc = (uint8_t)(crc << 1U);

            if (msb != 0U) {
                crc ^= CRC8_POLY;
            }
        }
    }

    return crc;
}
