#include <Python.h>
#include <iostream>
#include <vector>
#include "ParallelProcessor.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
//
// Author        : John Pazarzis
// Creation date : Fri Nov  1 21:52:42 EDT 2013
//
//////////////////////////////////////////////////////////////////////////////////
//
// This class is part of the Python - C++ interaction that allows the user to 
// descibe his models in Python and optimize them using the C++ platform that 
// is used for any other C++ based model.
//
// DESCRIPTION
// ============
//
// Calls a Python function implementing the signal decision. This function will
// always receive the standard parameters needs (time, bid and ask) and in top
// of them any other variable that is used for the implementation of the logic.
//
// These additional variables will be optimized by subsequent runs until we
// reach an acceptable level of the fitness function.
//
// Upon construction the user provides the module and the function of the
// Python code (always without the extension .py). The number of the additional
// needed variables (those to be optimized) will be detected automatically and
// their number will be made available to the called so he can later call the
// function providing the standard variables as individual parameters and the
// user defined as a vector of doubles. Note that all user defined parameters
// are treated as of double type.
//
// It is very important that your client code calls Py_Initialize() and Py_Finalize()
// in the begining and the end of the program othwerwise the Python-C API will throw 
// exceptions!
//
// The return value
// ================
//
// The return value of the signal function should follow the following
// conventions:
//
//
//      1  : for BUY
//      0  : DO NOTHING
//     -1  : SELL
//
// About the python code
// =====================
//
// (1) It needs to contain a function with the name process_tick with the following 
// signature:
//
// def process_tick(year,month,day, hour, minute,second, bid, ask, ....):
//  
// (2) After the ask variable the user can add any variable he needs to implement
// his logic.  These are the variables that will be later optimized
//
// (3) As descibed above the return value of this fuction can take the values 1, 0, -1
//
//
// COMPILATION
// ============
//
// To compile add the following to the g++ call:
//
// -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython2.7
//
///////////////////////////////////////////////////////////////////////////////

class PythonModelSignalCaller
{
        std::string _module_name;
        PyObject * _process_tick_function;
        int _count_optimizable_parameters;

        PyObject * import_name(const char * modname, const char * symbol)
        {
            PyObject * u_name, *module;
            u_name = PyUnicode_FromString(modname);
            module = PyImport_Import(u_name);
            Py_DECREF(u_name);
            return PyObject_GetAttrString(module, symbol);
        }

        int call_func(int year, int month, int day,
                      int hour, int minute, int second,
                      double bid, double ask,
                      std::vector<double>& optimizable_parameters)
        {
            PyObject *args ;
            PyObject *kwargs = NULL;
            PyObject *result = 0;
            vector<PyObject *> variables = 
            {
                    Py_BuildValue("i", year),
                    Py_BuildValue("i", month),
                    Py_BuildValue("i", day),
                    Py_BuildValue("i", hour),
                    Py_BuildValue("i", minute),
                    Py_BuildValue("i", second),
                    Py_BuildValue("d", bid),
                    Py_BuildValue("d", ask)
            };

            int retval;
            PyGILState_STATE state = PyGILState_Ensure();

            for(auto d : optimizable_parameters)
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

            if(PyErr_Occurred())
            {
                PyErr_Print();
                goto fail;
            }

            if(!PyInt_Check(result))
            {
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

        int count_number_of_parameters()
        {
            PyObject *pName, *pInspect, *argspec_tuple, *arglist;
            int size;
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
                        argspec_tuple = PyObject_CallMethodObjArgs(pInspect, pName, _process_tick_function, NULL);
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

    public:
        enum { NUMBER_OF_STANDARD_PARAMETERS = 8 };

        PythonModelSignalCaller(const std::string& name):
            _module_name(name), _count_optimizable_parameters(-1)

        {
            _process_tick_function = import_name(_module_name.c_str(), "process_tick");

            if(!PyCallable_Check(_process_tick_function))
            {
                throw "expected a callable object!";
            }

            _count_optimizable_parameters = count_number_of_parameters() - NUMBER_OF_STANDARD_PARAMETERS;
        }

        int count_optimizable_parameters() const
        {
            return _count_optimizable_parameters;
        }

        ~PythonModelSignalCaller()
        {
            Py_DECREF(_process_tick_function);
        }

        int get_signal(int year, int month, int day, int hour, int minute, int second,
                       double bid, double ask, vector<double>& optimazable_parameters)
        {
            return call_func(year, month, day, hour, minute, second, bid, ask, optimazable_parameters) ;
        }
};


;
void test(int count = 1)
{
    double bid = 10.1, ask = 10.1, triggering_delta = 10.1, minute_to_trade = 1, expiration_minute = 1;
    PythonModelSignalCaller pm("process_tick_sample");

    for(int i = 0; i < count; ++i)
    {
        vector<double> optimazable_parameters;
        int year = 1, month, day = 1, hour = 1,  minute = 1, second = 1;
        double bid = 100.1, ask = 100.2;

        for(int j = 0; j < pm.count_optimizable_parameters(); ++j)
        {
            optimazable_parameters.push_back(1012.1);
        }

        int s = pm.get_signal(year, month, day, hour, minute, second, bid, ask, optimazable_parameters) ;
    }
}


int main()
{
    int year = 1, month, day = 1, hour = 1,  minute = 1, second = 1;
    double bid = 10.1, ask = 10.1, triggering_delta = 10.1, minute_to_trade = 1, expiration_minute = 1;
    Py_Initialize();
    cout << "start" << endl;

    for(int i = 0; i < 200; ++i)
    {
        test(100);
        cout << i << endl;
    }

    cout << "finish" << endl;
    Py_Finalize();
    return 0;
}


