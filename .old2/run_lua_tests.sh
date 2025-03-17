#!/bin/sh

set -e

LIB=''
for f in engine/engine/util/*.lua; do
  LIB="$LIB -l ${f%%.*}"
done
for f in engine/engine/*.lua; do
  LIB="$LIB -l ${f%%.*}"
done

for f in engine/engine/tests/*.lua; do
  luajit $LIB $f
done