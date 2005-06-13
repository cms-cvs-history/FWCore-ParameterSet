/*
 *  makeprocess_t.cc
 *  EDMProto
 *
 *  Created by Chris Jones on 5/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 * 
 * $Id: makeprocess_t.cpp,v 1.4 2005/06/03 13:31:42 argiro Exp $
 */

#include <iostream>

#define BOOST_AUTO_TEST_MAIN
#include "boost/test/auto_unit_test.hpp"

#include "FWCore/ParameterSet/interface/Makers.h"
#include "FWCore/ParameterSet/interface/parse.h"
#include "FWCore/ParameterSet/interface/MakeProcessPSet.h"

#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

BOOST_AUTO_UNIT_TEST( simple_process_test )
{
   const char* kTest ="process test = { PSet dummy = {bool b = true} } ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK( 0 != nodeList.get() );
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess( nodeList );

   BOOST_CHECK( edm::getP<std::string>(test->pset_, "process_name") == "test" );   
}

BOOST_AUTO_UNIT_TEST( using_test )
{
   const char* kTest ="process test = { PSet dummy = {bool b = true}\n"
   " block dummy2 = {bool d = true} \n"
   " module include = Dummy {using dummy} \n"
   " module include2 = Dummy2 {using dummy2} } ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK( 0 != nodeList.get() );
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess( nodeList );
   
   //BOOST_CHECK( edm::getP<edm::ParameterSet>(test->pset_, "dummy").getBool("b") == true );   
   BOOST_CHECK( edm::getP<bool>(edm::getP<edm::ParameterSet>(test->pset_, "include"), "b") == true );   
   BOOST_CHECK( edm::getP<bool>(edm::getP<edm::ParameterSet>(test->pset_, "include2"), "d") == true );   
}

BOOST_AUTO_UNIT_TEST( path_test )
{
   const char* kTest ="process test = {\n"
   "module cone5 = PhonyConeJet { int32 i = 5 }\n"
   "module cone7 = PhonyConeJet { int32 i = 7 }\n"
   "sequence cones = { cone5, cone7 }\n" //NOTE: now requires curly braces
   "path term1 = { cones & jtanalyzer }\n"
   "endpath atEnd = {all,some}\n"
   "} ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK( 0 != nodeList.get() );
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess( nodeList );
   
   BOOST_CHECK( edm::getP<std::string>(test->pset_, "process_name") == "test" );
   BOOST_CHECK( test->pathFragments_.size() == 3 );

   const edm::ParameterSet& myparams = test->pset_;
   std::cout << "ParameterSet looks like:\n";
   std::cout << myparams.toString() << std::endl;
}

BOOST_AUTO_UNIT_TEST( simple_path_test )
{
  std::string str = "process X = { \n"
    " module a = A { int32 p = 3 } \n"
    " module b = A { int32 p = 3 } \n"
    " module c = A { int32 p = 3 } \n"
    " path p = { a,b,c } \n"
    " }";

  boost::shared_ptr<edm::ParameterSet> test = edm::makeProcessPSet(str);

  typedef std::vector<std::string> Strs;

  Strs s = edm::getP<std::vector<std::string> >(*test, "p");
  BOOST_CHECK( s[0]=="a" );
  BOOST_CHECK( s[1]=="b" );
  BOOST_CHECK( s[2]=="c" );
  
  std::cout << s[0] << " " << s[1] << " " << s[2] << std::endl;
  std::cout << test->toString() << std::endl;
}

edm::ParameterSet modulePSet( const std::string& iLabel, const std::string& iType ) {
   edm::ParameterSet temp;
   temp.insert( true , "s", edm::Entry(1,true) );
   temp.insert(true, "module_label", edm::Entry(iLabel, true) );
   temp.insert(true, "module_type", edm::Entry(iType, true) );
   return temp;
}
BOOST_AUTO_UNIT_TEST( module_test )
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
   BOOST_CHECK( 0 != nodeList.get() );
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess( nodeList );

   static const edm::ParameterSet kEmpty;
   const edm::ParameterSet kCone(modulePSet("cones", "Module") );
   std::cout <<kCone.toString()<<std::endl;
   std::cout <<edm::getP<edm::ParameterSet>(test->pset_, "cones").toString()<<std::endl;
   
   const edm::ParameterSet kMainInput(modulePSet("main_input","InputService") );
   const edm::ParameterSet kOther(modulePSet("other","OtherInputService") );
   
   const edm::ParameterSet kNoLabelModule(modulePSet("", "NoLabelModule") );
   const edm::ParameterSet kLabelModule(modulePSet("labeled", "LabelModule") );
   const edm::ParameterSet kNoLabelRetriever(modulePSet("", "NoLabelRetriever") );
   const edm::ParameterSet kLabelRetriever(modulePSet("label", "LabelRetriever") );
   
   BOOST_CHECK( kEmpty != ( edm::getP<edm::ParameterSet>(test->pset_, "cones") ) );
   BOOST_CHECK( kCone == edm::getP<edm::ParameterSet>(test->pset_, "cones") );
   
   BOOST_CHECK( kEmpty != ( edm::getP<edm::ParameterSet>(test->pset_, "main_input") ) );
   BOOST_CHECK( kMainInput == ( edm::getP<edm::ParameterSet>(test->pset_, "main_input") ) );

   BOOST_CHECK( kEmpty != ( edm::getP<edm::ParameterSet>(test->pset_, "other") ) );
   BOOST_CHECK( kOther == ( edm::getP<edm::ParameterSet>(test->pset_, "other") ) );
   
   BOOST_CHECK( kEmpty != ( edm::getP<edm::ParameterSet>(test->pset_, "NoLabelModule@") ) );
   BOOST_CHECK( kNoLabelModule == edm::getP<edm::ParameterSet>(test->pset_, "NoLabelModule@") );
   
   BOOST_CHECK( kEmpty != ( edm::getP<edm::ParameterSet>(test->pset_, "LabelModule@labeled") ) );
   BOOST_CHECK( kLabelModule == edm::getP<edm::ParameterSet>(test->pset_, "LabelModule@labeled") );

   BOOST_CHECK( kEmpty != ( edm::getP<edm::ParameterSet>(test->pset_, "NoLabelRetriever@") ) );
   BOOST_CHECK( kNoLabelRetriever == edm::getP<edm::ParameterSet>(test->pset_, "NoLabelRetriever@") );

   BOOST_CHECK( kEmpty != ( edm::getP<edm::ParameterSet>(test->pset_, "LabelRetriever@label") ) );
   BOOST_CHECK( kLabelRetriever == edm::getP<edm::ParameterSet>(test->pset_, "LabelRetriever@label") );
}

