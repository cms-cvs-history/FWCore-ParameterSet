/**
  
@file : processbuilder_t.cpp

@brief test suit for process building and schedule validation

@version: $Id: processbuilder_t.cpp,v 1.2 2005/06/20 15:45:04 argiro Exp $
@author : Stefano Argiro
@date : 2005 06 17

*/


#define BOOST_AUTO_TEST_MAIN
#include "boost/test/auto_unit_test.hpp"
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/ParameterSet/interface/ProcessPSetBuilder.h>
#include <FWCore/ParameterSet/src/ScheduleValidator.h>


using namespace edm;

BOOST_AUTO_UNIT_TEST( trivial_path_test )
{
  std::string str = "process X = { \n"
    " module a = A { int32 p = 3 } \n"
    " path p = { a,b,c } \n"
    " }";

  ProcessPSetBuilder b(str);
  boost::shared_ptr<ParameterSet> test = b.getProcessPSet();

  typedef std::vector<std::string> Strs;

  Strs s = (*test).getParameter<std::vector<std::string> >( "p");
  BOOST_CHECK( s[0]=="a" );
  BOOST_CHECK ( b.getDependencies("a")=="");
}

BOOST_AUTO_UNIT_TEST( simple_path_test )
{
  std::string str = "process X = { \n"
    " module a = A { int32 p = 3 } \n"
    " module b = A { int32 p = 3 } \n"
    " module c = A { int32 p = 3 } \n"
    " path p = { a,b,c } \n"
    " }";

  ProcessPSetBuilder b(str);
  boost::shared_ptr<ParameterSet> test = b.getProcessPSet();

  typedef std::vector<std::string> Strs;

  Strs s = (*test).getParameter<std::vector<std::string> >( "p");
  BOOST_CHECK( s[0]=="a" );
  BOOST_CHECK( s[1]=="b" );
  BOOST_CHECK( s[2]=="c" );
  
  BOOST_CHECK ( b.getDependencies("a")=="");
  BOOST_CHECK ( b.getDependencies("b")=="a,");
  BOOST_CHECK ( b.getDependencies("c")=="a,b,");
    
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


  ProcessPSetBuilder b(kTest);
  boost::shared_ptr<ParameterSet> test = b.getProcessPSet();

  typedef std::vector<std::string> Strs;
  
  Strs s = (*test).getParameter<std::vector<std::string> >("path1");
  BOOST_CHECK( s[0]=="cone1" );
  BOOST_CHECK( s[1]=="cone2" );
  BOOST_CHECK( s[2]=="somejet1" );
  BOOST_CHECK( s[3]=="somejet2" );
  BOOST_CHECK( s[4]=="jtanalyzer" );
 


  BOOST_CHECK ( b.getDependencies("cone1")=="");
  BOOST_CHECK ( b.getDependencies("cone2")=="cone1,");
  BOOST_CHECK ( b.getDependencies("somejet1")=="cone1,cone2,");
  BOOST_CHECK ( b.getDependencies("somejet2")=="cone1,cone2,somejet1,");
  BOOST_CHECK ( b.getDependencies("jtanalyzer")=="cone1,cone2,somejet1,somejet2,");

}

BOOST_AUTO_UNIT_TEST( nested_sequence_subst_test )
{
  const char * kTest = "process test = {\n"
   "module a = PhonyConeJet { int32 i = 5 }\n"
   "module b = PhonyConeJet { int32 i = 7 }\n"
   "module c = PhonyJet { int32 i = 7 }\n"
   "module d = PhonyJet { int32 i = 7 }\n"
   "sequence s1 = { a, b }\n"
   "sequence s2 = { s1, c }\n"
   "path path1 = { s2,d }\n"
   "} ";


  ProcessPSetBuilder b(kTest);
  boost::shared_ptr<ParameterSet> test = b.getProcessPSet();

  typedef std::vector<std::string> Strs;
  
  Strs s = (*test).getParameter<std::vector<std::string> >("path1");
  BOOST_CHECK( s[0]=="a" );
  BOOST_CHECK( s[1]=="b" );
  BOOST_CHECK( s[2]=="c" );
  BOOST_CHECK( s[3]=="d" );
 


  BOOST_CHECK ( b.getDependencies("a")=="");
  BOOST_CHECK ( b.getDependencies("b")=="a,");
  BOOST_CHECK ( b.getDependencies("c")=="a,b,");
  BOOST_CHECK ( b.getDependencies("d")=="a,b,c,");


}


