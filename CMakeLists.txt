cmake_minimum_required(VERSION 2.8)
include(TestBigEndian)
test_big_endian(WEBRTC_BIG_ENDIAN)
include_directories(. ilbc/interface signal_processing/include)
file(GLOB ilbc_source_files ilbc/*.c signal_processing/*.c)
add_library(ilbc SHARED ${ilbc_source_files})

set(CMAKE_C_FLAGS "-O2 -Wall -Wextra -Werror -fvisibility=hidden -Wno-unused-parameter")        ## Optimize
set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
