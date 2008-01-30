#include <iostream>
#include <string>

#include "FWCore/ParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include <boost/python.hpp>

//------------------------------------------------------------
// Test program for parameter set specification and configuration file
// parser.
//------------------------------------------------------------

using namespace std;
using namespace boost::python;
using namespace edm;

void checkVStrings(vector<string> & v1, const vector<string> & v2)
{
  if(v1.size() != v2.size())
  {
    cerr << "size mismatch \n";
    copy(v1.begin(),
         v1.end(),
         ostream_iterator<std::string>(cerr,","));
    cerr << "\n";
    copy(v2.begin(),
         v2.end(),
         ostream_iterator<std::string>(cerr,","));
  }
}


int main()
{

  FileInPath pyFile("FWCore/ParameterSet/test/complete.py");
  FileInPath cfgFile("FWCore/ParameterSet/test/complete.cfg");

  boost::shared_ptr<ProcessDesc> pythonProcessDesc
    = PythonProcessDesc(pyFile.fullPath()).processDesc();

  ProcessDesc cfgProcessDesc(edm::pset::read_whole_file(cfgFile.fullPath()));

  // check services
  assert(pythonProcessDesc->getServicesPSets()->size() == 
         cfgProcessDesc.getServicesPSets()->size());

  // get
  boost::shared_ptr<ParameterSet> pythonPSet = pythonProcessDesc->getProcessPSet();
  boost::shared_ptr<ParameterSet> cfgPSet =  cfgProcessDesc.getProcessPSet();

  vector<string> pythonNames = pythonPSet->getParameterNames();
  vector<string> cfgNames = cfgPSet->getParameterNames();
  checkVStrings(pythonNames, cfgNames);
}

