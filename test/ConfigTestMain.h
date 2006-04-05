#include <iostream>
#include <string>

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/test/ConfigFileTest.h"

int main(int argc, char* argv[])
{
  // Because 'scramv1 build runtests' will run this test, and because
  // we don't know how to pass this executable an argument, we make it
  // pass immediately if no argument is supplied.
  if ( argc == 1 ) return 0;

  // Do the real testing.
  // We require 1 argument: the name of the file we are to open.
  if (argc != 2) return -1;

  int rc = 0;
  try 
    {
      ConfigFileTest * test = ConfigFileTest::instance();
      rc = test->run(argv[1]);
    }
  catch ( edm::Exception const& x )
    {
      rc = 1;
      std::cerr << "Exception:\n" << x << '\n';
    }
  catch (...)
    {
      rc = 2;
      std::cerr << "Unknown exception\n";
    }
  return rc;
}
