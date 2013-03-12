#! /bin/bash
# make_info_file.sh : make /etc/info.ini
#

if [ $# != 2 ]; then
        echo "Usage : make_info_file.sh [model] [build] "
        exit
fi

MODEL=$1
BUILD=$2

cat >/etc/info.ini <<EOF
[Version]
Model=$MODEL;
Build=$BUILD;
[Build]
Date=`date +%Y.%m.%d`;
Time=`date +%H:%M:%S`;
EOF
