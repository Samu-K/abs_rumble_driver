#ifndef ISERIAL_H
#define ISERIAL_H

#include <QString>
#include <cstdint>
#include "client.h"

// Packet to send to board over USB
struct UsbPacket {
    uint8_t vibration_speed;
};

class ISerial {
public:
    virtual ~ISerial() = default;
    virtual void sendData(const UsbPacket& data) = 0;
};

#endif // ISERIAL_H
