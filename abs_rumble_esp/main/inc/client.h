#ifndef CLIENT_H
#define CLIENT_H

#include <lwip/sockets.h>

// 12-byte Handshake struct mimicking Assetto Corsa 
struct Handshake {
    int identifier = 1;
    int version = 1;
    int operationId = 1; // 1 = Subscribe to continuous updates
};

// simplified 24-byte telemetry struct
struct TelemetryPacket {
    float speed_kmh;
    float brake_input;
    float wheel_slip[4];
};

int setup_socket();
sockaddr_in setup_server_addr();
void send_handshake(int sock, sockaddr_in serverAddr);
void recv_telemetry();
void start_client();
int mapFloatToPwm(float x, float in_min, float in_max, int out_min, int out_max);

#endif // CLIENT_H