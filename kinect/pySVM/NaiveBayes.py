#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
=========================================================
SVM-SVC (Support Vector Classification)
=========================================================
The classification application of the SVM is used below on Kinect based
The decision boundaries, are shown with all the points in the training-set.
Tuto: http://scikit-learn.org/0.11/tutorial/basic/tutorial.html
"""
print __doc__


import sys
import numpy as np
import pylab as pl
from sklearn import datasets, metrics, cross_validation
from sklearn.naive_bayes import GaussianNB
from sklearn.naive_bayes import MultinomialNB
from sklearn.externals import joblib

def main():
    XTmp, Y = datasets.load_svmlight_file("./SVMData.txt")
    X = XTmp.toarray()
    XTestTmp, YTest = datasets.load_svmlight_file("./SVMTest.txt")
    XTest = XTestTmp.toarray()

    #clf = MultinomialNB(alpha=1).fit(X, Y)    #perfs pouraves
    clf = GaussianNB().fit(X, Y)
    predicted = clf.predict(XTest)
    print "Number of mislabeled points : %d" % (YTest != predicted).sum()
    print 'predicted:', predicted
    print metrics.classification_report(
                 YTest, predicted)
    print metrics.confusion_matrix(YTest, predicted)

    ''' Some model validation tests '''
    ''' Nested cross validation, 3 samples with best gamma '''
    scores = cross_validation.cross_val_score(clf, X, Y)
    print 'Optimized fitting scores:', scores
    print 'Raw test: ', clf.score(X, Y)

    ''' Saving model '''
    joblib.dump(clf, 'SVCmodel.data')

if __name__ == '__main__':
    main()
