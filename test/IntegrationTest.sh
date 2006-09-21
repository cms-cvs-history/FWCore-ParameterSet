#!/bin/sh


function pydie { 
  echo Failed in EdmConfigToPython $1 with status $2 ;  
  exit $2; 
}


function cfgdie {
  echo Failed in consconfig.py $1 with status $2 ;
  exit $2;
}

#export LOCAL_TMP_DIR=.
export PYOUTPUT=${LOCAL_TMP_DIR}/EdmconfigToPython_t_out.pycfg
export CFGOUTPUT=${LOCAL_TMP_DIR}/EdmconfigToPython_t_out.cfg
export PYOUTPUT2=${LOCAL_TMP_DIR}/EdmconfigToPython_t_out_2.pycfg

for CFG in ${CMSSW_RELEASE_BASE}/src/Configuration/CSA*/data/*cfg; do
  echo $CFG  
  EdmConfigToPython < ${CFG} > ${PYOUTPUT}  || pydie $CFG $?
  python cmsconfig.py ${PYOUTPUT} > ${CFGOUTPUT} || cfgdie $CFG  $?
  EdmConfigToPython < ${CFGOUTPUT} > ${PYOUTPUT2} || pydie $CFGOUTPUT $?
  python comparePythonOutput.py ${PYOUTPUT} ${PYOUTPUT2}
done

