#!/bin/sh

set -e

LIB=''
for f in engine/*.lua; do
  LIB="$LIB -l ${f%%.*}"
done

for f in engine/tests/*.lua; do
  luajit $LIB $f
done