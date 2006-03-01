#!/bin/sh 


# Pass in name and status
function die { echo Failed on $1 with status $2 ;  exit $2; }

pushd ${LOCAL_TEST_DIR}

# This call of the binary is expected to succeed
${LOCAL_TEST_BIN}/OverrideParameters_t override.cfg || die override.cfg $?
echo  OverrideParameters_t override.cfg passed


popd

