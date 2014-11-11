#!/bin/sh
# make_info_file.sh : make /etc/info.ini
#

. /etc/os-release

cat <<EOF > /etc/info.ini
[Version]
Model=$PRETTY_NAME;
Build=$BUILD_ID;
[Build]
Date=`date +%Y.%m.%d`;
Time=`date +%H:%M:%S`;
EOF
