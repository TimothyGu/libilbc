cmake_minimum_required(VERSION 2.8)
include(TestBigEndian)
test_big_endian(WEBRTC_BIG_ENDIAN)
include_directories(. ilbc/interface signal_processing/include)
file(GLOB ilbc_source_files ilbc/*.c signal_processing/*.c)
add_library(ilbc SHARED ${ilbc_source_files})
set_target_properties(ilbc PROPERTIES VERSION 1.1.1 SOVERSION 1)

set(CMAKE_C_FLAGS "-O2 -Wall -Wextra -Werror -fvisibility=hidden -Wno-unused-parameter")        ## Optimize
set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary

set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "\${prefix}")
set(libdir "\${exec_prefix}/lib")
set(includedir "\${prefix}/include")
configure_file(libilbc.pc.in libilbc.pc @ONLY)
install(FILES libilbc.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
install(FILES ilbc/interface/ilbc.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ilbc DESTINATION lib)
