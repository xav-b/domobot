#!/usr/bin/python
# -*- coding: utf8 -*-

"""==================================================
 * 	Filename:
 *	
 *	Description:
 *
 *	Version:
 * 	Created:
 * 	Compiler:
 *
 *	Author:
=================================================="""
#Interesting technic for reading lines one by one
#http://stackoverflow.com/questions/1093598/pyserial-how-to-read-last-line-sent-from-serial-device

import serial, sys

class RfUart:
	def __init__(self, port, baud_rate):
		print 'Initializing uart communication'
		self.port = port
		self.baud_rate = baud_rate

	def uart_connect(self):
		try:
			#sock = serial.Serial('/dev/ttyACM0',9600,timeout=1)
			self.sock = serial.Serial(self.port,self.baud_rate,timeout=1)
		except:
			print '[!] Error opening serial socket'
			sys.exit()

		print 'Opened successfully serial port ', self.sock.portstr
		print '[info] ', self.sock

	def send_cmd(self, cmd, check):
		msg = ''
		if cmd == 'switch':
			self.sock.write('s')
		elif cmd == 'report':
			self.sock.write('r')
		else:
			print 'Unknown command'
			check=False
	
		if check:
			msg = self.sock.read(1024)
			print '	[ -> Received %d byte(s) ]' % len(msg)
			print msg

d = RfUart('/dev/ttyACM0', 9600)
RfUart.uart_connect(d)
cmd = raw_input('Stalker > ')
while command != 'quit':
	if not cmd: break
	RfUart.send_cmd(d, cmd, True)
	cmd = raw_input('Stalker > ')

d.sock.close()
print 'RFCOMM ending...'
