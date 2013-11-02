/*  multithread_python_calling.cpp
 *
 *  Fri Nov  1 18:22:31 EDT 2013
 *
 *  Warning! This program does not work properly!
 *
 *  I keep it just for sampling of a boggus case
 *
 *  In this sample I am tryng to call python from multiple threads
 *
 *  There appears to be a bug that is causing me to quit this approach
 *
 *  What is happening is the following:
 *
 *
 *  (1) When Py_BEGIN_ALLOW_THREADS and Py_END_ALLOW_THREADS are uncommented
 *      then the program aborts and core dumps
 *
 *  (2) If I comment out BEGIN END THREADS then it deadlocks PyGILState_Ensure()
 *
 * */


#include <Python.h>
#include <iostream>
#include <vector>
#include "ParallelProcessor.h"
#include <mutex>

using namespace std;

enum { NUMBER_OF_STANDARD_PARAMETERS = 8 };

mutex mu;



PyObject * import_name(const char * modname, const char * symbol)
{
    PyObject * u_name, *module;
    u_name = PyUnicode_FromString(modname);
    module = PyImport_Import(u_name);
    Py_DECREF(u_name);
    return PyObject_GetAttrString(module, symbol);
}


class PythonModule
{
        PyObject * _process_tick_function;

        int call_func(int year, int month, int day,
                      int hour, int minute, int second,
                      double bid, double ask,
                      std::vector<double>& optimizable_parameters)
        {
            unique_lock<mutex> locker(mu);
            PyObject *args ;
            PyObject *kwargs = NULL;
            PyObject *result = 0;
            vector<PyObject *> variables;
            int retval;
            PyGILState_STATE state = PyGILState_Ensure();
            variables.push_back(Py_BuildValue("i", year));
            variables.push_back(Py_BuildValue("i", month));
            variables.push_back(Py_BuildValue("i", day));
            variables.push_back(Py_BuildValue("i", hour));
            variables.push_back(Py_BuildValue("i", minute));
            variables.push_back(Py_BuildValue("i", second));
            variables.push_back(Py_BuildValue("d", bid));
            variables.push_back(Py_BuildValue("d", ask));

        for(auto d: optimizable_parameters)
            {
                variables.push_back(Py_BuildValue("d", d));
            }

            args = PyTuple_New(8 + optimizable_parameters.size());

            for(int i = 0; i < variables.size(); ++i)
            {
                PyTuple_SetItem(args, i, variables[i]);
            }

            result = PyObject_Call(_process_tick_function, args, kwargs);
            Py_DECREF(args);

            /* Check for Python exceptions (if any) */
            if(PyErr_Occurred())
            {
                PyErr_Print();
                goto fail;
            }

            if(!PyInt_Check(result))
            {
                fprintf(stderr, "call_func: callable didn't return an int\n");
                goto fail;
            }

            retval = PyInt_AsLong(result);
            Py_DECREF(result);
            PyGILState_Release(state);
            return retval;
        fail:
            Py_XDECREF(result);
            PyGILState_Release(state);
            abort();
        }


    public:


        PythonModule(PyObject * process_tick_function):
            _process_tick_function(process_tick_function)

        {
        }


        ~PythonModule()
        {
            Py_DECREF(_process_tick_function);
        }

        int get_signal(int year, int month, int day, int hour, int minute, int second,
                       double bid, double ask, vector<double>& optimazable_parameters)
        {
            int rv;
            Py_BEGIN_ALLOW_THREADS
            rv = call_func(year, month, day, hour, minute, second, bid, ask, optimazable_parameters) ;
            Py_END_ALLOW_THREADS
            return rv;
        }

};


class MultiThreadedTester
{
        PyObject* _process_tick_function;
        int _count_optimizable_parameters;
    public:

        MultiThreadedTester(PyObject* process_tick_function, int count_optimizable_parameters):
            _process_tick_function(process_tick_function),
            _count_optimizable_parameters(count_optimizable_parameters)
        {
        }

        void test()
        {
            int count = 100;
            double bid = 10.1, ask = 10.1, triggering_delta = 10.1, minute_to_trade = 1, expiration_minute = 1;
            PythonModule pm(_process_tick_function);

            for(int i = 0; i < count; ++i)
            {
                vector<double> optimazable_parameters;

                for(int j = 0; j < _count_optimizable_parameters; ++j)
                {
                    optimazable_parameters.push_back(1012.1);
                }

                int year = 1, month, day = 1, hour = 1,  minute = 1, second = 1;
                double bid = 100.1, ask = 100.2;
                int s = pm.get_signal(year, month, day, hour, minute, second, bid, ask, optimazable_parameters) ;
            }
        }
};


int count_number_of_parameters(PyObject* process_tick_function)
{
    int size;
    PyObject *pName, *pInspect, *argspec_tuple, *arglist;
    pName = PyString_FromString("inspect");

    if(pName)
    {
        pInspect = PyImport_Import(pName);
        Py_DECREF(pName);

        if(pInspect)
        {
            pName = PyString_FromString("getargspec");

            if(pName)
            {
                argspec_tuple = PyObject_CallMethodObjArgs(pInspect, pName, process_tick_function, NULL);
                Py_DECREF(pName);

                if(argspec_tuple)
                {
                    arglist = PyTuple_GetItem(argspec_tuple, 0);

                    if(arglist)
                    {
                        size = PyObject_Size(arglist)
                               + (PyTuple_GetItem(argspec_tuple, 1) == Py_None ? 0 : 1)
                               + (PyTuple_GetItem(argspec_tuple, 2) == Py_None ? 0 : 1);  // Haven't actually tested this, but it should work
                    }
                }
            }
        }
    }

    return size;
}

int main()
{
    Py_Initialize();
    PyEval_InitThreads() ;
    //PyEval_SaveThread();
    PyGILState_STATE state = PyGILState_Ensure();
    PyObject* process_tick_function = import_name("process_tick_sample", "process_tick");
    int params_count = count_number_of_parameters(process_tick_function) - NUMBER_OF_STANDARD_PARAMETERS;

    if(!PyCallable_Check(process_tick_function))
    {
        throw "expected a callable object!";
    }

    int count_optimizable_parameters = count_number_of_parameters(process_tick_function) - NUMBER_OF_STANDARD_PARAMETERS;
    PyGILState_Release(state);
    vector<MultiThreadedTester> testers;
    testers.push_back(MultiThreadedTester(process_tick_function, params_count));
    testers.push_back(MultiThreadedTester(process_tick_function, params_count));
    testers.push_back(MultiThreadedTester(process_tick_function, params_count));
    /// testers.push_back(MultiThreadedTester(process_tick_function,params_count));
    ParallelProcessor<MultiThreadedTester> pp(&MultiThreadedTester::test, 12);

    for(int i = 0; i < testers.size(); ++i)
    {
        pp.add(&testers[i]);
    }

    pp.go();
    Py_Finalize();
    return -1;
}


