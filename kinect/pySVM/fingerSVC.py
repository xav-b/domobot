from sklearn import datasets
import pickle

XTestTmp, YTest = datasets.load_svmlight_file("./pySVM/SVMTest.txt")
XTest = XTestTmp.toarray()

''' Loading model '''
fd = open('./pySVM/SVCmodel.data', 'rb')
clf = pickle.load(fd)
fd.close()

''' Testing now new data '''
fd = open('./pySVM/predict.data', 'w')
for prediction in clf.predict(XTest):
    fd.write(str(prediction) + '\n')
fd.close
