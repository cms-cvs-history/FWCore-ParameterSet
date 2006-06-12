#!/bin/sh


# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }

#----------------------------------------------------------------------
#
# This test should:
#   * run the C++ tool that reads a .cfg file, and write the equivlanent .pycfg file, then
#   * run the Python tool that reads the .pycfg file, and writes a .cfg file, and then
#   * run parser_t.sh on the original .cfg file, then
#   * run parser_t.sh on the second (output) .cfg file, and finally
#   * compare the final form of the output of the last two tests, to make sure the
#     output of the Python-processed config file is the same as the original.
#

export PROC_DEBUG=0 # set to higher value for lots of debugging output...
EdmConfigToPython < ${LOCAL_TEST_DIR}/complete.cfg > ${LOCAL_TMP_DIR}/out.pycfg || die "Failed running PythonFormWriter_t" $?

(python ${LOCAL_TMP_DIR}/out.pycfg) || die "Python failed to parse" $?

# this file should have the correct output
${LOCAL_TEST_DIR}/comparePythonOutput.py ${LOCAL_TMP_DIR}/out.pycfg ${LOCAL_TEST_DIR}/complete.pycfg || die "PythonFormWriter did not match reference output" $?

# convert our file back to a cfg file
python ${LOCAL_TEST_DIR}/cmsconfig.py ${LOCAL_TMP_DIR}/out.pycfg > ${LOCAL_TMP_DIR}/complete2.cfg
 
# now re-run everything and make sure nothing has changed
EdmConfigToPython < ${LOCAL_TMP_DIR}/complete2.cfg > ${LOCAL_TMP_DIR}/out2.pycfg

${LOCAL_TEST_DIR}/comparePythonOutput.py ${LOCAL_TMP_DIR}/out.pycfg ${LOCAL_TMP_DIR}/out2.pycfg || die "Python rebuilding of config file failed" $?
 
#die "This test isn't really finished" 1






