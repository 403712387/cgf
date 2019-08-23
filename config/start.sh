#!/bin/bash
export LD_LIBRARY_PATH=./lib

ulimit -n 10240
ulimit -c unlimited
ulimit -v unlimited
ulimit -m unlimited
sysctl -w net.ipv4.tcp_tw_reuse=1 > /dev/null 2>&1
echo "core-%e-%p-%t" > /proc/sys/kernel/core_pattern

#export PPROF_PATH=/usr/local/bin/pprof
#env HEAPCHECK=normal   LD_PRELOAD="./lib/gperftools/libtcmalloc.so"

#MALLOC_CHECK_=1
#LIBC_FATAL_STDERR_=1

nohup ./cfg  > /dev/null 2>&1 &