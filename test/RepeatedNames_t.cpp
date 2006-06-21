//----------------------------------------------------------------------
//
// This program tests the behavior repeated names in configuration files.
// $Id: UntrackedParameters_t.cpp,v 1.6 2006/03/10 22:37:00 paterno Exp $
//----------------------------------------------------------------------
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "DataFormats/Common/interface/ParameterSetID.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"

using edm::ParameterSetID;
using edm::ParameterSet;
typedef std::vector<edm::ParameterSet> VPSet;


void testModuleNames()
{
  std::string filetext = 
    "process TEST =\n"
    "{\n"
    "source = DummySource { }\n"
    "module m = Thing {}\n"
    "module m = OtherThing {}\n"
    "}\n";
  boost::shared_ptr<ParameterSet> mainps;
  boost::shared_ptr<VPSet > services;
  try
    {
      edm::makeParameterSets(filetext, mainps, services);
      assert( "Failed to throw required exception" == 0 );
    }
  catch ( edm::Exception const& x )
    {
      // this is expected
    }
  catch ( ... )
    {
      assert( "Threw the wrong kind of exception\n" );
    }
}

void testPathNames()
{
  std::string filetext = 
    "process TEST =\n"
    "{\n"
    "source = DummySource { }\n"
    "module a = Thing {}\n"
    "module b = Thing {}\n"
    "path p1 = {  }\n"
    "path p1 = {  }\n"
    "}\n";
  boost::shared_ptr<ParameterSet> mainps;
  boost::shared_ptr<VPSet > services;
  try
    {
      edm::makeParameterSets(filetext, mainps, services);
      assert( "Failed to throw required exception" == 0 );
    }
  catch ( edm::Exception const& x )
    {
      // this is expected
    }
  catch ( ... )
    {
      assert( "Threw the wrong kind of exception\n" );
    }
}

int main()
{
  int rc = 1;
  try
    {
      testModuleNames();
      testPathNames();
      rc = 0;
    }
  catch ( edm::Exception& x)
    {
      std::cerr << "edm::Exception caught\n"
		<< x
		<< '\n';
      rc = -1;
    }
  catch ( ... )
    {
      std::cerr << "Unknown exception caught\n";
      rc = -2;
    }
  return rc;
}
