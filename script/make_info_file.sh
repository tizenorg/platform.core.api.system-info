#!/bin/sh
# make_info_file.sh : make /etc/info.ini
#

. /etc/tizen-build.conf

cat <<EOF > /etc/info.ini
[Version]
Model=$TZ_BUILD_RELEASE_NAME;
Build=$TZ_BUILD_ID;
[Build]
Date=$TZ_BUILD_DATE;
Time=$TZ_BUILD_TIME;
EOF
