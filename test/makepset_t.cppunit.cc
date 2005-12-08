/*
 *  makepset_t.cc
 *  EDMProto
 *
 *  Created by Chris Jones on 5/18/05.
 *  Changed by Viji Sundararajan on 11-Jul-05.
 *
 * $Id: makepset_t.cppunit.cc,v 1.14.2.1 2005/12/08 11:21:27 sashby Exp $
 */

#include <algorithm>
#include <iostream>
#include <string>

#include <iostream>

#include <stdlib.h> // for setenv; <cstdlib> is likely to fail

#include "cppunit/extensions/HelperMacros.h"
#include "boost/lambda/lambda.hpp"

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
  catch (std::exception& x) {
    std::cerr << "testmakepset::secsourceTest() caught a std::exception\n";
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
  // The body of this test is commented out so that tests during
  // official builds do not bomb. When the testing system is made
  // flexible enough to make tests such as this reasonable in
  // production, this code can be translated and re-activated. Until
  // then, core developers will need to tweak this code to work.

  const char* c = getenv("CMSSW_DATA_PATH");

  // We need this to transition from the original CMSDATA to the new
  // CMSSW_DATA_PATH.
  if (c==0) 
    {
      c = getenv("CMSDATA");
      CPPUNIT_ASSERT( c!= 0 );
      // should not need to overwrite, so last arg to setenv is 0
      CPPUNIT_ASSERT( setenv("CMSSW_DATA_PATH", c, 0) == 0 );
    }

//   c = getenv("CMSSW_SEARCH_PATH");
//   if ( c==0 )
//     {
//       c = getenv("CMS_SEARCH_PATH");
//       CPPUNIT_ASSERT( c != 0 );
//       // should not need to overwrite, so last arg to setenv is 0
//       CPPUNIT_ASSERT( setenv("CMSSW_SEARCH_PATH", c, 0) == 0 );
//     }

  // THE FOLLOWING setenv IS DONE FOR TESTING ONLY---THIS SHOULD NOT
  // BE DONE IN REAL CODE. IT SHOULD BE REMOVED WHEN THE CMS
  // ENVIRONMENT HAS BEEN UPDATED TO DEAL WITH DEFINING
  // CMSSW_SEARCH_PATH TO THE CORRECT DEFAULT.
  // setenv is in 4.3+BSD and derivatives. This is not very portable.

  //FIXME: this overrides SCRAM runtime...
  //setenv("CMSSW_SEARCH_PATH", "LOCAL:CMSSW_DATA_PATH", 1);
  
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
    "    FileInPath fip  = 'FWCore/ParameterSet/test/sample.cfg'"
    "    FileInPath topo = 'Geometry/TrackerSimData/data/trackerSimConfiguration.xml'"
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

  edm::ParameterSet innerps = ps->getParameter<edm::ParameterSet>("main");
  edm::FileInPath fip = innerps.getParameter<edm::FileInPath>("fip");
  CPPUNIT_ASSERT( fip.isLocal() == true );
  CPPUNIT_ASSERT( fip.relativePath() == "FWCore/ParameterSet/test/sample.cfg" );
  std::string fullpath = fip.fullPath();
  std::cerr << "fullPath is: " << fip.fullPath() << std::endl;
  std::cerr << "copy of fullPath is: " << fullpath << std::endl;

  CPPUNIT_ASSERT( !fullpath.empty() );

  edm::FileInPath topo = innerps.getParameter<edm::FileInPath>("topo");
  CPPUNIT_ASSERT( topo.isLocal() == false );
  CPPUNIT_ASSERT( topo.relativePath() == "Geometry/TrackerSimData/data/trackerSimConfiguration.xml" );
  fullpath = topo.fullPath();
  CPPUNIT_ASSERT( !fullpath.empty() );

  std::vector<edm::FileInPath> v(1);
  CPPUNIT_ASSERT ( innerps.getAllFileInPaths(v) == 2 );
  
  using boost::lambda::_1;
  using boost::lambda::constant;
  std::cerr << "\nHere comes the vector...\n";
  std::for_each(v.begin(), v.end(), std::cerr << constant("--> ") << _1 << '\n');
  std::cerr << "Done with vector\n";
  CPPUNIT_ASSERT( v.size() == 3 );
  CPPUNIT_ASSERT( std::count(v.begin(), v.end(), fip) == 1 );
  CPPUNIT_ASSERT( std::count(v.begin(), v.end(), topo) == 1 );

  edm::ParameterSet empty;
  v.clear();
  CPPUNIT_ASSERT(  empty.getAllFileInPaths(v) == 0 );
  CPPUNIT_ASSERT( v.empty() );  
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
