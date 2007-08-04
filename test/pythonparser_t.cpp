#include <iostream>
#include <string>

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/PythonProcessDesc.h"
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

  PythonProcessDesc processDesc(argv[1]);
  std::cout << processDesc.dump() << std::endl;
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
