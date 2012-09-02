#include <Python.h>
//#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <pthread.h>
#include <sys/resource.h>

using namespace std;

typedef struct {
    char* fn;
} threadArgs;

static void* threadFunction(void*);
typedef void* (*threadProcess)(void*);

void* threadFunction(void* parameters) {
    threadArgs* args = (threadArgs*)parameters;
    long status = 0;
    char* fn = (char*) args->fn;
    Py_Initialize();
    PyObject *file = PyFile_FromString(fn, "r");
    if( file == NULL )
        pthread_exit((void*) status);
    PyRun_SimpleFile(PyFile_AsFile(file), fn);
    Py_DECREF(file);
    Py_Finalize();
    pthread_exit((void*) status);
}

int threadRun(threadProcess process, threadArgs* thread_args, int waitFlag) {
    pthread_t thread_id;
    pthread_attr_t attr;
    void* status;
    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    if ( waitFlag )
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    else
        pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
    int rc = pthread_create(&thread_id, &attr, process, (void*) thread_args);
    if ( rc ) {
        //printf("** Error from pthread_create(): %d\n", rc);
        return(-1);
    }
    pthread_attr_destroy(&attr);
    if ( !waitFlag )
        return 0;
    rc = pthread_join(thread_id, &status);
    if ( rc ) {
        //printf("** Error from pthread_create(): %d\n", rc);
        return(-2);
    }
    //printf("[From function] Completed joined thread with status %ld\n", (long)status);
    //return (pthread_t)thread_id;
    return (long)status;
}

int main(int argc, char** argv) {
    //Threaded function parameters
    threadArgs* args;
    args->fn = "ClassifierSVC.py";
    int waitFlag = 1;

    // Running function in a new thread
    int thread_ret(0);
    if ( (thread_ret = threadRun(threadFunction, args, waitFlag)) < 0 ) {
        cout << "** Error from threadRun(): " << thread_ret << endl;;
        exit(-1);
    }

    cout << "[FromMain] Thread returned: " << thread_ret << endl;
    return 0;
}
