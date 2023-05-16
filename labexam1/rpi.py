import serial
import csv
import os
import time 
import socket

debug_flag = True # debug flag
HOST = "raunav-rpi.local"
PORT = 2000
PORT_INDICATOR = 3000

def debug(message):
	'''
		This function is used for debugging.
	'''
	if debug_flag == True:
		print(message)

def main():
	# arduino as altitute sensor
	alt_sensor = serial.Serial(port="/dev/ttyACM0",
								baudrate=9600,
								timeout=2,
								bytesize=8)
	debug("Serial connection started...")
	alti_file = "alti.csv"
	fuel_file = "fuel.csv"
	alt_sensor.reset_input_buffer()
	alt_sensor.reset_output_buffer()

	
	# arduino as indicator
	alt_indicator = serial.Serial(port="/dev/ttyACM2",
									baudrate=9600,
									timeout=2,
									bytesize=8)
	alt_indicator.reset_input_buffer()
	alt_indicator.reset_output_buffer()

	# alt_req = "ACK_"
	
	#socket object for NODE MCU
	s  = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((HOST, PORT))
	
	# socket object for NODE MCU INDICATOR
	p = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	p.bind((HOST, PORT_INDICATOR))
	


	time.sleep(3) #waiting for sensors to initialize 
	

	while True:
		# interecting with the arduino sensor
		# debug("Sending ACK...")
		# alt_sensor.write(alt_req.encode("utf-8"))
		alt_reading = alt_sensor.readline().strip().decode("utf-8")
		alt_indicator.write(alt_reading.encode("utf-8"))
		with open(alti_file, mode="a") as file:
			writer = csv.writer(file)
			writer.writerow([alt_reading])
			debug('File updated')
		
		#intercting with NodeMCU
		debug("Listening for sensor Node MCU")
		s.listen() # listening untill hears 
		debug("Heard")
		fuel_sensor, addr = s.accept() # when connection is established
		with fuel_sensor:
			fuel_reading = fuel_sensor.recv(1024).strip().decode("utf-8")
		debug("Listening for indicator Node MCU")
		p.listen() # listening untill heards 
		debug("Heard")
		fuel_indicator, addr = p.accept()
		with fuel_indicator:
			fuel_indicator.sendall(fuel_reading.encode("utf-8"))

		with open(fuel_file, mode="a") as file:
			writer = csv.writer(file)
			writer.writerow([fuel_reading])
			debug("File updated")

if __name__ == "__main__":
	main()
