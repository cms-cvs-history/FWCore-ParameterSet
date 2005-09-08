#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/interface/ProcessPSetBuilder.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>

//------------------------------------------------------------
// Test program for parameter set specification and configuration file
// parser.
//------------------------------------------------------------

using namespace std;
using namespace edm;
using namespace pset;

void read_from_stdin(std::string& buffer)
{
  std::string line;
  while (std::getline(cin, line)) { buffer += line; buffer += '\n'; }
}

int main()
{
  string buffer;
  read_from_stdin(buffer);
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

  ProcessPSetBuilder b(spec);
  boost::shared_ptr<ParameterSet> test = b.getProcessPSet();

  cout << test->toString() << endl;

  return 0;
}
