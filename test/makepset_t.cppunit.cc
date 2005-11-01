/*
 *  makepset_t.cc
 *  EDMProto
 *
 *  Created by Chris Jones on 5/18/05.
 *  Changed by Viji Sundararajan on 11-Jul-05.
 *
 * $Id: makepset_t.cppunit.cc,v 1.7 2005/10/21 12:58:24 paterno Exp $
 */

#include <iostream>
#include <string>

#include <cppunit/extensions/HelperMacros.h>

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/Makers.h"
#include "FWCore/ParameterSet/interface/MakeProcessPSet.h"
#include "FWCore/ParameterSet/interface/parse.h"


class testmakepset: public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(testmakepset);
  CPPUNIT_TEST_EXCEPTION(emptyTest,edm::Exception);
  CPPUNIT_TEST(typesTest);
  CPPUNIT_TEST(usingTest);
  CPPUNIT_TEST_EXCEPTION(usingExcTest,edm::Exception);
  CPPUNIT_TEST(psetRefTest);
  CPPUNIT_TEST_EXCEPTION(psetRefExcTest,edm::Exception);
  CPPUNIT_TEST(secsourceTest);
  CPPUNIT_TEST(usingBlockTest);
  CPPUNIT_TEST(fileinpathTest);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp(){}
  void tearDown(){}
  void emptyTest();
  void typesTest();
  void usingTest();
  void usingExcTest();
  void psetRefTest();
  void psetRefExcTest();
  void secsourceTest();
  void usingBlockTest();
  void fileinpathTest();

 private:
  void secsourceAux();
  void usingBlockAux();
  void fileinpathAux();
};
                                                                                                                   
///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testmakepset);

void testmakepset::secsourceTest()
{
  try { this->secsourceAux(); }
  catch (cms::Exception& x) { 
    std::cerr << "testmakepset::secsourceTest() caught a cms::Exception\n";
    std::cerr << x.what() << '\n';
    throw;
  }
  catch (...) {
    std::cerr << "testmakepset::secsourceTest() caught an unidentified exception\n";
    throw;
  }
}

void testmakepset::secsourceAux()
{
  const char* kTest = 
    "process PROD = {"
    "  source = PoolSource {"
    "    string fileName = 'main.root'"
    "    int32 maxEvents = 2"
    "  }"
    "  module out = PoolOutputModule {"
    "    string fileName = 'CumHits.root'"
    "  }"
    "  module mix = MixingModule {"
    "    secsource input = PoolSource  {"
    "      string fileName = 'pileup.root'"
    "    }"
    "    string type = 'fixed'"
    "    double average_number = 14.3"
    "    int32 min_bunch = -5"
    "    int32 max_bunch = 3"
    "  }"
    "  path p = { mix, out }"
    "}";

  std::string config(kTest);

  // Create the ParameterSet object from this configuration string.
  boost::shared_ptr<edm::ParameterSet> ps = edm::makeProcessPSet(config);
  CPPUNIT_ASSERT(0 != ps.get());

  // Make sure this ParameterSet object has the right contents
  edm::ParameterSet mixingModuleParams = ps->getParameter<edm::ParameterSet>("mix");
  edm::ParameterSet secondarySourceParams = mixingModuleParams.getParameter<edm::ParameterSet>("input");
  CPPUNIT_ASSERT(secondarySourceParams.getParameter<std::string>("@module_type") == "PoolSource"); 
  CPPUNIT_ASSERT(secondarySourceParams.getParameter<std::string>("@module_label") == "input");
  CPPUNIT_ASSERT(secondarySourceParams.getParameter<std::string>("fileName") == "pileup.root");
}

void testmakepset::usingBlockTest()
{
  try { this->usingBlockAux(); }
  catch (cms::Exception& x) { 
    std::cerr << "testmakepset::usingBlockTest() caught a cms::Exception\n";
    std::cerr << x.what() << '\n';
    throw;
  }
  catch (...) {
    std::cerr << "testmakepset::usingBlockTest() caught an unidentified exception\n";
    throw;
  }
}

