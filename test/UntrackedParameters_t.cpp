//----------------------------------------------------------------------
//
// This program tests the behavior of untracked parameters in
// ParameterSet objects.
// $Id: UntrackedParameters_t.cpp,v 1.3 2006/02/07 22:20:58 paterno Exp $
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


void testUntrackedInternal()
{
  ParameterSet p;
  assert( p.empty() );
  ParameterSetID empty_id = p.id();

  // ParameterSet should not longer be empty after adding an untracked
  // parameter.
  p.addUntrackedParameter<int>("i1", 2112);
  assert( !p.empty());

  // Adding an untracked parameter should not have changed the ID.
  assert( p.id() == empty_id );

  // We should find a parameter named 'i1'
  assert( p.getUntrackedParameter<int>("i1", 10) == 2112 );

  // We should not find one named 'nonesuch'.
  assert( p.getUntrackedParameter<int>("nonesuch", 1) == 1 );

  // If we grab out only the tracked parameters, we should be back to
  // the empty ParameterSet.
  ParameterSet also_empty = p.trackedPart();
  assert( also_empty.getUntrackedParameter<int>("i1", 10) == 10 );


  // We should not find a string named "i1", nor should we get back
  // our default, because we have an "i1" that is of a different type.
  try
    {
      (void)  p.getUntrackedParameter<std::string>("i1", "");
      assert( "Failed to throw required exception" == 0 );
    }
  catch ( edm::Exception& x)
    {
      // This is the expected exception
      assert( x.categoryCode() == edm::errors::Configuration);
    }
  catch (...)
    {
      assert("Threw the wrong kind of exception" == 0);
    }

  //
  try
    {
      (void)p.getParameter<int>("i2");
      assert( "Failed to throw required exception when getting i2" == 0);
    }
  catch ( edm::Exception& x)
    {
      // This is the expected exception..
      assert( x.categoryCode() == edm::errors::Configuration );
    }
  catch ( ... )
    {
      assert( "Threw the wrong exception when getting i2" == 0);
    }
}

void testUntrackedFromScript()
{
  std::string filetext = 
    "process TEST =\n"
    "{\n"
    "source = DummySource { }\n"
    "PSet p1 = { untracked int32 a = 6 }\n"
    "untracked PSet p2 = { int32 b = 10 }\n"
    "untracked PSet p3 = { untracked double d = 1.5}\n"
    "untracked VPSet vp =\n"
    "  {\n"
    "    { int32 i = 1 },\n"
    "    { untracked double f = 1.5 }\n"
    "  }\n"
    "}\n";

  boost::shared_ptr<ParameterSet> mainps;
  boost::shared_ptr<VPSet > services;
  edm::makeParameterSets(filetext, mainps, services);

  ParameterSet p1 = mainps->getParameter<ParameterSet>("p1");
  assert( p1.getUntrackedParameter<int>("a")  == 6 );  

  ParameterSet p2 = mainps->getUntrackedParameter<ParameterSet>("p2");
  assert( p2.getParameter<int>("b") == 10 );

  ParameterSet p3 = mainps->getUntrackedParameter<ParameterSet>("p3");
  assert( p3.getUntrackedParameter<double>("d") == 1.5);

  VPSet vp = mainps->getUntrackedParameter<VPSet >("vp", VPSet());
  assert( vp.size() == 2 );
  assert (vp[0].getParameter<int>("i") == 1);
  assert( vp[1].getUntrackedParameter<double>("f", 2.0) == 1.5);
}

void testTracked()
{

  // If we look for an 'untracked parameter', when we have a tracked
  // one, we should get an exception throw.
  ParameterSet p;
  ParameterSetID empty_id = p.id();

  p.addParameter<int>("a", 3);
  // Adding a tracked parameter should change the ID.
  assert( p.id() != empty_id );
  try
    {
      (void)p.getUntrackedParameter<int>("a", 10);
      assert ("Failed to throw the required exception" == 0);
    }
  catch ( edm::Exception& x )
    {
      // This is the expected exception
      assert (x.categoryCode() == edm::errors::Configuration);
    }
  catch (...)
    {
      assert ("Threw the wrong exception getting a" == 0);
    }
  
}


int main()
{
  int rc = 1;
  try
    {
      testUntrackedInternal();
      testUntrackedFromScript();
      testTracked();
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
