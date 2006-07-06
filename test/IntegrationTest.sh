#!/bin/sh


function pydie { 
  echo Failed in EdmConfigToPython $1 with status $2 ;  
  exit $2; 
}


function cfgdie {
  echo Failed in consconfig.py $1 with status $2 ;
  exit $2;
}


export PYOUTPUT=${LOCAL_TMP_DIR}/EdmconfigToPython_t_out.pycfg
export CFGOUTPUT=${LOCAL_TMP_DIR}/EdmconfigToPython_t_out.cfg

for CFG in ${CMSSW_RELEASE_BASE}/src/Configuration/Applications/data/*cfg; do
  
  EdmConfigToPython < ${CFG} > ${PYOUTPUT}  || pydie $CFG $?
  cmsconfig.py < ${PYOUTPUT} > ${CFGOUTPUT} || cfgdie $CFG  $?

done

