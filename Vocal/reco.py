#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2013 xavier <xavier@laptop-300E5A>
#
# Distributed under terms of the MIT license.


import subprocess
import requests
import pyaudio
import numpy as np


def record(sound_file='out.flac', rate=16000):
    #pid = subprocess.Popen(
        #'arecord -f cd -t wav -d 0 -r 16000', shell=True)

    #return_code = subprocess.call(
        #'arecord -f cd -t wav -d 5 -r {} | \
         #flac - -f --best --sample-rate {} -o {}'.format(
        #rate, ra#te, sound_file), shell=True)

    return_code = subprocess.call(
        'rec tmp.wav silence 1 0.3 1% 1 0.3 1%', shell=True
        #'rec -t flac - silence 1 0.3 1% 1 0.3 1% > out.flac', shell=True
    )
    return return_code


def convert_to_flac(in_file, out_file):
    return subprocess.call(
        'flac -f --best --sample-rate 16000 -o {} {}'.format(out_file, in_file),
        shell=True
    )


def get_google_translation(sound_file='out.flac', lang='en', rate=16000):
    api_url = 'http://www.google.com/speech-api/v1/recognize?lang={}'.format(lang)
    headers = {'content-type': 'audio/x-flac; rate={}'.format(rate)}
    data = open(sound_file, 'rb').read()

    result = requests.post(api_url, data=data, headers=headers)

    if not result.ok:
        print 'Post request failed with status: ', result.status_code
        print result.reason
        print result.content
        return {}

    traduction = result.json()
    print traduction['status']
    return traduction['hypotheses']


record()
#import sys
#res = get_google_translation(sys.argv[1])
convert_to_flac('tmp.wav', 'out.flac')
res = get_google_translation()
print res
