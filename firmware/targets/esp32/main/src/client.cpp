#include "client.h"
#include "motor.h"
#include "esp32.h"

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#define MOTOR_IN1_PIN 18
#define MOTOR_IN2_PIN 19
#define MOTOR_ENA_PIN 5

#define BUF_SIZE (1024)

void start_client() {

    ESP32Board board((gpio_num_t)MOTOR_ENA_PIN,(gpio_num_t)MOTOR_IN1_PIN,(gpio_num_t)MOTOR_IN2_PIN);

    Motor brakeHapticMotor(&board,MOTOR_ENA_PIN,MOTOR_IN1_PIN,MOTOR_IN2_PIN);

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    char buffer[64];
    int pos = 0;
    TickType_t last_char_time = xTaskGetTickCount();

    printf("ESP32 ABS Node Started. Listening for PWM commands via UART...\n");

    while (true) {
        uint8_t data;
        int len = uart_read_bytes(UART_NUM_0, &data, 1, pdMS_TO_TICKS(10));
        
        if (len > 0) {
            char c = (char)data;
            last_char_time = xTaskGetTickCount();

            // A newline indicates the end of the PC command
            if (c == '\n' || c == '\r') {
                buffer[pos] = '\0'; // Null-terminate the string
                
                int pwmValue = 0;
                // Look for the exact syntax sent by the PC: "<PWM:255>"
                if (sscanf(buffer, "<PWM:%d>", &pwmValue) == 1) {
                    brakeHapticMotor.setVibrationSpeed(pwmValue);
                }
                
                pos = 0; // Reset the buffer index for the next command
            } 
            else if (pos < sizeof(buffer) - 1) {
                buffer[pos++] = c;
            }
        } else {
            // Buffer timeout logic
            if (pos > 0 && (xTaskGetTickCount() - last_char_time) > pdMS_TO_TICKS(50)) {
                pos = 0;
            }
        }
    }
}