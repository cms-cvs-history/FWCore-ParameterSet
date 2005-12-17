#!/bin/sh


# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }

${LOCAL_TEST_DIR}/run_all_parser_t.sh || die 'Failed in run_all_parser_t.sh' $?

# This is a historical relic, and it probably should be removed.
echo --------------
echo Running string_parser_t...
${LOCAL_TEST_BIN}/string_parser_t || die 'Failed in string_parser_t' $?
echo string_parser_t succeeded
echo --------------





