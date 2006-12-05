/*
 *  makepset_t.cc
 *  EDMProto
 *
 *  Created by Chris Jones on 5/18/05.
 *  Changed by Viji Sundararajan on 11-Jul-05.
 *
 * $Id: makepset_t.cppunit.cc,v 1.41 2006/11/08 00:30:03 rpw Exp $
 */

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

#include <iostream>

#include <stdlib.h> // for setenv; <cstdlib> is likely to fail

#include "cppunit/extensions/HelperMacros.h"
#include "boost/lambda/lambda.hpp"

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/Makers.h"
#include "FWCore/ParameterSet/interface/parse.h"



class testmakepset: public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(testmakepset);
  //  CPPUNIT_TEST_EXCEPTION(emptyTest,edm::Exception);
  CPPUNIT_TEST(typesTest);
//  CPPUNIT_TEST(usingTest);
//  CPPUNIT_TEST_EXCEPTION(usingExcTest,edm::Exception);
//  CPPUNIT_TEST(psetRefTest);
//  CPPUNIT_TEST_EXCEPTION(psetRefExcTest,edm::Exception);
  CPPUNIT_TEST(secsourceTest);
  CPPUNIT_TEST(usingBlockTest);
  CPPUNIT_TEST(fileinpathTest);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp(){}
  void tearDown(){}
  //  void emptyTest();
  void typesTest();
  //void usingTest();
  //void usingExcTest();
  //void psetRefTest();
  //void psetRefExcTest();
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
    "    untracked vstring fileNames = {'file:main.root'}"
    "    untracked int32 maxEvents = 2"
    "  }"
    "  module out = PoolOutputModule {"
    "    string fileName = 'file:CumHits.root'"
    "  }"
    "  module mix = MixingModule {"
    "    secsource input = PoolSource  {"
    "      untracked vstring fileNames = {'file:pileup.root'}"
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
  edm::ProcessDesc builder(config);
  boost::shared_ptr<edm::ParameterSet> ps = builder.getProcessPSet();

  CPPUNIT_ASSERT(0 != ps.get());

  // Make sure this ParameterSet object has the right contents
  edm::ParameterSet mixingModuleParams = ps->getParameter<edm::ParameterSet>("mix");
  edm::ParameterSet secondarySourceParams = mixingModuleParams.getParameter<edm::ParameterSet>("input");
  CPPUNIT_ASSERT(secondarySourceParams.getParameter<std::string>("@module_type") == "PoolSource"); 
  CPPUNIT_ASSERT(secondarySourceParams.getParameter<std::string>("@module_label") == "input");
  CPPUNIT_ASSERT(secondarySourceParams.getUntrackedParameter<std::vector<std::string> >("fileNames")[0] == "file:pileup.root");
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
    "    untracked vstring fileNames = {'file:main.root'}"
    "    untracked int32 maxEvents = 2"
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
  edm::ProcessDesc builder(config);
  boost::shared_ptr<edm::ParameterSet> ps = builder.getProcessPSet();

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
    "    FileInPath fip  = 'FWCore/ParameterSet/test/sample.cfg'"
    "    FileInPath topo = 'Geometry/TrackerSimData/data/trackersens.xml'"
    "    untracked FileInPath ufip  = 'FWCore/ParameterSet/test/complete.cfg'"
    "  }"
    "  source = DummySource { } "
    "}";

  std::string config(kTest);

  // Create the ParameterSet object from this configuration string.
  edm::ProcessDesc builder(config);
  boost::shared_ptr<edm::ParameterSet> ps = builder.getProcessPSet();
  CPPUNIT_ASSERT(0 != ps.get());
  std::cerr << "\n-----------------------------\n";
  std::cerr << ps->toString();
  std::cerr << "\n-----------------------------\n";

  edm::ParameterSet innerps = ps->getParameter<edm::ParameterSet>("main");
  edm::FileInPath fip  = innerps.getParameter<edm::FileInPath>("fip");
  edm::FileInPath ufip = innerps.getUntrackedParameter<edm::FileInPath>("ufip");
  CPPUNIT_ASSERT( fip.isLocal() == true );
  CPPUNIT_ASSERT( fip.relativePath()  == "FWCore/ParameterSet/test/sample.cfg" );
  CPPUNIT_ASSERT( ufip.relativePath() == "FWCore/ParameterSet/test/complete.cfg" );
  std::string fullpath = fip.fullPath();
  std::cerr << "fullPath is: " << fip.fullPath() << std::endl;
  std::cerr << "copy of fullPath is: " << fullpath << std::endl;

  CPPUNIT_ASSERT( !fullpath.empty() );

  std::string tmpout = fullpath.substr(0, fullpath.find("FWCore/ParameterSet/test/sample.cfg")) + "tmp.cfg";

  edm::FileInPath topo = innerps.getParameter<edm::FileInPath>("topo");
  CPPUNIT_ASSERT( topo.isLocal() == false );
  CPPUNIT_ASSERT( topo.relativePath() == "Geometry/TrackerSimData/data/trackersens.xml" );
  fullpath = topo.fullPath();
  CPPUNIT_ASSERT( !fullpath.empty() );

  std::vector<edm::FileInPath> v(1);
  CPPUNIT_ASSERT ( innerps.getAllFileInPaths(v) == 3 );
  
  using boost::lambda::_1;
  using boost::lambda::constant;
  std::cerr << "\nHere comes the vector...\n";
  std::for_each(v.begin(), v.end(), std::cerr << constant("--> ") << _1 << '\n');
  std::cerr << "Done with vector\n";
  CPPUNIT_ASSERT( v.size() == 4 );
  CPPUNIT_ASSERT( std::count(v.begin(), v.end(), fip) == 1 );
  CPPUNIT_ASSERT( std::count(v.begin(), v.end(), topo) == 1 );

  edm::ParameterSet empty;
  v.clear();
  CPPUNIT_ASSERT(  empty.getAllFileInPaths(v) == 0 );
  CPPUNIT_ASSERT( v.empty() );  

  // This last test checks that a FileInPath parameter can be read
  // successfully even if the associated file no longer exists.
  std::ofstream out(tmpout.c_str());
  CPPUNIT_ASSERT(!(!out));

  const char* kTest2 = 
     "process PROD = {"
    "  PSet main =  {"
    "    FileInPath fip2  = 'tmp.cfg'"
    "  }"
    "  source = DummySource { } "
    "}";

  std::string config2(kTest2);
  // Create the ParameterSet object from this configuration string.
  edm::ProcessDesc builder2(config2);
  unlink(tmpout.c_str());
  boost::shared_ptr<edm::ParameterSet> ps2 = builder2.getProcessPSet();

  CPPUNIT_ASSERT(0 != ps2.get());
  std::cerr << "\n-----------------------------\n";
  std::cerr << ps2->toString();
  std::cerr << "\n-----------------------------\n";

  edm::ParameterSet innerps2 = ps2->getParameter<edm::ParameterSet>("main");
  edm::FileInPath fip2 = innerps2.getParameter<edm::FileInPath>("fip2");
  CPPUNIT_ASSERT( fip2.isLocal() == true );
  CPPUNIT_ASSERT( fip2.relativePath() == "tmp.cfg" );
  std::string fullpath2 = fip2.fullPath();
  std::cerr << "fullPath is: " << fip2.fullPath() << std::endl;
  std::cerr << "copy of fullPath is: " << fullpath2 << std::endl;
  CPPUNIT_ASSERT( !fullpath2.empty() );
}

