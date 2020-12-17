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

project(libilbc)

include(GenerateExportHeader)
include(GNUInstallDirs)

set(PACKAGE_VERSION 3.0.0-dev)

#############################################################################
## Options
#############################################################################

option(BUILD_SHARED_LIBS "Build a shared library instead of a static one"
       ON)

configure_file(libilbc.pc.in libilbc.pc @ONLY)

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
    common_audio/signal_processing/complex_bit_reverse.c
    common_audio/signal_processing/complex_fft.c
    common_audio/signal_processing/complex_fft_tables.h
    common_audio/signal_processing/copy_set_operations.c
    common_audio/signal_processing/cross_correlation.c
    common_audio/signal_processing/division_operations.c
    common_audio/signal_processing/dot_product_with_scale.cc
    common_audio/signal_processing/dot_product_with_scale.h
    common_audio/signal_processing/downsample_fast.c
    common_audio/signal_processing/energy.c
    common_audio/signal_processing/filter_ar.c
    common_audio/signal_processing/filter_ar_fast_q12.c
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
    common_audio/signal_processing/resample_by_2_internal.h
    common_audio/signal_processing/resample.c
    common_audio/signal_processing/resample_fractional.c
    common_audio/signal_processing/spl_init.c
    common_audio/signal_processing/spl_inl.c
    common_audio/signal_processing/splitting_filter.c
    common_audio/signal_processing/spl_sqrt.c
    common_audio/signal_processing/sqrt_of_one_minus_x_squared.c
    common_audio/signal_processing/vector_scaling_operations.c

    common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.c
    common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.h

    rtc_base/checks.cc)

# ARM-only files:
# common_audio/signal_processing/complex_bit_reverse_arm.S
# common_audio/signal_processing/cross_correlation_neon.c
# common_audio/signal_processing/downsample_fast_neon.c
# common_audio/signal_processing/filter_ar_fast_q12_armv7.S
# common_audio/signal_processing/min_max_operations_neon.c
# common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor_arm.S

# MIPS-only files:
# common_audio/signal_processing/complex_bit_reverse_mips.c
# common_audio/signal_processing/complex_fft_mips.c
# common_audio/signal_processing/cross_correlation_mips.c
# common_audio/signal_processing/downsample_fast_mips.c
# common_audio/signal_processing/filter_ar_fast_q12_mips.c
# common_audio/signal_processing/min_max_operations_mips.c
# common_audio/signal_processing/resample_by_2_mips.c
# common_audio/signal_processing/vector_scaling_operations_mips.c
# common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor_mips.c

#############################################################################
## Compiler flags
#############################################################################

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(${CMAKE_COMPILER_IS_GNUCC})
    set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wstrict-prototypes -Wno-unused-parameter -fno-strict-aliasing")
    set(CMAKE_C_FLAGS_DEBUG "-g")
    set(CMAKE_C_FLAGS_RELEASE "-O2")

    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wno-unused-parameter -fno-strict-aliasing")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

if(UNIX)
    add_definitions(-DWEBRTC_POSIX)
endif()
if(APPLE)
    add_definitions(-DWEBRTC_MAC)
elseif(UNIX)
    add_definitions(-DWEBRTC_LINUX)
endif()
if(WIN32)
    add_definitions(-DWEBRTC_WIN)
endif()

#############################################################################
## Configure output files
#############################################################################

add_library(ilbc ${ilbc_source_files})
generate_export_header(ilbc)
set_target_properties(ilbc PROPERTIES VERSION ${PACKAGE_VERSION} SOVERSION 3)
set_target_properties(ilbc PROPERTIES CLEAN_DIRECT_OUTPUT 1)

add_executable(ilbc_test modules/audio_coding/codecs/ilbc/test/iLBC_test.c)
target_link_libraries(ilbc_test ilbc)

###################################################################################################
## Installation
###################################################################################################

set(includedir "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
set(libdir "${CMAKE_INSTALL_FULL_LIBDIR}")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libilbc.pc
        DESTINATION ${libdir}/pkgconfig)
install(FILES ilbc.h ilbc_export.h
        DESTINATION ${includedir})
install(TARGETS ilbc ilbc_test
        ARCHIVE DESTINATION ${libdir}
        LIBRARY DESTINATION ${libdir}
        RUNTIME DESTINATION bin)
