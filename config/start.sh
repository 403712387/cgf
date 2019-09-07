#!/bin/bash
export LD_LIBRARY_PATH=./lib

ulimit -n 10240
ulimit -c unlimited
ulimit -v unlimited
ulimit -m unlimited
sysctl -w net.ipv4.tcp_tw_reuse=1 > /dev/null 2>&1
echo "core-%e-%p-%t" > /proc/sys/kernel/core_pattern

#export LD_PRELOAD=./lib/gperftools/libtcmalloc.so
#HEAPCHECK=normal

#MALLOC_CHECK_=1
#LIBC_FATAL_STDERR_=1

#HEAPPROFILE=./cgf ./cfg
#LD_PRELOAD=./lib/gperftools/libprofiler.so.5 CPUPROFILE=./cgf.prof ./cgf
nohup ./cfg  > /dev/null 2>&1 &

#pprof --svg ./cgf ./cgf.prof > cgf.svg