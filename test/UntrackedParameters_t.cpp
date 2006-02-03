//----------------------------------------------------------------------
//
// This program tests the behavior of untracked parameters in
// ParameterSet objects.
// $Id: UntrackedParameters_t.cpp,v 1.1 2006/02/02 15:46:27 paterno Exp $
//----------------------------------------------------------------------
#include <cassert>
#include <iostream>
#include <string>

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"

void testUntrackedInternal()
{
  edm::ParameterSet p;
  assert( p.empty() );

  // ParameterSet should not longer be empty after adding an untracked
  // parameter.
  p.addUntrackedParameter<int>("i1", 2112);
  assert( !p.empty());

  // We should find a parameter named 'i1'
  assert( p.getUntrackedParameter<int>("i1", 10) == 2112 );

  // We should not find one named 'nonesuch'.
  assert( p.getUntrackedParameter<int>("nonesuch", 1) == 1 );

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
}

void testTracked()
{

  // If we look for an 'untracked parameter', when we have a tracked
  // one, we should get an exception throw.
  edm::ParameterSet p;
  p.addParameter<int>("a", 3);
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
