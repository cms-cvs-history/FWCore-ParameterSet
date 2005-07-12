#include "FWCore/ParameterSet/interface/parse.h"
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include <cppunit/extensions/HelperMacros.h>

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
CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown(){}
  void mainparserTest();
};
                                                                                                            
///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testparser);
                                                                                                            
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
