#!/bin/sh 

DIR=../../../../test/`scramv1 arch`

# Pass in name and status
function die { echo Failed on $1 with status $2 ;  exit $2; }

for name in *.txt
  do 
    ./parser_t.sh $name || die $name $?
  done
echo  run_all_parser_t passed
