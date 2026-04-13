#include "Backend.h"
#include <iostream>
#include <QNetworkDatagram>

Backend::Backend(std::unique_ptr<ISerial> serial, QObject *parent)
    : QObject(parent), m_serial(std::move(serial)) {
    read_telemetry();
}

Backend::~Backend() {
    if (m_udpSocket) {
        struct Handshake {
            int identifier;
            int version;
            int operationId;
        } hb = {1, 1, 2}; // 2 is unsubscribe

        m_udpSocket->writeDatagram(reinterpret_cast<const char*>(&hb), sizeof(hb), QHostAddress::LocalHost, 9996);
        std::cout << "Unsubscribe packet sent to localhost:9996" << std::endl;
    }
}

void Backend::setAbsThreshold(double value) {
    if (m_absThreshold != value) {
        m_absThreshold = value;
        emit absThresholdChanged();
    }
}

void Backend::setRumbleIntensity(double value) {
    if (m_rumbleIntensity != value) {
        m_rumbleIntensity = value;
        emit rumbleIntensityChanged();
    }
}

int Backend::mapFloatToPwm(float x, float in_min, float in_max, int out_min, int out_max) {
    if (x > in_max) x = in_max;
    return (x-in_min)*(out_max-out_min) / (in_max-in_min)+out_min;
};


/*
* Takes in telemetry, and calculates the vibration speed for the motor
*/
UsbPacket Backend::parse_telemetry(TelemetryPacket telemetry) {
    std::cout << "Brake Pressure: " << telemetry.brake_input 
                << " | FL Tire Slip: " << telemetry.wheel_slip[0] << std::endl;
    
    // Your ABS Trigger Logic
    std::cout << "abs threshold: " << m_absThreshold << std::endl;
    std::cout << "rumble intensity: " << m_rumbleIntensity << std::endl;
    if (telemetry.brake_input > m_absThreshold && telemetry.wheel_slip[0] > 2.0) {
        int dynamicSpeed = mapFloatToPwm(telemetry.brake_input, m_absThreshold, 1.0f, 120*m_rumbleIntensity, 255*m_rumbleIntensity);
        UsbPacket ret;
        ret.vibration_speed = static_cast<uint8_t>(dynamicSpeed);
        return ret;
    } else {
        UsbPacket ret;
        ret.vibration_speed = 0;
        return ret;
    }
}

// sets us up to read localhost packets
void Backend::read_telemetry() {
    if (!m_udpSocket) {
        m_udpSocket = new QUdpSocket(this);
        
        connect(m_udpSocket, &QUdpSocket::readyRead, this, [this]() {
            while (m_udpSocket->hasPendingDatagrams()) {
                QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
                if (datagram.data().size() >= static_cast<qint64>(sizeof(TelemetryPacket))) {
                    TelemetryPacket packet;
                    memcpy(&packet, datagram.data().constData(), sizeof(TelemetryPacket));
                    
                    std::cout << "--- Telemetry In --- \n"
                              << "Speed (km/h): " << packet.speed_kmh << "\n"
                              << "Brake Input:  " << packet.brake_input << "\n"
                              << "Slip:         " << packet.wheel_slip[0] << "\n"
                              << "-------------------- \n";
                    
                    UsbPacket ret = parse_telemetry(packet);
                    m_serial->sendData(ret);
                }
            }
        });
    }

    struct Handshake {
        int identifier;
        int version;
        int operationId;
    } hb = {1, 1, 1};

    m_udpSocket->writeDatagram(reinterpret_cast<const char*>(&hb), sizeof(hb), QHostAddress::LocalHost, 9996);
    std::cout << "Handshake sent to localhost:9996" << std::endl;
}

void Backend::unsubscribe() {
    if (m_udpSocket) {
        struct Handshake {
            int identifier;
            int version;
            int operationId;
        } hb = {1, 1, 2};

        m_udpSocket->writeDatagram(reinterpret_cast<const char*>(&hb), sizeof(hb), QHostAddress::LocalHost, 9996);
        std::cout << "Unsubscribe packet sent to localhost:9996" << std::endl;
    }
}