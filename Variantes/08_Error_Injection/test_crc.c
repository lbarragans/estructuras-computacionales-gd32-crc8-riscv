#include <stdint.h>

extern uint8_t riscv_crc8(const uint8_t *data, uint32_t length);

#define CRC8_POLY 0x07U

static uint8_t crc8_c(const uint8_t *data, uint32_t length)
{
    uint8_t crc = 0U;

    while (length-- > 0U) {
        crc ^= *data++;

        for (uint32_t bit = 0U; bit < 8U; ++bit) {
            crc = (crc & 0x80U)
                ? (uint8_t)((crc << 1U) ^ CRC8_POLY)
                : (uint8_t)(crc << 1U);
        }
    }

    return crc;
}

int main(void)
{
    uint8_t message[] = {'1','2','3','4','5','6','7','8','9'};
    const uint32_t n = sizeof(message);

    const uint8_t original_c = crc8_c(message, n);
    const uint8_t original_asm = riscv_crc8(message, n);

    volatile uint32_t implementations_match =
        (original_c == original_asm) && (original_c == 0xF4U);

    uint32_t detected = 0U;
    uint32_t tested = 0U;

    for (uint32_t byte = 0U; byte < n; ++byte) {
        for (uint8_t bit = 0U; bit < 8U; ++bit) {
            message[byte] ^= (uint8_t)(1U << bit);

            const uint8_t changed_crc = crc8_c(message, n);
            if (changed_crc != original_c) {
                detected++;
            }
            tested++;

            message[byte] ^= (uint8_t)(1U << bit);
        }
    }

    volatile uint32_t single_bit_errors_tested = tested;
    volatile uint32_t single_bit_errors_detected = detected;

    (void)implementations_match;
    (void)single_bit_errors_tested;
    (void)single_bit_errors_detected;

    for (;;) {
    }
}
