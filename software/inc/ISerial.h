#ifndef ISERIAL_H
#define ISERIAL_H

#include <QString>
#include <cstdint>


#include "protocol.h"

class ISerial {
public:
    virtual ~ISerial() = default;
    virtual void sendData(const UsbPacket& data) = 0;
};

#endif // ISERIAL_H
