#include <cassert>
#include <iostream>
#include "FWCore/ParameterSet/src/ConfigurationPreprocessor.h"


int test_include(std::string const& line,
		  bool expected_result,
		  std::string const& expected_filename)
{
  int rc = 0;
  std::string filename;
  bool b = edm::pset::ConfigurationPreprocessor::is_include_line(line, filename);
  if ( b != expected_result ) 
    {
      rc = 1;
      std::cerr << "FAILURE --- Wrong result for: " << line << '\n';
    }

  if ( b ) 
    {
      if ( filename != expected_filename )
	{
	  rc = 1;
	  std::cerr << "FAILURE --- Unexpected filename for: " << line << '\n';
	}
    }
  return rc;
}

int work()
{
  int rc = 0;
  std::string filename;

  rc += test_include("\tinclude \"filexxx\"", true, "filexxx");
  rc += test_include("\tinclude \"file.txt\"\t# comment ok", true, "file.txt");
  rc += test_include("\tinclude \"some/path/file.cfg\"# comment ok", true, 
		     "some/path/file.cfg");
  rc += test_include("\tinclude \"/absolute/path/file.cfg\"// comment ok", true, 
		     "/absolute/path/file.cfg");
  rc += test_include("\tinclude \"/absolute/path/file.cfg\"  // comment ok", true, 
		     "/absolute/path/file.cfg");

  rc += test_include("include \"~/wacky/x.cfg\"", true, "~/wacky/x.cfg");
  rc += test_include("include \"../some/thing\"\t# comment ok", true, "../some/thing");
  rc += test_include("include \"filexxx\"# comment ok", true, "filexxx");
  rc += test_include("include \"filexxx\"// comment ok", true, "filexxx");
  rc += test_include("include \"filexxx\"  // comment ok", true, "filexxx");

  rc += test_include("  include \"file xxx\"", true, "file xxx");
  rc += test_include("  include \"file.xxx\"\t# comment ok", true, "file.xxx");
  rc += test_include("  include \"file\txxx\"# comment ok", true, "file\txxx");
  rc += test_include("  include \"filexxx\"// comment ok", true, "filexxx");
  rc += test_include("  include \"filexxx\"  // comment ok", true, "filexxx");

  rc += test_include("", false, "");
  rc += test_include("  ", false, "");
  rc += test_include("#include", false, "");
  rc += test_include("//include", false, "");
  rc += test_include("\ninclude", false, "");
  rc += test_include("\rinclude", false, "");
  rc += test_include("not an include line", false, "");

  rc += test_include("  include 'file xxx'", true, "file xxx");
  rc += test_include("  include 'file.xxx'\t# comment ok", true, "file.xxx");
  rc += test_include("  include 'file\txxx'# comment ok", true, "file\txxx");
  rc += test_include("  include 'filexxx'// comment ok", true, "filexxx");
  rc += test_include("  include 'filexxx'  // comment ok", true, "filexxx");


  return rc;
}

int main()
{
  int rc = 0;
  try { rc = work(); }
  catch (int i)
    {
      std::cerr << "Error: " << i << '\n';
      rc = i;
    }
  catch ( ... )
    {
      std::cerr << "Unknown error\n";
      rc = -1;
    }
  return rc;
}
