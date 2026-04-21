#ifndef BOARD_H
#define BOARD_H

class IBoard {
    public:
        virtual ~IBoard() = default;
        
        virtual void setPinHigh(int pin) = 0;
        virtual void setPinLow(int pin) = 0;
        virtual void setPWM(int pin, int value) = 0;
};


#endif