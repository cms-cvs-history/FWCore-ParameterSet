#!/bin/sh 


# Pass in name and status
function die { echo Failed on $1 with status $2 ;  exit $2; }

pushd ${LOCAL_TEST_DIR}

for name in *.cfg
  do 
    ./parser_t.sh $name || die $name $?
  done
echo  run_all_parser_t passed

popd
