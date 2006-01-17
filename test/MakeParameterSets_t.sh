#!/bin/sh 


# Pass in name and status
function die { echo Failed on $1 with status $2 ;  exit $2; }

pushd ${LOCAL_TEST_DIR}

# This call of the binary is expected to succeed
${LOCAL_TEST_BIN}/MakeParameterSets_t sample.txt || die sample.txt $?
echo  MakeParameterSets_t sample.txt passed

# This call of the binary is expected to fail.
(${LOCAL_TEST_BIN}/MakeParameterSets_t circular.txt) && die circular.txt -1
echo  MakeParameterSets_t circular.txt passed

popd

