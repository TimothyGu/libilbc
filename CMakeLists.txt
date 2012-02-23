# Copyright (c) 2012  Jeroen Dekkers <jeroen@dekkers.ch>
#
# Permission to use, copy, modify, and/or distribute this software for
# any purpose with or without fee is hereby granted, provided that the
# above copyright notice and this permission notice appear in all
# copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL ISC BE LIABLE FOR ANY
# SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
# AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
# OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
# SOFTWARE.

cmake_minimum_required(VERSION 2.8)
include(TestBigEndian)
test_big_endian(WEBRTC_BIG_ENDIAN)
include_directories(. ilbc/interface signal_processing/include)
file(GLOB ilbc_source_files ilbc/*.c signal_processing/*.c)
add_library(ilbc SHARED ${ilbc_source_files})
add_library(ilbc-static STATIC ${ilbc_source_files})
set_target_properties(ilbc PROPERTIES VERSION 1.1.1 SOVERSION 1)
set_target_properties(ilbc PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(ilbc-static PROPERTIES OUTPUT_NAME "ilbc")
set_target_properties(ilbc-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

set(CMAKE_C_FLAGS "-O2 -Wall -Wextra -Werror -fvisibility=hidden -Wno-unused-parameter -fno-strict-aliasing")

set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "\${prefix}")
set(libdir "\${exec_prefix}/lib")
set(includedir "\${prefix}/include")
configure_file(libilbc.pc.in libilbc.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libilbc.pc DESTINATION lib/${CMAKE_LIBRARY_ARCHITECTURE}/pkgconfig)
install(FILES ilbc/interface/ilbc.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ilbc ilbc-static DESTINATION lib/${CMAKE_LIBRARY_ARCHITECTURE}/)