void testmakepset::usingBlockAux()
{
  const char* kTest = 
    "process PROD = {"
    "  source = PoolSource {"
    "    string fileName = 'main.root'"
    "    int32 maxEvents = 2"
    "  }"
    "  block b = {"
    "    double r = 1.5"
    "    string s = 'original'"
    "  }"
    "  module m1 = Class1 {"
    "    using b"
    "    int32 i = 1"
    "  }"
    "  module m2 = Class2 {"
    "    int32 i = 2"
    "    int32 j = 3"
    "    using b"
    "  }"
    "}";

  std::string config(kTest);

  // Create the ParameterSet object from this configuration string.
  boost::shared_ptr<edm::ParameterSet> ps = edm::makeProcessPSet(config);
  CPPUNIT_ASSERT(0 != ps.get());

  // Make sure this ParameterSet object has the right contents
  edm::ParameterSet m1Params = ps->getParameter<edm::ParameterSet>("m1");
  edm::ParameterSet m2Params = ps->getParameter<edm::ParameterSet>("m2");
  CPPUNIT_ASSERT(m1Params.getParameter<int>("i") == 1);
  CPPUNIT_ASSERT(m2Params.getParameter<int>("i") == 2);
  CPPUNIT_ASSERT(m2Params.getParameter<int>("j") == 3);

  CPPUNIT_ASSERT(m1Params.getParameter<std::string>("s") == "original");
  CPPUNIT_ASSERT(m2Params.getParameter<std::string>("s") == "original");

  CPPUNIT_ASSERT(m1Params.getParameter<double>("r") == 1.5);
  CPPUNIT_ASSERT(m2Params.getParameter<double>("r") == 1.5);
}

void testmakepset::fileinpathTest()
{
  try { this->fileinpathAux(); }
  catch (cms::Exception& x) { 
    std::cerr << "testmakepset::fileinpathTest() caught a cms::Exception\n";
    std::cerr << x.what() << '\n';
    throw;
  }
  catch (...) {
    std::cerr << "testmakepset::fileinpathTest() caught an unidentified exception\n";
    throw;
  }
}

void testmakepset::fileinpathAux()
{
  const char* kTest = 
     "process PROD = {"
    "  PSet main =  {"
    "    int32 extraneous = 12"
    "    FileInPath fip = \"silly.file\""
    "  }"
    "  source = DummySource { } "
    "}";

  std::string config(kTest);

  // Create the ParameterSet object from this configuration string.
  boost::shared_ptr<edm::ParameterSet> ps = edm::makeProcessPSet(config);
  CPPUNIT_ASSERT(0 != ps.get());
  std::cerr << "\n-----------------------------\n";
  std::cerr << ps->toString();
  std::cerr << "\n-----------------------------\n";

  edm::ParameterSet mainps = ps->getParameter<edm::ParameterSet>("main");
  edm::FileInPath fip = mainps.getParameter<edm::FileInPath>("fip");
  CPPUNIT_ASSERT( fip.isLocal() == false );
  CPPUNIT_ASSERT( fip.relativePath() == "silly.file" );
}
                                                                                                                   
void testmakepset::emptyTest()
{
   const char* kTest ="PSet test = { } ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   // this should not be allowed, but is for the time being
   // only psets coming in from process sections cannot be empty
   //BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList), std::runtime_error);
   edm::pset::makePSet(*nodeList);
}

