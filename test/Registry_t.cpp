//----------------------------------------------------------------------
//
// This program test the behavior of pset::Registry.
//
// $Id: Registry_t.cpp,v 1.1 2006/02/24 19:15:52 paterno Exp $
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
  const int NUM_THREADS = 100;
  const int NUM_PSETS_PER_THREAD = 500;
  const int NUM_LOOKUPS_PER_THREAD = 100;
  for (int i = 0; i < NUM_THREADS; ++i)
    threads.create_thread(ThreadWorker(NUM_PSETS_PER_THREAD,
				       NUM_LOOKUPS_PER_THREAD,
				       NUM_PSETS_PER_THREAD));
  threads.join_all();

  // Make sure all is well.
}


int main()
{
  int rc = 1;
  try
    {
      work();
      rc = 0;
    }
  catch ( edm::Exception& x)
    {
      std::cerr << "edm::Exception caught\n"
		<< x
		<< '\n';
      rc = -1;
    }
  catch ( ... )
    {
      std::cerr << "Unknown exception caught\n";
      rc = -2;
    }
  return rc;
}
