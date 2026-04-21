#include "mock_board.h"

void MockBoard::setPinHigh(int pin) {
    std::cout << " [MOCK HW] Pin" << pin << "-> HIGH\n";
}

void MockBoard::setPinLow(int pin) {
    std::cout << " [MOCK HW] Pin" << pin << "-> LOW\n";
}

void MockBoard::setPWM(int pin, int value) {
    std::cout << " [MOCK HW] Pin" << pin << "-> PWM" << value << "\n";
}