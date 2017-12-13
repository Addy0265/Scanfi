#front 2
#right 1
import os
import math
import time
import numpy as np
import csv
import matplotlib.pyplot as plt
import serial

# Leap of faith
d0 = 50

flag = 0
ack = 101

#RSSI Values
P0 = 0
P1 = 0
P2 = 0

delta_x = 0
delta_y = 0
theta = 0
dist = 0
lamda = 1
dmax = 100


ser = serial.Serial("/dev/ttyACM0", 38400)

fwd = "30 30\n"
rev = "0 50\n"
turn90p = "90 0\n"
turn90n = "-90 0\n"
motionCmd = ""
 
def sendMotionCmd(data):
	global flag, ack
	while(ser.inWaiting() == 0):
		if(flag == 0): 
			time.sleep(0.5)
			ser.write(data)
			flag = 1

	data_read = ser.readline()
	   
	if(int(data_read) == ack):
		print data_read
		flag = 0

def getRSSI():	
	sum = 0	
	for i in range(0, 20):
		cmd = "iwconfig wlan0 | grep -i --color quality | grep -i --color signal"

		data = os.popen(cmd).read()

		
		link_quality = ''
		signal_strength = ''	
		link_quality = data[23:25]
		signal_strength = data[43:46]

		rssi_dBm = int(signal_strength)
		quality = int(link_quality)
		rssi_dB = rssi_dBm - 30

		freq = 2437
			
		#distance = math.pow(10.0, (27.55 - (20*math.log10(freq)) + math.fabs(rssi_dBm))/20.0)

		#data_to_write =  "wlan0: RSSI = " + str(rssi_dBm) + " dBm"

		#print data_to_write

		sum = sum + rssi_dBm

	rssi_dBm = sum/20
			
	return rssi_dBm

def analyse():
	global delta_x, delta_y, theta, dist, lamda, motionCmd, dmax
	delta_y = P1 - P0
	delta_x = P2 - P0

	lamda = dmax/(math.pow(delta_x,2), math.pow(delta_y,2))

	theta = math.ceil(math.degrees((math.atan2(delta_y, delta_x))))
	dist = math.ceil(lamda * math.sqrt(math.pow(delta_x,2), math.pow(delta_y,2)))

	print "Delta_y :" + str(delta_y) + "Delta_x :" + str(delta_x) + "lamda :" + str(lamda) + "Theta :" + str(theta) + "Dist :" + str(dist)

	motionCmd = str(theta) + " " + str(dist) + "\n"
	#print motionCmd

	sendMotionCmd(motionCmd)

def probe():
	global P0, P1, P2
	P0 = getRSSI()
	if(P0 < -25):
		sendMotionCmd(fwd)
		P1 = getRSSI()
		sendMotionCmd(rev)
		sendMotionCmd(turn90p)
		sendMotionCmd(fwd)
		P2 = getRSSI()
		sendMotionCmd(rev)
		print str(P0) + "  " + str(P1) + "  " + str(P2)
		analyse()
 

time.sleep(1)
while True:
	sendMotionCmd(fwd)
		

