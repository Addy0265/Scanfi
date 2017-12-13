import os
import math
import time
import numpy as np
import csv
import matplotlib.pyplot as plt

def main():
	mycsvfile = open(r'dataLog.csv', 'w')
	while True:
		cmd = "iwconfig wlan2 | grep -i --color quality | grep -i --color signal"

		data = os.popen(cmd).read()
		
		data_csv = np.zeros(shape=(3))
		#print data

		#print data[23:25]

		flag = 0
		link_quality = ''
		signal_strength = ''	
		# for i in range(0, len(data)):
			
		# 	if flag == 1 and data[i] != '\\':
		# 		link_quality += data[i]	


		# 	if flag == 2 and data[i] != ' ':
		# 		signal_strength += data[i]
		# 		break

		# 	if data[i] == '=' and flag == 0:
		# 		flag = 1

		# 	if data[i] == '=' and flag == 1:
		# 		flag = 2 

		# link_quality += '\0'
		# signal_strength += '\0'

		link_quality = data[23:25]
		signal_strength = data[43:46]
		#print link_quality, signal_strength

		rssi_dBm = int(signal_strength)
		quality = int(link_quality)
		rssi_dB = rssi_dBm - 30
		#print rssi_dBm, quality
		freq = 2437
		
		#distance = math.pow(10.0, (27.55 - (20*math.log10(freq)) + math.fabs(rssi_dBm))/20.0)
		
		distance = math.pow(10.0, (27.55 - (20*math.log10(freq)) + math.fabs(rssi_dBm))/20.0)

		data_to_write =  "RSSI = " + str(rssi_dBm) + " dBm  |  Quality = " + str(quality) + "/70" + "  |  Distance = " + str(distance) + " m" + "\n"
		print data_to_write
		dataFile.write(data_to_write)

				
		data_csv[0] = rssi_dBm
		data_csv[1] = quality
		data_csv[2] = distance
		mycsvfile = open(r'dataLog.csv', 'a')
		thedatawriter = csv.writer(mycsvfile)
		thedatawriter.writerow(data_csv)

		time.sleep(0.05)


dataFile = open("6m.txt", "w")

if __name__ == '__main__':
	main()
	dataFile.close()