import socket
import time
import struct

# Define the destination IP address and port
ip_address = "10.42.0.4"
port = 1883

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Define the message you want to send
message = b"<s>"

try:
    # Send the UDP packet
    for i in range(10):

        print(f"[{time.time()}] Sent: {message}")
        sock.sendto(message, (ip_address, port))
        data, addr = sock.recvfrom(1024)  # Buffer size is 1024 bytes
        print(f"Message received [{len(data)}] from  [{addr}]: [{data}]")

        print()
        time.sleep(1)

    print("UDP packet sent successfully!")
except socket.error as e:
    print("Error occurred while sending UDP packet:", e)
finally:
    # Close the socket
    sock.close()