BOOST_AUTO_UNIT_TEST( sequence_subst_test2 ){
  const char * kTest = "process test = {\n"
   "module cone1 = PhonyConeJet { int32 i = 5 }\n"
   "module cone2 = PhonyConeJet { int32 i = 7 }\n"
   "module cone3 = PhonyConeJet { int32 i = 7 }\n"
   "module somejet1 = PhonyJet { int32 i = 7 }\n"
   "module somejet2 = PhonyJet { int32 i = 7 }\n"
   "module jtanalyzer = PhonyConeJet { int32 i = 7 }\n"
   "sequence cones = { cone1, cone2, cone3 }\n"
   "sequence jets = { somejet1, somejet2 }\n"
   "path path1 = { cones,jets, jtanalyzer }\n"
   "} ";


  ProcessPSetBuilder b(kTest);
  boost::shared_ptr<ParameterSet> test = b.getProcessPSet();

  typedef std::vector<std::string> Strs;
  
  Strs s = (*test).getParameter<std::vector<std::string> >("path1");
  BOOST_CHECK( s[0]=="cone1" );
  BOOST_CHECK( s[1]=="cone2" );
  BOOST_CHECK( s[2]=="cone3" );
  BOOST_CHECK( s[3]=="somejet1" );
  BOOST_CHECK( s[4]=="somejet2" );
  BOOST_CHECK( s[5]=="jtanalyzer" );
 

  BOOST_CHECK ( b.getDependencies("cone1")=="");
  BOOST_CHECK ( b.getDependencies("cone2")=="cone1,");
  BOOST_CHECK ( b.getDependencies("cone3")=="cone1,cone2,");
  BOOST_CHECK ( b.getDependencies("somejet1")=="cone1,cone2,cone3,");
  BOOST_CHECK ( b.getDependencies("somejet2")=="cone1,cone2,cone3,somejet1,");
  BOOST_CHECK ( b.getDependencies("jtanalyzer")=="cone1,cone2,cone3,somejet1,somejet2,"); 
}

BOOST_AUTO_UNIT_TEST( sequence_subst_test3 ){
   const char * kTest = "process test = {\n"
   "module a = PhonyConeJet { int32 i = 5 }\n"
   "module b = PhonyConeJet { int32 i = 7 }\n"
   "module c = PhonyConeJet { int32 i = 7 }\n"
   "module aa = PhonyJet { int32 i = 7 }\n"
   "module bb = PhonyJet { int32 i = 7 }\n"
   "module cc = PhonyConeJet { int32 i = 7 }\n"
   "module dd = PhonyConeJet { int32 i = 7 }\n"
   "module aaa = PhonyConeJet { int32 i = 7 }\n"
   "module bbb = PhonyConeJet { int32 i = 7 }\n"
   "module ccc = PhonyConeJet { int32 i = 7 }\n"
   "module ddd = PhonyConeJet { int32 i = 7 }\n"
   "module eee = PhonyConeJet { int32 i = 7 }\n"
   "module last = PhonyConeJet { int32 i = 7 }\n" 

   "sequence s1 = { a, b, c }\n"
   "sequence s2 = { aa,bb,cc,dd }\n"
   "sequence s3 = { aaa,bbb,ccc,ddd,eee }\n"
   "path path1 = { s1,s3,s2,last }\n"
   "} ";

 ProcessPSetBuilder b(kTest);
  boost::shared_ptr<ParameterSet> test = b.getProcessPSet();

  typedef std::vector<std::string> Strs;
  
  Strs s = (*test).getParameter<std::vector<std::string> >("path1");
  BOOST_CHECK( s[0]=="a" );
  BOOST_CHECK( s[1]=="b" );
  BOOST_CHECK( s[2]=="c" );
  BOOST_CHECK( s[3]=="aaa" );
  BOOST_CHECK( s[4]=="bbb" );
  BOOST_CHECK( s[5]=="ccc" );
  BOOST_CHECK( s[6]=="ddd" );
  BOOST_CHECK( s[7]=="eee" );
  BOOST_CHECK( s[8]=="aa" );
  BOOST_CHECK( s[9]=="bb" );
  BOOST_CHECK( s[10]=="cc" );
  BOOST_CHECK( s[11]=="dd" );
  BOOST_CHECK( s[12]=="last" );

  

  BOOST_CHECK ( b.getDependencies("a")=="");
  BOOST_CHECK ( b.getDependencies("b")=="a,");
  BOOST_CHECK ( b.getDependencies("c")=="a,b,");
  BOOST_CHECK ( b.getDependencies("aaa")=="a,b,c,");
  BOOST_CHECK ( b.getDependencies("bbb")=="a,aaa,b,c,");
  BOOST_CHECK ( b.getDependencies("ccc")=="a,aaa,b,bbb,c,");
  BOOST_CHECK ( b.getDependencies("ddd")=="a,aaa,b,bbb,c,ccc,");
  BOOST_CHECK ( b.getDependencies("eee")=="a,aaa,b,bbb,c,ccc,ddd,");
  BOOST_CHECK ( b.getDependencies("aa")=="a,aaa,b,bbb,c,ccc,ddd,eee,");
  BOOST_CHECK ( b.getDependencies("bb")=="a,aa,aaa,b,bbb,c,ccc,ddd,eee,");
  BOOST_CHECK ( b.getDependencies("cc")=="a,aa,aaa,b,bb,bbb,c,ccc,ddd,eee,");
  BOOST_CHECK ( b.getDependencies("dd")=="a,aa,aaa,b,bb,bbb,c,cc,ccc,ddd,eee,");
  BOOST_CHECK ( b.getDependencies("last")=="a,aa,aaa,b,bb,bbb,c,cc,ccc,dd,ddd,eee,");

}


