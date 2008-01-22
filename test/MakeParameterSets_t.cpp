#include "FWCore/ParameterSet/test/ConfigFileTest.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h" 
#include "FWCore/ParameterSet/test/ConfigTestMain.h"
#include <iostream>

class MakeParameterSetsTest : public ConfigFileTest {
public:
 virtual int test(const edm::ParameterSet & p)
  {

//    edm::ParameterSet ps =
//      p.getParameter<edm::ParameterSet>("@main_input");


    edm::ParameterSet m1(p.getParameter<edm::ParameterSet>("m1"));
    edm::ParameterSet m2(p.getParameter<edm::ParameterSet>("m2"));
  
    assert( m1.getParameter<int>("a") == 1); // not from blob
    assert( m2.getParameter<int>("a") == 2); // not from blob
//   assert( m1.getParameter<double>("radius") == 0.5);  // from blob
//   assert( m2.getParameter<double>("radius") == 0.75); // override blob
    assert( m1.getParameter<double>("threshold") == 1.0); // from blob
    assert( m2.getParameter<double>("threshold") == 1.0); // from blob

    edm::ParameterSet ps = p.getParameter<edm::ParameterSet>("silly");
    std::vector<std::string> empty;
    std::vector<std::string> vs;

    vs = ps.getUntrackedParameter<std::vector<std::string> >("vs1", empty);
    assert ( vs.size() == 2 );


    vs = ps.getUntrackedParameter<std::vector<std::string> >("vs2", empty);
    assert ( vs.size() == 1 );

    vs = ps.getUntrackedParameter<std::vector<std::string> >("not_there", empty);
    assert ( vs.size() == 0 );
    return 0;
  }
};


ConfigFileTest * ConfigFileTest::instance_ = 0;
// instantiate, so singleton mechanism picks it up
static MakeParameterSetsTest test;

