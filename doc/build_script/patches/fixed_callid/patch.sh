#!/bin/bash -e

. ../../config.conf

PJPROJECT_BASE_FOLDER="$DOWNLOAD_DIR/$PJSIP_DIR_NAME"

cp callid.patch $PJPROJECT_BASE_FOLDER

CURDIR=$(pwd)
cd "$PJPROJECT_BASE_FOLDER"
patch -p0 < callid.patch
rm callid.patch

# Changing PJSIP_MAX_URL_SIZE --- see README.md for more information
sed -i -r 's~(#[ \t]*(define[ \t]+PJSIP_MAX_URL_SIZE)[ \t]+)(256){1}~\1512~g' $PJPROJECT_BASE_FOLDER/pjsip/include/pjsip/sip_config.h

cd "$CURDIR"
