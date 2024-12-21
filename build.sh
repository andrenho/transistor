#!/bin/sh

if [ "$1" = "-r" ]; then
	git submodule update --init --recursive
  mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j10
elif [ "$1" = "-d" ]; then
	git submodule update --init --recursive
  mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j10
elif [ "$1" = "-t" ]; then
	git submodule update --init --recursive
  mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make check -j10
elif [ "$1" = "-c" ]; then
  rm -rf build
else
  echo "Usage: $0 [-r|-d|-t|-c]    (-r: release, -d: debug, -t: tests, -c: clear build)"
  exit 1
fi

# TODO - choose build mode
