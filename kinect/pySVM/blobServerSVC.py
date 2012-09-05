# -*- coding: utf-8 -*-

import socket
import os, os.path
from sklearn.externals import joblib
from sklearn import datasets
import cPickle as pickle
#import io
import time

########## Prepare SVC computation #########################################################

''' Loading model '''
''' Pickle try '''
#fd = open('./pySVM/SVCmodel.data', 'rb')
#fd = io.BytesIO(open("./pySVM/SVCmodel.data", "rb").read())
#clf = pickle.load(fd)
#fd.close()
''' Joblist try '''
clf = joblib.load('./pySVM/SVCmodel.data')

########## ####################### #########################################################

if os.path.exists( "/tmp/socket" ):
  os.remove( "/tmp/socket" )
 
print "Opening socket..."
server = socket.socket( socket.AF_UNIX, socket.SOCK_STREAM )
#server = socket.socket( socket.AF_UNIX, socket.SOCK_DGRAM )
server.bind("/tmp/socket")
server.listen(5)
 
while True:
    print "Listening..."
    client, infos = server.accept()
    datagram = client.recv(1024)
    if not datagram:
        print 'No datagram'
        break
    else:
        print datagram
        if datagram == "done":
            print 'done sent'
            break
        else:
            XTestTmp, YTest = datasets.load_svmlight_file("./pySVM/SVMTest.txt")
            XTest = XTestTmp.toarray()
            ''' Testing now new data '''
            for prediction in clf.predict(XTest):
                client.send(str(prediction))
            client.send(" ")
print "-" * 20
print "Shutting down..."
server.close()
os.remove( "/tmp/socket" )
print "Done"
