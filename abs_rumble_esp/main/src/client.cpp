#include "client.h"
#include "motor.h"
#include "esp32.h"

int mapFloatToPwm(float x, float in_min, float in_max, int out_min, int out_max) {
    if (x > in_max) x = in_max;
    return (x-in_min)*(out_max-out_min) / (in_max-in_min)+out_min;
};


int setup_socket() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    return sock;
}

sockaddr_in setup_server_addr() {
    // Set up the target address (Our Python Fake Game on localhost:9996)
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr)); // Clear the struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9996);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    return serverAddr;
}

void send_handshake(int sock, sockaddr_in serverAddr) {
    Handshake hs;
    std::cout << "Sending Subscribe Handshake to Server..." << std::endl;
    sendto(sock, (const void*)&hs, sizeof(Handshake), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

void recv_telemetry(sockaddr_in serverAddr, int sock, Motor brakeHapticMotor) {
    TelemetryPacket telemetry;
    socklen_t serverAddrLen = sizeof(serverAddr);
    
    std::cout << "Listening for telemetry data...\n" << std::endl;
    
    while (true) {
        // Receive the bytes directly into our TelemetryPacket struct
        int bytesReceived = recvfrom(sock, (char*)&telemetry, sizeof(TelemetryPacket), 0, (sockaddr*)&serverAddr, &serverAddrLen);
        
        if (bytesReceived == sizeof(TelemetryPacket)) {
            std::cout << "Brake Pressure: " << telemetry.brake_input 
                      << " | FL Tire Slip: " << telemetry.wheel_slip[0] << std::endl;
            
            // Your ABS Trigger Logic
            if (telemetry.brake_input > 0.8 && telemetry.wheel_slip[0] > 2.0) {
                int dynamicSpeed = mapFloatToPwm(telemetry.brake_input, 0.8f, 1.0f, 120,255);
                brakeHapticMotor.setVibrationSpeed(dynamicSpeed);
            } else {
                brakeHapticMotor.stop();
            }
        }
    }
}

#define MOTOR_IN1_PIN 18
#define MOTOR_IN2_PIN 19
#define MOTOR_ENA_PIN 5

void start_client() {

    ESP32Board board((gpio_num_t)MOTOR_ENA_PIN,(gpio_num_t)MOTOR_IN1_PIN,(gpio_num_t)MOTOR_IN2_PIN);

    Motor brakeHapticMotor(&board,MOTOR_ENA_PIN,MOTOR_IN1_PIN,MOTOR_IN2_PIN);
;

    int sock = setup_socket();

    sockaddr_in serverAddr = setup_server_addr();

    send_handshake(sock, serverAddr);

    recv_telemetry(serverAddr, sock, brakeHapticMotor);

    // Cleanup
    close(sock); 
}