void testmakepset::typesTest()
{
   //vbool vb = {true, false};
   const char* kTest =
     "uint32 ui=1\n"
     "int32 i = +1\n"
     "double d=1\n"
     "vuint32 vui ={1,2}\n"
     "vint32 vi = {+1,-2}\n"
     "bool b = true\n"
     "PSet ps = {bool b2=true}\n"
     "string s=\"this string\"\n"
     "vstring vs={\"1\",\"2\",\"a\"}\n"
     "VPSet vps ={ {bool b3=false} }\n";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   boost::shared_ptr<edm::ParameterSet> test = edm::pset::makePSet(*nodeList);
   //std::cout << test->toString() << std::endl;
   
   CPPUNIT_ASSERT(1 == test->getParameter<int>("i"));
   CPPUNIT_ASSERT(test->retrieve("i").isTracked());
   CPPUNIT_ASSERT(1 == test->getParameter<unsigned int>("ui"));
   CPPUNIT_ASSERT(1 == test->getParameter<double>("d"));
   //std::cout << test->getParameter<std::string>("s") << std::endl;
   CPPUNIT_ASSERT("this string" == test->getParameter<std::string>("s"));
   CPPUNIT_ASSERT(3 == test->getParameter<std::vector<std::string> >("vs").size());
   CPPUNIT_ASSERT(test->getParameter<std::vector<std::string> >("vs").size() && "1" == test->getParameter<std::vector<std::string> >("vs")[0]);
   CPPUNIT_ASSERT(test->getParameter<std::vector<std::string> >("vs").size() >1 && "2" == test->getParameter<std::vector<std::string> >("vs")[1]);
   CPPUNIT_ASSERT(test->getParameter<std::vector<std::string> >("vs").size() >1 && "a" == test->getParameter<std::vector<std::string> >("vs")[2]);
   //std::cout <<"\""<<test->getParameter<std::vector<std::string> >("vs")[0]<<"\" \""<<test->getParameter<std::vector<std::string> >("vs")[1]<<"\" \""
   //<<test->getParameter<std::vector<std::string> >("vs")[2]<<"\""<<std::endl;
   
   static const unsigned int vuia[] = {1,2};
   static const std::vector<unsigned int> vui(vuia, vuia+sizeof(vuia)/sizeof(unsigned int));
   CPPUNIT_ASSERT(vui == test->getParameter<std::vector<unsigned int> >("vui"));
   
   static const  int via[] = {1,-2};
   static const std::vector< int> vi(via, via+sizeof(vuia)/sizeof(unsigned int));
   test->getParameter<std::vector<int> >("vi");
   CPPUNIT_ASSERT(true == test->getParameter<bool>("b"));
   CPPUNIT_ASSERT(test->retrieve("vi").isTracked());
   //test->getParameter<std::vector<bool> >("vb");
   edm::ParameterSet ps = test->getParameter<edm::ParameterSet>("ps");
   CPPUNIT_ASSERT(true == ps.getUntrackedParameter<bool>("b2", false));
   std::vector<edm::ParameterSet> vps = test->getParameter<std::vector<edm::ParameterSet> >("vps");
   CPPUNIT_ASSERT(1 == vps.size());
   CPPUNIT_ASSERT(false == vps.front().getParameter<bool>("b3"));
   
   //BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList), std::runtime_error);
}

void testmakepset::usingTest()
{
   //vbool vb = {true, false};
   const char* kTest ="using ublock";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   //BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList), std::runtime_error);

   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > blocks;
   const std::string kName("ublock");
   boost::shared_ptr<edm::ParameterSet> ublock(new edm::ParameterSet);
   ublock->insert(true, kName, edm::Entry(true, true));
   blocks.insert(make_pair(kName, ublock));
   boost::shared_ptr<edm::ParameterSet> test = edm::pset::makePSet(*nodeList, blocks);

   CPPUNIT_ASSERT(true == test->getParameter<bool>(kName));
}

void testmakepset::usingExcTest()
{
   //vbool vb = {true, false};
   const char* kTest ="using ublock";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   //BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList), std::runtime_error);
   edm::pset::makePSet(*nodeList);
}

void testmakepset::psetRefTest()
{
   //vbool vb = {true, false};
   const char* kTest ="PSet test = ref";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   //BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList), std::runtime_error);
   
   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > blocks;
   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > psets;
   const std::string kName("ref");
   boost::shared_ptr<edm::ParameterSet> ref(new edm::ParameterSet);
   ref->insert(true, kName, edm::Entry(true, true));
   psets.insert(make_pair(kName, ref));
   boost::shared_ptr<edm::ParameterSet> test = edm::pset::makePSet(*nodeList, blocks,psets);
   
   CPPUNIT_ASSERT(true == test->getParameter<edm::ParameterSet>("test").getParameter<bool>("ref"));
}

void testmakepset::psetRefExcTest()
{
   //vbool vb = {true, false};
   const char* kTest ="PSet test = ref";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   //BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList), std::runtime_error);
   edm::pset::makePSet(*nodeList);
}
