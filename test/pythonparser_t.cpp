#include <iostream>
#include <string>

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/src/PythonModule.h"
#include "FWCore/ParameterSet/interface/PythonParameterSet.h"
#include <boost/python.hpp>

//------------------------------------------------------------
// Test program for parameter set specification and configuration file
// parser.
//------------------------------------------------------------

using namespace std;
using namespace boost::python;

int work(int argc, char* argv[])
{
  // Because 'scramv1 build runtests' will run this test, and because
  // we don't know how to pass this executable an argument, we make it
  // pass immediately if no argument is supplied.
  if ( argc == 1 ) return 0;

  // If given an argument, that argument must be the name of a file to read.
  Py_Initialize();
  PyImport_AppendInittab( "libFWCoreParameterSet", &initlibFWCoreParameterSet );


  object main_module((
    handle<>(borrowed(PyImport_AddModule("__main__")))));

  object main_namespace = main_module.attr("__dict__");

  // load the library
  object libModule( (handle<>(PyImport_ImportModule("libFWCoreParameterSet"))) );
  // put it in the main namespace
  main_namespace["libFWCoreParameterSet"] = libModule;

  // make an instance in python-land
  PythonParameterSet pythonParameterSet;
  scope(libModule).attr("pythonParameterSet") = ptr(&pythonParameterSet);

  try {
      std::string initCommand("import FWCore.ParameterSet.Config as cms\n"
                          "fileDict = dict()\n"
                          "execfile('");
      std::string fileName(argv[1]);
      initCommand += fileName + "',fileDict)";


      handle<>(PyRun_String(initCommand.c_str(),
                            Py_file_input,
                            main_namespace.ptr(),
                            main_namespace.ptr()));
      std::string command("cms.findProcess(fileDict).dumpConfig()");
      handle<>(PyRun_String(command.c_str(),
                            Py_eval_input,
                            main_namespace.ptr(),
                            main_namespace.ptr()));

  }catch(...) {
    Py_Finalize();
    throw;
  }
  Py_Finalize();

  return 0;
}

int main(int argc, char* argv[])
{
  int rc = 0;
  try 
    {
      rc = work(argc, argv);
    }
  catch ( edm::Exception const& x )
    {
      rc = 1;
      cerr << "Exception: " << x << '\n';
    }
  catch (...)
    {
      rc = 2;
      cerr << "Unknown exception\n";
    }
  return rc;
}
