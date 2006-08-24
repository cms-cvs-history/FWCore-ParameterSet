#include <ostream>
#include <iostream>
#include <string>
#include <vector>

#include "boost/shared_ptr.hpp"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/src/PythonFormWriter.h"
//#include "FWCore/ParameterSet/interface/MakeParameterSets.h"

using namespace edm;
using namespace edm::pset;
using namespace std;

int main(int argc, char * argv[])
{
  if(argc != 3)
  {
    std::cout << "Usage: EdmConfigSearch <cfgfile> <searchstring>" << std::endl;
    return 1;
  }

  // Now parse this configuration string, writing the Python format to
  // standard out.

  int rc = 1;  // failure
  try  
  { 
    string fileName = argv[1];
    string searchString = argv[2];
    string configString;
    read_whole_file(fileName, configString);
    edm::pset::ParseResults parseTree = fullParse(configString);

    // top node should be process Node
    ostringstream result;
    assert(parseTree->size() == 1);
    parseTree->front()->locate(searchString, result);
    std::cout << result.str() << std::endl;
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
