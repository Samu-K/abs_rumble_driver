#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <memory>
#include <QUdpSocket>
#include "ISerial.h"

struct TelemetryPacket {
    float speed_kmh;
    float brake_input;
    float wheel_slip[4];
};



class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(double absThreshold READ absThreshold WRITE setAbsThreshold NOTIFY absThresholdChanged)
    Q_PROPERTY(double rumbleIntensity READ rumbleIntensity WRITE setRumbleIntensity NOTIFY rumbleIntensityChanged)

public:
    explicit Backend(std::unique_ptr<ISerial> serial, QObject *parent = nullptr);
    ~Backend() override;

    double absThreshold() const { return m_absThreshold; }
    void setAbsThreshold(double value);

    double rumbleIntensity() const { return m_rumbleIntensity; }
    void setRumbleIntensity(double value);

    UsbPacket parse_telemetry(TelemetryPacket telemetry);
    void read_telemetry();

    void unsubscribe();
signals:
    void absThresholdChanged();
    void rumbleIntensityChanged();


private:
    QUdpSocket *m_udpSocket = nullptr;
    std::unique_ptr<ISerial> m_serial;
    double m_absThreshold = 0.8;
    double m_rumbleIntensity = 1.0;

    int mapFloatToPwm(float x, float in_min, float in_max, int out_min, int out_max);

};

#endif // BACKEND_H
