#include <iostream>
#include <string>

#include "boost/shared_ptr.hpp"

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/interface/ParseTree.h"
#include "FWCore/ParameterSet/src/PythonFormWriter.h"

using namespace edm;
using namespace edm::pset;

void writePythonForm(std::string const& filename, std::ostream& out)
{
  try
    {
      edm::pset::ParseTree parsetree(edm::pset::read_whole_file(filename));
      PythonFormWriter writer;
      writer.write(parsetree, out);
    }
  catch ( edm::Exception& x )
    {
      std::cerr << "Caught an edm::Exception: " << x << '\n';
      abort();
    }
  catch ( ... )
    {
      std::cerr << "Caught an unknown exception\n";
      abort();
    }
}


int main(int argc, char* argv[])
{
  // If called with no argument, just return. This is just protection
  // against scram calling us with no argument.
  if (argc == 1) return 0;

  std::string filename(argv[1]);

  // Now parse this configuration string, writing the Python format to
  // standard out.

  int rc = 1;
  try  
    { 
      writePythonForm(filename, std::cout);
      rc = 0; // success
    }
  catch ( ... )
    {
      std::cerr << "writePythonForm failed\n";
    }
  return rc;  
}
