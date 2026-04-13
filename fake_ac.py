import socket
import struct
import time
import math

# use localhost and copy port from assetto corsa
UDP_IP = "127.0.0.1"
UDP_PORT = 9996

def game_telemetry(t):
    speed_kmh = 100.0

    brake_input =  (math.sin(t)+1.0) / 2.0

    # simulate tire slip, if brake pressed "hard" (>0.8)
    slip = 2.5 if brake_input > 0.8 else 0.1
    wheel_slip = [slip,slip,slip,slip]

    packet = struct.pack('ffffff', speed_kmh, brake_input,*wheel_slip)
    return packet
 

def handshake_listener(sock):
    while True:
        data,addr = sock.recvfrom(1024)

        if len(data) == 12:
            identifier, version, operationId = struct.unpack('iii', data)
            print(f"Handshake recv from {addr} | id:{identifier} Ver:{version} Op:{operationId}")

            if operationId == 1:
                print("Client subscribed. Starting telemetry stream")
                return addr


def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    print(f"Game sim listening on port {UDP_PORT}")

    while True:
        sock.settimeout(None) # Block until handshake
        print("Waiting for client handshake")
        client_addr = handshake_listener(sock)

        sock.settimeout(0.5) # Act as the game tick delay
        t=0
        subscribed = True
        
        while subscribed:
            packet = game_telemetry(t)
            sock.sendto(packet, client_addr)

            try:
                data, addr = sock.recvfrom(1024)
                if len(data) == 12:
                    identifier, version, operationId = struct.unpack('iii', data)
                    if operationId == 2: # 2 for unsubscribe
                        print(f"Client {addr} unsubscribed.")
                        subscribed = False
            except socket.timeout:
                pass

            t += 0.1

if __name__ == "__main__":
    main()