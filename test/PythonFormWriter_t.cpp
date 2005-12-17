#include <fstream>
#include <istream>
#include <iostream>
#include <string>

#include "boost/shared_ptr.hpp"

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/src/PythonFormWriter.h"

using namespace edm;
using namespace edm::pset;

void writePythonForm(std::string const& config, std::ostream& out)
{
  try
    {
      PythonFormWriter writer;
      boost::shared_ptr<edm::pset::NodePtrList> parsetree = edm::pset::parse(config.c_str());
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

  // To really run a test, call with 1 argument, specifying the name
  // of the configuration file to read.
  std::ifstream configFile(argv[1]);
  if (!configFile)
    {
      std::cerr << argv[0] << " could not open input file " << argv[1] << '\n';
      return 1;
    }

  std::string configstring, line;
  while (std::getline(configFile, line))
    {
      configstring += line;
      configstring += '\n';
    }
  
  // Now parse this configuration string, writing the Python format to
  // standard out.

  int rc = 1;
  try  
    { 
      writePythonForm(configstring, std::cout);
      rc = 0; // success
    }
  catch ( ... )
    {
      std::cerr << "writePythonForm failed\n";
    }
  return rc;  
}
