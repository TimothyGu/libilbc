/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

/*
 * ilbc.h
 *
 * This header file contains all of the API's for iLBC.
 *
 */

#ifndef WEBRTC_MODULES_AUDIO_CODING_CODECS_ILBC_MAIN_INTERFACE_ILBC_H_
#define WEBRTC_MODULES_AUDIO_CODING_CODECS_ILBC_MAIN_INTERFACE_ILBC_H_

/*
 * Define the fixpoint numeric formats
 */

#include "typedefs.h"

/*
 * Solution to support multiple instances
 * Customer has to cast instance to proper type
 */

typedef struct iLBC_encinst_t_ iLBC_encinst_t;

typedef struct iLBC_decinst_t_ iLBC_decinst_t;

/*
 * Comfort noise constants
 */

#define ILBC_SPEECH 1
#define ILBC_CNG  2

#ifdef __cplusplus
extern "C" {
#endif

  /****************************************************************************
   * WebRtcIlbcfix_XxxAssign(...)
   *
   * These functions assigns the encoder/decoder instance to the specified
   * memory location
   *
   * Input:
   *      - XXX_xxxinst       : Pointer to created instance that should be
   *                            assigned
   *      - ILBCXXX_inst_Addr : Pointer to the desired memory space
   *      - size              : The size that this structure occupies (in Word16)
   *
   * Return value             :  0 - Ok
   *                            -1 - Error
   */

  WebRtc_Word16 WebRtcIlbcfix_EncoderAssign(iLBC_encinst_t **iLBC_encinst,
					    WebRtc_Word16 *ILBCENC_inst_Addr,
					    WebRtc_Word16 *size);
  WebRtc_Word16 WebRtcIlbcfix_DecoderAssign(iLBC_decinst_t **iLBC_decinst,
					    WebRtc_Word16 *ILBCDEC_inst_Addr,
					    WebRtc_Word16 *size);


  /****************************************************************************
   * WebRtcIlbcfix_XxxAssign(...)
   *
   * These functions create a instance to the specified structure
   *
   * Input:
   *      - XXX_inst          : Pointer to created instance that should be created
   *
   * Return value             :  0 - Ok
   *                            -1 - Error
   */

  WebRtc_Word16 WebRtcIlbcfix_EncoderCreate(iLBC_encinst_t **iLBC_encinst);
  WebRtc_Word16 WebRtcIlbcfix_DecoderCreate(iLBC_decinst_t **iLBC_decinst);

  /****************************************************************************
   * WebRtcIlbcfix_XxxFree(...)
   *
   * These functions frees the dynamic memory of a specified instance
   *
   * Input:
   *      - XXX_inst          : Pointer to created instance that should be freed
   *
   * Return value             :  0 - Ok
   *                            -1 - Error
   */

  WebRtc_Word16 WebRtcIlbcfix_EncoderFree(iLBC_encinst_t *iLBC_encinst);
  WebRtc_Word16 WebRtcIlbcfix_DecoderFree(iLBC_decinst_t *iLBC_decinst);


  /****************************************************************************
   * WebRtcIlbcfix_EncoderInit(...)
   *
   * This function initializes a iLBC instance
   *
   * Input:
   *      - iLBCenc_inst      : iLBC instance, i.e. the user that should receive
   *                            be initialized
   *      - frameLen          : The frame length of the codec 20/30 (ms)
   *
   * Return value             :  0 - Ok
   *                            -1 - Error
   */

  WebRtc_Word16 WebRtcIlbcfix_EncoderInit(iLBC_encinst_t *iLBCenc_inst,
					  WebRtc_Word16 frameLen);

  /****************************************************************************
   * WebRtcIlbcfix_Encode(...)
   *
   * This function encodes one iLBC frame. Input speech length has be a
   * multiple of the frame length.
   *
   * Input:
   *      - iLBCenc_inst      : iLBC instance, i.e. the user that should encode
   *                            a package
   *      - speechIn          : Input speech vector
   *      - len               : Samples in speechIn (160, 240, 320 or 480)
   *
   * Output:
   *  - encoded               : The encoded data vector
   *
   * Return value             : >0 - Length (in bytes) of coded data
   *                            -1 - Error
   */

  WebRtc_Word16 WebRtcIlbcfix_Encode(iLBC_encinst_t *iLBCenc_inst,
				     WebRtc_Word16 *speechIn,
				     WebRtc_Word16 len,
				     WebRtc_Word16 *encoded);

  /****************************************************************************
   * WebRtcIlbcfix_DecoderInit(...)
   *
   * This function initializes a iLBC instance with either 20 or 30 ms frames
   * Alternatively the WebRtcIlbcfix_DecoderInit_XXms can be used. Then it's
   * not needed to specify the frame length with a variable.
   *
   * Input:
   *      - iLBC_decinst_t    : iLBC instance, i.e. the user that should receive
   *                            be initialized
   *      - frameLen          : The frame length of the codec 20/30 (ms)
   *
   * Return value             :  0 - Ok
   *                            -1 - Error
   */

  WebRtc_Word16 WebRtcIlbcfix_DecoderInit(iLBC_decinst_t *iLBCdec_inst,
					  WebRtc_Word16 frameLen);
  WebRtc_Word16 WebRtcIlbcfix_DecoderInit20Ms(iLBC_decinst_t *iLBCdec_inst);
  WebRtc_Word16 WebRtcIlbcfix_Decoderinit30Ms(iLBC_decinst_t *iLBCdec_inst);

  /****************************************************************************
   * WebRtcIlbcfix_Decode(...)
   *
   * This function decodes a packet with iLBC frame(s). Output speech length
   * will be a multiple of 160 or 240 samples ((160 or 240)*frames/packet).
   *
   * Input:
   *      - iLBCdec_inst      : iLBC instance, i.e. the user that should decode
   *                            a packet
   *      - encoded           : Encoded iLBC frame(s)
   *      - len               : Bytes in encoded vector
   *
   * Output:
   *      - decoded           : The decoded vector
   *      - speechType        : 1 normal, 2 CNG
   *
   * Return value             : >0 - Samples in decoded vector
   *                            -1 - Error
   */

  WebRtc_Word16 WebRtcIlbcfix_Decode(iLBC_decinst_t *iLBCdec_inst,
				     WebRtc_Word16* encoded,
				     WebRtc_Word16 len,
				     WebRtc_Word16 *decoded,
				     WebRtc_Word16 *speechType);
  WebRtc_Word16 WebRtcIlbcfix_Decode20Ms(iLBC_decinst_t *iLBCdec_inst,
					 WebRtc_Word16 *encoded,
					 WebRtc_Word16 len,
					 WebRtc_Word16 *decoded,
					 WebRtc_Word16 *speechType);
  WebRtc_Word16 WebRtcIlbcfix_Decode30Ms(iLBC_decinst_t *iLBCdec_inst,
					 WebRtc_Word16 *encoded,
					 WebRtc_Word16 len,
					 WebRtc_Word16 *decoded,
					 WebRtc_Word16 *speechType);

  /****************************************************************************
   * WebRtcIlbcfix_DecodePlc(...)
   *
   * This function conducts PLC for iLBC frame(s). Output speech length
   * will be a multiple of 160 or 240 samples.
   *
   * Input:
   *      - iLBCdec_inst      : iLBC instance, i.e. the user that should perform
   *                            a PLC
   *      - noOfLostFrames    : Number of PLC frames to produce
   *
   * Output:
   *      - decoded           : The "decoded" vector
   *
   * Return value             : >0 - Samples in decoded PLC vector
   *                            -1 - Error
   */

  WebRtc_Word16 WebRtcIlbcfix_DecodePlc(iLBC_decinst_t *iLBCdec_inst,
					WebRtc_Word16 *decoded,
					WebRtc_Word16 noOfLostFrames);

  /****************************************************************************
   * WebRtcIlbcfix_NetEqPlc(...)
   *
   * This function updates the decoder when a packet loss has occured, but it
   * does not produce any PLC data. Function can be used if another PLC method
   * is used (i.e NetEq).
   *
   * Input:
   *      - iLBCdec_inst      : iLBC instance that should be updated
   *      - noOfLostFrames    : Number of lost frames
   *
   * Output:
   *      - decoded           : The "decoded" vector (nothing in this case)
   *
   * Return value             : >0 - Samples in decoded PLC vector
   *                            -1 - Error
   */

  WebRtc_Word16 WebRtcIlbcfix_NetEqPlc(iLBC_decinst_t *iLBCdec_inst,
				       WebRtc_Word16 *decoded,
				       WebRtc_Word16 noOfLostFrames);

  /****************************************************************************
   * WebRtcIlbcfix_version(...)
   *
   * This function returns the version number of iLBC
   *
   * Output:
   *      - version           : Version number of iLBC (maximum 20 char)
   */

  void WebRtcIlbcfix_version(WebRtc_Word8 *version);


  /****************************************************************************
   * Compatibility with the library code from RFC3951.
   *
   */

  /* general codec settings */

#define FS       8000
#define BLOCKL_20MS     160
#define BLOCKL_30MS     240
#define BLOCKL_MAX     240
#define NSUB_20MS     4
#define NSUB_30MS     6
#define NSUB_MAX     6
#define NASUB_20MS     2
#define NASUB_30MS     4
#define NASUB_MAX     4
#define SUBL      40
#define STATE_LEN     80
#define STATE_SHORT_LEN_30MS  58
#define STATE_SHORT_LEN_20MS  57

  /* LPC settings */

#define LPC_FILTERORDER    10
#define LPC_LOOKBACK    60
#define LPC_N_20MS     1
#define LPC_N_30MS     2
#define LPC_N_MAX     2
#define LPC_ASYMDIFF    20
#define LSF_NSPLIT     3
#define LSF_NUMBER_OF_STEPS   4
#define LPC_HALFORDER    5
#define COS_GRID_POINTS 60

  /* enhancer */

#define ENH_BLOCKL     80  /* block length */
#define ENH_BLOCKL_HALF    (ENH_BLOCKL/2)
#define ENH_HL      3  /* 2*ENH_HL+1 is number blocks
			  in said second sequence */
#define ENH_SLOP     2  /* max difference estimated and
			   correct pitch period */
#define ENH_PLOCSL     8  /* pitch-estimates and
			     pitch-locations buffer length */
#define ENH_OVERHANG    2
#define ENH_UPS0     4  /* upsampling rate */
#define ENH_FL0      3  /* 2*FLO+1 is the length of each filter */
#define ENH_FLO_MULT2_PLUS1   7
#define ENH_VECTL     (ENH_BLOCKL+2*ENH_FL0)
#define ENH_CORRDIM     (2*ENH_SLOP+1)
#define ENH_NBLOCKS     (BLOCKL/ENH_BLOCKL)
#define ENH_NBLOCKS_EXTRA   5
#define ENH_NBLOCKS_TOT    8 /* ENH_NBLOCKS+ENH_NBLOCKS_EXTRA */
#define ENH_BUFL     (ENH_NBLOCKS_TOT)*ENH_BLOCKL
#define ENH_BUFL_FILTEROVERHEAD  3
#define ENH_A0      819   /* Q14 */
#define ENH_A0_MINUS_A0A0DIV4  848256041 /* Q34 */
#define ENH_A0DIV2     26843546 /* Q30 */

  /* type definition encoder instance */
  typedef struct iLBC_Enc_Inst_t_ {

    /* flag for frame size mode */
    WebRtc_Word16 mode;

    /* basic parameters for different frame sizes */
    WebRtc_Word16 blockl;
    WebRtc_Word16 nsub;
    WebRtc_Word16 nasub;
    WebRtc_Word16 no_of_bytes, no_of_words;
    WebRtc_Word16 lpc_n;
    WebRtc_Word16 state_short_len;

    /* analysis filter state */
    WebRtc_Word16 anaMem[LPC_FILTERORDER];

    /* Fix-point old lsf parameters for interpolation */
    WebRtc_Word16 lsfold[LPC_FILTERORDER];
    WebRtc_Word16 lsfdeqold[LPC_FILTERORDER];

    /* signal buffer for LP analysis */
    WebRtc_Word16 lpc_buffer[LPC_LOOKBACK + BLOCKL_MAX];

    /* state of input HP filter */
    WebRtc_Word16 hpimemx[2];
    WebRtc_Word16 hpimemy[4];

#ifdef SPLIT_10MS
    WebRtc_Word16 weightdenumbuf[66];
    WebRtc_Word16 past_samples[160];
    WebRtc_UWord16 bytes[25];
    WebRtc_Word16 section;
    WebRtc_Word16 Nfor_flag;
    WebRtc_Word16 Nback_flag;
    WebRtc_Word16 start_pos;
    WebRtc_Word16 diff;
#endif

  } iLBC_Enc_Inst_t;

  /* type definition decoder instance */
  typedef struct iLBC_Dec_Inst_t_ {

    /* flag for frame size mode */
    WebRtc_Word16 mode;

    /* basic parameters for different frame sizes */
    WebRtc_Word16 blockl;
    WebRtc_Word16 nsub;
    WebRtc_Word16 nasub;
    WebRtc_Word16 no_of_bytes, no_of_words;
    WebRtc_Word16 lpc_n;
    WebRtc_Word16 state_short_len;

    /* synthesis filter state */
    WebRtc_Word16 syntMem[LPC_FILTERORDER];

    /* old LSF for interpolation */
    WebRtc_Word16 lsfdeqold[LPC_FILTERORDER];

    /* pitch lag estimated in enhancer and used in PLC */
    int last_lag;

    /* PLC state information */
    int consPLICount, prev_enh_pl;
    WebRtc_Word16 perSquare;

    WebRtc_Word16 prevScale, prevPLI;
    WebRtc_Word16 prevLag, prevLpc[LPC_FILTERORDER+1];
    WebRtc_Word16 prevResidual[NSUB_MAX*SUBL];
    WebRtc_Word16 seed;

    /* previous synthesis filter parameters */

    WebRtc_Word16 old_syntdenum[(LPC_FILTERORDER + 1)*NSUB_MAX];

    /* state of output HP filter */
    WebRtc_Word16 hpimemx[2];
    WebRtc_Word16 hpimemy[4];

    /* enhancer state information */
    int use_enhancer;
    WebRtc_Word16 enh_buf[ENH_BUFL+ENH_BUFL_FILTEROVERHEAD];
    WebRtc_Word16 enh_period[ENH_NBLOCKS_TOT];

  } iLBC_Dec_Inst_t;

#define initDecode WebRtcIlbcfix_InitDecode
#define initEncode WebRtcIlbcfix_InitEncode
#define iLBC_decode WebRtcIlbcfix_DecodeImpl
#define iLBC_encode WebRtcIlbcfix_EncodeImpl
  WebRtc_Word16 WebRtcIlbcfix_InitDecode(iLBC_Dec_Inst_t *iLBCdec_inst,
					 WebRtc_Word16 mode,
					 int use_enhancer);
  WebRtc_Word16 WebRtcIlbcfix_InitEncode(iLBC_Enc_Inst_t *iLBCenc_inst,
					 WebRtc_Word16 mode);
  void WebRtcIlbcfix_DecodeImpl(WebRtc_Word16 *decblock,
				WebRtc_UWord16 *bytes,
				iLBC_Dec_Inst_t *iLBCdec_inst,
				WebRtc_Word16 mode);
  void WebRtcIlbcfix_EncodeImpl(WebRtc_UWord16 *bytes,
				WebRtc_Word16 *block,
				iLBC_Enc_Inst_t *iLBCenc_inst);

#ifdef __cplusplus
}
#endif

#endif

