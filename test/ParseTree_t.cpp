
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

using namespace std;

int work(int argc, char* argv[])
{

  // If given an argument, that argument must be the name of a file to read.
  string buffer;
  edm::pset::ParseTree tree(edm::pset::read_whole_file(argv[1]));
  assert(tree.value("rome.date") == "100");
  vector<string> cohorts = tree.values("rome.legion.cohorts");
  assert(cohorts.size() == 10);
  assert(cohorts[9] == "480");

  tree.replace("rome.date", "500");
  tree.process();
  assert(tree.value("rome.date") == "500");
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
      cerr << "Exception: " << x << '\n';
    }
  catch (...)
    {
      rc = 2;
      cerr << "Unknown exception\n";
    }
  return rc;
}
