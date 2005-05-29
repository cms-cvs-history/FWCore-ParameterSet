
#include "parser.hh"
#include "Makers.hh"
#include "PSet.hh"
#include "ProcessDesc.hh"

#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
  if(argc<2)
    {
      cerr << "Usage: " << argv[0] << ": process|pset\n"
	   << " process means that input is expected to be a process type\n"
	   << " pset means input is expected to be ParameterSet contents"
	   << endl;
      return -1;
    }

  string arg(argv[1]);
  bool isprocess = arg=="process" ? true : false;

  const int bsize=65536;
  char spec[bsize];
  // fill spec from standard input

  std::cin.getline(spec, bsize, '\0');
  ParseResults pr = parse(buf);

  if(!pr)
    {
      cerr << "No output from parser!" << endl;
      return -1;
    }

  if(isprocess)
    {
      auto_ptr<ProcessDesc> desc = makeProcess(pr);
      cout << *desc << endl;
    }
  else
    {
      auto_ptr<ParameterSet> pset = makePSet(pr);
      cout << *pset << endl;
    }

  
  cout << "DONE" << endl;
  return 0;
}
