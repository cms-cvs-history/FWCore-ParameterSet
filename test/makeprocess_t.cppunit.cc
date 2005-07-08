/*
 *  makeprocess_t.cc
 *  EDMProto
 *
 *  Created by Chris Jones on 5/18/05.
 *  Changed by Viji Sundararajan on 8-Jul-05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 * 
 * $Id: makeprocess_t.cpp,v 1.9 2005/06/23 19:57:23 wmtan Exp $
 */


#include <cppunit/extensions/HelperMacros.h>

#include "FWCore/ParameterSet/interface/Makers.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/interface/MakeProcessPSet.h"

#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

class testmakeprocess: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(testmakeprocess);
                                                                                                                     
CPPUNIT_TEST(simpleProcessTest);
CPPUNIT_TEST(usingTest);
CPPUNIT_TEST(pathTest);
CPPUNIT_TEST(moduleTest);
CPPUNIT_TEST(emptyModuleTest);
CPPUNIT_TEST(emptyPsetTest);
                                                                                                                     
CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown(){}
                                                                                                                     
  void simpleProcessTest();
  void usingTest();
  void pathTest();
  void moduleTest();
  void emptyModuleTest();
  void emptyPsetTest();
};
                                                                                                                     
///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testmakeprocess);

void testmakeprocess::simpleProcessTest()
{
   const char* kTest ="process test = { PSet dummy = {bool b = true} } ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);

   CPPUNIT_ASSERT(test->pset_.getParameter<std::string>("process_name") == "test");   
}

void testmakeprocess::usingTest()
{
   const char* kTest ="process test = { PSet dummy = {bool b = true}\n"
   " block dummy2 = {bool d = true} \n"
   " module include = Dummy {using dummy} \n"
   " module include2 = Dummy2 {using dummy2} } ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);
   
   //CPPUNIT_ASSERT(test->pset_.getParameter<edm::ParameterSet>("dummy").getBool("b") == true);   
   CPPUNIT_ASSERT(test->pset_.getParameter<edm::ParameterSet>("include").getParameter<bool>("b") == true);   
   CPPUNIT_ASSERT(test->pset_.getParameter<edm::ParameterSet>("include2").getParameter<bool>("d") == true);   
}

void testmakeprocess::pathTest()
{
   const char* kTest ="process test = {\n"
   "module cone5 = PhonyConeJet { int32 i = 5 }\n"
   "module cone7 = PhonyConeJet { int32 i = 7 }\n"
   "sequence cones = { cone5, cone7 }\n" //NOTE: now requires curly braces
   "path term1 = { cones & jtanalyzer }\n"
   "endpath atEnd = {all,some}\n"
   "} ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);
   
   CPPUNIT_ASSERT(test->pset_.getParameter<std::string>("process_name") == "test");
   CPPUNIT_ASSERT(test->pathFragments_.size() == 3);

   const edm::ParameterSet& myparams = test->pset_;
   std::cout << "ParameterSet looks like:\n";
   std::cout << myparams.toString() << std::endl;
}


edm::ParameterSet modulePSet(const std::string& iLabel, const std::string& iType) {
   edm::ParameterSet temp;
   temp.insert(true , "s", edm::Entry(1,true));
   temp.insert(true, "module_label", edm::Entry(iLabel, true));
   temp.insert(true, "module_type", edm::Entry(iType, true));
   return temp;
}
void testmakeprocess::moduleTest()
{
   const char* kTest ="process test = { "
   "module cones = Module{ int32 s=1 }\n" //NOTE: now requires curly braces
   "es_module = NoLabelModule{ int32 s=1 }\n"
   "es_module labeled= LabelModule{ int32 s=1 }\n"
   "source = InputService{int32 s=1}\n"
   "source other = OtherInputService{int32 s=1}\n"
   "es_source = NoLabelRetriever{int32 s=1}\n"
   "es_source label = LabelRetriever{int32 s=1}\n"
   "} ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);

   static const edm::ParameterSet kEmpty;
   const edm::ParameterSet kCone(modulePSet("cones", "Module"));
   std::cout << kCone.toString() << std::endl;
   std::cout << test->pset_.getParameter<edm::ParameterSet>("cones").toString() << std::endl;
   
   const edm::ParameterSet kMainInput(modulePSet("main_input","InputService"));
   const edm::ParameterSet kOther(modulePSet("other","OtherInputService"));
   
   const edm::ParameterSet kNoLabelModule(modulePSet("", "NoLabelModule"));
   const edm::ParameterSet kLabelModule(modulePSet("labeled", "LabelModule"));
   const edm::ParameterSet kNoLabelRetriever(modulePSet("", "NoLabelRetriever"));
   const edm::ParameterSet kLabelRetriever(modulePSet("label", "LabelRetriever"));
   
   CPPUNIT_ASSERT(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("cones")));
   CPPUNIT_ASSERT(kCone == test->pset_.getParameter<edm::ParameterSet>("cones"));
   
   CPPUNIT_ASSERT(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("main_input")));
   CPPUNIT_ASSERT(kMainInput == (test->pset_.getParameter<edm::ParameterSet>("main_input")));

   CPPUNIT_ASSERT(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("other")));
   CPPUNIT_ASSERT(kOther == (test->pset_.getParameter<edm::ParameterSet>("other")));
   
   CPPUNIT_ASSERT(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("NoLabelModule@")));
   CPPUNIT_ASSERT(kNoLabelModule == test->pset_.getParameter<edm::ParameterSet>("NoLabelModule@"));
   
   CPPUNIT_ASSERT(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("LabelModule@labeled")));
   CPPUNIT_ASSERT(kLabelModule == test->pset_.getParameter<edm::ParameterSet>("LabelModule@labeled"));

   CPPUNIT_ASSERT(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("NoLabelRetriever@")));
   CPPUNIT_ASSERT(kNoLabelRetriever == test->pset_.getParameter<edm::ParameterSet>("NoLabelRetriever@"));

   CPPUNIT_ASSERT(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("LabelRetriever@label")));
   CPPUNIT_ASSERT(kLabelRetriever == test->pset_.getParameter<edm::ParameterSet>("LabelRetriever@label"));
}

void testmakeprocess::emptyModuleTest()
{
   const char* kTest ="process test = {\n"
   "#PSet thing1 = {  }\n"
   "module thing = XX {  }\n"
   "} ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);
   
   CPPUNIT_ASSERT(test->pset_.getParameter<std::string>("process_name") == "test");

   const edm::ParameterSet& myparams = test->pset_;
   std::cout << "ParameterSet looks like:\n";
   std::cout << myparams.toString() << std::endl;
}

void testmakeprocess::emptyPsetTest()
{
   const char* kTest ="process test = {\n"
   "PSet thing1 = {  }\n"
   "module thing = XX {  }\n"
   "} ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   try {
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);
   }
   catch(std::exception& e)
   {
       std::cout << "empty pset detected" << std::endl;
	return ;
    }

   throw  std::runtime_error("empty pset not discovered");
}
