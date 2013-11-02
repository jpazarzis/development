#include <Python.h>
#include <iostream>
#include <vector>

using namespace std;

/* Execute func(x,y) in the Python interpreter. The
arguments and return result of the function must
be Python floats */




double call_func(PyObject * func, int year, int month, int day,
                 int hour, int minute, int second,
                 double bid, double ask,
                 std::vector<double>& optimizable_parameters)

{
    PyObject * args ;
    PyObject * kwargs = NULL;
    PyObject * result = 0;
    vector<PyObject *> variables;
    double retval;
    PyGILState_STATE state = PyGILState_Ensure();

    if(!PyCallable_Check(func))
    {
        fprintf(stderr, "call_func: expected a callable\n");
        goto fail;
    }

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

    result = PyObject_Call(func, args, kwargs);
    Py_XDECREF(kwargs);

for(auto v: variables)
    {
        Py_DECREF(v);
    }

    Py_DECREF(args);

    /* Check for Python exceptions (if any) */
    if(PyErr_Occurred())
    {
        PyErr_Print();
        goto fail;
    }

    /* Verify the result is a float object */
    if(!PyFloat_Check(result))
    {
        fprintf(stderr, "call_func: callable didn't return a float\n");
        goto fail;
    }

    /* Create the return value */
    retval = PyFloat_AsDouble(result);
    Py_DECREF(result);
    /* Restore previous GIL state and return */
    PyGILState_Release(state);
    return retval;
fail:
    Py_XDECREF(result);
    PyGILState_Release(state);
    abort();
}


/* Load a symbol from a module */
PyObject * import_name(const char * modname, const char * symbol)
{
    PyObject * u_name, *module;
    u_name = PyUnicode_FromString(modname);
    module = PyImport_Import(u_name);
    Py_DECREF(u_name);
    return PyObject_GetAttrString(module, symbol);
}

void get_dir(PyObject * pyobj)
{
    PyObject * pydir = PyObject_Dir(pyobj);
    int size = PyList_Size(pydir);

    for(int i = 0; i < size; ++i)
    {
        PyObject * list_item =  PyList_GetItem(pydir, i);
        std::string attr_name(PyString_AsString(list_item));

        if(attr_name[0] != '_')
        {
            cout << attr_name << endl;
        }

        Py_DECREF(list_item);
    }

    Py_DECREF(pydir);
}

class PythonModule
{
        std::string _module_name;

    public:

        PythonModule(const std::string & name):
            _module_name(name)
        {
        }

        int get_global_int(const std::string & name)
        {
            PyObject * info = import_name(_module_name.c_str(), name.c_str());
            int c = PyLong_AsLong(info);
            Py_DECREF(info);
            return c;
        }
};

/* Simple embedding example */
int main()
{
    PyObject * process_tick;
    PyObject * info;
    PyObject * parameters;
    double x;
    Py_Initialize();
    process_tick = import_name("my_functions", "process_tick");
    info = import_name("my_functions", "number_of_parameters");
    int c = PyLong_AsLong(info);
    cout << "pars: " << c << endl;
    auto pm = PythonModule("my_functions");
    int m = pm.get_global_int("number_of_parameters");
    cout << "m:" << m << endl;
    parameters = import_name("my_functions", "parameters");
    Py_ssize_t st = PyTuple_GET_SIZE(parameters);
    cout << "size of params: " << st << endl;
    PyObject * item;

    for(int i = 0; i < st; ++i)
    {
        item = PyTuple_GetItem(parameters, i);
        get_dir(item);
    }

    int year = 1, month, day = 1, hour = 1, minute = 1, second = 1;
    double bid = 10.1, ask = 10.1, triggering_delta = 10.1, minute_to_trade = 1, expiration_minute = 1;
    vector<double> optimazable_parameters = {minute_to_trade, expiration_minute, triggering_delta} ;
    cout << call_func(process_tick, year, month, day, hour, minute, second, bid, ask, optimazable_parameters) << endl;
    /* Done */
    Py_DECREF(process_tick);
    Py_DECREF(info);
    Py_DECREF(parameters);
    Py_Finalize();
    return 0;
}


