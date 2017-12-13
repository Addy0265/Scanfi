import serial
import time

cnt = 0
ser = serial.Serial("com9", 38400)
data_write = "-19 43\n"
flag = 0
ack = 101

while True:
    if(ser.inWaiting() > 0):
        data_read = ser.readline()
        print int(data_read)
        if(int(data_read) == ack):
            flag = 0
    elif(flag == 0):
        time.sleep(1)
        ser.write(data_write)
        flag = 1


