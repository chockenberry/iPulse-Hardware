
# install pyserial with: pip3 install pyserial

import serial
import time
import sys

ser = serial.Serial("/dev/cu.usbmodem1444201", 115200)

print("startup")
time.sleep(3)

if len(sys.argv) == 1:
	parameter = "500\r"
else:
	parameter = sys.argv[1] + "\r"

ser.write(parameter.encode('utf-8'))

print("sent")
time.sleep(3)


