/*
 *  makeprocess_t.cc
 *  EDMProto
 *
 *  Created by Chris Jones on 5/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 * 
 * $Id: makeprocess_t.cpp,v 1.9 2005/06/23 19:57:23 wmtan Exp $
 */


#define BOOST_AUTO_TEST_MAIN
#include "boost/test/auto_unit_test.hpp"

#include "FWCore/ParameterSet/interface/Makers.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/interface/MakeProcessPSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

BOOST_AUTO_UNIT_TEST(simple_process_test)
{
   const char* kTest ="process test = { PSet dummy = {bool b = true} } ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK(0 != nodeList.get());
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);

   BOOST_CHECK(test->pset_.getParameter<std::string>("process_name") == "test");   
}

BOOST_AUTO_UNIT_TEST(using_test)
{
   const char* kTest ="process test = { PSet dummy = {bool b = true}\n"
   " block dummy2 = {bool d = true} \n"
   " module include = Dummy {using dummy} \n"
   " module include2 = Dummy2 {using dummy2} } ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK(0 != nodeList.get());
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);
   
   //BOOST_CHECK(test->pset_.getParameter<edm::ParameterSet>("dummy").getBool("b") == true);   
   BOOST_CHECK(test->pset_.getParameter<edm::ParameterSet>("include").getParameter<bool>("b") == true);   
   BOOST_CHECK(test->pset_.getParameter<edm::ParameterSet>("include2").getParameter<bool>("d") == true);   
}

BOOST_AUTO_UNIT_TEST(path_test)
{
   const char* kTest ="process test = {\n"
   "module cone5 = PhonyConeJet { int32 i = 5 }\n"
   "module cone7 = PhonyConeJet { int32 i = 7 }\n"
   "sequence cones = { cone5, cone7 }\n" //NOTE: now requires curly braces
   "path term1 = { cones & jtanalyzer }\n"
   "endpath atEnd = {all,some}\n"
   "} ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK(0 != nodeList.get());
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);
   
   BOOST_CHECK(test->pset_.getParameter<std::string>("process_name") == "test");
   BOOST_CHECK(test->pathFragments_.size() == 3);

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
BOOST_AUTO_UNIT_TEST(module_test)
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
   BOOST_CHECK(0 != nodeList.get());
   
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
   
   BOOST_CHECK(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("cones")));
   BOOST_CHECK(kCone == test->pset_.getParameter<edm::ParameterSet>("cones"));
   
   BOOST_CHECK(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("main_input")));
   BOOST_CHECK(kMainInput == (test->pset_.getParameter<edm::ParameterSet>("main_input")));

   BOOST_CHECK(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("other")));
   BOOST_CHECK(kOther == (test->pset_.getParameter<edm::ParameterSet>("other")));
   
   BOOST_CHECK(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("NoLabelModule@")));
   BOOST_CHECK(kNoLabelModule == test->pset_.getParameter<edm::ParameterSet>("NoLabelModule@"));
   
   BOOST_CHECK(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("LabelModule@labeled")));
   BOOST_CHECK(kLabelModule == test->pset_.getParameter<edm::ParameterSet>("LabelModule@labeled"));

   BOOST_CHECK(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("NoLabelRetriever@")));
   BOOST_CHECK(kNoLabelRetriever == test->pset_.getParameter<edm::ParameterSet>("NoLabelRetriever@"));

   BOOST_CHECK(kEmpty != (test->pset_.getParameter<edm::ParameterSet>("LabelRetriever@label")));
   BOOST_CHECK(kLabelRetriever == test->pset_.getParameter<edm::ParameterSet>("LabelRetriever@label"));
}

BOOST_AUTO_UNIT_TEST(empty_module_test)
{
   const char* kTest ="process test = {\n"
   "#PSet thing1 = {  }\n"
   "module thing = XX {  }\n"
   "} ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK(0 != nodeList.get());
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);
   
   BOOST_CHECK(test->pset_.getParameter<std::string>("process_name") == "test");

   const edm::ParameterSet& myparams = test->pset_;
   std::cout << "ParameterSet looks like:\n";
   std::cout << myparams.toString() << std::endl;
}

BOOST_AUTO_UNIT_TEST(empty_pset_test)
{
   const char* kTest ="process test = {\n"
   "PSet thing1 = {  }\n"
   "module thing = XX {  }\n"
   "} ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK(0 != nodeList.get());
   
   try {
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);
   }
   catch(edm::Exception& e)
   {
       std::cout << "empty pset detected" << std::endl;
	return ;
    }

   throw  std::runtime_error("empty pset not discovered");
}
