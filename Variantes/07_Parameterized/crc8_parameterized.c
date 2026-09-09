#include <stdint.h>

typedef struct {
    uint8_t polynomial;
    uint8_t init;
    uint8_t xorout;
} crc8_params_t;

uint8_t crc8_compute_non_reflected(
    const uint8_t *data,
    uint32_t length,
    const crc8_params_t *params
)
{
    uint8_t crc = params->init;

    while (length-- > 0U) {
        crc ^= *data++;

        for (uint32_t bit = 0U; bit < 8U; ++bit) {
            if ((crc & 0x80U) != 0U) {
                crc = (uint8_t)((crc << 1U) ^ params->polynomial);
            } else {
                crc = (uint8_t)(crc << 1U);
            }
        }
    }

    return (uint8_t)(crc ^ params->xorout);
}
