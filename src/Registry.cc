// ----------------------------------------------------------------------
// $Id: Registry.cc,v 1.6.2.1 2006/06/29 19:00:19 paterno Exp $
//
// ----------------------------------------------------------------------

#include "FWCore/ParameterSet/interface/Registry.h"


namespace edm
{
  namespace pset
  {

    bool
    insertParameterSetIntoRegistry(Registry* reg, ParameterSet const& p)
    {
      ParameterSet tracked_part = p.trackedPart();
      return reg->insertMapped(tracked_part);
    }

    void 
    loadAllNestedParameterSets(Registry* reg, ParameterSet const& main)
    {
      std::vector<ParameterSet> all_main_psets;
      explode(main, all_main_psets);
      std::vector<ParameterSet>::const_iterator i = all_main_psets.begin();
      std::vector<ParameterSet>::const_iterator e = all_main_psets.end();
      for (; i != e; ++i) reg->insertMapped(*i);
    }

    void fill(Registry* reg, regmap_type& fillme)
    {
      typedef Registry::const_iterator iter;
      fillme.clear();
      for (iter i=reg->begin(), e=reg->end(); i!=e; ++i)
	fillme[i->first].pset_ = i->second.toStringOfTracked();
    }
    

  } // namespace pset
} // namespace edm

