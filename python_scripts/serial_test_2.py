#!/usr/bin/python

import serial
import socket

UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", UDP_PORT))
ser = serial.Serial('/dev/ttyACM0', 9600)

#TODO: Add condition to this loop
while True:
    if( sock.recvfrom(1024) ):
        ser.write('3')
        continue
