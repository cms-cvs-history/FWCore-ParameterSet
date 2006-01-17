
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/Utilities/interface/EDMException.h"

//------------------------------------------------------------
// Test program for parameter set specification and configuration file
// parser.
//------------------------------------------------------------

using namespace std;

void read_from_file(const char* filename,
		     string& buffer)
{
  ifstream ifs(filename);
  if (ifs)
    {
      string line;
      while (getline(ifs, line)) { buffer += line; buffer += '\n'; }
    }
}

int work(int argc, char* argv[])
{
  // Because 'scramv1 build runtests' will run this test, and because
  // we don't know how to pass this executable an argument, we make it
  // pass immediately if no argument is supplied.
  if ( argc == 1 ) return 0;

  // If given an argument, that argument must be the name of a file to read.
  string buffer;
  read_from_file(argv[1], buffer);
  const char* spec = buffer.c_str();
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      cerr << "Null output from parser" << endl;
      return -1;
    }

  copy(pr->begin(),
       pr->end(),
       ostream_iterator<edm::pset::NodePtr>(cout,"\n"));

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
