//----------------------------------------------------------------------
//
// This program tests the behavior of untracked parameters in
// ParameterSet objects.
// $Id:$
//----------------------------------------------------------------------
#include <cassert>
#include <iostream>

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"

void testUntrackedInternal()
{
  edm::ParameterSet p;
  assert( p.empty() );

  p.addUntrackedParameter<int>("i1", 2112);
  assert( !p.empty());
  assert( p.getUntrackedParameter<int>("i1", 10) == 2112 );
  assert( p.getUntrackedParameter<int>("nonesuch", 1) == 1 );

  #if 0
  // The following code tests for an error in handling of untracked
  // parameters. It fails in the current release.
  try
    {
      int i = p.getParameter<int>("i1");
      assert( "Failed to throw required exception when getting i1" == 0);
    }
  catch ( edm::Exception& x)
    {
      // This is the expected exception..
      assert( x.categoryCode() == edm::errors::Configuration );
    }
  catch ( ... )
    {
      assert( "Threw the wrong exception when getting i1" == 0);
    }
  #endif
}


void testUntrackedFromScript()
{
}

int main()
{
  int rc = 1;
  try
    {
      testUntrackedInternal();
      testUntrackedFromScript();
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
