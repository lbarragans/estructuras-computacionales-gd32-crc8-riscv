#ifndef CRC8_STREAM_H
#define CRC8_STREAM_H

#include <stdint.h>

typedef struct {
    uint8_t value;
} crc8_stream_t;

void crc8_stream_init(crc8_stream_t *ctx);
void crc8_stream_update(crc8_stream_t *ctx, const uint8_t *data, uint32_t length);
uint8_t crc8_stream_final(const crc8_stream_t *ctx);

#endif
