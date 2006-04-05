#include "FWCore/ParameterSet/test/ConfigFileTest.h"
#include "FWCore/ParameterSet/test/ConfigTestMain.h"
#include <iostream>

class OverrideParametersTest : public ConfigFileTest {
public:
 virtual int test(const edm::ParameterSet & p) 
  {

    edm::ParameterSet ps = 
      p.getParameter<edm::ParameterSet>("@main_input");

    assert( ps.getParameter<int>("a") == 1 );
    std::cout << "The winner is: " << ps.getParameter<std::string>("new")
           << std::endl;
    return 0;
  }
};


ConfigFileTest * ConfigFileTest::instance_ = 0;
// instantiate, so singleton mechanism picks it up
static OverrideParametersTest test;

