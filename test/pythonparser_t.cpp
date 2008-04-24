#include <iostream>
#include <string>

#include "FWCore/ParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/Utilities/interface/Algorithms.h"
#include <boost/python.hpp>

//------------------------------------------------------------
// Test program for parameter set specification and configuration file
// parser.
//------------------------------------------------------------

using namespace boost::python;
using namespace edm;

void compareVStrings(std::vector<std::string> & v1, std::vector<std::string> & v2)
{
  sort_all(v1);
  sort_all(v2);
  if(v1.size() != v2.size())
  {
    std::cerr << "size mismatch " << v1.size() << " " << v2.size();
    copy_all(v1, std::ostream_iterator<std::string>(std::cerr,",\n"));
    std::cerr << "\n";
    copy_all(v2, std::ostream_iterator<std::string>(std::cerr,",\n"));
    std::cerr << "DIFFERENCES " << std::endl;
    std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::ostream_iterator<std::string>(std::cerr, " "));
    std::cerr << std::endl;
  }
}

bool checkString(const std::string & name, const ParameterSet & p1,
                                     const ParameterSet & p2)
{
  assert(p1.getParameter<std::string>(name) == p2.getParameter<std::string>(name));
  return true;
}


bool compareModule(const std::string & name, const ParameterSet & p1,
                                       const ParameterSet & p2)
{
  ParameterSet m1 = p1.getParameter<ParameterSet>(name);
  ParameterSet m2 = p2.getParameter<ParameterSet>(name);
  checkString("@module_label", m1, m2);
  checkString("@module_type", m1, m2);
  std::vector<std::string> names1 = m1.getParameterNames();
  std::vector<std::string> names2 = m2.getParameterNames();
  compareVStrings(names1, names2);
  return true;
}


// parameter of name should be a vstring made up of module names
bool checkModules(const std::string & name, const ParameterSet & p1,
                                      const ParameterSet & p2)
{
  //std::cout << "CHECKMODULES " << name << std::endl;
  std::vector<std::string> names1 = p1.getParameter<std::vector<std::string> >(name);
  std::vector<std::string> names2 = p2.getParameter<std::vector<std::string> >(name);
  compareVStrings(names1, names2);

  for(std::vector<std::string>::const_iterator moduleItr = names1.begin();
      moduleItr != names1.end(); ++moduleItr)
  {
    //std::cout << "COMPAREMOD : " << *moduleItr << std::endl;
    compareModule(*moduleItr, p1, p2);
  }
  return true;
}


bool checkPaths(const std::string & name, const ParameterSet & p1,
                                          const ParameterSet & p2)
{
  std::vector<std::string> pathnames1 = p1.getParameter<std::vector<std::string> >(name);
  std::vector<std::string> pathnames2 = p2.getParameter<std::vector<std::string> >(name);
  compareVStrings(pathnames1, pathnames2);

  for(std::vector<std::string>::const_iterator pathItr = pathnames1.begin();
      pathItr != pathnames1.end(); ++pathItr)
  {
    std::vector<std::string> modulesInPath1 = p1.getParameter<std::vector<std::string> >(*pathItr); 
    std::vector<std::string> modulesInPath2 = p2.getParameter<std::vector<std::string> >(*pathItr);

      for(std::vector<std::string>::const_iterator moduleItr = modulesInPath1.begin();
          moduleItr != modulesInPath1.end(); ++moduleItr)
      {
         // Can't handle ! or - yet
         //compareModule(*moduleItr, p1, p2);
      }

  }
  return true;
}



 
bool test(const std::string & cfgPath, const std::string & pyPath)
{
  FileInPath pyFile(pyPath);
  FileInPath cfgFile(cfgPath);

  boost::shared_ptr<ProcessDesc> pythonProcessDesc
    = PythonProcessDesc(pyFile.fullPath()).processDesc();

  ProcessDesc cfgProcessDesc(edm::read_whole_file(cfgFile.fullPath()));

  // check services
  assert(pythonProcessDesc->getServicesPSets()->size() == 
         cfgProcessDesc.getServicesPSets()->size());

  // get
  boost::shared_ptr<ParameterSet> pythonPSet = pythonProcessDesc->getProcessPSet();
  boost::shared_ptr<ParameterSet> cfgPSet =  cfgProcessDesc.getProcessPSet();

  std::vector<std::string> pythonNames = pythonPSet->getParameterNames();
  std::vector<std::string> cfgNames = cfgPSet->getParameterNames();
  //std::cout << "CFG " << *cfgPSet << std::endl;
  //std::cout << "PY " << *pythonPSet << std::endl;
  // Python includes blocks, so comparison is invalid
  //compareVStrings(pythonNames, cfgNames);
  checkModules("@all_modules", *cfgPSet, *pythonPSet);
  checkModules("@all_sources", *cfgPSet, *pythonPSet);
  checkModules("@all_loopers", *cfgPSet, *pythonPSet);
  checkModules("@all_esmodules", *cfgPSet, *pythonPSet);
  checkModules("@all_essources", *cfgPSet, *pythonPSet);
  //checkModules("@all_esprefers", *cfgPSet, *pythonPSet);
  checkPaths("@paths", *cfgPSet, *pythonPSet);
  checkPaths("@end_paths", *cfgPSet, *pythonPSet);
  compareModule("@main_input", *cfgPSet, *pythonPSet);
  return true;
}

int main(int argc, char* argv[])
{
  test("FWCore/ParameterSet/test/complete.cfg",
       "FWCore/ParameterSet/test/complete.py");
//  test("Configuration/Examples/data/FullChainExample.cfg",
//       "FWCore/ParameterSet/test/FullChainWrapper.py");
  return 0;
}

