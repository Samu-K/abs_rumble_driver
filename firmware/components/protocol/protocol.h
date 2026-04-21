#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>

struct TelemetryPacket {
    float speed_kmh;
    float brake_input;
    float wheel_slip[4];
};

struct UsbPacket {
    uint8_t vibration_speed;
};

#endif // PROTOCOL_H
