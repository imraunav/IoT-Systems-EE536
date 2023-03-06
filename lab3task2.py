import serial
import time
import csv
from datetime import datetime
import os

debug_flag = False # debug flag

def debug(message):
	''' This function is used for debugging '''
	if debug_flag == True:
		print(message)

def main():
	ser = serial.Serial(port="/dev/ttyACM0",
				baudrate=9600,
				timeout=2,
				bytesize=8)
	debug('Serial connection opened...')
	filename = "data.csv"
	if os.path.exists(filename) == False:
		header = ['Log-time',
			'Sensor-1',
			'Sensor-2',
			'Sensor-3',
			'Sensor-4',
			'Sensor-5',
			'Sum-value']
		with open(filename, mode='w') as file:
			writer = csv.writer(file)
			writer.writerow(header)
			debug('New file created')

	ser.reset_input_buffer()
	ser.reset_output_buffer()
	time.sleep(3) # waiting for arduino to initialize 
	ack = "ACK"
	while True:
		debug('Sending ACK...')
		ser.write(ack.encode('utf-8'))
		time.sleep(1)
		sensors = [datetime.now()]
		debug(ser.out_waiting)
		running_sum = 0
		for _ in range(5):
			reading = ser.readline().strip().decode()
			sensors.append(reading)
			debug('Received sensor data...')
			running_sum += int(reading)
		sensors.append(running_sum)

		with open(filename, mode='a') as file:
			writer = csv.writer(file)
			writer.writerow(sensors)
			debug('File updated')

if __name__ == "__main__":
	main()
