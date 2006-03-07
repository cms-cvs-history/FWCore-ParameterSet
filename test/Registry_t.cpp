//----------------------------------------------------------------------
//
// This program test the behavior of pset::Registry.
//
// $Id: Registry_t.cpp,v 1.3 2006/02/28 23:24:29 paterno Exp $
//----------------------------------------------------------------------
#include <cassert>
#include <vector>

#include "boost/thread.hpp"

#include "DataFormats/Common/interface/ParameterSetID.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/Registry.h"

struct ThreadWorker
{
  ThreadWorker(int n_to_add, int n_of_lookups, int ofs) :
    number_to_add(n_to_add),
    number_of_lookups(n_of_lookups),
    offset(ofs)
  { }

  void operator()();

  int number_to_add;
  int number_of_lookups;
  int offset;
};

void
ThreadWorker::operator()()
{
  pset::Registry* reg = pset::Registry::instance();
  assert (reg);

  // Add a bunch of items
  std::vector<edm::ParameterSetID> ids_issued;  
  for (int i = offset; i < number_to_add+offset; ++i)
    {
      edm::ParameterSet ps;
      ps.addParameter<int>("i", i);
      ps.addUntrackedParameter<double>("d", 2.5);
      ids_issued.push_back(ps.id());
      reg->insertParameterSet(ps);
    }
  
  // Look up items we have just put in.
  typedef std::vector<edm::ParameterSetID>::const_iterator iter;
  for ( iter i = ids_issued.begin();
	i != ids_issued.end();
	++i)
    {
      edm::ParameterSet ps;
      assert( reg->getParameterSet(*i, ps) );
      assert( ps.id() == *i );
      assert( ps.getParameter<int>("i") > 0 ); // fixme: check exact value
      assert( ps.getUntrackedParameter<double>("d", 1.0) == 1.0);
    }
}

void work()
{
  pset::Registry* reg = pset::Registry::instance();
  assert (reg);

  // Launch a bunch of threads, which beat on the Registry...
  boost::thread_group threads;
  // don't make this too high, or resource exhaustion may kill
  // the whole test...
  const int NUM_THREADS = 10; 
  const int NUM_PSETS_PER_THREAD = 500;
  const int NUM_LOOKUPS_PER_THREAD = 1000;
  for (int i = 0; i < NUM_THREADS; ++i)
    threads.create_thread(ThreadWorker(NUM_PSETS_PER_THREAD,
 				       NUM_LOOKUPS_PER_THREAD,
 				       NUM_PSETS_PER_THREAD));
  threads.join_all();
}

void work2()
{
  // This part is not multi-threaded; it checks only the return value
  // of the insertParameterSet member function.
  pset::Registry* reg = pset::Registry::instance();
  
  // Make a new ParameterSet, not like those already in the Registry.
  edm::ParameterSet ps;
  std::string value("and now for something completely different...");
  ps.addParameter<std::string>("s", value);

  // First call should insert the new ParameterSet; second call should
  // not.
  assert( reg->insertParameterSet(ps) );
  assert( !reg->insertParameterSet(ps) );  
}


int main()
{
  int rc = 1;
  try
    {
      work();
      work2();
      rc = 0;
    }
  catch ( edm::Exception& x)
    {
      std::cerr << "edm::Exception caught\n"
		<< x
		<< '\n';
      rc = -1;
    }
  catch ( std::bad_alloc & x)
    {
      std::cerr << "bad alloc: " << x.what() << '\n';
      rc = -2;
    }

  catch ( boost::thread_resource_error & x)
    {
      std::cerr << "resource error: " << x.what()
		<< "\nTry decreasing NUM_THREADS, and recompiling\n";	
      rc = -3;
    }


  catch ( boost::lock_error & x)
    {
      std::cerr << "lock error: " << x.what() << '\n';
      rc = -4;
    }


  
  
  catch ( ... )
    {
      std::cerr << "Unknown exception caught\n";
      rc = 1;
    }
  return rc;
}
