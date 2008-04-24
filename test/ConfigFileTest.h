#ifndef ParameterSet_ConfigFileTest_h
#define ParameterSet_ConfigFileTest_h

#include <cassert>
#include <string>
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include <fstream>
#include <iostream>

/** framework to read in parameter sets from files, for testing
 */

class ConfigFileTest {
public:
  ConfigFileTest() {
    instance_ = this;
  }

  int run(const std::string & filename) {
    std::string filecontents = edm::read_whole_file(filename);

    boost::shared_ptr<edm::ParameterSet> mainps;
    boost::shared_ptr<std::vector<edm::ParameterSet> > services;

    makeParameterSets(filecontents, mainps, services);
    //edm::ParameterSet ps = 
    //  mainps->getParameter<edm::ParameterSet>("@main_input");

    return test(*mainps);
  }

  static ConfigFileTest * instance() {return instance_;}

protected:
  virtual int test(const edm::ParameterSet & p) = 0;
  
  static ConfigFileTest * instance_;

};

#endif

