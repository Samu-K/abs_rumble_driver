#ifndef MOCKSERIAL_H
#define MOCKSERIAL_H

#include "ISerial.h"
#include <QObject>
#include <QDebug>

class MockSerial : public ISerial {
public:
    void sendData(const UsbPacket& data) override {
        qDebug() << "MockSerial sending data:" << data.vibration_speed;
    }
};

#endif // MOCKSERIAL_H
