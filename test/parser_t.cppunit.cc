#include "FWCore/ParameterSet/interface/parse.h"
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include <fstream>
#include "FWCore/Utilities/interface/Algorithms.h"


//------------------------------------------------------------
// Test program for parameter set specification and configuration file
// parser.
// Changed by Viji on 13/07/2005
//------------------------------------------------------------

void read_from_stdin(std::string& buffer)
{
  std::string line;
  while (std::getline(std::cin, line)) { buffer += line; buffer += '\n'; }
}
                                                                                                                   
class testparser: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(testparser);
CPPUNIT_TEST(mainparser1Test);
CPPUNIT_TEST(mainparser2Test);
CPPUNIT_TEST(mainparser3Test);
CPPUNIT_TEST(mainparser4Test);
CPPUNIT_TEST(mainparser5Test);
CPPUNIT_TEST(mainparserLooperTest);
CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown(){}
  void mainparser1Test();
  void mainparser2Test();
  void mainparser3Test();
  void mainparser4Test();
  void mainparser5Test();
  void mainparserLooperTest();
};
                                                                                                            
///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testparser);
                                                                                                            
void testparser::mainparser1Test()
{
const char* spec = 
"bool b1 = true\n"
"untracked bool b2 = false\n"
"bool b3 = T\n"
"bool b4 = F\n"
"bool b5  = yes   bool b6 = 0 bool b7 = 1 bool b8 = no\n"
"bool b9 = on\n"
"bool b10 = off\n"
"int32 i1 = 1\n"
"int32 i2 = 10 int32 i3 = 0x1234\n"
"int32 i4 = -2\n"
"vint32 vi1 = { 1 }\n"
"vint32 vi3 = {1, 2, 3}\n"
"vint32 vi2 = { }\n"
"vint32 vi4 = {}\n"
"string s1=\"s1test\"\n"
"string s2 = \"s2test\"  string s3= \"s3test\"\n"
"string s4 = \" \"t\"  \"\n"
"string s5 = \" 'y' \" \n"
"string s6 = \"this\" \n"
"string s1a='s1test' \n"
"string s2a = 's2test'  string s3a= 's3test' \n"
"string s4a = ' \"t\" ' \n"
"string s5a = ' \'t\' ' \n"
"string s6a = 'this' \n"
"vstring vs1 = { \"1\",\"2\" , \"3\"} \n"
"vstring vs2={'1','2', '3'} \n"
"double d1 = 5. \n"
"double d2 = 0.5 \n"
"double d3=5.00 \n"
"double d4=.004 \n"
"double d5=2 \n"
"double d6=-4.0 \n"
"double d7=-4 \n"
"double d8=2E+3 \n"
"double d9=2E-3 \n"
"double d10=+2E+3 \n"
"double d11=-2E-3 \n"
"double d12=+4 \n"
"vdouble vd1={ 1,2,3.3,4.,5.0,.6,0.7,.08,9.00} \n"
"vdouble vd2={} \n"
"vdouble vd3={ 1E-03,1E+4,-1E-3,+1E-4 } \n"
"vdouble vd4={ -1,-1.1, -.1 ,-.01,-1., +0 } \n"
"vdouble vd5={ +1,+1.1, +.1 ,+.01,+1., -0 } \n"
"uint32 u1=5 \n"
"vuint32 u2={  1,2, 3, 4 ,5 } \n"
"uint32 u3=0xCFDFEFFF\n"
"PSet p1 = { bool b1=true int32 i1=3 double q=9.00} \n"
"PSet px = { int32 i=1 \n"
"            uint32 ui=1 \n"
"            PSet px = { int32 j = 1 } \n"
"          } \n"
"#soon... \n"
"PSet px2 = 0x1234123412341234 \n"
"# delayed \n"
"# PSet px3 = /x/y/z \n"
"# ok \n"
"VPSet vp1 = { p1 } \n"
"VPSet vp1a = { p1,px } \n"
"VPSet vp1b = { { int32 i=1 } } \n"
"VPSet vp1c = { { int32 i=1 },{ int32 i=1 },p1 } \n"
"VPSet vp2 = { \n"
"      { int32 i=1 }, \n"
"      p1, \n"
"      px, \n"
"      { int32 i=2 int32 j=3 } \n"
"} \n";
       
  std::cout << std::endl;
  return;
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      std::cerr << "Null output from parser" << std::endl;
    }

  edm::copy_all(*pr, std::ostream_iterator<edm::pset::NodePtr>(std::cout,"\n"));
}
                                                                                                            
