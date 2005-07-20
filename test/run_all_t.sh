#!/bin/sh

DIR=../../../../test/slc3_ia32_gcc323

# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }
# This following is a trivial test, and probably should be removed
split || die 'Failed in split' $?

./run_all_parser_t.sh || die 'Failed in run_all_parser_t.sh' $?

# This is a historical relic, and it probably should be removed.
string_parser_t

echo --------------
echo Running makepset_t
${DIR}/makepset_t || die 'Failed in makepset_t' $?
echo makepset_t succeeded
echo --------------


echo --------------
echo Running makeprocess_t
${DIR}/makeprocess_t || die 'Failed in makeprocess_t' $?
echo makeprocess_t succeeded
echo --------------










