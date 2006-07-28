#!/bin/bash
#------------------------------------------------------------
# Shell script to run the parser_t program.
#
#   REQUIREMENT: Read and write permission to /tmp
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

FILE1="includeOne.cfg"
FILE2="includeTwice.cfg"

if [ ! -f "${FILE1}" ]; then
  echo File ${FILE1} does not exist
  exit -1
fi
if [ ! -f "${FILE2}" ]; then
  echo File ${FILE2} does not exist
  exit -1
fi

WORKDIR=${LOCAL_TMP_DIR}

if [ ! -d "${LOCAL_TMP_DIR}" ]; then
  echo Directory ${LOCAL_TMP_DIR} does not exist
  exit -1
fi

OUTPUT_FILE_1=${WORKDIR}/${FILE1}_1
OUTPUT_FILE_2=${WORKDIR}/${FILE2}_2

if ! touch ${OUTPUT_FILE_1}; then
   echo Unable to write to directory ${WORKDIR}
   exit -1
fi

# Clean up any debris from earlier runs
rm -f ${OUTPUT_FILE_1} ${OUTPUT_FILE_2}

# First pass: read from FILE1, write to OUTPUT_FILE_1
# OUTPUT_FILE_1 will contain a script in 'canonical form'
echo Starting first pass for ${FILE1} ...
if ! ${LOCAL_TEST_BIN}/parser_t ${FILE1} > ${OUTPUT_FILE_1}
then
  echo Failure reading ${SCRIPTFILE}
  exit 1
fi

# Second pass: read from FILE2, write to OUTPUT_FILE_2
# OUTPUT_FILE_2 should contain the same 'canonical form' as OUTPUT_FILE_1
echo Starting second pass for ${FILE2} ...
if ! ${LOCAL_TEST_BIN}/parser_t ${FILE2} > ${OUTPUT_FILE_2}
then
  echo Failure reading ${FILE2}
  exit 2
fi

# Compare text of OUTPUT_FILE_1 and OUTPUT_FILE_2
echo Starting comparison ${SCRIPTFILE} ...
if ! diff -B ${OUTPUT_FILE_1} ${OUTPUT_FILE_2}
then
  echo Failure ${OUTPUT_FILE_1} and ${OUTPUT_FILE_2} differ
  exit 3
fi

# First pass: read from FILE1, write to OUTPUT_FILE_1
# OUTPUT_FILE_1 will contain a script in 'canonical form'
echo Starting first pass for ${FILE1} ...
if ! EdmConfigToPython < ${FILE1} > ${OUTPUT_FILE_1}
then
  echo Failure reading ${SCRIPTFILE}
  exit 1
fi

# Second pass: read from FILE2, write to OUTPUT_FILE_2
# OUTPUT_FILE_2 should contain the same 'canonical form' as OUTPUT_FILE_1
echo Starting second pass for ${FILE2} ...
if ! EdmConfigToPython < ${FILE2} > ${OUTPUT_FILE_2}
then
  echo Failure reading ${FILE2}
  exit 2
fi

# Compare text of OUTPUT_FILE_1 and OUTPUT_FILE_2
echo Starting comparison ${SCRIPTFILE} ...
if ! diff -B ${OUTPUT_FILE_1} ${OUTPUT_FILE_2}
then
  echo Failure ${OUTPUT_FILE_1} and ${OUTPUT_FILE_2} differ
  exit 3
fi

chmod 666 $OUTPUT_FILE_1 $OUTPUT_FILE_2


exit 0
