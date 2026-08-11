#include <stdint.h>

#include "crc8_riscv.h"
#include "gd32vw55x.h"

#define LED_GPIO_PORT          GPIOC
#define LED_GPIO_PIN           GPIO_PIN_13
#define LED_GPIO_CLOCK         RCU_GPIOC

#define CRC8_POLYNOMIAL        0x07U
#define CRC8_EXPECTED          0xF4U

#define BIT_ZERO_MS            150U
#define BIT_ONE_MS             600U
#define BIT_GAP_MS             250U
#define FRAME_GAP_MS           2000U
#define ERROR_INTERVAL_MS      100U

static const uint8_t test_message[] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

static const uint8_t corrupted_message[] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '8'
};

volatile uint8_t g_crc_c = 0U;
volatile uint8_t g_crc_asm = 0U;
volatile uint8_t g_crc_corrupted = 0U;
volatile uint32_t g_error_detected = 0U;
volatile uint32_t g_results_match = 0U;

static void busy_wait_delay_ms(uint32_t milliseconds)
{
    while (milliseconds-- > 0U) {
        for (volatile uint32_t cycles = 0U; cycles < 16000U; ++cycles) {
            __asm volatile ("nop");
        }
    }
}

static void led_init(void)
{
    rcu_periph_clock_enable(LED_GPIO_CLOCK);
    gpio_mode_set(
        LED_GPIO_PORT,
        GPIO_MODE_OUTPUT,
        GPIO_PUPD_NONE,
        LED_GPIO_PIN
    );
    gpio_output_options_set(
        LED_GPIO_PORT,
        GPIO_OTYPE_PP,
        GPIO_OSPEED_10MHZ,
        LED_GPIO_PIN
    );
    gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
}

/* El LED de la placa es activo en bajo. */
static void led_on(void)
{
    gpio_bit_reset(LED_GPIO_PORT, LED_GPIO_PIN);
}

static void led_off(void)
{
    gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
}

static void led_pulse(uint32_t duration_ms)
{
    led_on();
    busy_wait_delay_ms(duration_ms);
    led_off();
    busy_wait_delay_ms(BIT_GAP_MS);
}

static void led_show_byte(uint8_t value)
{
    /* Dos destellos cortos anuncian el comienzo de la trama. */
    led_pulse(100U);
    led_pulse(100U);
    busy_wait_delay_ms(500U);

    for (uint8_t mask = 0x80U; mask != 0U; mask >>= 1U) {
        if ((value & mask) != 0U) {
            led_pulse(BIT_ONE_MS);
        } else {
            led_pulse(BIT_ZERO_MS);
        }
    }

    busy_wait_delay_ms(FRAME_GAP_MS);
}

static uint8_t crc8_c(const uint8_t *data, uint32_t length)
{
    uint8_t crc = 0U;

    while (length-- > 0U) {
        crc ^= *data++;

        for (uint32_t bit = 0U; bit < 8U; ++bit) {
            if ((crc & 0x80U) != 0U) {
                crc = (uint8_t)((crc << 1U) ^ CRC8_POLYNOMIAL);
            } else {
                crc = (uint8_t)(crc << 1U);
            }
        }
    }

    return crc;
}

int main(void)
{
    const uint32_t message_length =
        sizeof(test_message) / sizeof(test_message[0]);

    led_init();

    g_crc_c = crc8_c(test_message, message_length);
    g_crc_asm = riscv_crc8(test_message, message_length);
    g_crc_corrupted = crc8_c(corrupted_message, message_length);

    g_error_detected = (g_crc_corrupted != g_crc_c) ? 1U : 0U;
    g_results_match =
        ((g_crc_c == g_crc_asm) &&
         (g_crc_c == CRC8_EXPECTED) &&
         (g_error_detected != 0U)) ? 1U : 0U;

    while (1) {
        if (g_results_match != 0U) {
            /* 0xF4 = 11110100: largo=1, corto=0, MSB primero. */
            led_show_byte(g_crc_asm);
        } else {
            /* Parpadeo rapido continuo: calculo o prueba incorrectos. */
            gpio_bit_toggle(LED_GPIO_PORT, LED_GPIO_PIN);
            busy_wait_delay_ms(ERROR_INTERVAL_MS);
        }
    }
}
