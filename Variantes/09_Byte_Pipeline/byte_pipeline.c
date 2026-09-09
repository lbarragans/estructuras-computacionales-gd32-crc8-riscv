#include <stdint.h>

#define CRC8_POLY 0x07U

typedef struct {
    uint8_t crc;
    uint32_t bytes_processed;
} crc_pipeline_t;

void crc_pipeline_init(crc_pipeline_t *pipeline)
{
    pipeline->crc = 0U;
    pipeline->bytes_processed = 0U;
}

void crc_pipeline_push_byte(crc_pipeline_t *pipeline, uint8_t byte)
{
    pipeline->crc ^= byte;

    for (uint32_t bit = 0U; bit < 8U; ++bit) {
        if ((pipeline->crc & 0x80U) != 0U) {
            pipeline->crc =
                (uint8_t)((pipeline->crc << 1U) ^ CRC8_POLY);
        } else {
            pipeline->crc = (uint8_t)(pipeline->crc << 1U);
        }
    }

    pipeline->bytes_processed++;
}
