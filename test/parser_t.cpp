
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "FWCore/ParameterSet/interface/ParseTree.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/interface/PSetNode.h"
#include "FWCore/Utilities/interface/EDMException.h"

//------------------------------------------------------------
// Test program for parameter set specification and configuration file
// parser.
//------------------------------------------------------------

int work(int argc, char* argv[])
{
  // Because 'scramv1 build runtests' will run this test, and because
  // we don't know how to pass this executable an argument, we make it
  // pass immediately if no argument is supplied.
  if ( argc == 1 ) return 0;

  // If given an argument, that argument must be the name of a file to read.
  std::string buffer;
  edm::pset::ParseTree tree(edm::pset::read_whole_file(argv[1]));
  tree.top()->print(std::cout, edm::pset::Node::COMPRESSED);

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
      std::cerr << "Exception: " << x << '\n';
    }
  catch (...)
    {
      rc = 2;
      std::cerr << "Unknown exception\n";
    }
  return rc;
}
