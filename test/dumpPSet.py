"A test program which dumps the ParameterSet created by a python config file"

import sys
targetName = sys.argv[1]
__import__(targetName)
import libFWCoreParameterSet

processDesc = libFWCoreParameterSet.ProcessDesc()
processPSet = libFWCoreParameterSet.ParameterSet()

sys.modules[targetName].process.fillProcessDesc(processDesc, processPSet)
print processDesc.dump()
print processPSet.dump()


