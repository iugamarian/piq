#!/usr/bin/env python
import socket

# SERVER DETAILS
TCP_IP = "127.0.0.1"
TCP_PORT = 8000
BUFFER_SIZE = 20  # Normally 1024, but we want fast response


if __name__ == "__main__":
    # bind socket
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((TCP_IP, TCP_PORT))
    s.listen(1)

    # accep incomming connections
    conn, addr = s.accept()
    print "Connection address:", addr
    while 1:
        data = 1
        data = conn.recv(BUFFER_SIZE)
        print "received data:", data

    # close connection
    conn.close()
