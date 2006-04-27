#include "FWCore/ParameterSet/test/ConfigFileTest.h"
#include "FWCore/ParameterSet/test/ConfigTestMain.h"
#include <iostream>

class ReplaceTest : public ConfigFileTest {
public:
 virtual int test(const edm::ParameterSet & p) {
   edm::ParameterSet rome =
    p.getParameter<edm::ParameterSet>("rome");

   assert(rome.getParameter<int>("date") == 100);
   assert(rome.getParameter<std::string>("motto") == "Toga! Toga!");
   assert(rome.getParameter<std::vector<std::string> >("victorVector")[1] == "Russell Crowe");

   edm::ParameterSet legion = rome.getParameter<edm::ParameterSet>("legion");
   assert(legion.getParameter<std::vector<int> >("cohorts").size() == 10);

   edm::ParameterSet government= rome.getParameter<edm::ParameterSet>("government");
   assert(government.getParameter<std::string>("leader") == "Emperor");

   edm::ParameterSet skygod = p.getParameter<edm::ParameterSet>("jupiter");
   assert(skygod.getParameter<std::string>("wife") == "Juno");
   // I don't know how to test the source replacement

   return 0;
 }

};

ConfigFileTest * ConfigFileTest::instance_ = 0;
// instantiate, so singleton mechanism picks it up
static ReplaceTest test;


