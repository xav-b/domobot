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
from sklearn import svm, datasets, metrics, cross_validation
from sklearn.grid_search import GridSearchCV

def plotLinearKernels(X, Y, clf):
    h = .02
    x_min, x_max = X[:, 0].min() - .5, X[:, 0].max() + .5
    y_min, y_max = X[:, 1].min() - .5, X[:, 1].max() + .5
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])

    # Put the result into a color plot
    Z = Z.reshape(xx.shape)
    pl.figure(1, figsize=(4, 3))
    pl.pcolormesh(xx, yy, Z, cmap=pl.cm.Paired)

    # Plot also the training points
    pl.scatter(X[:, 0], X[:, 1], c=Y, cmap=pl.cm.Paired)
    pl.xlabel('Sepal length')
    pl.ylabel('Sepal width')

    pl.xlim(xx.min(), xx.max())
    pl.ylim(yy.min(), yy.max())
    pl.xticks(())
    pl.yticks(())

    pl.show()

def main():
    # import some data to play with
    #iris = datasets.load_iris()
    #X = iris.data[:, :2]  # we only take the first two features.
    #Y = iris.target
    XTmp, Y = datasets.load_svmlight_file("./SVMData.txt")
    X = XTmp.toarray()
    XTestTmp, YTest = datasets.load_svmlight_file("./SVMTest.txt")
    XTest = XTestTmp.toarray()


    clf = svm.NuSVC(nu=0.1, kernel='rbf', degree=3, gamma=0.0, coef0=0.0)
    #clf = svm.SVC(kernel='rbf', gamma=1e-4, C=1000)
    #clf = svm.SVC(C=1.0, kernel='linear', probability=True)
    #clf = svm.SVC(gamma=0.1)
    #clf = svm.SVC(kernel='poly', gamma=0.001)

    # we create an instance of SVM Classifier and fit the data.
    print 'Training data and scoring it'
    #probas = clf.fit(X,Y).predict_proba(X)
    ''' Score computed on one sample '''
    #scores = clf.fit(X, Y).score(XTest, YTest)
    ''' Score computed on k samples '''
    #kfold = cross_validation.KFold(len(X), k=3)
    #scores = cross_validation.cross_val_score(clf, X, Y, cv=kfold, n_jobs=-1)
    ''' Method to figure out best parameters '''
    gammas = np.logspace(-6, -1, 10)
    gridClf = GridSearchCV(estimator=clf, param_grid=dict(gamma=gammas), n_jobs=-1)
    gridClf.fit(X, Y)
    print 'Best score:', gridClf.best_score_
    print 'Best gamma estimator:', gridClf.best_estimator_.gamma
    clf = svm.NuSVC(nu=0.1, kernel='poly', degree=3, gamma=gridClf.best_estimator_.gamma, coef0=0.0)
    clf.fit(X,Y)

    ''' Some model validation tests '''
    ''' Nested cross validation, 3 samples with best gamma '''
    scores = cross_validation.cross_val_score(clf, X, Y)
    print 'Optimized fitting scores:', scores
    print 'Raw test: ', clf.score(X, Y)

    ''' Visulazing model '''
    plotLinearKernels(X, Y, clf)

    ''' Saving model '''
    try:
        import cPickle as pickle
    except:
        import pickle
    fd = open('SVCmodel.data', 'wb')
    pickle.dump(clf, fd)
    fd.close()


if __name__ == '__main__':
    main()
