#include "client.h"
#include "motor.h"
#include "esp32.h"

#define MOTOR_IN1_PIN 18
#define MOTOR_IN2_PIN 19
#define MOTOR_ENA_PIN 5

void start_client() {

    ESP32Board board((gpio_num_t)MOTOR_ENA_PIN,(gpio_num_t)MOTOR_IN1_PIN,(gpio_num_t)MOTOR_IN2_PIN);

    Motor brakeHapticMotor(&board,MOTOR_ENA_PIN,MOTOR_IN1_PIN,MOTOR_IN2_PIN);



}