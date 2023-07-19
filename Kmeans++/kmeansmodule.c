# define PY_SSIZE_T_CLEAN
# include <Python.h>       /* MUST include <Python.h>, this implies inclusion of the following standard headers:                        <stdio.h>, <string.h>, <errno.h>, <limits.h>, <assert.h> and <stdlib.h> (if available). */
# include <math.h>         /* include <Python.h> has to be before any standard headers are included */
# include "kmeansh.h"

static PyObject* fit(PyObject *self, PyObject *args)
{
    PyObject *py_datapoints, *py_centroids, *py_point, *new_py_centroids;
    double num, eps;
    int dimension, iter, k, N, i, j;

    if (!PyArg_ParseTuple(args, "O|O|i|i|i|i|d", &py_datapoints, &py_centroids, &N, &k, &dimension, &iter, &eps)) {
        return NULL;
    }

    //check if the lists are indeed lists and check that the parameters make sense
    if (!PyList_Check(py_centroids) || !PyList_Check(py_datapoints) || N < 0 || k < 0 || dimension < 0){
        return NULL;
    }

    //parse the py_datapoints list 
    double **datapoints = malloc(N * sizeof(double *));
    for (i = 0; i < N; i++) {
        py_point = PyList_GetItem(py_datapoints, i);
        datapoints[i] = malloc(dimension * sizeof(double));
        for (j = 0; j < dimension; j++){
            num = PyFloat_AsDouble(PyList_GetItem(py_point, j));
            datapoints[i][j] = num;
        }
    }

    //parse the py_centroids list
    double **centroids = malloc(k * sizeof(double *));
    for (i = 0; i < k; i++) {
        py_point = PyList_GetItem(py_centroids, i);
        centroids[i] = malloc(dimension * sizeof(double));
        for (j = 0; j < dimension; j++){
            num = PyFloat_AsDouble(PyList_GetItem(py_point, j));
            centroids[i][j] = num;
        }
    }
    
    //calling kmeans algorithm - this frees datapoints and manipulates centroids
    kmeans(datapoints, centroids, N, k, dimension, iter, eps);

    //building a PyList - new_py_centroids, after changing the centroids information in kmeans
    new_py_centroids = PyList_New(k);
    for (i = 0; i < k; i++) {
        py_point = PyList_New(dimension);
        for (j = 0; j < dimension; j++){
            PyList_SetItem(py_point, j, PyFloat_FromDouble(centroids[i][j]));
        }
        PyList_SetItem(new_py_centroids, i, py_point);
    }

    /* free used memory */
    for (i = 0; i< k;i++) 
    {
        free(centroids[i]); /* free each centroid dimensions array*/
    }
    free(centroids);/*free centorid array*/

    return new_py_centroids;
}


static PyMethodDef kmeansMethods[] = {
    {"fit",                   
      fit,
      METH_VARARGS,         
      PyDoc_STR("fit(py_datapoints, py_centroids, N, k, dimension, iter, eps)\npy_datapoints - a python list of datapoints\n"
                "py_centroids - a python list of cluster centroids\nN - the number of datapoints (int)\nk - the number of clusters (int)\n"
                "dimension - the dimension of each datapoint (int)\niter - the number of iterations (int)\neps - the epsilon of convergence (float)")}, 
    {NULL, NULL, 0, NULL}     
};


static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", 
    NULL, 
    -1,  
    kmeansMethods 
};

PyMODINIT_FUNC
PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}