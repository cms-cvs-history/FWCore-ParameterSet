#include <ostream>
#include <iostream>
#include <string>
#include <vector>

#include "boost/shared_ptr.hpp"

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/src/ParseResultsTweaker.h"
#include "FWCore/ParameterSet/src/PythonFormWriter.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"

using namespace edm;
using namespace edm::pset;

void
writePythonForm(std::string const& config, std::ostream& out)
{
  
  PythonFormWriter writer;

  std::string preprocessedConfigString;
  std::vector<std::string> openFileStack;
  edm::pset::preprocessConfigString(config, 
				    preprocessedConfigString,
				    openFileStack);

  boost::shared_ptr<edm::pset::NodePtrList> parsetree = 
    edm::pset::parse(preprocessedConfigString.c_str());

  ParseResultsTweaker tweaker;
  tweaker.process(parsetree);

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
