#!/usr/bin/python
# -*- coding: utf8 -*-

"""==================================================
 * 	Filename: bt_comm.py
 *	
 *	Description: Handle serial communication between a computer
 *				 and the domotic embedded system
 *
 *	Version: 2.1
 * 	Created: 22 - 02 - 2012
 * 	Compiler: none, python interpreter
 *
 *	Author: Xavier Bruhière
=================================================="""

import bluetooth
import select
import time
import sys
from pylab import *
import re

class BtUart:
	def __init__(self, target_name, target_address, nb_tries):
		#Manage close from remote target
		print '===    Serial communication over bluetooth    ==='
		print 'Initializing...' 
		print 'Checking arguments'
		if target_address == None and target_name == None:
			print 'At least one device caracteristic needed'
			sys.exit(1)
		print '	-> ok'
		print 'checking address format'
		if not bluetooth.is_valid_address(target_address):
			sys.exit('Bad syntax address')
		print '	-> ok'
		self.target_name = target_name
		self.target_address = target_address
		self.nb_tries = nb_tries
		self.fd = open('domo.dat', 'w+')

	def scan(self):
		#all is for just scanning
		#not only traget_name, target_address as well
		#self.target_address = None	#Will figure it out later
		found = False
		nb_test = self.nb_tries
		print 'Performing inquiry...'
		print '	-> requesting %s@%s' % (self.target_name, self.target_address)
		while found == False and nb_test > 0:
			if nb_test < self.nb_tries:
				print 'No suitable device detected, giving an other shot'
			nb_test -= 1
			try:
				btdevices = bluetooth.discover_devices(flush_cache=True)
			except:
				print '[!] Couldn\'t get any bluetooth socket'
				sys.exit(1)
			print '%d nearby device(s) detected' %len(btdevices)
			for bdaddr in btdevices:
				if self.target_name == bluetooth.lookup_name(bdaddr):
					self.target_address = bdaddr
					found = True
					break
				elif self.target_address == bdaddr:
					self.target_name = bluetooth.lookup_name(bdaddr)
					found = True
					break
			if found == False and nb_test == 0:
				print '[!] Couldn\'t find bluetooth target called ', self.target_name 
				print '[!] Make sure it is powered or nearby enough (about less than 8 meters distant)'
				sys.exit(1)

	def get_infos(self):
		print 'Catch bluetooth target, inquiring infos'
		services = bluetooth.find_service(address=self.target_address)
		print '	%d services available on it' % len(services)
		for svc in services:
			print "	Service Name: %s"   % svc["name"]
			print "    Host:        %s" % svc["host"]
			print "    Description: %s" % svc["description"]
			print "    Provided By: %s" % svc["provider"]
			print "    Protocol:    %s" % svc["protocol"]
			print "    channel/PSM: %s" % svc["port"]
			print "    svc classes: %s "% svc["service-classes"]
			print "    profiles:    %s "% svc["profiles"]
			print "    service id:  %s "% svc["service-id"]
			print

	def rfcomm_connect(self, port):
		print 'Connecting to %s[%s] on port %s' % (self.target_name, self.target_address, port)
		try:
			self.sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
			self.sock.connect((self.target_address, port))
			#sock.setblocking(False)   #to check
		except:
			print '[!] Error connecting socket'
			sys.exit(1)
		print '	-> done' 
		print 'Setting timeout feature'
		self.sock.settimeout(5.0)
		#print 'This client advises serial port services and is either writable and readable'
		print 'Communication ready...'
		print 
	
	def raw_listen(self, stop):
		stop = float(stop)
		print '[%s] beginning store function, it will last %f seconds' % (time.ctime(), stop)
		t0 = time.time()
		x = arange(0, stop, 0.1)
		line, = plot(x, x)
		line.axes.set_ylim(1,400)
		eot = 0
		try: 
			self.sock.send('l')
			print '	[ -> Sending %s byte(s) ] ' % len('r\r\n')
		except:
			print 'Error sending command'
		while ((time.time()-t0) < stop):
			update = ''
			while (eot==0):
				data = self.sock.recv(1024)
				update += data
				if re.search('\r', data, re.I):
					eot = 1
			print update.split()
			if (len(update.split()) == 2):
				line.set_ydata(update.split()[1])
				draw()
			else:
				print 'bad answer'
		self.sock.send('m')

	def send_cmd(self, cmd, check):
		chaine = ''
		start_t = time.time()
		if (cmd == 'switch'):
			try:
				self.sock.send('s')
				print '	[ -> Sending %s byte(s) ] ' % len('s\r\n')
			except:
				print 'Error sending command'
		elif (cmd == 'report'):
			try: 
				self.sock.send('r')
				print '	[ -> Sending %s byte(s) ] ' % len('r\r\n')
			except:
				print 'Error sending command'
		elif (cmd[0:6]== 'listen'):
			if (len(cmd.split()) == 2):
				self.raw_listen(cmd.split()[1])
			else:
				print 'Wrong args number'
				
			check = False
		elif (cmd == 'help'):
			print 'Command display'
			print '\'switch\' - turn off/on lights'
			print '\'report\' - request a report about module activitie'
			print '\'listen x\' - begin a thread saving raw captor data for x seconds'
			check = False
		else:
			print 'Unknown command'
			check = False
		if check:
			time.sleep(1)
			chaine =self.sock.recv(1024)
			print '	[ -> Received %s byte(s) in %f s] ' % (len(chaine), time.time()-start_t)
			print chaine


#target_name = 'linvor'
#target_address = '00:11:04:28:14:17' for linvor
#target_name = 'B o s s Xav'
#xavier.bruhiere, 'A8:26:D9:FF:95:AC
#port=1

#ion()
start = time.time()
c = BtUart(r'xavier.bruhiere', 'A8:26:D9:FF:95:AC', 1)
BtUart.scan(c)
BtUart.get_infos(c)
BtUart.rfcomm_connect(c, 1)
medium = time.time()
print 'Elapsed time: ' + str(medium - start) + ' s'
cmd = raw_input('Send > ')
while cmd != 'quit':
	if not cmd: break
	BtUart.send_cmd(c, cmd, True)	
	cmd = raw_input('Send > ')

#print 'Final string: ', chaine
c.fd.close()
c.sock.close()
print
print 'Communication ended, closing connection...'
