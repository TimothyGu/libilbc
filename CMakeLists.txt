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

cmake_minimum_required(VERSION 2.8)

find_package(Threads)

if(CMAKE_USE_WIN32_THREADS_INIT)
    set(THREADS TRUE)
elseif(CMAKE_USE_PTHREADS_INIT)
    set(WEBRTC_POSIX TRUE)
    set(THREADS TRUE)
endif()

# Disable unsupported threads implementation
if(NOT THREADS)
    set(CMAKE_THREAD_LIBS_INIT "")
else()
    set(PTHREAD_LIBS ${CMAKE_THREAD_LIBS_INIT})
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmakein
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(. ilbc/interface signal_processing/include
                    ${CMAKE_CURRENT_BINARY_DIR})
set(ilbc_source_files
    ilbc/abs_quant.c
    ilbc/abs_quant_loop.c
    ilbc/augmented_cb_corr.c
    ilbc/bw_expand.c
    ilbc/cb_construct.c
    ilbc/cb_mem_energy.c
    ilbc/cb_mem_energy_augmentation.c
    ilbc/cb_mem_energy_calc.c
    ilbc/cb_search.c
    ilbc/cb_search_core.c
    ilbc/cb_update_best_index.c
    ilbc/chebyshev.c
    ilbc/comp_corr.c
    ilbc/constants.c
    ilbc/create_augmented_vec.c
    ilbc/decode.c
    ilbc/decode_residual.c
    ilbc/decoder_interpolate_lsf.c
    ilbc/do_plc.c
    ilbc/encode.c
    ilbc/energy_inverse.c
    ilbc/enh_upsample.c
    ilbc/enhancer.c
    ilbc/enhancer_interface.c
    ilbc/filtered_cb_vecs.c
    ilbc/frame_classify.c
    ilbc/gain_dequant.c
    ilbc/gain_quant.c
    ilbc/get_cd_vec.c
    ilbc/get_lsp_poly.c
    ilbc/get_sync_seq.c
    ilbc/hp_input.c
    ilbc/hp_output.c
    ilbc/ilbc.c
    ilbc/index_conv_dec.c
    ilbc/index_conv_enc.c
    ilbc/init_decode.c
    ilbc/init_encode.c
    ilbc/interpolate.c
    ilbc/interpolate_samples.c
    ilbc/lpc_encode.c
    ilbc/lsf_check.c
    ilbc/lsf_interpolate_to_poly_dec.c
    ilbc/lsf_interpolate_to_poly_enc.c
    ilbc/lsf_to_lsp.c
    ilbc/lsf_to_poly.c
    ilbc/lsp_to_lsf.c
    ilbc/my_corr.c
    ilbc/nearest_neighbor.c
    ilbc/pack_bits.c
    ilbc/poly_to_lsf.c
    ilbc/poly_to_lsp.c
    ilbc/refiner.c
    ilbc/simple_interpolate_lsf.c
    ilbc/simple_lpc_analysis.c
    ilbc/simple_lsf_dequant.c
    ilbc/simple_lsf_quant.c
    ilbc/smooth.c
    ilbc/smooth_out_data.c
    ilbc/sort_sq.c
    ilbc/split_vq.c
    ilbc/state_construct.c
    ilbc/state_search.c
    ilbc/swap_bytes.c
    ilbc/unpack_bits.c
    ilbc/vq3.c
    ilbc/vq4.c
    ilbc/window32_w32.c
    ilbc/xcorr_coef.c
    signal_processing/auto_corr_to_refl_coef.c
    signal_processing/auto_correlation.c
    signal_processing/complex_bit_reverse.c
    signal_processing/complex_fft.c
    signal_processing/copy_set_operations.c
    signal_processing/cross_correlation.c
    signal_processing/division_operations.c
    signal_processing/dot_product_with_scale.c
    signal_processing/downsample_fast.c
    signal_processing/energy.c
    signal_processing/filter_ar.c
    signal_processing/filter_ar_fast_q12.c
    signal_processing/filter_ma_fast_q12.c
    signal_processing/get_hanning_window.c
    signal_processing/get_scaling_square.c
    signal_processing/ilbc_specific_functions.c
    signal_processing/levinson_durbin.c
    signal_processing/lpc_to_refl_coef.c
    signal_processing/min_max_operations.c
    signal_processing/randomization_functions.c
    signal_processing/real_fft.c
    signal_processing/refl_coef_to_lpc.c
    signal_processing/resample.c
    signal_processing/resample_48khz.c
    signal_processing/resample_by_2.c
    signal_processing/resample_by_2_internal.c
    signal_processing/resample_fractional.c
    signal_processing/spl_init.c
    signal_processing/spl_sqrt.c
    signal_processing/spl_sqrt_floor.c
    signal_processing/splitting_filter.c
    signal_processing/sqrt_of_one_minus_x_squared.c
    signal_processing/vector_scaling_operations.c)

add_library(ilbc SHARED ${ilbc_source_files})
set_target_properties(ilbc PROPERTIES VERSION 1.1.1 SOVERSION 1)
set_target_properties(ilbc PROPERTIES CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(ilbc ${CMAKE_THREAD_LIBS_INIT})

add_library(ilbc-static STATIC ${ilbc_source_files})
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
