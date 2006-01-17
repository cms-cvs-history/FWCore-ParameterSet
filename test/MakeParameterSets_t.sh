#!/bin/sh 


# Pass in name and status
function die { echo Failed on $1 with status $2 ;  exit $2; }

pushd ${LOCAL_TEST_DIR}

${LOCAL_TEST_BIN}/MakeParameterSets_t sample.txt || die sample.txt $?
echo  MakeParameterSets_t.sh passed

popd

