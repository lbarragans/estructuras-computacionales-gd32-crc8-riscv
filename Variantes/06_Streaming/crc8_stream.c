#include "crc8_stream.h"

#define CRC8_POLY 0x07U

void crc8_stream_init(crc8_stream_t *ctx)
{
    ctx->value = 0U;
}

void crc8_stream_update(crc8_stream_t *ctx, const uint8_t *data, uint32_t length)
{
    while (length-- > 0U) {
        ctx->value ^= *data++;

        for (uint32_t bit = 0U; bit < 8U; ++bit) {
            if ((ctx->value & 0x80U) != 0U) {
                ctx->value = (uint8_t)((ctx->value << 1U) ^ CRC8_POLY);
            } else {
                ctx->value = (uint8_t)(ctx->value << 1U);
            }
        }
    }
}

uint8_t crc8_stream_final(const crc8_stream_t *ctx)
{
    return ctx->value;
}
