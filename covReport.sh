#!/bin/bash -e

if [ $# -lt 2 ];
then
exit
fi

mkdir -p $2

gcovr -v \
      -r $1 \
      --html \
      --html-details \
      --exclude="gtest/*" \
      --exclude="test/*" \
       -o $2/coverageReport.html
