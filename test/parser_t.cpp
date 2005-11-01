#include "FWCore/ParameterSet/interface/parse.h"
#include <iterator>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

//------------------------------------------------------------
// Test program for parameter set specification and configuration file
// parser.
//------------------------------------------------------------

using namespace std;

void read_from_file(const char* filename,
		     std::string& buffer)
{
  std::ifstream ifs(filename);
  if (ifs)
    {
      std::string line;
      while (std::getline(ifs, line)) { buffer += line; buffer += '\n'; }
    }
}

int main(int argc, char* argv[])
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
