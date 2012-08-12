#!/usr/bin/python

import time		#for delay
import pygst	#for playing mp3 stream
import gst		# " "
import sys

# Prepare string given in argument for google request
input_string = sys.argv
input_string.pop(0)
tts_string = '+'.join(input_string)
print tts_string

#Play an .mp3 file from the internet
music_stream_uri = 'http://translate.google.com/translate_tts?tl=en&q=' + tts_string
player = gst.element_factory_make("playbin", "player")
player.set_property('uri', music_stream_uri)
player.set_state(gst.STATE_PLAYING)

#requires a delay, if the py process closes before the mp3 has finished it will be cut off.
time.sleep(10)
