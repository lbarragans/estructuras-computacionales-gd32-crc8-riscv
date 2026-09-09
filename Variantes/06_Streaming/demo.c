#include <stdint.h>
#include "crc8_stream.h"

int main(void)
{
    static const uint8_t a[] = {'1','2','3'};
    static const uint8_t b[] = {'4','5','6'};
    static const uint8_t c[] = {'7','8','9'};

    crc8_stream_t ctx;

    crc8_stream_init(&ctx);
    crc8_stream_update(&ctx, a, sizeof(a));
    crc8_stream_update(&ctx, b, sizeof(b));
    crc8_stream_update(&ctx, c, sizeof(c));

    volatile uint8_t crc = crc8_stream_final(&ctx);
    volatile uint32_t ok = (crc == 0xF4U);

    (void)ok;

    for (;;) {
    }
}
