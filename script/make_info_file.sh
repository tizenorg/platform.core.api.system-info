#!/bin/sh
# make_info_file.sh : make /etc/info.ini
#

. /etc/tizen-build.conf

TYPE=$(echo $TZ_BUILD_RELEASE_TYPE | tr '[:upper:]' '[:lower:]')

DATE=$(echo $TZ_BUILD_DATE | awk -F"[-_.]" '{ print $1 }')

RELEASE=$(echo $TZ_BUILD_ID | sed "s/.*$DATE/$DATE/")
RELEASE=$(echo $RELEASE | awk -F"[-_]" '{ print $1 }')

ID=$(echo $TZ_BUILD_ID | sed "s/$DATE.*//")
ID=$(echo $ID | sed "s/[-_.]*$//")


cat <<EOF > /etc/info.ini
[Version]
Model=$TZ_BUILD_RELEASE_NAME;
Build=$TZ_BUILD_ID;
Release=$RELEASE;
[Build]
Type=$TYPE;
Date=$TZ_BUILD_DATE;
Time=$TZ_BUILD_TIME;
Variant=$TZ_BUILD_VARIANT;
ID=$ID;
EOF
