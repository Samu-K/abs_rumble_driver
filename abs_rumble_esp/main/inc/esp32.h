#include "board.h"
#include "driver/gpio.h"

class ESP32Board : public IBoard {
    public:
        ESP32Board(gpio_num_t in1, gpio_num_t in2, gpio_num_t ena);
        void setPinHigh(int pin) override;
        void setPinLow(int pin) override;
        void setPWM(int pin, int value) override;
};