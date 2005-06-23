/*
 *  makepset_t.cc
 *  EDMProto
 *
 *  Created by Chris Jones on 5/18/05.
 *
 * $Id: makepset_t.cpp,v 1.4 2005/06/18 02:18:10 wmtan Exp $
 */

#include <iostream>
#define BOOST_AUTO_TEST_MAIN
#include "boost/test/auto_unit_test.hpp"

#include "FWCore/ParameterSet/interface/Makers.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include <iostream>

BOOST_AUTO_UNIT_TEST(empty_test)
{
   const char* kTest ="PSet test = { } ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK(0 != nodeList.get());
   
   // this should not be allowed, but is for the time being
   // only psets coming in from process sections cannot be empty
   BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList), std::runtime_error);
}

BOOST_AUTO_UNIT_TEST(types_test)
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
   BOOST_CHECK(0 != nodeList.get());
   
   boost::shared_ptr<edm::ParameterSet> test = edm::pset::makePSet(*nodeList);
   std::cout << test->toString() << std::endl;
   
   BOOST_CHECK(1 == test->getParameter<int>("i"));
   BOOST_CHECK(test->retrieve("i").isTracked());
   BOOST_CHECK(1 == test->getParameter<unsigned int>("ui"));
   BOOST_CHECK(1 == test->getParameter<double>("d"));
   std::cout << test->getParameter<std::string>("s") << std::endl;
   BOOST_CHECK("this string" == test->getParameter<std::string>("s"));
   BOOST_CHECK(3 == test->getParameter<std::vector<std::string> >("vs").size());
   BOOST_CHECK(test->getParameter<std::vector<std::string> >("vs").size() && "1" == test->getParameter<std::vector<std::string> >("vs")[0]);
   BOOST_CHECK(test->getParameter<std::vector<std::string> >("vs").size() >1 && "2" == test->getParameter<std::vector<std::string> >("vs")[1]);
   BOOST_CHECK(test->getParameter<std::vector<std::string> >("vs").size() >1 && "a" == test->getParameter<std::vector<std::string> >("vs")[2]);
   std::cout <<"\""<<test->getParameter<std::vector<std::string> >("vs")[0]<<"\" \""<<test->getParameter<std::vector<std::string> >("vs")[1]<<"\" \""
      <<test->getParameter<std::vector<std::string> >("vs")[2]<<"\""<<std::endl;
   
   static const unsigned int vuia[] = {1,2};
   static const std::vector<unsigned int> vui(vuia, vuia+sizeof(vuia)/sizeof(unsigned int));
   BOOST_CHECK(vui == test->getParameter<std::vector<unsigned int> >("vui"));
   
   static const  int via[] = {1,-2};
   static const std::vector< int> vi(via, via+sizeof(vuia)/sizeof(unsigned int));
   test->getParameter<std::vector<int> >("vi");
   BOOST_CHECK(true == test->getParameter<bool>("b"));
   BOOST_CHECK(test->retrieve("vi").isTracked());
   //test->getParameter<std::vector<bool> >("vb");
   edm::ParameterSet ps = test->getParameter<edm::ParameterSet>("ps");
   BOOST_CHECK(true == ps.getUntrackedParameter<bool>("b2", false));
   std::vector<edm::ParameterSet> vps = test->getParameter<std::vector<edm::ParameterSet> >("vps");
   BOOST_CHECK(1 == vps.size());
   BOOST_CHECK(false == vps.front().getParameter<bool>("b3"));
   
   //BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList), std::runtime_error);

}

BOOST_AUTO_UNIT_TEST(using_test)
{
   //vbool vb = {true, false};
   const char* kTest ="using ublock";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK(0 != nodeList.get());
   
   BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList), std::runtime_error);

   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > blocks;
   const std::string kName("ublock");
   boost::shared_ptr<edm::ParameterSet> ublock(new edm::ParameterSet);
   ublock->insert(true, kName, edm::Entry(true, true));
   blocks.insert(make_pair(kName, ublock));
   boost::shared_ptr<edm::ParameterSet> test = edm::pset::makePSet(*nodeList, blocks);

   BOOST_CHECK(true == test->getParameter<bool>(kName));
}


BOOST_AUTO_UNIT_TEST(pset_ref_test)
{
   //vbool vb = {true, false};
   const char* kTest ="PSet test = ref";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK(0 != nodeList.get());
   
   BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList), std::runtime_error);
   
   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > blocks;
   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > psets;
   const std::string kName("ref");
   boost::shared_ptr<edm::ParameterSet> ref(new edm::ParameterSet);
   ref->insert(true, kName, edm::Entry(true, true));
   psets.insert(make_pair(kName, ref));
   boost::shared_ptr<edm::ParameterSet> test = edm::pset::makePSet(*nodeList, blocks,psets);
   
   BOOST_CHECK(true == test->getParameter<edm::ParameterSet>("test").getParameter<bool>("ref"));
}
