/*
 *  makepset_t.cc
 *  EDMProto
 *
 *  Created by Chris Jones on 5/18/05.
 *
 * $Id: makepset_t.cc,v 1.6 2005/05/27 03:47:12 jbk Exp $
 */

#include <iostream>
#define BOOST_AUTO_TEST_MAIN
#include "boost/test/auto_unit_test.hpp"

#include "FWCore/ParameterSet/interface/Makers.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include <iostream>

BOOST_AUTO_UNIT_TEST( empty_test )
{
   const char* kTest ="PSet test = { } ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK( 0 != nodeList.get() );
   
   // this should not be allowed, but is for the time being
   // only psets coming in from process sections cannot be empty
   BOOST_CHECK_THROW(edm::pset::makePSet( *nodeList ), std::runtime_error );
}

BOOST_AUTO_UNIT_TEST( types_test )
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
   BOOST_CHECK( 0 != nodeList.get() );
   
   boost::shared_ptr<edm::ParameterSet> test = edm::pset::makePSet(*nodeList );
   std::cout << test->toString() << std::endl;
   
   BOOST_CHECK( 1 == test->getInt32("i") );
   BOOST_CHECK( test->retrieve("i").isTracked() );
   BOOST_CHECK( 1 == test->getUInt32("ui"));
   BOOST_CHECK( 1 == test->getDouble("d"));
   std::cout <<test->getString("s")<<std::endl;
   BOOST_CHECK( "this string" == test->getString("s") );
   BOOST_CHECK( 3 == test->getVString("vs").size() );
   BOOST_CHECK( test->getVString("vs").size() && "1" == test->getVString("vs")[0] );
   BOOST_CHECK( test->getVString("vs").size() >1 && "2" == test->getVString("vs")[1] );
   BOOST_CHECK( test->getVString("vs").size() >1 && "a" == test->getVString("vs")[2] );
   std::cout <<"\""<<test->getVString("vs")[0]<<"\" \""<<test->getVString("vs")[1]<<"\" \""
      <<test->getVString("vs")[2]<<"\""<<std::endl;
   
   static const unsigned int vuia[] = {1,2};
   static const std::vector<unsigned int> vui(vuia, vuia+sizeof(vuia)/sizeof(unsigned int));
   BOOST_CHECK( vui == test->getVUInt32("vui") );
   
   static const  int via[] = {1,-2};
   static const std::vector< int> vi(via, via+sizeof(vuia)/sizeof(unsigned int));
   test->getVInt32("vi");
   BOOST_CHECK( true == test->getBool("b") );
   BOOST_CHECK( test->retrieve("vi").isTracked() );
   //test->get_vBool("vb");
   edm::ParameterSet ps = test->getPSet("ps");
   BOOST_CHECK( true == ps.getBool("b2") );
   std::vector<edm::ParameterSet> vps = test->getVPSet("vps");
   BOOST_CHECK( 1 == vps.size() );
   BOOST_CHECK( false == vps.front().getBool("b3") );
   
   //BOOST_CHECK_THROW(edm::pset::makePSet( *nodeList ), std::runtime_error );

}

BOOST_AUTO_UNIT_TEST( using_test )
{
   //vbool vb = {true, false};
   const char* kTest ="using ublock";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK( 0 != nodeList.get() );
   
   BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList ), std::runtime_error);

   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > blocks;
   const std::string kName("ublock");
   boost::shared_ptr<edm::ParameterSet> ublock( new edm::ParameterSet);
   ublock->insert(true, kName, edm::Entry(true, true) );
   blocks.insert( make_pair(kName, ublock ) );
   boost::shared_ptr<edm::ParameterSet> test = edm::pset::makePSet(*nodeList, blocks );

   BOOST_CHECK( true == test->getBool(kName) );
}


BOOST_AUTO_UNIT_TEST( pset_ref_test )
{
   //vbool vb = {true, false};
   const char* kTest ="PSet test = ref";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK( 0 != nodeList.get() );
   
   BOOST_CHECK_THROW(edm::pset::makePSet(*nodeList ), std::runtime_error);
   
   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > blocks;
   std::map< std::string, boost::shared_ptr<edm::ParameterSet> > psets;
   const std::string kName("ref");
   boost::shared_ptr<edm::ParameterSet> ref( new edm::ParameterSet);
   ref->insert(true, kName, edm::Entry(true, true) );
   psets.insert( make_pair(kName, ref ) );
   boost::shared_ptr<edm::ParameterSet> test = edm::pset::makePSet(*nodeList, blocks,psets );
   
   BOOST_CHECK( true == test->getPSet("test").getBool("ref") );
}
