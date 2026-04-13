#ifndef SERIAL_H
#define SERIAL_H

#include "ISerial.h"
#include <QSerialPort>
#include <QString>

class Serial : public ISerial {
public:
    // Defaults to the standard ESP32 USB path, but can be overridden
    explicit Serial(const QString& portName = "/dev/ttyACM0");
    ~Serial() override;

    void sendData(const UsbPacket& data) override;

private:
    QSerialPort m_serial;
};

#endif // LINUXSERIAL_H