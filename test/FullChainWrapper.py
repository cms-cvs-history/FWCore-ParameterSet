#!/usr/bin/env python
cfgFile="Configuration/Examples/data/FullChainExample.cfg"
#cfgFile = "tmp.cfg"
#cfgFile = "CSCDigitizerTest.cfg"
from FWCore.ParameterSet.Config import include
process = include(cfgFile)

