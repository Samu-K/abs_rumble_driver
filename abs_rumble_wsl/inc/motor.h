#ifndef MOTOR_H
#define MOTOR_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "board.h"

class Motor {
    private:
        IBoard* board;
        int enaPin;
        int in1Pin;
        int in2Pin;
        bool isSpinning;

    public:
        Motor(IBoard* hardwareBoard, int ena, int in1, int in2);
        
        void setVibrationSpeed(int pwmValue);
        void stop();
};

#endif //MOTOR_H