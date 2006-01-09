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
(${LOCAL_TEST_BIN}/PythonFormWriter_t ${LOCAL_TEST_DIR}/complete.cfg > ${LOCAL_TMP_DIR}/out.pycfg) || die "Failed running PythonFormWriter_t" $?

#die "This test isn't really finished" 1






