#!/bin/sh
# make_info_file.sh : make /etc/info.ini
#

. /etc/tizen-build.conf

TYPE=eng
VARIANT=NONE

while getopts "t:v:" opt
do
	case "$opt" in
	t)
		TYPE=$OPTARG
		;;
	v)
		VARIANT=$OPTARG
		;;
	esac
done

TYPE=$(echo $TYPE | tr '[:upper:]' '[:lower:]')

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
