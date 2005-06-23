/**
  
@file : processbuilder_t.cpp

@brief test suit for process building and schedule validation

@version: $Id: processbuilder_t.cpp,v 1.3 2005/06/22 07:19:00 argiro Exp $
@author : Stefano Argiro
@date : 2005 06 17

*/



#include <cppunit/extensions/HelperMacros.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/ParameterSet/interface/ProcessPSetBuilder.h>
#include <FWCore/ParameterSet/src/ScheduleValidator.h>


using namespace edm;

class testProcessPSetBuilder: public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(testProcessPSetBuilder);

  CPPUNIT_TEST(trivialPathTest);
  CPPUNIT_TEST(simplePathTest);
  CPPUNIT_TEST(sequenceSubstitutionTest);
  CPPUNIT_TEST(nestedSequenceSubstitutionTest);
  CPPUNIT_TEST(sequenceSubstitutionTest2);
  CPPUNIT_TEST(sequenceSubstitutionTest3);
  CPPUNIT_TEST(multiplePathsTest);
  CPPUNIT_TEST_EXCEPTION(inconsistentPathTest,std::runtime_error);
  CPPUNIT_TEST_EXCEPTION(inconsistentMultiplePathTest,std::runtime_error);

  CPPUNIT_TEST_SUITE_END();

public:


  void setUp(){}
  void tearDown(){}
  
  void trivialPathTest();
  void simplePathTest();
  void sequenceSubstitutionTest();
  void nestedSequenceSubstitutionTest();
  void sequenceSubstitutionTest2();
  void sequenceSubstitutionTest3();
  void multiplePathsTest();
  void inconsistentPathTest();
  void inconsistentMultiplePathTest();
    

}; 

///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testProcessPSetBuilder);


void testProcessPSetBuilder::trivialPathTest(){

  std::string str = "process X = { \n"
    " module a = A { int32 p = 3 } \n"
    " path p = { a,b,c } \n"
    " }";

  ProcessPSetBuilder b(str);
  boost::shared_ptr<ParameterSet> test = b.getProcessPSet();

  typedef std::vector<std::string> Strs;

  Strs s = (*test).getParameter<std::vector<std::string> >( "p");
  CPPUNIT_ASSERT( s[0]=="a" );
  CPPUNIT_ASSERT( b.getDependencies("a")=="");
}

void testProcessPSetBuilder::simplePathTest(){

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
  CPPUNIT_ASSERT( s[0]=="a" );
  CPPUNIT_ASSERT( s[1]=="b" );
  CPPUNIT_ASSERT( s[2]=="c" );
  
  CPPUNIT_ASSERT ( b.getDependencies("a")=="");
  CPPUNIT_ASSERT ( b.getDependencies("b")=="a,");
  CPPUNIT_ASSERT ( b.getDependencies("c")=="a,b,");

}


void testProcessPSetBuilder:: sequenceSubstitutionTest (){

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
  CPPUNIT_ASSERT( s[0]=="cone1" );
  CPPUNIT_ASSERT( s[1]=="cone2" );
  CPPUNIT_ASSERT( s[2]=="somejet1" );
  CPPUNIT_ASSERT( s[3]=="somejet2" );
  CPPUNIT_ASSERT( s[4]=="jtanalyzer" );
 


  CPPUNIT_ASSERT ( b.getDependencies("cone1")=="");
  CPPUNIT_ASSERT ( b.getDependencies("cone2")=="cone1,");
  CPPUNIT_ASSERT ( b.getDependencies("somejet1")=="cone1,cone2,");
  CPPUNIT_ASSERT ( b.getDependencies("somejet2")=="cone1,cone2,somejet1,");
  CPPUNIT_ASSERT ( b.getDependencies("jtanalyzer")=="cone1,cone2,somejet1,somejet2,");

}

void testProcessPSetBuilder::nestedSequenceSubstitutionTest(){

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
  CPPUNIT_ASSERT( s[0]=="a" );
  CPPUNIT_ASSERT( s[1]=="b" );
  CPPUNIT_ASSERT( s[2]=="c" );
  CPPUNIT_ASSERT( s[3]=="d" );
 


  CPPUNIT_ASSERT ( b.getDependencies("a")=="");
  CPPUNIT_ASSERT ( b.getDependencies("b")=="a,");
  CPPUNIT_ASSERT ( b.getDependencies("c")=="a,b,");
  CPPUNIT_ASSERT ( b.getDependencies("d")=="a,b,c,");


}


