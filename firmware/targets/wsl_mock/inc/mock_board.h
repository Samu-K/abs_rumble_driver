#include <iostream>
#include "board.h"

class MockBoard : public IBoard {
    public:
        void setPinHigh(int pin) override; 
        void setPinLow(int pin) override;
        void setPWM(int pin, int value) override;
};
