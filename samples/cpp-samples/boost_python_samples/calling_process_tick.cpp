#include <Python.h>
#include <iostream>
#include <vector>
#include "ParallelProcessor.h"
using namespace std;




/* Load a symbol from a module */

void get_dir( PyObject * pyobj )
{
    PyObject * pydir = PyObject_Dir( pyobj );
    int size = PyList_Size( pydir );

    for ( int i = 0; i < size; ++i ) {
        PyObject * list_item =  PyList_GetItem( pydir, i );
        std::string attr_name( PyString_AsString( list_item ) );

        if ( attr_name[0] != '_' ) {
            cout << attr_name << endl;
        }

        Py_DECREF( list_item );
    }

    Py_DECREF( pydir );
}

class PythonModule
{
        std::string _module_name;
        PyObject * _process_tick_function;
        int _count_optimizable_parameters;

        PyObject * import_name( const char * modname, const char * symbol )
        {
            PyObject * u_name, *module;
            u_name = PyUnicode_FromString( modname );
            module = PyImport_Import( u_name );
            Py_DECREF( u_name );
            return PyObject_GetAttrString( module, symbol );
        }

        int call_func(  int year, int month,int day,
                  int hour, int minute,int second,
                  double bid, double ask,
                  std::vector<double>& optimizable_parameters )
        {
            PyObject *args ;
            PyObject *kwargs = NULL;
            PyObject *result = 0;

            vector<PyObject *> variables;

            int retval;

            PyGILState_STATE state = PyGILState_Ensure();

            

            variables.push_back( Py_BuildValue( "i", year ) );
            variables.push_back( Py_BuildValue( "i", month ) );
            variables.push_back( Py_BuildValue( "i", day ) );
            variables.push_back( Py_BuildValue( "i", hour ) );
            variables.push_back( Py_BuildValue( "i", minute ) );
            variables.push_back( Py_BuildValue( "i", second ) );
            variables.push_back( Py_BuildValue( "d", bid ) );
            variables.push_back( Py_BuildValue( "d", ask ) );

            for ( auto d: optimizable_parameters ) {
                variables.push_back( Py_BuildValue( "d", d ) );
            }

            args = PyTuple_New( 8 + optimizable_parameters.size() );

            for ( int i = 0; i < variables.size(); ++i ) {
                PyTuple_SetItem( args, i, variables[i] );
            }

            result = PyObject_Call(_process_tick_function, args, kwargs );

            Py_DECREF( args );

            /* Check for Python exceptions (if any) */
            if ( PyErr_Occurred() ) {
                PyErr_Print();
                goto fail;
            }

            if ( !PyInt_Check( result ) ) {
                fprintf( stderr,"call_func: callable didn't return an int\n" );
                goto fail;
            }

            retval = PyInt_AsLong( result );
            Py_DECREF( result );

            PyGILState_Release( state );
            return retval;

        fail:
            Py_XDECREF( result );
            PyGILState_Release( state );
            abort();
        }

        int count_number_of_parameters()
        {
                PyObject *pName, *pInspect, *argspec_tuple, *arglist;
                int size;

                pName = PyString_FromString("inspect");

                if (pName)
                {
                    pInspect = PyImport_Import(pName);
                    Py_DECREF(pName);


                    if (pInspect)
                    {
                        pName = PyString_FromString("getargspec");

                        if (pName)
                        {
                            argspec_tuple = PyObject_CallMethodObjArgs(pInspect, pName, _process_tick_function, NULL);
                            Py_DECREF(pName);

                            if (argspec_tuple)
                            {
                                arglist = PyTuple_GetItem(argspec_tuple, 0);

                                if (arglist)
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

        PythonModule( const std::string & name ):
            _module_name( name ), _count_optimizable_parameters(-1) 
            
        {
            Py_Initialize();
            _process_tick_function = import_name( _module_name.c_str(),"process_tick" );
            if ( !PyCallable_Check( _process_tick_function ) ) {
                throw "expected a callable object!";
            }
            _count_optimizable_parameters = count_number_of_parameters() - NUMBER_OF_STANDARD_PARAMETERS;
            
            
        }

        int count_optimizable_parameters() const 
        { 
            return _count_optimizable_parameters;
        }

        ~PythonModule() {
            Py_DECREF( _process_tick_function );
            Py_Finalize();
        }

        int get_signal(int year, int month, int day, int hour, int minute,int second,
                       double bid, double ask, vector<double>& optimazable_parameters)
        {
            
            int rv; 
            Py_BEGIN_ALLOW_THREADS
            rv = call_func(year,month,day, hour, minute,second, bid, ask, optimazable_parameters ) ;
            Py_END_ALLOW_THREADS
            return rv;
        }

        int get_global_int( const std::string & name ) {
            PyObject * info = import_name( _module_name.c_str(),name.c_str() );
            int c = PyLong_AsLong( info );
            Py_DECREF( info );
            return c;
        }
};


class MultiThreadedTester
{
    public:
        void test()
        {
            int count =1;
            double bid=10.1, ask=10.1, triggering_delta=10.1, minute_to_trade=1, expiration_minute=1;
            PythonModule pm("process_tick_sample");
            for(int i = 0; i < count; ++i)
            {
                vector<double> optimazable_parameters;
                int year=1,month,day=1, hour=1,  minute=1,second=1;
                double bid = 100.1, ask = 100.2;

                for(int j = 0; j < pm.count_optimizable_parameters(); ++j)
                {
                    optimazable_parameters.push_back(1012.1);
                }

                int s = pm.get_signal(year,month,day, hour, minute,second, bid, ask,optimazable_parameters) ;
            }

        }
};
void test(int count =1)
{
    double bid=10.1, ask=10.1, triggering_delta=10.1, minute_to_trade=1, expiration_minute=1;
    PythonModule pm("process_tick_sample");
    for(int i = 0; i < count; ++i)
    {
        vector<double> optimazable_parameters;
        int year=1,month,day=1, hour=1,  minute=1,second=1;
        double bid = 100.1, ask = 100.2;

        for(int j = 0; j < pm.count_optimizable_parameters(); ++j)
        {
            optimazable_parameters.push_back(1012.1);
        }

        int s = pm.get_signal(year,month,day, hour, minute,second, bid, ask,optimazable_parameters) ;
    }
}


int main()
{


    vector<MultiThreadedTester> testers;
    testers.push_back(MultiThreadedTester());
    testers.push_back(MultiThreadedTester());

    Py_Initialize();
    PyEval_InitThreads() ;
    ParallelProcessor<MultiThreadedTester> pp(&MultiThreadedTester::test,1);

   for(int i = 0; i < testers.size(); ++i){
            pp.add(&testers[i]);
        }

     pp.go();
     Py_Finalize();

     return -1;


    int year=1,month,day=1, hour=1,  minute=1,second=1;
    double bid=10.1, ask=10.1, triggering_delta=10.1, minute_to_trade=1, expiration_minute=1;
    Py_Initialize();
    {
        // has to leave within a scope..
        PythonModule pm("process_tick_sample");
    }

    cout << "start" << endl;
    for(int i = 0; i < 200; ++i)
    {
        test(1);
        //cout << i << endl;    

    }
    cout << "finish" << endl;    
    Py_Finalize();
    return 0;
}


