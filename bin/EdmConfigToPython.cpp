#include <ostream>
#include <iostream>
#include <string>

#include "boost/shared_ptr.hpp"

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/src/PythonFormWriter.h"

using namespace edm;
using namespace edm::pset;

void
writePythonForm(std::string const& config, std::ostream& out)
{
  
  PythonFormWriter writer;
  boost::shared_ptr<edm::pset::NodePtrList> parsetree = 
    edm::pset::parse(config.c_str());

  writer.write(parsetree, out);
}


int main()
{
  // Read input from cin into configstring..
  std::string configstring;
  {
    std::string line;
    while (std::getline(std::cin, line))
      {
	configstring += line;
	configstring += '\n';
      }
  }

  // Now parse this configuration string, writing the Python format to
  // standard out.

  int rc = 1;  // failure
  try  
    { 
      writePythonForm(configstring, std::cout);
      rc = 0; // success
    }
  catch ( edm::Exception const& x )
    {
      std::cerr << x << '\n';
    }
  catch ( ... )
    {
      std::cerr << "Unidentified exception caught\n";	
    }
  return rc;  
}
