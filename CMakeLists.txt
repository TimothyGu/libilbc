# Copyright (c) 2012  Jeroen Dekkers <jeroen@dekkers.ch>
# Copyright (c) 2014  Tiancheng "Timothy" Gu <timothygu99@gmail.com>
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

cmake_minimum_required(VERSION 3.5)

project(libilbc
        VERSION 3.0.4
        DESCRIPTION "WebRTC's implementation of the Internet Low Bitrate Codec (iLBC)"
        HOMEPAGE_URL "https://github.com/TimothyGu/libilbc"
        LANGUAGES C CXX)

include(CheckCSourceCompiles)
include(GNUInstallDirs)
include(GenerateExportHeader)

#############################################################################
## Options
#############################################################################

option(BUILD_SHARED_LIBS "Build a shared library instead of a static one"
       ON)

configure_file(libilbc.pc.in libilbc.pc @ONLY)

#############################################################################
## Compiler flags
#############################################################################

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_CXX_STANDARD 14)

if((CMAKE_C_COMPILER_ID STREQUAL "GNU") OR
   (CMAKE_C_COMPILER_ID MATCHES "Clang" AND CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "GNU"))
    set(CMAKE_C_FLAGS "-Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wno-unused-parameter -fno-strict-aliasing")
    set(CMAKE_C_FLAGS_DEBUG "-g")
    set(CMAKE_C_FLAGS_RELEASE "-O2")
