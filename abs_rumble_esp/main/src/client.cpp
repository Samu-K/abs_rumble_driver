#include "client.h"
#include "motor.h"
#include "esp32.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define MOTOR_IN1_PIN 18
#define MOTOR_IN2_PIN 19
#define MOTOR_ENA_PIN 5

void start_client() {

    ESP32Board board((gpio_num_t)MOTOR_ENA_PIN,(gpio_num_t)MOTOR_IN1_PIN,(gpio_num_t)MOTOR_IN2_PIN);

    Motor brakeHapticMotor(&board,MOTOR_ENA_PIN,MOTOR_IN1_PIN,MOTOR_IN2_PIN);

    char buffer[64];
    int pos = 0;

    printf("ESP32 ABS Node Started. Listening for PWM commands...\n");

    while (true) {
        int c = fgetc(stdin);
        
        if (c == EOF) {
            // Yield to FreeRTOS to prevent watchdog triggers if no data is available
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        // A newline indicates the end of the LinuxSerial command
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
            buffer[pos++] = (char)c;
        }
    }
}