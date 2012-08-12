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
#import bluetooth as bt
import select
import time
import sys

target_name = 'linvor'
#target_name = 'B o s s Xav'
target_address = '00:11:04:28:14:17'
#target_address = None
find = False
nb_test = 2

#Manage close from remote target
#Manage several tries !
print 'Serial communication over bluetooth'
'''print ''
print 'Performing inquiry...'
while target_address is None and nb_test > 0:
	if nb_test < 2:
		print 'No suitable device detected, giving an other shot'
	nb_test -= 1
	try:
		btdevices = bluetooth.discover_devices(flush_cache=True)
	except:
		print '[!] Couldn\'t get any bluetooth socket'
		sys.exit(1)
	print '%d nearby device(s) found' %len(btdevices)
	if len(btdevices) == 0 and nb_test == 0:
		print '[!] Couldn\'t find any accessible device'
		print '[!] Make sure it is powered or nearby enough (about less than 8 meters distant)'
		sys.exit(1)
	for bdaddr in btdevices:
		if target_name == bluetooth.lookup_name(bdaddr):
			target_address = bdaddr
			break

	if target_address is not None:
		print '	Found bluetooth target ', target_address, ':', bluetooth.lookup_name(target_address)
		services = bluetooth.find_service(address=target_address)
		print '%d services available on it' % len(services)
		#service_matches = find_service( name = "SampleServer", uuid = SERIAL_PORT_CLASS )
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
		break
	elif target_address is None and nb_test == 0:
		print '[!] Couldn\'t find bluetooth target called ', target_name 
		print '[!] Make sure your device is powered or nearby enough (about less than 8 meters distant)'
		sys.exit(1)
'''
#epc_addr = btdevices[0]
port=1

sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
try:
	sock.connect((target_address, port))
	#sock.setblocking(False)   #to check
except:
	print '[!] Error connecting device'
	sys.exit(1)

#readable, writable, excepts = select( [sock], [sock], [], 1 )
#if not sock in writable:
#	sys.exit(1)
#select module can tell if data is ready, to check !
#if not sock in readable:
#	sys.exit(1)

#print 'Connected to %s on port %s' % (btdevices[0], port)
print 'This client advises serial port services and is either writable and readable'
print 'Communication ready...'
#time.sleep(1)
#sock.send("r\r\n")
#time.sleep(1)
command = raw_input('Send > ')
while command != 'quit':
	if not command: break
	if (command == 'switch'):
		line_msg = 1
		sock.send('s')
	if (command == 'report'):
		line_msg = 7
		sock.send('r')
	#else:
	#	sock.send('o\r\n')
	#eot=0
	count=0
	time.sleep(1)
	chaine =sock.recv(256)
	'''data = 'init'
	while count < line_msg:
		#eot=0
		data =''
		while (data!='\r'):
			data = sock.recv(1)
			chaine += data
			#if (data=='\r'):
			#	eot = 1
		#if not data: break
		#if data == '': break
		#if len(data) == 0: break
		count += 1'''
	print '	[Received %s byte(s)] ' % len(chaine)
	print chaine
	command = raw_input('Send > ')

#print 'Final string: ', chaine
sock.close()
print 'Communication ended, closing connection...'
