/*
 *  makeprocess_t.cc
 *  EDMProto
 *
 *  Created by Chris Jones on 5/18/05.
 *  Changed by Viji Sundararajan on 8-Jul-05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 * 
 * $Id: makeprocess_t.cppunit.cc,v 1.12 2006/05/23 20:02:10 rpw Exp $
 */


#include <cppunit/extensions/HelperMacros.h>

#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class testmakeprocess: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(testmakeprocess);
CPPUNIT_TEST(simpleProcessTest);
CPPUNIT_TEST(usingTest);
CPPUNIT_TEST(pathTest);
CPPUNIT_TEST(moduleTest);
CPPUNIT_TEST(serviceTest);
CPPUNIT_TEST(emptyModuleTest);
CPPUNIT_TEST(windowsLineEndingTest);
//CPPUNIT_TEST_EXCEPTION(emptyPsetTest,edm::Exception);
CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown(){}
  void simpleProcessTest();
  void usingTest();
  void pathTest();
  void moduleTest();
  void serviceTest();
  void emptyModuleTest();
  void windowsLineEndingTest();
private:

  typedef boost::shared_ptr<edm::ProcessDesc> ProcDescPtr;
  ProcDescPtr procDesc(const char * c) {

    ProcDescPtr result( new edm::ProcessDesc(std::string(c)) );
    CPPUNIT_ASSERT(result->getProcessPSet()->getParameter<std::string>("@process_name") == "test");
    return result;
  }
  //  void emptyPsetTest();
};



///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testmakeprocess);

void testmakeprocess::simpleProcessTest()
{
   const char* kTest ="process test = { PSet dummy = {bool b = true} } ";
   ProcDescPtr test = procDesc(kTest);
}

void testmakeprocess::usingTest()
{
   const char* kTest ="process test = { PSet dummy = {bool b = true}\n"
   " block dummy2 = {bool d = true} \n"
   " module include = Dummy {using dummy} \n"
   " module include2 = Dummy2 {using dummy2} } ";
   
   ProcDescPtr test = procDesc(kTest);

   //CPPUNIT_ASSERT(test->getProcessPSet()->getParameter<edm::ParameterSet>("dummy").getBool("b") == true);   
   CPPUNIT_ASSERT(test->getProcessPSet()->getParameter<edm::ParameterSet>("include").getParameter<bool>("b") == true);   
   CPPUNIT_ASSERT(test->getProcessPSet()->getParameter<edm::ParameterSet>("include2").getParameter<bool>("d") == true);   
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

   ProcDescPtr test = procDesc(kTest);

   CPPUNIT_ASSERT(test->pathFragments().size() == 3);

   const edm::ParameterSet& myparams = *(test->getProcessPSet());
//    std::cout << "ParameterSet looks like:\n";
//    std::cout << myparams.toString() << std::endl;
   std::string rep = myparams.toString();
   edm::ParameterSet copy(rep);
   CPPUNIT_ASSERT(copy == myparams);
}


edm::ParameterSet modulePSet(const std::string& iLabel, const std::string& iType) {
   edm::ParameterSet temp;
   temp.insert(true , "s", edm::Entry(1,true));
   temp.insert(true, "@module_label", edm::Entry(iLabel, true));
   temp.insert(true, "@module_type", edm::Entry(iType, true));
   return temp;
}
void testmakeprocess::moduleTest()
{
   const char* kTest ="process test = { "
   "module cones = Module{ int32 s=1 }\n" //NOTE: now requires curly braces
   "es_module = NoLabelModule{ int32 s=1 }\n"
   "es_module labeled= LabelModule{ int32 s=1 }\n"
   "source = InputSource{int32 s=1}\n"
   "source other = OtherInputSource{int32 s=1}\n"
   "es_source = NoLabelRetriever{int32 s=1}\n"
   "es_source label = LabelRetriever{int32 s=1}\n"
   "} ";

   ProcDescPtr test = procDesc(kTest);

   static const edm::ParameterSet kEmpty;
   const edm::ParameterSet kCone(modulePSet("cones", "Module"));
   std::ostringstream out;
   out << kCone.toString() << std::endl;
   out << test->getProcessPSet()->getParameter<edm::ParameterSet>("cones").toString() << std::endl;
   
   const edm::ParameterSet kMainInput(modulePSet("@main_input","InputSource"));
   const edm::ParameterSet kOther(modulePSet("other","OtherInputSource"));
   
   const edm::ParameterSet kNoLabelModule(modulePSet("", "NoLabelModule"));
   const edm::ParameterSet kLabelModule(modulePSet("labeled", "LabelModule"));
   const edm::ParameterSet kNoLabelRetriever(modulePSet("", "NoLabelRetriever"));
   const edm::ParameterSet kLabelRetriever(modulePSet("label", "LabelRetriever"));
   
   CPPUNIT_ASSERT(kEmpty != (test->getProcessPSet()->getParameter<edm::ParameterSet>("cones")));
   CPPUNIT_ASSERT(kCone == test->getProcessPSet()->getParameter<edm::ParameterSet>("cones"));
   
   CPPUNIT_ASSERT(kEmpty != (test->getProcessPSet()->getParameter<edm::ParameterSet>("@main_input")));
   CPPUNIT_ASSERT(kMainInput == (test->getProcessPSet()->getParameter<edm::ParameterSet>("@main_input")));

   CPPUNIT_ASSERT(kEmpty != (test->getProcessPSet()->getParameter<edm::ParameterSet>("other")));
   CPPUNIT_ASSERT(kOther == (test->getProcessPSet()->getParameter<edm::ParameterSet>("other")));
   
   CPPUNIT_ASSERT(kEmpty != (test->getProcessPSet()->getParameter<edm::ParameterSet>("NoLabelModule@")));
   CPPUNIT_ASSERT(kNoLabelModule == test->getProcessPSet()->getParameter<edm::ParameterSet>("NoLabelModule@"));
   
   CPPUNIT_ASSERT(kEmpty != (test->getProcessPSet()->getParameter<edm::ParameterSet>("LabelModule@labeled")));
   CPPUNIT_ASSERT(kLabelModule == test->getProcessPSet()->getParameter<edm::ParameterSet>("LabelModule@labeled"));

   CPPUNIT_ASSERT(kEmpty != (test->getProcessPSet()->getParameter<edm::ParameterSet>("NoLabelRetriever@")));
   CPPUNIT_ASSERT(kNoLabelRetriever == test->getProcessPSet()->getParameter<edm::ParameterSet>("NoLabelRetriever@"));

   CPPUNIT_ASSERT(kEmpty != (test->getProcessPSet()->getParameter<edm::ParameterSet>("LabelRetriever@label")));
   CPPUNIT_ASSERT(kLabelRetriever == test->getProcessPSet()->getParameter<edm::ParameterSet>("LabelRetriever@label"));
}