// This is a wrong test. Empty parameter sets are OK.
                                                                                                                   
// void testmakepset::emptyTest()
// {
//    const char* kTest ="PSet test = { } ";
//    boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
//    CPPUNIT_ASSERT(0 != nodeList.get());
   
//    // this should not be allowed, but is for the time being
//    // only psets coming in from process sections cannot be empty
//    //BOOST_CHECK_THROW(makePSet(*nodeList), std::runtime_error);
//    makePSet(*nodeList);
// }

void testmakepset::typesTest()
{
   //vbool vb = {true, false};
   const char* kTest =
     "uint32 ui=1\n"
     "int32 i = +1\n"
     "double d=1\n"
     "double big = inf\n"
     "double justasbig = +inf\n"
     "double indebt = -inf\n"
     "int32 h1 = 0x004A\n"
     "uint32 h2 = 0Xff\n"
     "untracked uint32 h3 = 0xCFDFEFFF\n" 
     "vuint32 vui ={1,2}\n"
     "vint32 vi = {+1,-2}\n"
     "untracked bool b = true\n"
     "PSet ps = {untracked bool b2=true}\n"
     "string s=\"this string\"\n"
     "string sb2=\"\"\n"
     "string sb1=''\n"
     "string sb3='    '\n"
     "vstring vs={\"1\",\"2\",\"a\"}\n"
     "VPSet vps ={ {bool b3=false} }\n"
     "InputTag input = Label\n"
     "InputTag input2 = Label2:Instance2\n"
     "untracked InputTag input3 = Label3:Instance3\n"
     "InputTag input4 = Label4:Instance4:Process4\n"
     "InputTag input5 = Label5::Process5\n"
     "InputTag input6 = source\n"
     "InputTag input7 = source:sink\n"
     "VInputTag vinput = { l1:i1, l2, l3:i3:p3, l4::p4, source, source:sink }\n"
     "EventID eventID = 1:1\n"
     "VEventID vEventID = {1:1, 2:2, 3:3}\n"
     ;
   
   boost::shared_ptr<edm::ParameterSet> test = edm::pset::makePSet(kTest);
   //std::cout << test->toString() << std::endl;
   
   CPPUNIT_ASSERT(1 == test->getParameter<int>("i"));
   CPPUNIT_ASSERT(test->retrieve("i").isTracked());
   CPPUNIT_ASSERT(1 == test->getParameter<unsigned int>("ui"));
   CPPUNIT_ASSERT(1 == test->getParameter<double>("d"));
   CPPUNIT_ASSERT(100000. < test->getParameter<double>("big"));
   CPPUNIT_ASSERT(100000. < test->getParameter<double>("justasbig"));
   CPPUNIT_ASSERT(-1000000. > test->getParameter<double>("indebt"));

   // test hex numbers
   CPPUNIT_ASSERT(74 == test->getParameter<int>("h1"));
   CPPUNIT_ASSERT(255 == test->getParameter<unsigned int>("h2"));
   CPPUNIT_ASSERT(3487559679U == test->getUntrackedParameter<unsigned int>("h3"));

   //std::cout << test->getParameter<std::string>("s") << std::endl;
   CPPUNIT_ASSERT("this string" == test->getParameter<std::string>("s"));
   std::cout <<"blank string using single quotes returns \""<<test->getParameter<std::string>("sb1")<<"\""<<std::endl;
   std::cout <<"blank string using double quotes returns \""<<test->getParameter<std::string>("sb2")<<"\""<<std::endl;
   CPPUNIT_ASSERT("" == test->getParameter<std::string>("sb1"));
   CPPUNIT_ASSERT("" == test->getParameter<std::string>("sb2"));
   CPPUNIT_ASSERT(4  == test->getParameter<std::string>("sb3").size());
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
   CPPUNIT_ASSERT(true == test->getUntrackedParameter<bool>("b", false));
   CPPUNIT_ASSERT(test->retrieve("vi").isTracked());
   //test->getParameter<std::vector<bool> >("vb");
   edm::ParameterSet ps = test->getParameter<edm::ParameterSet>("ps");
   CPPUNIT_ASSERT(true == ps.getUntrackedParameter<bool>("b2", false));
   std::vector<edm::ParameterSet> vps = test->getParameter<std::vector<edm::ParameterSet> >("vps");
   CPPUNIT_ASSERT(1 == vps.size());
   CPPUNIT_ASSERT(false == vps.front().getParameter<bool>("b3"));
   
   // InputTag
   edm::InputTag inputProduct  = test->getParameter<edm::InputTag>("input");
   edm::InputTag inputProduct2 = test->getParameter<edm::InputTag>("input2");
   edm::InputTag inputProduct3 = test->getUntrackedParameter<edm::InputTag>("input3");
   edm::InputTag inputProduct4 = test->getParameter<edm::InputTag>("input4");
   edm::InputTag inputProduct5 = test->getParameter<edm::InputTag>("input5");
   edm::InputTag inputProduct6 = test->getParameter<edm::InputTag>("input6");
   edm::InputTag inputProduct7 = test->getParameter<edm::InputTag>("input7");

   //edm::OutputTag outputProduct = test->getParameter<edm::OutputTag>("output");

   CPPUNIT_ASSERT("Label"    == inputProduct.label());
   CPPUNIT_ASSERT("Label2"    == inputProduct2.label());
   CPPUNIT_ASSERT("Instance2" == inputProduct2.instance());
   CPPUNIT_ASSERT("Label3"    == inputProduct3.label());
   CPPUNIT_ASSERT("Instance3" == inputProduct3.instance());
   CPPUNIT_ASSERT("Label4" == inputProduct4.label());
   CPPUNIT_ASSERT("Instance4" == inputProduct4.instance());
   CPPUNIT_ASSERT("Process4" == inputProduct4.process());
   CPPUNIT_ASSERT("Label5" == inputProduct5.label());
   CPPUNIT_ASSERT("" == inputProduct5.instance());
   CPPUNIT_ASSERT("Process5" == inputProduct5.process());
   CPPUNIT_ASSERT("source" == inputProduct6.label());
   CPPUNIT_ASSERT("source" == inputProduct7.label());
   

   // vector of InputTags

   std::vector<edm::InputTag> vtags = test->getParameter<std::vector<edm::InputTag> >("vinput");
   CPPUNIT_ASSERT("l1" == vtags[0].label());
   CPPUNIT_ASSERT("i1" == vtags[0].instance());
   CPPUNIT_ASSERT("l2" == vtags[1].label());
   CPPUNIT_ASSERT("l3" == vtags[2].label());
   CPPUNIT_ASSERT("i3" == vtags[2].instance());
   CPPUNIT_ASSERT("p3" == vtags[2].process());
   CPPUNIT_ASSERT("l4" == vtags[3].label());
   CPPUNIT_ASSERT(""   == vtags[3].instance());
   CPPUNIT_ASSERT("p4" == vtags[3].process());
   CPPUNIT_ASSERT("source" == vtags[4].label());
   CPPUNIT_ASSERT("source" == vtags[5].label());

   edm::EventID eventID = test->getParameter<edm::EventID>("eventID");
   std::vector<edm::EventID> vEventID = test->getParameter<std::vector<edm::EventID> >("vEventID");
   CPPUNIT_ASSERT(1 == eventID.run());
   CPPUNIT_ASSERT(1 == eventID.event());
   CPPUNIT_ASSERT(1 == vEventID[0].run());
   CPPUNIT_ASSERT(1 == vEventID[0].event());
   CPPUNIT_ASSERT(3 == vEventID[2].run());
   CPPUNIT_ASSERT(3 == vEventID[2].event());

 
   //CPPUNIT_ASSERT("Label2" == outputProduct.label());
   //CPPUNIT_ASSERT(""       == outputProduct.instance());
   //CPPUNIT_ASSERT("Alias2" == outputProduct.alias());
   //BOOST_CHECK_THROW(makePSet(*nodeList), std::runtime_error);
}

