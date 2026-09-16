#include <stdint.h>

#include "app_cfg.h"
#include "gd32vw55x_platform.h"
#include "wrapper_os.h"

#include "gd32vw55x.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#define LED_GPIO_PORT       GPIOC
#define LED_GPIO_PIN        GPIO_PIN_13
#define LED_GPIO_CLOCK      RCU_GPIOC
#define CRC8_POLYNOMIAL     0x07U
#define CRC8_EXPECTED       0xF4U
#define FRAME_PERIOD_MS     10000U
#define BIT_ZERO_MS         150U
#define BIT_ONE_MS          600U
#define BIT_GAP_MS          250U
#define FRAME_GAP_MS        2000U

typedef struct {
    const uint8_t *data;
    uint32_t length;
    uint32_t identifier;
} crc_job_t;

typedef struct {
    uint8_t crc;
    uint32_t identifier;
} crc_result_t;

static const uint8_t test_message[] = "123456789";
static const uint8_t corrupted_message[] = "123456788";
static QueueHandle_t job_queue;
static QueueHandle_t result_queue;

volatile uint8_t g_freertos_crc = 0U;
volatile uint8_t g_freertos_crc_corrupted = 0U;
volatile uint32_t g_freertos_ok = 0U;

static void led_init(void)
{
    rcu_periph_clock_enable(LED_GPIO_CLOCK);
    gpio_mode_set(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
    gpio_output_options_set(
        LED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, LED_GPIO_PIN);
    gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
}

static void led_on(void)
{
    gpio_bit_reset(LED_GPIO_PORT, LED_GPIO_PIN);
}

static void led_off(void)
{
    gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
}

static uint8_t crc8_atm(const uint8_t *data, uint32_t length)
{
    uint8_t crc = 0U;

    while (length-- > 0U) {
        crc ^= *data++;
        for (uint32_t bit = 0U; bit < 8U; ++bit) {
            crc = ((crc & 0x80U) != 0U)
                ? (uint8_t)((crc << 1U) ^ CRC8_POLYNOMIAL)
                : (uint8_t)(crc << 1U);
        }
    }
    return crc;
}

static void producer_task(void *argument)
{
    (void)argument;
    const crc_job_t jobs[] = {
        {test_message, 9U, 0U},
        {corrupted_message, 9U, 1U}
    };

    for (;;) {
        for (uint32_t index = 0U; index < 2U; ++index) {
            (void)xQueueSend(job_queue, &jobs[index], portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(FRAME_PERIOD_MS));
    }
}

static void processor_task(void *argument)
{
    (void)argument;
    crc_job_t job;

    for (;;) {
        if (xQueueReceive(job_queue, &job, portMAX_DELAY) == pdPASS) {
            const crc_result_t result = {
                crc8_atm(job.data, job.length),
                job.identifier
            };
            (void)xQueueSend(result_queue, &result, portMAX_DELAY);
        }
    }
}

static void show_crc_byte(uint8_t value)
{
    for (uint8_t mask = 0x80U; mask != 0U; mask >>= 1U) {
        led_on();
        vTaskDelay(pdMS_TO_TICKS(
            ((value & mask) != 0U) ? BIT_ONE_MS : BIT_ZERO_MS));
        led_off();
        vTaskDelay(pdMS_TO_TICKS(BIT_GAP_MS));
    }
    vTaskDelay(pdMS_TO_TICKS(FRAME_GAP_MS));
}

static void validator_task(void *argument)
{
    (void)argument;
    crc_result_t result;
    uint8_t main_crc = 0U;
    uint8_t altered_crc = 0U;
    uint32_t received = 0U;

    for (;;) {
        if (xQueueReceive(result_queue, &result, portMAX_DELAY) != pdPASS) {
            continue;
        }

        if (result.identifier == 0U) {
            main_crc = result.crc;
            received |= 1U;
        } else {
            altered_crc = result.crc;
            received |= 2U;
        }

        if (received == 3U) {
            g_freertos_crc = main_crc;
            g_freertos_crc_corrupted = altered_crc;
            g_freertos_ok =
                ((main_crc == CRC8_EXPECTED) && (altered_crc != main_crc))
                ? 1U : 0U;

            if (g_freertos_ok != 0U) {
                show_crc_byte(main_crc);
            } else {
                for (uint32_t pulse = 0U; pulse < 10U; ++pulse) {
                    gpio_bit_toggle(LED_GPIO_PORT, LED_GPIO_PIN);
                    vTaskDelay(pdMS_TO_TICKS(100U));
                }
                led_off();
            }
            received = 0U;
        }
    }
}

int main(void)
{
    sys_os_init();
    platform_init();
    led_init();
    job_queue = xQueueCreate(4U, sizeof(crc_job_t));
    result_queue = xQueueCreate(4U, sizeof(crc_result_t));

    if ((job_queue == NULL) || (result_queue == NULL)) {
        for (;;) {
        }
    }

    BaseType_t producer_ok = xTaskCreate(
        producer_task, "Producer", configMINIMAL_STACK_SIZE,
        NULL, tskIDLE_PRIORITY + 1U, NULL);
    BaseType_t processor_ok = xTaskCreate(
        processor_task, "CRC", configMINIMAL_STACK_SIZE,
        NULL, tskIDLE_PRIORITY + 2U, NULL);
    BaseType_t validator_ok = xTaskCreate(
        validator_task, "Validator", configMINIMAL_STACK_SIZE,
        NULL, tskIDLE_PRIORITY + 1U, NULL);

    if ((producer_ok != pdPASS) || (processor_ok != pdPASS) ||
        (validator_ok != pdPASS)) {
        for (;;) {
        }
    }

    sys_os_start();
    for (;;) {
    }
}
