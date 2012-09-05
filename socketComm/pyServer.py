# -*- coding: utf-8 -*-

import socket
import os, os.path
import time
from sklearn.externals import joblib
from sklearn import datasets
import cPickle as pickle
#import io
import time

########## Prepare SVC computation #########################################################
t1 = time.time()
print '[CHRONO] Beginning:', time.time() - t1
XTestTmp, YTest = datasets.load_svmlight_file("./pySVM/SVMTest.txt")
XTest = XTestTmp.toarray()
print '[CHRONO] Data loaded:', time.time() - t1

''' Loading model '''
''' Pickle try '''
#fd = open('./pySVM/SVCmodel.data', 'rb')
#fd = io.BytesIO(open("./pySVM/SVCmodel.data", "rb").read())
#clf = pickle.load(fd)
#fd.close()
''' Joblist test '''
clf = joblib.load('./pySVM/SVCmodel.data')
print '[CHRONO] Classifier loaded:', time.time() - t1

########## ####################### #########################################################

if os.path.exists( "/tmp/socket" ):
  os.remove( "/tmp/socket" )
 
print "Opening socket..."
#server = socket.socket( socket.AF_UNIX, socket.SOCK_STREAM )
server = socket.socket( socket.AF_UNIX, socket.SOCK_DGRAM )
server.bind("/tmp/socket")
 
print "Listening..."
while True:
  datagram = server.recv(1024)
  if not datagram:
    break
  else:
    print datagram
    if "done" == datagram:
      break
    else:
        ''' Testing now new data '''
        fd = open('./pySVM/predict.data', 'w')
        for prediction in clf.predict(XTest):
            fd.write(str(prediction) + '\n')
            print '[CHRONO] Each prediction:', time.time() - t1
        fd.close
        print '[CHRONO] End:', time.time() - t1
print "-" * 20
print "Shutting down..."
server.close()
os.remove( "/tmp/socket" )
print "Done"




