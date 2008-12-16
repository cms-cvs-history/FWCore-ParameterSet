#include <iostream>
#include <string>
#include <vector>

#include "boost/shared_ptr.hpp"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h" 

int main(int argc, char **argv) {
  // config can either be a name or a string
  std::string config;

  if(argc == 1) {
    // Read input from cin into configstring..
    {
      std::string line;
      while (std::getline(std::cin, line)) {
    	config += line;
    	config += '\n';
      }
    }

  } else if (argc == 2) {
    config = std::string(argv[1]);
  }

  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfig(config);

  std::cout << processDesc->getProcessPSet()->id() << std::endl;
  return 0;
}