void testProcessPSetBuilder::sequenceSubstitutionTest2(){

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
  CPPUNIT_ASSERT( s[0]=="cone1" );
  CPPUNIT_ASSERT( s[1]=="cone2" );
  CPPUNIT_ASSERT( s[2]=="cone3" );
  CPPUNIT_ASSERT( s[3]=="somejet1" );
  CPPUNIT_ASSERT( s[4]=="somejet2" );
  CPPUNIT_ASSERT( s[5]=="jtanalyzer" );
 

  CPPUNIT_ASSERT ( b.getDependencies("cone1")=="");
  CPPUNIT_ASSERT ( b.getDependencies("cone2")=="cone1,");
  CPPUNIT_ASSERT ( b.getDependencies("cone3")=="cone1,cone2,");
  CPPUNIT_ASSERT ( b.getDependencies("somejet1")=="cone1,cone2,cone3,");
  CPPUNIT_ASSERT ( b.getDependencies("somejet2")=="cone1,cone2,cone3,somejet1,");
  CPPUNIT_ASSERT ( b.getDependencies("jtanalyzer")=="cone1,cone2,cone3,somejet1,somejet2,"); 
}

void testProcessPSetBuilder::sequenceSubstitutionTest3(){

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
  CPPUNIT_ASSERT( s[0]=="a" );
  CPPUNIT_ASSERT( s[1]=="b" );
  CPPUNIT_ASSERT( s[2]=="c" );
  CPPUNIT_ASSERT( s[3]=="aaa" );
  CPPUNIT_ASSERT( s[4]=="bbb" );
  CPPUNIT_ASSERT( s[5]=="ccc" );
  CPPUNIT_ASSERT( s[6]=="ddd" );
  CPPUNIT_ASSERT( s[7]=="eee" );
  CPPUNIT_ASSERT( s[8]=="aa" );
  CPPUNIT_ASSERT( s[9]=="bb" );
  CPPUNIT_ASSERT( s[10]=="cc" );
  CPPUNIT_ASSERT( s[11]=="dd" );
  CPPUNIT_ASSERT( s[12]=="last" );

  

  CPPUNIT_ASSERT ( b.getDependencies("a")=="");
  CPPUNIT_ASSERT ( b.getDependencies("b")=="a,");
  CPPUNIT_ASSERT ( b.getDependencies("c")=="a,b,");
  CPPUNIT_ASSERT ( b.getDependencies("aaa")=="a,b,c,");
  CPPUNIT_ASSERT ( b.getDependencies("bbb")=="a,aaa,b,c,");
  CPPUNIT_ASSERT ( b.getDependencies("ccc")=="a,aaa,b,bbb,c,");
  CPPUNIT_ASSERT ( b.getDependencies("ddd")=="a,aaa,b,bbb,c,ccc,");
  CPPUNIT_ASSERT ( b.getDependencies("eee")=="a,aaa,b,bbb,c,ccc,ddd,");
  CPPUNIT_ASSERT ( b.getDependencies("aa")=="a,aaa,b,bbb,c,ccc,ddd,eee,");
  CPPUNIT_ASSERT ( b.getDependencies("bb")=="a,aa,aaa,b,bbb,c,ccc,ddd,eee,");
  CPPUNIT_ASSERT ( b.getDependencies("cc")=="a,aa,aaa,b,bb,bbb,c,ccc,ddd,eee,");
  CPPUNIT_ASSERT ( b.getDependencies("dd")=="a,aa,aaa,b,bb,bbb,c,cc,ccc,ddd,eee,");
  CPPUNIT_ASSERT ( b.getDependencies("last")=="a,aa,aaa,b,bb,bbb,c,cc,ccc,dd,ddd,eee,");

}


void testProcessPSetBuilder::multiplePathsTest(){

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
  CPPUNIT_ASSERT( s[0]=="cone1" );
  CPPUNIT_ASSERT( s[1]=="cone2" );
  CPPUNIT_ASSERT( s[2]=="cone3" );
  CPPUNIT_ASSERT( s[3]=="jtanalyzer" );


  CPPUNIT_ASSERT ( b.getDependencies("cone1")=="");
  CPPUNIT_ASSERT ( b.getDependencies("cone2")=="cone1,");
  CPPUNIT_ASSERT ( b.getDependencies("cone3")=="cone1,cone2,");
  CPPUNIT_ASSERT ( b.getDependencies("jtanalyzer")=="cone1,cone2,cone3,");
  
  s = (*test).getParameter<std::vector<std::string> >("path2");
  CPPUNIT_ASSERT( s[0]=="somejet1" );
  CPPUNIT_ASSERT( s[1]=="somejet2" );
  CPPUNIT_ASSERT( s[2]=="anotherjtanalyzer" );
 
  CPPUNIT_ASSERT ( b.getDependencies("somejet1")=="");
  CPPUNIT_ASSERT ( b.getDependencies("somejet2")=="somejet1,");
  CPPUNIT_ASSERT ( b.getDependencies("anotherjtanalyzer")=="somejet1,somejet2,");

}


void testProcessPSetBuilder::inconsistentPathTest(){

  const char * kTest = "process test = {\n"
   "module a = PhonyConeJet { int32 i = 5 }\n"
   "module b = PhonyConeJet { int32 i = 7 }\n"
   "module c = PhonyJet { int32 i = 7 }\n"
   "path path1 = { (a,b)& (c,b) }\n"
    "} ";

  ProcessPSetBuilder b(kTest);
}


void testProcessPSetBuilder::inconsistentMultiplePathTest(){

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

ProcessPSetBuilder b(kTest);

}


