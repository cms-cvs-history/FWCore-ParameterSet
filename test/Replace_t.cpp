#include "FWCore/ParameterSet/test/ConfigFileTest.h"
#include "FWCore/ParameterSet/test/ConfigTestMain.h"
#include <iostream>

using std::string;
using std::vector;

class ReplaceTest : public ConfigFileTest {
public:
 virtual int test(const edm::ParameterSet & p) {
   edm::ParameterSet rome =
    p.getParameter<edm::ParameterSet>("rome");

   assert(rome.getParameter<int>("date") == 100);
   assert(rome.getParameter<string>("motto") == "Toga! Toga!");
   assert(rome.getParameter<vector<string> >("victorVector")[1] == "Russell Crowe");

   edm::ParameterSet legion = rome.getParameter<edm::ParameterSet>("legion");
   assert(legion.getParameter<vector<int> >("cohorts").size() == 10);

   edm::ParameterSet government= rome.getParameter<edm::ParameterSet>("government");
   assert(government.getParameter<string>("leader") == "Emperor");

   edm::ParameterSet skygod = p.getParameter<edm::ParameterSet>("jupiter");
   assert(skygod.getParameter<string>("wife") == "Juno");

   // see if the replacement in the block got propagated
   assert(rome.getParameter<string>("loverboy") == "Marc Antony");
   
   edm::InputTag tag = rome.getParameter<edm::InputTag>("africans");
   assert(tag.label() == "egyptians");
   assert(tag.instance() == "cleopatra");

   std::vector<edm::InputTag> tags
    = rome.getParameter<std::vector<edm::InputTag> >("europeans");
   assert(tags[2].label() == "helvetics");

   
   tags = rome.getParameter<std::vector<edm::InputTag> >("europeans");
   assert(tags[2].label() == "judeans");

   std::vector<std::string> poets
    = rome.getParameter<std::vector<std::string> >("poets");
   assert(poets[2] == "Ovid");

   edm::ParameterSet y = p.getParameter<edm::ParameterSet>("Y");
   std::string special = y.getParameter<std::string>("special");
   assert(special == "creep");

   // test "from" syntax
   edm::ParameterSet y2 = p.getParameter<edm::ParameterSet>("Y2");
   std::string special2 = y2.getParameter<std::string>("special");
   assert(special2 == "general");


   /// see if the copy worked
/*
   edm::ParameterSet byzantium =
    p.getParameter<edm::ParameterSet>("byzantium");
   assert(byzantium.getParameter<int>("date") == 330);
   assert(byzantium.getParameter<std::string>("motto") == "SPQR");
*/
   // I don't know how to test the source replacement

   return 0;
 }

};

ConfigFileTest * ConfigFileTest::instance_ = 0;
// instantiate, so singleton mechanism picks it up
static ReplaceTest test;