BOOST_AUTO_UNIT_TEST( multiple_paths_test){

  const char * kTest = "process test = {\n"
    "module cone1 = PhonyConeJet { int32 i = 5 }\n"
    "module cone2 = PhonyConeJet { int32 i = 7 }\n"
    "module cone3 = PhonyConeJet { int32 i = 7 }\n"
    "module somejet1 = PhonyJet { int32 i = 7 }\n"
    "module somejet2 = PhonyJet { int32 i = 7 }\n"
    "module jtanalyzer = PhonyConeJet { int32 i = 7 }\n"
    "module anotherjtanalyzer = PhonyConeJet { int32 i = 7 }\n"
    "sequence cones = { cone1, cone2, cone3 }\n"
    "sequence jets = { somejet1, somejet2 }\n"
    "path path1 = { cones, jtanalyzer }\n"
    "path path2 = { jets, anotherjtanalyzer }\n"
    "} ";


  ProcessPSetBuilder b(kTest);
  boost::shared_ptr<ParameterSet> test = b.getProcessPSet();
  
  typedef std::vector<std::string> Strs;
  
  Strs s = (*test).getParameter<std::vector<std::string> >("path1");
  BOOST_CHECK( s[0]=="cone1" );
  BOOST_CHECK( s[1]=="cone2" );
  BOOST_CHECK( s[2]=="cone3" );
  BOOST_CHECK( s[3]=="jtanalyzer" );


  BOOST_CHECK ( b.getDependencies("cone1")=="");
  BOOST_CHECK ( b.getDependencies("cone2")=="cone1,");
  BOOST_CHECK ( b.getDependencies("cone3")=="cone1,cone2,");
  BOOST_CHECK ( b.getDependencies("jtanalyzer")=="cone1,cone2,cone3,");
  
  s = (*test).getParameter<std::vector<std::string> >("path2");
  BOOST_CHECK( s[0]=="somejet1" );
  BOOST_CHECK( s[1]=="somejet2" );
  BOOST_CHECK( s[2]=="anotherjtanalyzer" );
 
  BOOST_CHECK ( b.getDependencies("somejet1")=="");
  BOOST_CHECK ( b.getDependencies("somejet2")=="somejet1,");
  BOOST_CHECK ( b.getDependencies("anotherjtanalyzer")=="somejet1,somejet2,");

}


BOOST_AUTO_UNIT_TEST( inconsistent_path_test){
  const char * kTest = "process test = {\n"
   "module a = PhonyConeJet { int32 i = 5 }\n"
   "module b = PhonyConeJet { int32 i = 7 }\n"
   "module c = PhonyJet { int32 i = 7 }\n"
   "path path1 = { (a,b)& (c,b) }\n"
    "} ";

  BOOST_CHECK_THROW(ProcessPSetBuilder b(kTest),
		    std::runtime_error);
}


BOOST_AUTO_UNIT_TEST( inconsistent_multiple_paths_test){

   const char * kTest = "process test = {\n"
   "module cone1 = PhonyConeJet { int32 i = 5 }\n"
   "module cone2 = PhonyConeJet { int32 i = 7 }\n"
   "module somejet1 = PhonyJet { int32 i = 7 }\n"
   "module somejet2 = PhonyJet { int32 i = 7 }\n"
   "module jtanalyzer = PhonyConeJet { int32 i = 7 }\n"
   "sequence cones = { cone1, cone2 }\n"
   "sequence jets = { somejet1, somejet2 }\n"
   "path path1 = { cones, jtanalyzer }\n"
   "path path2 = { jets,  jtanalyzer }\n"
   "} ";

   BOOST_CHECK_THROW(ProcessPSetBuilder b(kTest),
		     std::runtime_error);

}


