/*
 *  makepset_t.cc
 *  EDMProto
 *
 *  Created by Chris Jones on 5/18/05.
 *  Changed by Viji Sundararajan on 11-Jul-05.
 *
 * $Id: makepset_t.cppunit.cc,v 1.1 2005/07/11 13:43:48 viji Exp $
 */

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/Makers.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include <iostream>

class testmakepset: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(testmakepset);
CPPUNIT_TEST_EXCEPTION(emptyTest,edm::Exception);
CPPUNIT_TEST(typesTest);
CPPUNIT_TEST(usingTest);
CPPUNIT_TEST_EXCEPTION(usingExcTest,edm::Exception);
CPPUNIT_TEST(psetRefTest);
CPPUNIT_TEST_EXCEPTION(psetRefExcTest,edm::Exception);
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
};
                                                                                                                   
///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testmakepset);
                                                                                                                   
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
   std::cout << test->toString() << std::endl;
   
   CPPUNIT_ASSERT(1 == test->getParameter<int>("i"));
   CPPUNIT_ASSERT(test->retrieve("i").isTracked());
   CPPUNIT_ASSERT(1 == test->getParameter<unsigned int>("ui"));
   CPPUNIT_ASSERT(1 == test->getParameter<double>("d"));
   std::cout << test->getParameter<std::string>("s") << std::endl;
   CPPUNIT_ASSERT("this string" == test->getParameter<std::string>("s"));
   CPPUNIT_ASSERT(3 == test->getParameter<std::vector<std::string> >("vs").size());
   CPPUNIT_ASSERT(test->getParameter<std::vector<std::string> >("vs").size() && "1" == test->getParameter<std::vector<std::string> >("vs")[0]);
   CPPUNIT_ASSERT(test->getParameter<std::vector<std::string> >("vs").size() >1 && "2" == test->getParameter<std::vector<std::string> >("vs")[1]);
   CPPUNIT_ASSERT(test->getParameter<std::vector<std::string> >("vs").size() >1 && "a" == test->getParameter<std::vector<std::string> >("vs")[2]);
   std::cout <<"\""<<test->getParameter<std::vector<std::string> >("vs")[0]<<"\" \""<<test->getParameter<std::vector<std::string> >("vs")[1]<<"\" \""
      <<test->getParameter<std::vector<std::string> >("vs")[2]<<"\""<<std::endl;
   
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
