#!/bin/bash
set -eux

NEWLIB_DIR=$1
PROCESSOR_DIR=$2
ARCH=$3
CORE=$4

REGISTER_HEADER=$PROCESSOR_DIR/kvx-family/BE/TDH/${ARCH}/${CORE}/registers.h
NEWLIB_HEADER=$NEWLIB_DIR/libgloss/kvx-mbr/include/${ARCH}/${CORE}/registers.h

NEWLIB_TMP_FILE=$(mktemp)
PROCESSOR_TMP_FILE=$(mktemp)

# Remove copyright
sed '1,1d' $REGISTER_HEADER > $PROCESSOR_TMP_FILE
# Remove copyright
sed '1,32d' $NEWLIB_HEADER > $NEWLIB_TMP_FILE

# Will fail if there some diffs
diff $PROCESSOR_TMP_FILE $NEWLIB_TMP_FILE

# Remove tmp files
rm -f ${PROCESSOR_TMP_FILE} ${NEWLIB_TMP_FILE}
