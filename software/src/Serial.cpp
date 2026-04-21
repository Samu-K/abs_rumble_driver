#include "LinuxSerial.h"
#include <QDebug>

Serial::Serial(const QString& portName) {
    m_serial.setPortName(portName);
    
    // Standard ESP32 hardware UART configuration
    m_serial.setBaudRate(QSerialPort::Baud115200);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);

    if (m_serial.open(QIODevice::WriteOnly)) {
        qDebug() << "[LinuxSerial] Successfully connected to hardware on:" << portName;
    } else {
        qWarning() << "[LinuxSerial] CRITICAL: Failed to open port" << portName 
                   << "Error:" << m_serial.errorString();
    }
}

LinuxSerial::~LinuxSerial() {
    if (m_serial.isOpen()) {
        m_serial.close();
        qDebug() << "[LinuxSerial] Hardware port safely closed.";
    }
}

void LinuxSerial::sendData(const UsbPacket& data) {
    if (m_serial.isOpen()) {
        // We format this into a string wrapper like "<PWM:255>\n" 
        QString cmd = QString("<PWM:%1>\n").arg(data.vibration_speed);
        
        m_serial.write(cmd.toUtf8());
        
        // Force the OS buffer to push the data down the USB cable immediately
        // to ensure zero-latency haptic feedback.
        m_serial.flush(); 
    } else {
        // Failsafe so we don't crash if the USB cable is yanked mid-game
        qWarning() << "[LinuxSerial] Data dropped. Port is closed.";
    }
}