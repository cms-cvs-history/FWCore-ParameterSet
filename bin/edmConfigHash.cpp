#include <iostream>
#include <string>
#include <vector>

#include "boost/shared_ptr.hpp"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h" 

using namespace edm;
using namespace edm::pset;

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

  // Create the ParameterSets, and report the ParameterSetID of the
  // main ParameterSet.

  int rc = 1;  // failure
  try  
    { 
      boost::shared_ptr<edm::ParameterSet> main;
      boost::shared_ptr<std::vector<edm::ParameterSet> > serviceparams;
      edm::makeParameterSets(configstring, main, serviceparams);
      std::cout << main->id() << std::endl;
      rc = 0;
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
