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
// $Id: edmParameterSetDump.cpp,v 1.1 2008/03/03 18:00:35 rpw Exp $
//

// system include files
#include <iostream>
// user include files
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/foreach.hpp>

int main (int argc, char **argv)
{
  if(argc != 2) {
    std::cout << "Usage: edmParameterSetDump <cfgfile>" << std::endl;
  }
  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName);
  std::cout << "====Main Process====" << std::endl;
  std::cout << processDesc->getProcessPSet()->dump() << std::endl;
  std::cout << "====Services====" << std::endl;
  BOOST_FOREACH(edm::ParameterSet servicePSet, *(processDesc->getServicesPSets()))
  {
    std::cout << servicePSet.dump() << std::endl;
  }
}