endif()
if((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
   (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU"))
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -fno-strict-aliasing")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
endif()

if((CMAKE_C_COMPILER_ID MATCHES "Clang" AND CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wno-unused-parameter /clang:-fno-strict-aliasing")
endif()
if((CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-unused-parameter /clang:-fno-strict-aliasing")
endif()

# https://cmake.org/cmake/help/latest/command/add_compile_definitions.html
# add_compile_definitions is new in CMake 3.12.
if(NOT DEFINED add_compile_definitions)
    function(add_compile_definitions)
        foreach(def ${ARGN})
            add_definitions("-D${def}")
        endforeach()
    endfunction()
endif()

if(UNIX)
    add_compile_definitions(WEBRTC_POSIX)
endif()
if(APPLE)
    add_compile_definitions(WEBRTC_MAC)
elseif(UNIX)
    add_compile_definitions(WEBRTC_LINUX)
endif()
if(WIN32)
    add_compile_definitions(WEBRTC_WIN)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "mipsel")
    set(MIPS32EL ON)
    add_compile_definitions(MIPS32_LE)
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_GENERATOR_PLATFORM STREQUAL "ARM64")
    set(ARM64 ON)
    set(WITH_NEON ON)
    add_compile_definitions(WEBRTC_ARCH_ARM64)

    message(STATUS "NEON optimizations - on")

 elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm.*" OR CMAKE_GENERATOR_PLATFORM STREQUAL "ARM")
    set(ARM ON)

    enable_language(ASM)

    option(WITH_NEON "Enable NEON optimization." OFF)

    check_c_source_compiles("
        #include <arm_neon.h>
        int main(void) { return 0; }
    " SUPPORTS_NEON)
    if(NOT SUPPORTS_NEON AND WITH_NEON)
        message(FATAL_ERROR "WITH_NEON is set to ON but compiler does not support NEON.")
    endif()

    check_c_source_compiles("\
        #ifndef __ARM_FEATURE_DSP\
        #error \"No DSP instructions\"\
        #endif\
    " ARM_DSP_INSN)

endif()

if(WITH_NEON)
    message(STATUS "NEON optimizations - on")
    add_compile_definitions(WEBRTC_HAS_NEON)
elseif(ARM OR ARM64)
    message(STATUS "NEON optimizations - off")
endif()

#############################################################################
## Configure input files
#############################################################################

include_directories(. abseil-cpp ${CMAKE_CURRENT_BINARY_DIR})

set(ilbc_source_files
    modules/audio_coding/codecs/ilbc/abs_quant.c
    modules/audio_coding/codecs/ilbc/abs_quant_loop.c
    modules/audio_coding/codecs/ilbc/augmented_cb_corr.c
    modules/audio_coding/codecs/ilbc/bw_expand.c
    modules/audio_coding/codecs/ilbc/cb_construct.c
    modules/audio_coding/codecs/ilbc/cb_mem_energy_augmentation.c
    modules/audio_coding/codecs/ilbc/cb_mem_energy.c
    modules/audio_coding/codecs/ilbc/cb_mem_energy_calc.c
    modules/audio_coding/codecs/ilbc/cb_search.c
    modules/audio_coding/codecs/ilbc/cb_search_core.c
    modules/audio_coding/codecs/ilbc/cb_update_best_index.c
    modules/audio_coding/codecs/ilbc/chebyshev.c
    modules/audio_coding/codecs/ilbc/comp_corr.c
    modules/audio_coding/codecs/ilbc/constants.c
    modules/audio_coding/codecs/ilbc/create_augmented_vec.c
    modules/audio_coding/codecs/ilbc/decode.c
    modules/audio_coding/codecs/ilbc/decode_residual.c
    modules/audio_coding/codecs/ilbc/decoder_interpolate_lsf.c
    modules/audio_coding/codecs/ilbc/do_plc.c
    modules/audio_coding/codecs/ilbc/encode.c
    modules/audio_coding/codecs/ilbc/energy_inverse.c
    modules/audio_coding/codecs/ilbc/enhancer.c
    modules/audio_coding/codecs/ilbc/enhancer_interface.c
    modules/audio_coding/codecs/ilbc/enh_upsample.c
    modules/audio_coding/codecs/ilbc/filtered_cb_vecs.c
    modules/audio_coding/codecs/ilbc/frame_classify.c
    modules/audio_coding/codecs/ilbc/gain_dequant.c
    modules/audio_coding/codecs/ilbc/gain_quant.c
    modules/audio_coding/codecs/ilbc/get_cd_vec.c
    modules/audio_coding/codecs/ilbc/get_lsp_poly.c
    modules/audio_coding/codecs/ilbc/get_sync_seq.c
    modules/audio_coding/codecs/ilbc/hp_input.c
    modules/audio_coding/codecs/ilbc/hp_output.c
    modules/audio_coding/codecs/ilbc/ilbc.c
    modules/audio_coding/codecs/ilbc/index_conv_dec.c
    modules/audio_coding/codecs/ilbc/index_conv_enc.c
    modules/audio_coding/codecs/ilbc/init_decode.c
    modules/audio_coding/codecs/ilbc/init_encode.c
    modules/audio_coding/codecs/ilbc/interpolate.c
    modules/audio_coding/codecs/ilbc/interpolate_samples.c
    modules/audio_coding/codecs/ilbc/lpc_encode.c
    modules/audio_coding/codecs/ilbc/lsf_check.c
    modules/audio_coding/codecs/ilbc/lsf_interpolate_to_poly_dec.c
    modules/audio_coding/codecs/ilbc/lsf_interpolate_to_poly_enc.c
    modules/audio_coding/codecs/ilbc/lsf_to_lsp.c
    modules/audio_coding/codecs/ilbc/lsf_to_poly.c
    modules/audio_coding/codecs/ilbc/lsp_to_lsf.c
    modules/audio_coding/codecs/ilbc/my_corr.c
    modules/audio_coding/codecs/ilbc/nearest_neighbor.c
    modules/audio_coding/codecs/ilbc/pack_bits.c
    modules/audio_coding/codecs/ilbc/poly_to_lsf.c
    modules/audio_coding/codecs/ilbc/poly_to_lsp.c
    modules/audio_coding/codecs/ilbc/refiner.c
    modules/audio_coding/codecs/ilbc/simple_interpolate_lsf.c
    modules/audio_coding/codecs/ilbc/simple_lpc_analysis.c
    modules/audio_coding/codecs/ilbc/simple_lsf_dequant.c
    modules/audio_coding/codecs/ilbc/simple_lsf_quant.c
    modules/audio_coding/codecs/ilbc/smooth.c
    modules/audio_coding/codecs/ilbc/smooth_out_data.c
    modules/audio_coding/codecs/ilbc/sort_sq.c
    modules/audio_coding/codecs/ilbc/split_vq.c
    modules/audio_coding/codecs/ilbc/state_construct.c
    modules/audio_coding/codecs/ilbc/state_search.c
    modules/audio_coding/codecs/ilbc/swap_bytes.c
    modules/audio_coding/codecs/ilbc/unpack_bits.c
    modules/audio_coding/codecs/ilbc/vq3.c
    modules/audio_coding/codecs/ilbc/vq4.c
    modules/audio_coding/codecs/ilbc/window32_w32.c
    modules/audio_coding/codecs/ilbc/xcorr_coef.c

    common_audio/signal_processing/auto_correlation.c
    common_audio/signal_processing/auto_corr_to_refl_coef.c
    common_audio/signal_processing/copy_set_operations.c
    common_audio/signal_processing/cross_correlation.c
    common_audio/signal_processing/division_operations.c
    common_audio/signal_processing/dot_product_with_scale.cc
    common_audio/signal_processing/downsample_fast.c
    common_audio/signal_processing/energy.c
    common_audio/signal_processing/filter_ar.c
    common_audio/signal_processing/filter_ma_fast_q12.c
    common_audio/signal_processing/get_hanning_window.c
    common_audio/signal_processing/get_scaling_square.c
    common_audio/signal_processing/ilbc_specific_functions.c
    common_audio/signal_processing/levinson_durbin.c
    common_audio/signal_processing/lpc_to_refl_coef.c
    common_audio/signal_processing/min_max_operations.c
    common_audio/signal_processing/randomization_functions.c
    common_audio/signal_processing/real_fft.c
    common_audio/signal_processing/refl_coef_to_lpc.c
    common_audio/signal_processing/resample_48khz.c
    common_audio/signal_processing/resample_by_2.c
    common_audio/signal_processing/resample_by_2_internal.c
    common_audio/signal_processing/resample.c
    common_audio/signal_processing/resample_fractional.c
    common_audio/signal_processing/spl_init.c
    common_audio/signal_processing/spl_inl.c
    common_audio/signal_processing/splitting_filter.c
    common_audio/signal_processing/spl_sqrt.c
    common_audio/signal_processing/sqrt_of_one_minus_x_squared.c
    common_audio/signal_processing/vector_scaling_operations.c


    rtc_base/checks.cc)

if(WITH_NEON)
    set(ilbc_source_files ${ilbc_source_files}
        common_audio/signal_processing/cross_correlation_neon.c
        common_audio/signal_processing/downsample_fast_neon.c
        common_audio/signal_processing/min_max_operations_neon.c)
elseif(MIPS32EL)
    set(ilbc_source_files ${ilbc_source_files}
        common_audio/signal_processing/cross_correlation_mips.c
        common_audio/signal_processing/downsample_fast_mips.c
        common_audio/signal_processing/min_max_operations_mips.c
        common_audio/signal_processing/resample_by_2_mips.c)
endif()

if(MIPS32EL)
    set(ilbc_source_files ${ilbc_source_files}
        common_audio/signal_processing/complex_fft_mips.c)
else()
    set(ilbc_source_files ${ilbc_source_files}
        common_audio/signal_processing/complex_fft.c)
endif()

if(ARM)
    set(ilbc_source_files ${ilbc_source_files}
        common_audio/signal_processing/complex_bit_reverse_arm.S
        common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor_arm.S)
elseif(MIPS32EL)
    set(ilbc_source_files ${ilbc_source_files}
        common_audio/signal_processing/complex_bit_reverse_mips.c
        common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor_mips.c)
else()
    set(ilbc_source_files ${ilbc_source_files}
        common_audio/signal_processing/complex_bit_reverse.c
        common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.c)
endif()

if(ARM_DSP_INSN)
    set(ilbc_source_files ${ilbc_source_files}
        common_audio/signal_processing/filter_ar_fast_q12_armv7.S)
elseif(MIPS32EL)
    set(ilbc_source_files ${ilbc_source_files}
        common_audio/signal_processing/filter_ar_fast_q12_mips.c)
else()
    set(ilbc_source_files ${ilbc_source_files}
        common_audio/signal_processing/filter_ar_fast_q12.c)
endif()

# Other MIPS-only files:
# common_audio/signal_processing/vector_scaling_operations_mips.c (requires MIPS DSP to be available)

#############################################################################
## Configure output files
#############################################################################

add_library(ilbc ${ilbc_source_files})
generate_export_header(ilbc)
set_target_properties(ilbc PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 3)
set_target_properties(ilbc PROPERTIES CLEAN_DIRECT_OUTPUT 1)

add_executable(ilbc_test modules/audio_coding/codecs/ilbc/test/iLBC_test.c)
target_link_libraries(ilbc_test ilbc)

add_executable(ilbc_test2 iLBC_test2.c)
target_link_libraries(ilbc_test2 ilbc)

add_custom_target(ilbc_test-sample
                  ilbc_test 20 ${CMAKE_CURRENT_SOURCE_DIR}/sample.pcm encoded.ilbc decoded.pcm)
add_custom_target(ilbc_test2-sample
                  ilbc_test2 20 ${CMAKE_CURRENT_SOURCE_DIR}/sample.pcm encoded2.ilbc decoded2.pcm)

###################################################################################################
## Installation
###################################################################################################

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libilbc.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
install(FILES ilbc.h ${CMAKE_CURRENT_BINARY_DIR}/ilbc_export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES CONTRIBUTING.md NEWS.md README.md
        DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(TARGETS ilbc ilbc_test
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
