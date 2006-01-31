#include <cassert>
#include <iostream>

#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/Utilities/interface/EDMException.h"


#include <fstream>


//------------------------------------------------------------
// Program to test reading of configuration file to create
// ParameterSet objects.
//------------------------------------------------------------

int work(int argc, char* argv[])
{
  // Because 'scramv1 build runtests' will run this test, and because
  // we don't know how to pass this executable an argument, we make it
  // pass immediately if no argument is supplied.
  if ( argc == 1 ) return 0;

  // Do the real testing.
  // We require 1 argument: the name of the file we are to open.
  if (argc != 2) return -1;

  std::string filename(argv[1]);
  std::string filecontents;
  assert ( edm::pset::read_whole_file(filename, filecontents) );

  boost::shared_ptr<edm::ParameterSet> mainps;
  boost::shared_ptr<std::vector<edm::ParameterSet> > services;

  makeParameterSets(filecontents, mainps, services);

  edm::ParameterSet m1(mainps->getParameter<edm::ParameterSet>("m1"));
  edm::ParameterSet m2(mainps->getParameter<edm::ParameterSet>("m2"));
  
  assert( m1.getParameter<int>("a") == 1); // not from blob
  assert( m2.getParameter<int>("a") == 2); // not from blob
//   assert( m1.getParameter<double>("radius") == 0.5);  // from blob
//   assert( m2.getParameter<double>("radius") == 0.75); // override blob
  assert( m1.getParameter<double>("threshold") == 1.0); // from blob
  assert( m2.getParameter<double>("threshold") == 1.0); // from blob

  return 0;
}

int main(int argc, char* argv[])
{
  int rc = 0;
  try 
    {
      rc = work(argc, argv);
    }
  catch ( edm::Exception const& x )
    {
      rc = 1;
      std::cerr << "Exception:\n" << x << '\n';
    }
  catch (...)
    {
      rc = 2;
      std::cerr << "Unknown exception\n";
    }
  return rc;
}
