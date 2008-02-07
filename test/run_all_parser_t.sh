#!/bin/sh 


# Pass in name and status
function die { echo Failed on $1 with status $2 ;  exit $2; }

pushd ${LOCAL_TEST_DIR}

# Make sure all the properly-formed configuration files work.
for name in *.cfg
  do 
    ./parser_t.sh $name || die $name $?
  done


# Make sure all the improperly-formed configuration files fail.
for name in *.cfg.bad
    do
      ./parser_bad_t.sh $name || die $name $?
    done

echo  run_all_parser_t passed
popd
