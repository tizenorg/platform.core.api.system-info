#!/bin/sh
# make_info_file.sh : make /etc/info.ini
#

. /etc/tizen-build.conf

# First parameter is Release type.
# "eng" and "user" are available
# The default value is "eng"
if [ -z "$1" ]
then
	TYPE=eng
else
	TYPE=$1
fi

# Second parameter is Build variant
# The default value is "NONE"
if [ -z "$2" ]
then
	VARIANT=NONE
else
	VARIANT=$2
fi

RELEASE=$(echo $TZ_BUILD_ID | sed 's/[^0-9.]//g')

BUILDID=$(sed -n '/BUILD_ID=/p' /etc/tizen-release)
BUILDID=$(echo $BUILDID | sed 's/BUILD_ID=//g')

cat <<EOF > /etc/info.ini
[Version]
Model=$TZ_BUILD_RELEASE_NAME;
Build=$TZ_BUILD_ID;
Release=$RELEASE;
[Build]
Type=$TYPE;
Date=$TZ_BUILD_DATE;
Time=$TZ_BUILD_TIME;
Variant=$VARIANT;
ID=$BUILDID;
EOF
