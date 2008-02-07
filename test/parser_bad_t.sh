#!/bin/bash
#------------------------------------------------------------
# Shell script to run the parser_t program, expecting failure.
#
#   REQUIREMENT: Read and write permission to /tmp
#
#   INPUT: $1 -- the name of the configuration file to be read
#
#   ENVIRONMENT: this script relies upon
#      WORKDIR: must be set to a directory into which we have
#               read and write permission
#      PATH: the program parser_t must be found on the path.
#
# CAVEATS:
#
# This script is a slightly bad citizen; it writes files to /tmp and
# does not clean them up. The files are not large, and there are not
# too many of them.
#
# $Id: parser_t.sh,v 1.9 2006/07/24 23:35:42 wmtan Exp $
#-----------------------------------------------------------
#set -o verbose -o xtrace

SCRIPTFILE=$1

if [ -z $1 ]; then
  echo Required argument was not supplied
  exit -1
fi

if [ ! -f "${SCRIPTFILE}" ]; then
  echo File ${SCRIPTFILE} does not exist
  exit -1
fi

WORKDIR=${LOCAL_TMP_DIR}

if [ ! -d "${LOCAL_TMP_DIR}" ]; then
  echo Directory ${LOCAL_TMP_DIR} does not exist
  exit -1
fi

OUTPUT_FILE_1=${WORKDIR}/bad_${SCRIPTFILE}_1


if ! touch ${OUTPUT_FILE_1}; then
   echo Unable to write to directory ${WORKDIR}
   exit -1
fi

# Clean up any debris from earlier runs
rm -f ${OUTPUT_FILE_1}

# First pass: read from SCRIPTFILE, write to OUTPUT_FILE_1.
# Parsing the original input is expected to fail.
echo Starting first pass for ${SCRIPTFILE} ...
if ${LOCAL_TEST_BIN}/parser_t ${SCRIPTFILE} > ${OUTPUT_FILE_1}
then
  echo ERROR -- Malformed configuration ${SCRIPTFILE} not identified as malformed.
  exit 1
fi

exit 0
