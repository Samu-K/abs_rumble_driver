#include "motor.h" 

#include <iostream>

Motor::Motor(IBoard* hardwareBoard, int ena, int in1, int in2) : board(hardwareBoard), enaPin(ena), in1Pin(in1), in2Pin(in2), isSpinning(false) {}

void Motor::setVibrationSpeed(int pwmValue) {
    if (pwmValue > 255) pwmValue = 255;
    if (pwmValue < 0 ) pwmValue = 0;

    if (pwmValue > 0 ) {
        if (!isSpinning) {
            board->setPinHigh(in1Pin);
            board->setPinLow(in2Pin);
            board->setPWM(enaPin, pwmValue);
            isSpinning = true;
        } else {
            std::cout << "Updating PWM" << std::endl;
            board->setPWM(enaPin, pwmValue);
        }
    } else {
        stop();
    }
}

void Motor::stop() {
    if (isSpinning) {
        board->setPinLow(in1Pin);
        board->setPinLow(in2Pin);
        board->setPWM(enaPin, 0);
        isSpinning = false;
    }
    }