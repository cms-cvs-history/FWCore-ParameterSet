// -*- C++ -*-
//
// Package:     ParameterSet
// Class  :     edmParameterSetDump
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Rick Wilkinson
//         Created:  Thu Aug  2 13:33:53 EDT 2007
// $Id: edmParamDesc.cpp,v 1.2 2008/01/18 20:10:27 wmtan Exp $
//

// system include files
#include <iostream>
// user include files
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

int main (int argc, char **argv)
{
  if(argc != 2) {
    std::cout << "Usage: edmParameterSetDump <cfgfile>" << std::endl;
  }
  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName);

  std::cout << processDesc->getProcessPSet()->dump() << std::endl;
}

