#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import SocketServer
from googleTTS import *

class RequestHandler(SocketServer.BaseRequestHandler):
    def setup(self):
        print self.client_address, 'connected!'
        self.request.send('hi ' + str(self.client_address) + '\n')

    def handle(self):
        voice = pyTTS()
        while 1:
            data = self.request.recv(1024)
            self.request.send("OK: "+data)
            if data.strip() == 'bye':
                return
            else:
                input_string = data.split()
                lang = input_string[0]
                print 'lang:', lang
                input_string.pop(0)
                tts = '+'.join(input_string)
                voice.synthetize(tts, lang)

    def finish(self):
        print self.client_address, 'disconnected!'
        self.request.send('bye ' + str(self.client_address) + '\n')

#server host is a tuple ('host', port)
server = SocketServer.ThreadingTCPServer(('localhost', 49002), RequestHandler)
server.serve_forever()