/*
void testmakepset::usingTest()
{
   //vbool vb = {true, false};
   const char* kTest ="using ublock";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   //BOOST_CHECK_THROW(makePSet(*nodeList), std::runtime_error);

   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > blocks;
   const std::string kName("ublock");
   boost::shared_ptr<edm::ParameterSet> ublock(new edm::ParameterSet);
   ublock->insert(true, kName, edm::Entry(true, true));
   blocks.insert(make_pair(kName, ublock));
   boost::shared_ptr<edm::ParameterSet> test = makePSet(*nodeList, blocks);

   CPPUNIT_ASSERT(true == test->getParameter<bool>(kName));
}

void testmakepset::usingExcTest()
{
   //vbool vb = {true, false};
   const char* kTest ="using ublock";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   //BOOST_CHECK_THROW(makePSet(*nodeList), std::runtime_error);
   makePSet(*nodeList);
}

void testmakepset::psetRefTest()
{
   //vbool vb = {true, false};
   const char* kTest ="PSet test = ref";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   //BOOST_CHECK_THROW(makePSet(*nodeList), std::runtime_error);
   
   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > blocks;
   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > psets;
   const std::string kName("ref");
   boost::shared_ptr<edm::ParameterSet> ref(new edm::ParameterSet);
   ref->insert(true, kName, edm::Entry(true, true));
   psets.insert(make_pair(kName, ref));
   boost::shared_ptr<edm::ParameterSet> test = makePSet(*nodeList, blocks,psets);
   
   CPPUNIT_ASSERT(true == test->getParameter<edm::ParameterSet>("test").getParameter<bool>("ref"));
}

void testmakepset::psetRefExcTest()
{
   //vbool vb = {true, false};
   const char* kTest ="PSet test = ref";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   CPPUNIT_ASSERT(0 != nodeList.get());
   
   //BOOST_CHECK_THROW(makePSet(*nodeList), std::runtime_error);
   makePSet(*nodeList);
}
*/
