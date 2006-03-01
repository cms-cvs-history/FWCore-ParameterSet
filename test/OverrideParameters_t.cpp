#include <cassert>
#include <iostream>
#include <string>

#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/Utilities/interface/EDMException.h"


#include <fstream>

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

  edm::ParameterSet ps = 
    mainps->getParameter<edm::ParameterSet>("@main_input");

  assert( ps.getParameter<int>("a") == 1 );
  std::cout << "The winner is: " << ps.getParameter<std::string>("new")
	    << std::endl;
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