void testmakeprocess::serviceTest()
{
   const char* kTest ="process test = { "
   "service = XService{ int32 s=1 }\n"
   "service = YService{ int32 s=1 }\n"
   "} ";

   ProcDescPtr test = procDesc(kTest);

   CPPUNIT_ASSERT(test->getServicesPSets()->size() == 2);
   CPPUNIT_ASSERT("XService" == test->getServicesPSets()->at(0).getParameter<std::string>("@service_type"));
   CPPUNIT_ASSERT("YService" == test->getServicesPSets()->at(1).getParameter<std::string>("@service_type"));
}
void testmakeprocess::emptyModuleTest()
{
   const char* kTest ="process test = {\n"
   "#PSet thing1 = {  }\n"
   "module thing = XX {  }\n"
   "} ";

   ProcDescPtr test = procDesc(kTest);

   const edm::ParameterSet& myparams = *(test->getProcessPSet());
//    std::cout << "ParameterSet looks like:\n";
//    std::cout << myparams.toString() << std::endl;
   std::string rep = myparams.toString();
   edm::ParameterSet copy(rep);
   CPPUNIT_ASSERT(copy == myparams);
}

void testmakeprocess::windowsLineEndingTest()
{
#if 0
  const char* kTest = "\r\n"
    "process test = {\r\n"
    "source = InputSource{\r\n"
    "  int32 i=1\r\n"
    "  string s1 = \"\r\"  \r\n"
    "  string s2 = \"\\r\"  \r\n"
    "}\r\n"
    "//this is a comment\r\n"
    "# this is a comment\r\n"
    "}\r\n";
#endif

  std::ostringstream oss;
  const char ret = '\r';
  const char nl = '\n';
  const char dquote = '"';
  const char backsl = '\\';

  oss << ret << nl
      << "process test = {" << ret << nl
      << "  source = InputSource{" << ret << nl
      << "    int32 i=1" << ret << nl
      << "    string s1 = " << dquote << ret << dquote << ret << nl
      << "    string s2 = " << dquote << backsl << backsl << 'r' << dquote << ret << nl
      << "  }" << ret << nl
      << "}" << ret << nl;
  const char* kTest = oss.str().c_str();
  std::cerr << "\n------------------------------\n";
  std::cerr << "s1 will look funky because of the embedded return\n";
  std::cerr << "s2 shows how to get the chars backslash-r into a string\n";
  std::cerr << kTest;
  std::cerr << "\n------------------------------\n";

   ProcDescPtr test = procDesc(kTest);

   edm::ParameterSet const& p = *(test->getProcessPSet());
   
   edm::ParameterSet src = p.getParameter<edm::ParameterSet>("@main_input");
   CPPUNIT_ASSERT(src.getParameter<int>("i") == 1);
   std::string s1 = src.getParameter<std::string>("s1");
   std::string s2 = src.getParameter<std::string>("s2");
   
   std::cerr << "\nsize of s1 is: " << s1.size();
   std::cerr << "\nsize of s2 is: " << s2.size() << '\n';

   CPPUNIT_ASSERT(s1.size() == 1);
   CPPUNIT_ASSERT(s1[0] == ret);

   CPPUNIT_ASSERT(s2.size() == 2);
   CPPUNIT_ASSERT(s2[0] == backsl);
   CPPUNIT_ASSERT(s2[1] == 'r');
}

// void testmakeprocess::emptyPsetTest()
// {
//    const char* kTest ="process test = {\n"
//    "PSet thing1 = {  }\n"
//    "module thing = XX {  }\n"
//    "} ";
//    boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
//    CPPUNIT_ASSERT(0 != nodeList.get());
//    boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);
   
//    /*try {
//    boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess(nodeList);
//    }
//    catch(std::exception& e)
//    {
//        std::cout << "empty pset detected" << std::endl;
// 	return ;
//     }

//    throw  std::runtime_error("empty pset not discovered");*/
// }
