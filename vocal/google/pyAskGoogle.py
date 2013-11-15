from scikits.samplerate import resample
from tempfile import mkstemp
from os import remove
import scikits.audiolab as audiolab
# if you want make the down sample rate using scipy.signal
#import scipy.signal
import urllib2
import sys

if len(sys.argv)<2 :
        print 'Usage: %s <audio file.wav>' %sys.argv[0]
        sys.exit(0)
File=sys.argv[1]

#making a file temp for manipulation
cd, FileNameTmp    = mkstemp('TmpSpeechFile.flac')

#Frame Rate used by api speech from google
fr=16000.

#using audiolab to read wav file
Signal, fs = audiolab.wavread(File)[:2]

#changing the original sample rate to 16000fs fast mode
Signal = resample(Signal, fr/float(fs), 'sinc_best')

#changing sample rate from audio file using scipy this is a bit slow
#Signal=scipy.signal.resample(Signal,int(round(len(Getsignal)*fr)/float(fs)),window=None)

# file Format type
fmt         = audiolab.Format('flac', 'pcm16')
nchannels   = 1

# making the file .flac
afile =  audiolab.Sndfile(FileNameTmp, 'w', fmt, nchannels, fr)

#writing in the file
afile.write_frames(Signal)

#Sending to google the file .flac
url = "https://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=pt-BR"
flac=open(FileNameTmp,"rb").read()
header = {'Content-Type' : 'audio/x-flac; rate=16000'}
req = urllib2.Request(url, flac, header)
data = urllib2.urlopen(req)
print data.read()
remove(FileNameTmp)
