#include <Python.h>

bool runPyScript(char *fn) {
    Py_Initialize();
    PyObject *file = PyFile_FromString(fn, "r");
    if( file == NULL )
        return false;
    PyRun_SimpleFile(PyFile_AsFile(fichier), fn);
    Py_DECREF(fichier);
    Py_Finalize();
    return true;
}

int main() {
    runPyScript("test.py");

    return 0;
}