void testparser::mainparser2Test()
{
const char* spec = 
"process USER =\n"
"{\n"
"source = PoolInput\n"
"{\n"
"string filename = \"here\" \n"  
"}\n"
"source s2 = PoolInput\n"
"{\n"
"string filename = \"not here\"\n" 
"}\n"
"block p1 = {\n"
"int32 x = 2\n"
"int32 y = 9\n"
"}\n"
"PSet  p2 = {\n"
"using p1\n"
"int32 x = 3\n"
"}\n"
"module m1 = MidpointJetProducer\n"
"{\n"
"using p2\n"
"}\n"
"module m2 = MidpointJetProducer\n"
"{\n"
"using p2\n"
"int32 x=4\n"
"}\n"
"block b1 = { double y = 1.1 }\n"
"block b2 = { double x = 2.2 }\n"
"module A = UnconfigurableModule { }\n"
"module B = UnconfigurableModule { }\n"
"module C = UnconfigurableModule { }\n"
"module D = UnconfigurableModule { }\n"
"module E = UnconfigurableModule { }\n"
"module G = UnconfigurableModule { }\n"
"module source = UnconfigurableModule { }\n"
"module filter = UnconfigurableFilter { }\n"
"sequence s0a = { A}\n"
"sequence s0b = { A,B}\n"
"sequence s0c = { A&B}\n"
"sequence s1 = { A,B&C,(D,E)&G }\n"
"sequence s2 = { C&(A,B), m1,m2,source,s1 }\n"
"path t1 = { (A,B&C,D),s0a,filter }\n"
"path t2 = { A,B,C,D }\n"
"endpath te = { A&B }\n"
"}\n";
std::cout << std::endl;
  return ;
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      std::cerr << "Null output from parser" << std::endl;
    }

  edm::copy_all(*pr, std::ostream_iterator<edm::pset::NodePtr>(std::cout,"\n"));
}
                                                                                                            
void testparser::mainparser3Test()
{
const char* spec = 
"process PROD = {\n"
"#source = PoolInput(p1)\n"
"#source s2 = PoolInput(p2)\n"
"PSet p1 = { int32 x = 2 }\n"
"PSet p2 = {\n"
"using b1\n"
"int32 x = 3\n"
"}\n"
"block b1 = { double y = 1.1 }\n"
"#block b2 = { double x = 2.2 }\n"
"path t1 = { (A,B&C,D),F }\n"
"path t2 = { A,B,C,D }\n"
"endpath te = { A&B }\n"
"}\n";
std::cout << std::endl;
  return ;
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      std::cerr << "Null output from parser" << std::endl;
    }

  edm::copy_all(*pr, std::ostream_iterator<edm::pset::NodePtr>(std::cout,"\n"));
}
                                                                                                            
void testparser::mainparser4Test()
{
const char* spec = 
"VPSet vp1 = { p1 } \n"
"VPSet vp1a = { p1,px } \n"
"VPSet vp1b = { { int32 i=1 } } \n"
"VPSet vp1c = { { int32 i=1 },{ int32 i=1 },p1 } \n"
                                                                                                   
"VPSet vp2 = { \n"
"      { int32 i=1 }, \n"
"      p1, \n"
"      px, \n"
"      { int32 i=2 int32 j=3 } \n"
"} \n";
std::cout << std::endl;
  return;
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      std::cerr << "Null output from parser" << std::endl;
    }

  edm::copy_all(*pr, std::ostream_iterator<edm::pset::NodePtr>(std::cout,"\n"));
}
                                                                                                            
void testparser::mainparser5Test()
{
const char* spec = 
"process PROD =\n"
"{\n"
"        source = PoolInput { }\n"
"        es_source = S1 { int32 x = 1 }\n"
"        es_source es1 = S2 { int32 x=2 }\n"
"        es_module = M1 { int32 x = 3 }\n"
"        es_module esm1 = M2 { int32 x = 4 }\n"
"        es_prefer = S1 {}"
"        es_prefer esm1 = M2 {}"
"        PSet p1 = { int32 x = 2 }\n"
"        PSet p2 = {\n"
"              int32 x = 3\n"
"               }\n"
"        module m1 = ThingMod { int32 x = 5 }\n"
"        path t1 = { m1 }\n"
"        endpath te = { a}\n"
"} \n";
 std::cout << std::endl;
  return ;
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      std::cerr << "Null output from parser" << std::endl;
    }

  edm::copy_all(*pr, std::ostream_iterator<edm::pset::NodePtr>(std::cout,"\n"));
}

void testparser::mainparserLooperTest()
{
  const char* spec = 
  "process LOOP =\n"
  "{\n"
  "source = PoolInput\n"
  "{\n"
  "string filename = \"here\" \n"  
  "}\n"
  "looper  = AlignmentLooper\n"
  "{\n"
  "string filename = \"not here\"\n" 
  "}\n"
  "module m1 = MidpointJetProducer\n"
  "{\n"
  "}\n"
  "path p = { m1 }\n"
  "}\n";
  std::cout << std::endl;
  return ;
  ParseResults pr = edm::pset::parse(spec);
  
  if(!pr)
  {
    std::cerr << "Null output from parser" << std::endl;
  }
  
  edm::copy_all(*pr, std::ostream_iterator<edm::pset::NodePtr>(std::cout,"\n"));
}
