#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Algorithms.h"
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>

//------------------------------------------------------------
// Test program for parameter set specification and configuration file
// parser.
//------------------------------------------------------------

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
      std::cerr << "Null output from parser" << std::endl;
      return -1;
    }

  edm::copy_all(*pr, std::ostream_iterator<edm::pset::NodePtr>(cout,"\n"));

  ProcessDesc b(spec);
  boost::shared_ptr<ParameterSet> test = b.getProcessPSet();

  std::cout << test->toString() << std::endl;

  return 0;
}
