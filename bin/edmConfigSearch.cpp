#include <ostream>
#include <iostream>
#include <string>
#include <vector>

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/interface/ParseTree.h"
#include "FWCore/ParameterSet/src/PythonFormWriter.h"
//#include "FWCore/ParameterSet/interface/MakeParameterSets.h"

using namespace edm;
using namespace edm::pset;
int main(int argc, char * argv[])
{
  if(argc < 3)
  {
    std::cout << "Usage: EdmConfigSearch <searchstring> <cfgfile>" << std::endl;
    return 1;
  }

  // Now parse this configuration string, writing the Python format to
  // standard out.

  int rc = 1;  // failure
  try  
  { 
    std::string searchString = argv[1];
    int nfiles = argc-2;
    for(int ifile = 0; ifile < nfiles; ++ifile)
    {
      std::string fileName = argv[ifile+2];
      edm::pset::ParseTree parseTree(read_whole_file(fileName));

      std::ostringstream result;
      parseTree.top()->locate(searchString, result);
      if(!result.str().empty())
      {
        std::cout << "======== " << fileName << std::endl;
        std::cout << result.str() << std::endl;
      }
    } // loop over files

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
