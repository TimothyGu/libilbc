#!/bin/sh

if [ $AUTOTOOLS ]; then
    autoreconf -fi && mkdir build && cd build && ../configure && make -j2 V=1
elif [ $CMAKE ]; then
    mkdir build && cd build && cmake .. && make -j2 VERBOSE=1
    mkdir build.static && cd build.static && \
        cmake .. -DBUILD_SHARED_LIBRARIES=OFF && \
        make -j2 VERBOSE=1
fi
