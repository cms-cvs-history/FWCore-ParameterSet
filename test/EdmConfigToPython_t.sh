#!/bin/sh -x

export INPUT=${LOCAL_TEST_DIR}/complete.cfg
export OUTPUT=${LOCAL_TMP_DIR}/EdmconfigToPython_t_out.pycfg
EdmConfigToPython < ${INPUT} > ${OUTPUT}






