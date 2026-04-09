#include <iostream>
#include "board.h"

class MockBoard : public IBoard {
    public:
        void setPinHigh(int pin) override {
            std::cout << " [MOCK HW] Pin" << pin << "-> HIGH\n";
        }
        void setPinLow(int pin) override {
            std::cout << " [MOCK HW] Pin" << pin << "-> LOW\n";
        }
        void setPWM(int pin, int value) override {
            std::cout << " [MOCK HW] Pin" << pin << "-> PWM" << value << "\n";
        }
};
