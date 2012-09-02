from sklearn.externals import joblib
from sklearn import datasets
import cPickle as pickle
#import io
import time

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

''' Testing now new data '''
fd = open('./pySVM/predict.data', 'w')
for prediction in clf.predict(XTest):
    fd.write(str(prediction) + '\n')
    print '[CHRONO] Each prediction:', time.time() - t1
fd.close
print '[CHRONO] End:', time.time() - t1
