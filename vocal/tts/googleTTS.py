#!/usr/bin/python
# -*- coding: utf-8 -*-
import time		#for delay
#import pygst	#for playing mp3 stream
import gst		# " " - binding
import sys
import gobject
from urllib2 import Request, urlopen

class pyTTS():
    """tts in python, powered by google"""
    def onFinish(self, bus, msg):
        '''callback called when eos received'''
        self.player.set_state(gst.STATE_NULL)
        self.mainloop.quit()

    def __init__(self):
        self.player = None
        self.mainloop = gobject.MainLoop()

    def synthetize(self, text, lang='en', volume=5.0):
        '''Play an .mp3 file from network'''
        music_stream_uri = 'http://translate.google.com/translate_tts?'+'q='+text+'&tl='+lang+'&ie=UTF-8'
        self.player = gst.element_factory_make("playbin", "player")
        self.player.set_property('uri', music_stream_uri)
        #self.player.set_property('volume', volume)
        self.setVolume(volume)
        self.player.set_state(gst.STATE_PLAYING)
        # Handling wait issue
        bus = self.player.get_bus()
        bus.add_signal_watch_full(1)
        bus.connect("message::eos", self.onFinish)
        self.mainloop.run()     #Wait an event

    def NBsynthetyze(self, text, lang='en', volume=5.0):
        music_stream_uri = 'http://translate.google.com/translate_tts?tl='+lang+'&q='+text
        self.player = gst.element_factory_make("playbin", "player")
        self.player.set_property('uri', music_stream_uri)
        self.player.set_property('volume', volume)
        self.player.set_state(gst.STATE_PLAYING)

    def download(self, text, lang='en', filename='translate_tts.mp3'):
        req = Request(url='http://translate.google.com/translate_tts')
        req.add_header('User-Agent', 'Custom agent !')
        req.add_data('tl='+lang+'&q='+text+'&ie=UTF-8')
        fin = urlopen(req)
        mp3 = fin.read()
        fout = file(filename, 'wb')
        fout.write(mp3)
        fout.close()

    def setVolume(self, value):
        """set gstreamer volume"""
        self.player.set_property('volume', value)
        pass

if __name__=="__main__":
    # Prepare string given in argument for google request
    input_string = sys.argv
    if len( input_string ) < 3:
        print( "Usage: ./tts.py language_code text" )
        sys.exit(1)

    # Handling cmd line args
    input_string.pop(0)  
    lang = input_string[0]
    input_string.pop(0)  
    #TODO checking language validity (enum)
    tts_string = '+'.join(input_string)
    
    pyTTS().synthetize(tts_string, lang)
    pyTTS().download(tts_string, lang)

    sys.exit(0)
