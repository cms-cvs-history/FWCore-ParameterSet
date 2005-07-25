#!/bin/bash
#------------------------------------------------------------
# Shell script to run the parser_t program.
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
# $Id: parser_t.sh,v 1.2 2005/07/20 02:54:13 jbk Exp $
#-----------------------------------------------------------
#set -o verbose -o xtrace

SCRIPTFILE=$1
DIR=../../../../test/`scramv1 arch`

if [ -z $1 ]; then
  echo Required argument was not supplied
  exit -1
fi

if [ ! -f "${SCRIPTFILE}" ]; then
  echo File ${SCRIPTFILE} does not exist
  exit -1
fi

WORKDIR=/tmp

OUTPUT_FILE_1=${WORKDIR}/${USER}/${SCRIPTFILE}_1
OUTPUT_FILE_2=${WORKDIR}/${USER}/${SCRIPTFILE}_2

if ! touch ${OUTPUT_FILE_1}; then
   echo Unable to write to directory ${WORKDIR}
   exit -1
fi

# Clean up any debris from earlier runs
rm -f ${OUTPUT_FILE_1} ${OUTPUT_FILE_2}

# First pass: read from SCRIPTFILE, write to OUTPUT_FILE_1
# OUTPUT_FILE_1 will contain a script in 'canonical form'
if ! ${DIR}/parser_t < ${SCRIPTFILE} > ${OUTPUT_FILE_1}
then
  echo Failure reading ${SCRIPTFILE}
  exit 1
fi

# Second pass: read from OUTPUT_FILE_1, write to OUTPUT_FILE_2
# OUTPUT_FILE_2 should contain the same 'canonical form' as OUTPUT_FILE_1
if ! ${DIR}/parser_t < ${OUTPUT_FILE_1} > ${OUTPUT_FILE_2}
then
  echo Failure reading ${OUTPUT_FILE_1}
  exit 2
fi

# Compare text of OUTPUT_FILE_1 and OUTPUT_FILE_2
if ! diff ${OUTPUT_FILE_1} ${OUTPUT_FILE_2}
then
  echo Failure ${OUTPUT_FILE_1} and ${OUTPUT_FILE_2} differ
  exit 3
fi

chmod 666 $OUTPUT_FILE_1 $OUTPUT_FILE_2

exit 0
