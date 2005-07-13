#include "FWCore/ParameterSet/interface/parse.h"
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include <fstream>


//------------------------------------------------------------
// Test program for parameter set specification and configuration file
// parser.
//------------------------------------------------------------

using namespace std;

void read_from_stdin(std::string& buffer)
{
  std::string line;
  while (std::getline(cin, line)) { buffer += line; buffer += '\n'; }
}
                                                                                                                   
class testparser: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(testparser);
CPPUNIT_TEST(mainparserTest);
CPPUNIT_TEST(mainparser1Test);
CPPUNIT_TEST(mainparser2Test);
CPPUNIT_TEST(mainparser3Test);
CPPUNIT_TEST(mainparser4Test);
CPPUNIT_TEST(mainparser5Test);
CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown(){}
  void mainparserTest();
  void mainparser1Test();
  void mainparser2Test();
  void mainparser3Test();
  void mainparser4Test();
  void mainparser5Test();
};
                                                                                                            
///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testparser);
                                                                                                            
void testparser::mainparser1Test()
{
  string buffer;
// read_from_stdin(buffer);
// const char* spec = buffer.c_str();
// reading input from file "test_input1.txt" 
        char spec[2000];
        fstream file_op("test_input1.txt",ios::in);
        while(!file_op.eof()) 
        {
              file_op.getline(spec,2000);
// cout <<spec;
        }         file_op.close();
        cout <<endl;
  return 0;
// reading from file is over
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      cerr << "Null output from parser" << endl;
//      return -1;
    }

  copy(pr->begin(),
       pr->end(),
       ostream_iterator<edm::pset::NodePtr>(cout,"\n"));
 // return 0;
}
                                                                                                            
void testparser::mainparser2Test()
{
  string buffer;
// read_from_stdin(buffer);
// const char* spec = buffer.c_str();
// reading input from file "test_input2.txt" 
        char spec[2000];
        fstream file_op("test_input2.txt",ios::in);
        while(!file_op.eof()) 
        {
              file_op.getline(spec,2000);
// cout <<spec;
        }         file_op.close();
        cout <<endl;
  return 0;
// reading from file is over
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      cerr << "Null output from parser" << endl;
//      return -1;
    }

  copy(pr->begin(),
       pr->end(),
       ostream_iterator<edm::pset::NodePtr>(cout,"\n"));
 // return 0;
}
                                                                                                            
void testparser::mainparser3Test()
{
  string buffer;
// read_from_stdin(buffer);
// const char* spec = buffer.c_str();
// reading input from file "test_input3.txt" 
        char spec[2000];
        fstream file_op("test_input3.txt",ios::in);
        while(!file_op.eof()) 
        {
              file_op.getline(spec,2000);
// cout <<spec;
        }         file_op.close();
        cout <<endl;
  return 0;
// reading from file is over
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      cerr << "Null output from parser" << endl;
//      return -1;
    }

  copy(pr->begin(),
       pr->end(),
       ostream_iterator<edm::pset::NodePtr>(cout,"\n"));
 // return 0;
}
                                                                                                            
void testparser::mainparser4Test()
{
  string buffer;
// read_from_stdin(buffer);
// const char* spec = buffer.c_str();
// reading input from file "test_input4.txt" 
        char spec[2000];
        fstream file_op("test_input4.txt",ios::in);
        while(!file_op.eof()) 
        {
              file_op.getline(spec,2000);
// cout <<spec;
        }         file_op.close();
        cout <<endl;
  return 0;
// reading from file is over
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      cerr << "Null output from parser" << endl;
//      return -1;
    }

  copy(pr->begin(),
       pr->end(),
       ostream_iterator<edm::pset::NodePtr>(cout,"\n"));
 // return 0;
}
                                                                                                            
void testparser::mainparser5Test()
{
  string buffer;
// read_from_stdin(buffer);
// const char* spec = buffer.c_str();
// reading input from file "test_input5.txt" 
        char spec[2000];
        fstream file_op("test_input5.txt",ios::in);
        while(!file_op.eof()) 
        {
              file_op.getline(spec,2000);
// cout <<spec;
        }         file_op.close();
        cout <<endl;
  return 0;
// reading from file is over
  ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      cerr << "Null output from parser" << endl;
//      return -1;
    }

  copy(pr->begin(),
       pr->end(),
       ostream_iterator<edm::pset::NodePtr>(cout,"\n"));
 // return 0;
}
                                                                                                            
void testparser::mainparserTest()
{
 string buffer;
 read_from_stdin(buffer);
 const char* spec = buffer.c_str();
 ParseResults pr = edm::pset::parse(spec);

  if(!pr)
    {
      cerr << "Null output from parser" << endl;
//      return -1;
    }

  copy(pr->begin(),
       pr->end(),
       ostream_iterator<edm::pset::NodePtr>(cout,"\n"));
 // return 0;
}