BOOST_AUTO_UNIT_TEST( empty_module_test )
{
   const char* kTest ="process test = {\n"
   "#PSet thing1 = {  }\n"
   "module thing = XX {  }\n"
   "} ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK( 0 != nodeList.get() );
   
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess( nodeList );
   
   BOOST_CHECK( edm::getP<std::string>(test->pset_, "process_name") == "test" );

   const edm::ParameterSet& myparams = test->pset_;
   std::cout << "ParameterSet looks like:\n";
   std::cout << myparams.toString() << std::endl;
}

BOOST_AUTO_UNIT_TEST( empty_pset_test )
{
   const char* kTest ="process test = {\n"
   "PSet thing1 = {  }\n"
   "module thing = XX {  }\n"
   "} ";
   boost::shared_ptr<edm::pset::NodePtrList> nodeList = edm::pset::parse(kTest);
   BOOST_CHECK( 0 != nodeList.get() );
   
   try {
   boost::shared_ptr<edm::ProcessDesc> test = edm::pset::makeProcess( nodeList );
   }
   catch(std::exception& e)
   {
       std::cout << "empty pset detected" << std::endl;
	return ;
    }

   throw  std::runtime_error("empty pset not discovered");
}


BOOST_AUTO_UNIT_TEST( sequence_subst_test )
{
  const char * kTest = "process test = {\n"
   "module cone1 = PhonyConeJet { int32 i = 5 }\n"
   "module cone2 = PhonyConeJet { int32 i = 7 }\n"
   "module somejet1 = PhonyJet { int32 i = 7 }\n"
   "module somejet2 = PhonyJet { int32 i = 7 }\n"
   "module jtanalyzer = PhonyConeJet { int32 i = 7 }\n"
   "sequence cones = { cone1, cone2 }\n"
   "sequence jets = { somejet1, somejet2 }\n"
   "path path1 = { cones,jets, jtanalyzer }\n"
   "} ";

  boost::shared_ptr<edm::ParameterSet> test = edm::makeProcessPSet(kTest);
  
  typedef std::vector<std::string> Strs;
  
  Strs s = edm::getP<std::vector<std::string> >(*test, "path1");
  BOOST_CHECK( s[0]=="cone1" );
  BOOST_CHECK( s[1]=="cone2" );
  BOOST_CHECK( s[2]=="somejet1" );
  BOOST_CHECK( s[3]=="somejet2" );
  BOOST_CHECK( s[4]=="jtanalyzer" );
  

}

BOOST_AUTO_UNIT_TEST( multiple_paths_test){

   const char * kTest = "process test = {\n"
   "module cone1 = PhonyConeJet { int32 i = 5 }\n"
   "module cone2 = PhonyConeJet { int32 i = 7 }\n"
   "module somejet1 = PhonyJet { int32 i = 7 }\n"
   "module somejet2 = PhonyJet { int32 i = 7 }\n"
   "module jtanalyzer = PhonyConeJet { int32 i = 7 }\n"
   "sequence cones = { cone1, cone2 }\n"
   "sequence jets = { somejet1, somejet2 }\n"
   "path path1 = { cones, jtanalyzer }\n"
   "path path2 = { jets, jtanalyzer }\n"
   "} ";

  boost::shared_ptr<edm::ParameterSet> test = edm::makeProcessPSet(kTest);
  
  typedef std::vector<std::string> Strs;

  Strs s = edm::getP<std::vector<std::string> >(*test, "path1");
  BOOST_CHECK( s[0]=="cone1" );
  BOOST_CHECK( s[1]=="cone2" );
  BOOST_CHECK( s[2]=="jtanalyzer" );


  s = edm::getP<std::vector<std::string> >(*test, "path2");
  BOOST_CHECK( s[0]=="somejet1" );
  BOOST_CHECK( s[1]=="somejet2" );
  BOOST_CHECK( s[2]=="jtanalyzer" );

}
