#!/bin/sh 


# Pass in name and status
function die { echo Failed on $1 with status $2 ;  exit $2; }

pushd ${LOCAL_TEST_DIR}

# This call of the binary is expected to succeed
${LOCAL_TEST_BIN}/ParseTree_t Replace.cfg || die Replace.cfg $?
echo  ParseTree_t Replace.cfg passed

# This call of the binary is expected to fail.
(${LOCAL_TEST_BIN}/Replace_t Replace.cfg) || die Replace.cfg $?
echo  Replace_t Replace.cfg passed

# not sure if this is the right way
ln -s ../../../../lib/*/libFWCoreParameterSet.so .
python PythonParseTree_t.py || die PythonParseTree_t.py $?
rm -f libFWCoreParameterSet.so

popd

