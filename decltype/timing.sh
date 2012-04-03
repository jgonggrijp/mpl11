#!/bin/bash
# Copyright David Abrahams 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
for ((x = 0; x < 80; ++x)); do
    echo $x
    time -p g++-mp-4.4 -std=gnu++0x timing.cpp -c -o /tmp/timing.o -DSTEPS=$x
    echo --------------------------------
 done
