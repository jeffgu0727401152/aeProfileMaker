/*************************************************************************
                             All Rights Reserved
                                 R&D Dept.
*************************************************************************/


/*************************************************************************

Filename:       roland_cmd.h

Description:

Author:

Date:

Modify Log:
 ------------------------------------------------------------------------
1.
 修改人  :
 修改时间:
 修改原因:
 修改方法:
 ------------------------------------------------------------------------
2.
 修改人  :
 修改时间:
 修改原因:
 修改方法:
 ------------------------------------------------------------------------
*************************************************************************/
#ifndef _ROLAND_CMD_H_
#define _ROLAND_CMD_H_

#include <stdint.h>
#include "wsd_api.h"

/***********************************************************************
*                                包含头文件
************************************************************************/


/***********************************************************************
*                                常量定义
***********************************************************************/
typedef enum roland_function_block
{
    roland_mixer_block        = 0xB0,
    roland_mic_effects_block   = 0xB1,
    roland_echo_block         = 0xB2,
    roland_music_effects_block = 0xB3,
    roland_output_block     = 0xB4,
    roland_system_block       = 0xBF
} roland_block_e;

/*
 * (1)Mixer Block
 * Command Min Max Default Parameter
 * b0 00    00 7f   40      mic1 input level
 * b0 01    00 7f   40      mic2 input level
 * b0 02    00 7f   40      mic input level
 * b0 03    00 7f   7f      mic direct level
 *
 * b0 05    00 7f   7f      front mic dry level
 * b0 06    00 7f   40      music input level
 * b0 07    00 7f   7f      front music level
 * b0 08    00 7f   7f      rear output level
 * b0 09    00 7f   7f      front output level
 * b0 0a    00 7f   7f      subwoofer output level
 * b0 0b    00 7f   00      aux to mic
 * b0 0c    00 7f   00      aux to music
 * b0 0d    00 7f   7f      center output level
 * b0 0e    00 7f   7f      center mic level
 * b0 0f    00 7f   00      center music level
 *
 * b0 1d    00 7f   7f      front mic delay level
 * b0 1e    00 7f   7f      front mic reverb level
 *
 * b0 38    00 7f   7f      subwoofer mic dry level
 * b0 39    00 7f   7f      subwoofer music level
 * b0 3a    00 7f   7f      subwoofer mic delay level
 * b0 3b    00 7f   7f      subwoofer mic reverb level
 *
 * b0 40    00 7f   7f      rear mic dry level
 * b0 41    00 7f   7f      rear music level
 * b0 42    00 7f   7f      rear mic delay level
 * b0 43    00 7f   7f      rear mic reverb level
 *
 * b0 52    00 7f   7f      center mic delay level
 * b0 53    00 7f   7f      center mic reverb level
 * B0 60    00 01   00      Stereo Switch for Center/Sub Output(0=Mono, 1Stereo)
*/
typedef enum roland_mixer_cmd
{
    mic1_input_level = 0,
    mic2_input_level,
    mic_input_level,
    mic_direct_level,

    front_mic_dry_level = 5,
    music_input_level,
    front_music_level,
    rear_output_level,
    front_output_level,
    subwoofer_output_level,
    aux_to_mic,
    aux_to_music,
    center_output_level,
    center_mic_level,
    center_music_level,

    front_mic_delay_level = 0x1d,
    front_mic_reverb_level,

    subwoofer_mic_dry_level = 0x38,
    subwoofer_music_level,
    subwoofer_mic_delay_level,
    subwoofer_mic_reverb_level,

    rear_mic_dry_level = 0x40,
    rear_music_level,
    rear_mic_delay_level,
    rear_mic_reverb_level,

    center_mic_delay_level = 0x52,
    center_mic_reverb_level,
    center_subwoofer_switch_stereo = 0x60,
    mixer_end
} roland_mixer_cmd_e;


/*
 * (2)MIC Effects Block
 * Command Min Max Default Parameter
 * b1 00    00 01   00      bass cut switch
 * b1 01    01 2a   01      bass cut frequency
 *
 * b1 10    00 01   00      noise supp. switch
 * b1 11    00 0f   03      noise supp. threshold
 *
 * b1 20    00 01   00      compressor switch
 * b1 21    00 0f   07      compressor sustain
 * b1 22    00 60   20      compressor out level
 *
 * b1 40    36 4a   40      frequency shift value
 *
 * b1 60    00 01   00      peq switch
 *
 * b1 62    00 01   00      peq band1 type
 * b1 63    04 7c   40      peq band1 gain
 * b1 64    02 2a   0c      peq band1 frequency
 * b1 65    00 1f   07      peq band1 q
 * b1 66    04 7c   40      peq band2 gain
 * b1 67    02 3e   12      peq band2 frequency
 * b1 68    00 1f   07      peq band2 q
 * b1 69    04 7c   40      peq band3 gain
 * b1 6a    02 3e   18      peq band3 frequency
 * b1 6b    00 1f   07      peq band3 q
 * b1 6c    04 7c   40      peq band4 gain
 * b1 6d    02 3e   1e      peq band4 frequency
 * b1 6e    00 1f   07      peq band4 q
 * b1 6f    04 7c   40      peq band5 gain
 * b1 70    02 3e   24      peq band5 frequency
 * b1 71    00 1f   07      peq band5 q
 * b1 72    04 7c   40      peq band6 gain
 * b1 73    02 3e   2a      peq band6 frequency
 * b1 74    00 1f   07      peq band6 q
 * b1 75    00 01   00      peq band7 type
 * b1 76    04 7c   40      peq band7 gain
 * b1 77    02 3e   30      peq band7 frequency
 * b1 78    00 1f   07      peq band7 q
*/
typedef enum roland_mic_effects_cmd
{
    bass_cut_switch = 0x00,
    bass_cut_frequency,

    noise_supp_switch = 0x10,
    noise_supp_threshold,

    compressor_switch = 0x20,
    compressor_sustain,
    compressor_out_level,

    frequency_shift_value = 0x40,

    peq_switch = 0x60,

    peq_band1_type = 0x62,
    peq_band1_gain,
    peq_band1_frequency,
    peq_band1_q,
    peq_band2_gain,
    peq_band2_frequency,
    peq_band2_q,
    peq_band3_gain,
    peq_band3_frequency,
    peq_band3_q,
    peq_band4_gain,
    peq_band4_frequency,
    peq_band4_q,
    peq_band5_gain,
    peq_band5_frequency,
    peq_band5_q,
    peq_band6_gain,
    peq_band6_frequency,
    peq_band6_q,
    peq_band7_type,
    peq_band7_gain,
    peq_band7_frequency,
    peq_band7_q,
    mic_effects_end
} roland_mic_effects_cmd_e;


/*
 * (3)Echo Block
 * Command Min Max Default Parameter
 * b2 00    00 7f   00      delay input level
 *
 * b2 02    39 47   40      delay tone gain
 *
 * b2 04    00 7f   14      delay time
 * b2 05    00 7f   32      delay feedback level
 * b2 06    00 7f   40      delay pan
 * b2 07    00 7f   7f      delay level
 * b2 08    00 7f   00      delay tap1 level
 * b2 09    00 7f   00      delay tap2 level
 * b2 0a    00 7f   00      delay tap3 level
 * b2 0b    00 7f   3f      delay tap1 time
 * b2 0c    00 7f   3f      delay tap2 time
 * b2 0d    00 7f   3f      delay tap3 time
 * b2 0e    00 7f   40      delay tap1 pan
 * b2 0f    00 7f   40      delay tap2 pan
 * b2 10    00 7f   40      delay tap3 pan
 * b2 11    00 7f   40      delay master level
 * b2 12    00 7f   00      delay to reverb level
 *
 * b2 18    00 64   00      delay pre delay time
 * b2 19    00 01   00      delay phase
 * b2 20    00 7f   00      reverb input level
 *
 * b2 22    00 03   02      reverb type
 * b2 23    39 47   40      reverb tone gain
 *
 * b2 25    00 13   0a      reverb time
 * b2 26    00 7f   00      reverb pre delay time
 * b2 27    00 7f   40      reverb output level
 *
 * b2 2c    00 01   00      reverb phase
 *
 * b2 30    00 01   00      mic dry phase
 *
 * b2 40    00 01   00      delay eq switch
 * b2 41    00 01   00      delay eq band1 type
 * b2 42    04 7c   40      delay eq band1 gain
 * b2 43    02 2a   0c      delay eq band1 frequency
 * b2 44    00 1f   07      delay eq band1 q
 * b2 45    04 7c   40      delay eq band2 gain
 * b2 46    02 3e   12      delay eq band2 frequency
 * b2 47    00 1f   07      delay eq band2 q
 * b2 48    04 7c   40      delay eq band3 gain
 * b2 49    02 3e   18      delay eq band3 frequency
 * b2 4a    00 1f   07      delay eq band3 q
 * b2 4b    04 7c   40      delay eq band4 gain
 * b2 4c    02 3e   1e      delay eq band4 frequency
 * b2 4d    00 1f   07      delay eq band4 q
 * b2 4e    00 01   00      delay eq band5 type
 * b2 4f    04 7c   40      delay eq band5 gain
 * b2 50    02 3e   24      delay eq band5 frequency
 * b2 51    00 1f   07      delay eq band5 q
 * b2 52    00 07   06      delay eq hpf type
 * b2 53    00 45   00      delay eq hpf frequency
 * b2 54    00 0a   06      delay eq lpf type
 * b2 55    00 6f   6f      delay eq lpf frequency
 *
 * b2 60    00 01   00      reverb eq switch
 * b2 61    00 01   00      reverb eq band1 type
 * b2 62    04 7c   40      reverb eq band1 gain
 * b2 63    02 2a   0c      reverb eq band1 frequency
 * b2 64    00 1f   07      reverb eq band1 q
 * b2 65    04 7c   40      reverb eq band2 gain
 * b2 66    02 3e   12      reverb eq band2 frequency
 * b2 67    00 1f   07      reverb eq band2 q
 * b2 68    04 7c   40      reverb eq band3 gain
 * b2 69    02 3e   18      reverb eq band3 frequency
 * b2 6a    00 1f   07      reverb eq band3 q
 * b2 6b    04 7c   40      reverb eq band4 gain
 * b2 6c    02 3e   1e      reverb eq band4 frequency
 * b2 6d    00 1f   07      reverb eq band4 q
 * b2 6e    00 01   00      reverb eq band5 type
 * b2 6f    04 7c   40      reverb eq band5 gain
 * b2 70    02 3e   24      reverb eq band5 frequency
 * b2 71    00 1f   07      reverb eq band5 q
 * b2 72    00 07   06      reverb eq hpf type
 * b2 73    00 45   00      reverb eq hpf frequency
 * b2 74    00 0a   06      reverb eq lpf type
 * b2 75    00 6f   6f      reverb eq lpf frequency
*/
typedef enum roland_echo_effects_cmd
{
    delay_input_level = 0x00,

    delay_tone_gain = 0x02,

    delay_time = 0x04,
    delay_feedback_level,
    delay_pan,
    delay_level,
    delay_tap1_level,
    delay_tap2_level,
    delay_tap3_level,
    delay_tap1_time,
    delay_tap2_time,
    delay_tap3_time,
    delay_tap1_pan,
    delay_tap2_pan,
    delay_tap3_pan,
    delay_master_level,
    delay_to_reverb_level,

    delay_pre_delay_time = 0x18,
    delay_phase,

    reverb_input_level = 0x20,

    reverb_type = 0x22,
    reverb_tone_gain,

    reverb_time = 0x25,
    reverb_pre_delay_time,
    reverb_output_level,

    reverb_phase = 0x2c,

    mic_dry_phase = 0x30,

    delay_eq_switch = 0x40,
    delay_eq_band1_type,
    delay_eq_band1_gain,
    delay_eq_band1_frequency,
    delay_eq_band1_q,
    delay_eq_band2_gain,
    delay_eq_band2_frequency,
    delay_eq_band2_q,
    delay_eq_band3_gain,
    delay_eq_band3_frequency,
    delay_eq_band3_q,
    delay_eq_band4_gain,
    delay_eq_band4_frequency,
    delay_eq_band4_q,
    delay_eq_band5_type,
    delay_eq_band5_gain,
    delay_eq_band5_frequency,
    delay_eq_band5_q,
    delay_eq_hpf_type,
    delay_eq_hpf_frequency,
    delay_eq_lpf_type,
    delay_eq_lpf_frequency,

    reverb_eq_switch = 0x60,
    reverb_eq_band1_type,
    reverb_eq_band1_gain,
    reverb_eq_band1_frequency,
    reverb_eq_band1_q,
    reverb_eq_band2_gain,
    reverb_eq_band2_frequency,
    reverb_eq_band2_q,
    reverb_eq_band3_gain,
    reverb_eq_band3_frequency,
    reverb_eq_band3_q,
    reverb_eq_band4_gain,
    reverb_eq_band4_frequency,
    reverb_eq_band4_q,
    reverb_eq_band5_type,
    reverb_eq_band5_gain,
    reverb_eq_band5_frequency,
    reverb_eq_band5_q,
    reverb_eq_hpf_type,
    reverb_eq_hpf_frequency,
    reverb_eq_lpf_type,
    reverb_eq_lpf_frequency,
    delay_end
} roland_echo_cmd_e;


/*
 * (4)Music Effects Block
 * command min max default parameter
 * b3 00    00 01   00      key control switch
 *
 * b3 02    34 4c   40      key control pitch
 * b3 03    0e 72   40      key control pitch fine
 *
 * b3 20    00 01   00      eq switch
 * b3 21    04 7c   40      eq band1 gain
 * b3 22    04 7c   40      eq band2 gain
 * b3 23    04 7c   40      eq band3 gain
 * b3 24    04 7c   40      eq band4 gain
 * b3 25    04 7c   40      eq band5 gain
 * b3 26    04 7c   40      eq band6 gain
 * b3 27    04 7c   40      eq band7 gain
 * b3 28    04 7c   40      eq band8 gain
 * b3 29    04 7c   40      eq band9 gain
 * b3 2a    04 7c   40      eq band10 gain
 * b3 2b    04 7c   40      eq band11 gain
 * b3 2c    04 7c   40      eq band12 gain
 * b3 2d    04 7c   40      eq band13 gain
 * b3 2e    04 7c   40      eq band14 gain
 * b3 2f    04 7c   40      eq band15 gain
 * b3 30    04 7c   40      eq band16 gain
 * b3 31    04 7c   40      eq band17 gain
 * b3 32    04 7c   40      eq band18 gain
 * b3 33    04 7c   40      eq band19 gain
 * b3 34    04 7c   40      eq band20 gain
 * b3 35    04 7c   40      eq band21 gain
 * b3 36    04 7c   40      eq band22 gain
 * b3 37    04 7c   40      eq band23 gain
 * b3 38    04 7c   40      eq band24 gain
 * b3 39    04 7c   40      eq band25 gain
 * b3 3a    04 7c   40      eq band26 gain
 * b3 3b    04 7c   40      eq band27 gain
 * b3 3c    04 7c   40      eq band28 gain
 * b3 3d    04 7c   40      eq band29 gain
 * b3 3e    04 7c   40      eq band30 gain
 * b3 3f    04 7c   40      eq band31 gain
 *
 * b3 40    00 01   00      music phase
*/
typedef enum roland_music_effects_cmd
{
    key_control_switch = 0x00,
    key_control_pitch = 0x02,
    key_control_pitch_fine,
    eq_switch = 0x20,
    eq_band1_gain,
    eq_band2_gain,
    eq_band3_gain,
    eq_band4_gain,
    eq_band5_gain,
    eq_band6_gain,
    eq_band7_gain,
    eq_band8_gain,
    eq_band9_gain,
    eq_band10_gain,
    eq_band11_gain,
    eq_band12_gain,
    eq_band13_gain,
    eq_band14_gain,
    eq_band15_gain,
    eq_band16_gain,
    eq_band17_gain,
    eq_band18_gain,
    eq_band19_gain,
    eq_band20_gain,
    eq_band21_gain,
    eq_band22_gain,
    eq_band23_gain,
    eq_band24_gain,
    eq_band25_gain,
    eq_band26_gain,
    eq_band27_gain,
    eq_band28_gain,
    eq_band29_gain,
    eq_band30_gain,
    eq_band31_gain,
    music_phase,
    music_effects_end
} roland_music_effects_cmd_e;

/*
 * (5)Output Block
 * command min max default parameter
 * b4 00    00 01   00      front eq switch
 * b4 01    00 01   00      front eq band1 type
 * b4 02    04 7c   40      front eq band1 gain
 * b4 03    02 2a   0c      front eq band1 frequency
 * b4 04    00 1f   07      front eq band1 q
 * b4 05    04 7c   40      front eq band2 gain
 * b4 06    02 3e   12      front eq band2 frequency
 * b4 07    00 1f   07      front eq band2 q
 * b4 08    04 7c   40      front eq band3 gain
 * b4 09    02 3e   18      front eq band3 frequency
 * b4 0a    00 1f   07      front eq band3 q
 * b4 0b    04 7c   40      front eq band4 gain
 * b4 0c    02 3e   1e      front eq band4 frequency
 * b4 0d    00 1f   07      front eq band4 q
 * b4 0e    00 01   00      front eq band5 type
 * b4 0f    04 7c   40      front eq band5 gain
 * b4 10    02 3e   24      front eq band5 frequency
 * b4 11    00 1f   07      front eq band5 q
 * b4 12    00 07   06      front eq hpf type
 * b4 13    00 45   00      front eq hpf frequency
 * b4 14    00 0a   06      front eq lpf type
 * b4 15    00 6f   6f      front eq lpf frequency
 * b4 16    00 64   00      front delay time
 * b4 17    00 01   00      front phase
 *
 * b4 20    00 01   00      rear eq switch
 * b4 21    00 01   00      rear eq band1 type
 * b4 22    04 7c   40      rear eq band1 gain
 * b4 23    02 2a   0c      rear eq band1 frequency
 * b4 24    00 1f   07      rear eq band1 q
 * b4 25    04 7c   40      rear eq band2 gain
 * b4 26    02 3e   12      rear eq band2 frequency
 * b4 27    00 1f   07      rear eq band2 q
 * b4 28    04 7c   40      rear eq band3 gain
 * b4 29    02 3e   18      rear eq band3 frequency
 * b4 2a    00 1f   07      rear eq band3 q
 * b4 2b    04 7c   40      rear eq band4 gain
 * b4 2c    02 3e   1e      rear eq band4 frequency
 * b4 2d    00 1f   07      rear eq band4 q
 * b4 2e    00 01   00      rear eq band5 type
 * b4 2f    04 7c   40      rear eq band5 gain
 * b4 30    02 3e   24      rear eq band5 frequency
 * b4 31    00 1f   07      rear eq band5 q
 * b4 32    00 07   06      rear eq hpf type
 * b4 33    00 45   00      rear eq hpf frequency
 * b4 34    00 0a   06      rear eq lpf type
 * b4 35    00 6f   6f      rear eq lpf frequency
 * b4 36    00 64   00      rear delay time
 * b4 37    00 01   00      rear phase
 *
 * b4 40    00 01   00      center eq switch
 * b4 41    00 01   00      center eq band1 type
 * b4 42    04 7c   40      center eq band1 gain
 * b4 43    02 2a   0c      center eq band1 frequency
 * b4 44    00 1f   07      center eq band1 q
 * b4 45    04 7c   40      center eq band2 gain
 * b4 46    02 3e   12      center eq band2 frequency
 * b4 47    00 1f   07      center eq band2 q
 * b4 48    04 7c   40      center eq band3 gain
 * b4 49    02 3e   18      center eq band3 frequency
 * b4 4a    00 1f   07      center eq band3 q
 * b4 4b    04 7c   40      center eq band4 gain
 * b4 4c    02 3e   1e      center eq band4 frequency
 * b4 4d    00 1f   07      center eq band4 q
 * b4 4e    00 01   00      center eq band5 type
 * b4 4f    04 7c   40      center eq band5 gain
 * b4 50    02 3e   24      center eq band5 frequency
 * b4 51    00 1f   07      center eq band5 q
 * b4 52    00 07   06      center eq hpf type
 * b4 53    00 45   00      center eq hpf frequency
 * b4 54    00 0a   06      center eq lpf type
 * b4 55    00 6f   6f      center eq lpf frequency
 * b4 56    00 64   00      center delay time
 * b4 57    00 01   00      center phase
 *
 * b4 60    00 01   01      subwoofer eq switch
 * b4 61    00 01   00      subwoofer eq band1 type
 * b4 62    04 7c   40      subwoofer eq band1 gain
 * b4 63    02 2a   06      subwoofer eq band1 frequency
 * b4 64    00 1f   13      subwoofer eq band1 q
 * b4 65    04 7c   40      subwoofer eq band2 gain
 * b4 66    02 3e   0a      subwoofer eq band2 frequency
 * b4 67    00 1f   13      subwoofer eq band2 q
 * b4 68    04 7c   40      subwoofer eq band3 gain
 * b4 69    02 3e   0d      subwoofer eq band3 frequency
 * b4 6a    00 1f   13      subwoofer eq band3 q
 * b4 6b    04 7c   40      subwoofer eq band4 gain
 * b4 6c    02 3e   10      subwoofer eq band4 frequency
 * b4 6d    00 1f   13      subwoofer eq band4 q
 * b4 6e    00 01   00      subwoofer eq band5 type
 * b4 6f    04 7c   40      subwoofer eq band5 gain
 * b4 70    02 3e   14      subwoofer eq band5 frequency
 * b4 71    00 1f   13      subwoofer eq band5 q
 * b4 72    00 07   06      subwoofer eq hpf type
 * b4 73    00 45   05      subwoofer eq hpf frequency
 * b4 74    00 0a   06      subwoofer eq lpf type
 * b4 75    00 6f   16      subwoofer eq lpf frequency
 * b4 76    00 64   00      subwoofer delay time
 * b4 77    00 01   00      subwoofer phase
 * b4 78    00 01   01      subwoofer compressor switch
 * b4 79    00 64   50      subwoofer compressor attack
 * b4 7a    00 64   05      subwoofer compressor release
 * b4 7b    00 64   46      subwoofer compressor threshold
 * b4 7c    00 0d   09      subwoofer compressor ratio
 * b4 7d    00 12   05      subwoofer compressor gain
*/
typedef enum roland_output_cmd
{
    front_eq_switch = 0x00,
    front_eq_band1_type,
    front_eq_band1_gain,
    front_eq_band1_frequency,
    front_eq_band1_q,
    front_eq_band2_gain,
    front_eq_band2_frequency,
    front_eq_band2_q,
    front_eq_band3_gain,
    front_eq_band3_frequency,
    front_eq_band3_q,
    front_eq_band4_gain,
    front_eq_band4_frequency,
    front_eq_band4_q,
    front_eq_band5_type,
    front_eq_band5_gain,
    front_eq_band5_frequency,
    front_eq_band5_q,
    front_eq_hpf_type,
    front_eq_hpf_frequency,
    front_eq_lpf_type,
    front_eq_lpf_frequency,
    front_delay_time,
    front_phase,

    rear_eq_switch = 0x20,
    rear_eq_band1_type,
    rear_eq_band1_gain,
    rear_eq_band1_frequency,
    rear_eq_band1_q,
    rear_eq_band2_gain,
    rear_eq_band2_frequency,
    rear_eq_band2_q,
    rear_eq_band3_gain,
    rear_eq_band3_frequency,
    rear_eq_band3_q,
    rear_eq_band4_gain,
    rear_eq_band4_frequency,
    rear_eq_band4_q,
    rear_eq_band5_type,
    rear_eq_band5_gain,
    rear_eq_band5_frequency,
    rear_eq_band5_q,
    rear_eq_hpf_type,
    rear_eq_hpf_frequency,
    rear_eq_lpf_type,
    rear_eq_lpf_frequency,
    rear_delay_time,
    rear_phase,

    center_eq_switch = 0x40,
    center_eq_band1_type,
    center_eq_band1_gain,
    center_eq_band1_frequency,
    center_eq_band1_q,
    center_eq_band2_gain,
    center_eq_band2_frequency,
    center_eq_band2_q,
    center_eq_band3_gain,
    center_eq_band3_frequency,
    center_eq_band3_q,
    center_eq_band4_gain,
    center_eq_band4_frequency,
    center_eq_band4_q,
    center_eq_band5_type,
    center_eq_band5_gain,
    center_eq_band5_frequency,
    center_eq_band5_q,
    center_eq_hpf_type,
    center_eq_hpf_frequency,
    center_eq_lpf_type,
    center_eq_lpf_frequency,
    center_delay_time,
    center_phase,

    subwoofer_eq_switch = 0x60,
    subwoofer_eq_band1_type,
    subwoofer_eq_band1_gain,
    subwoofer_eq_band1_frequency,
    subwoofer_eq_band1_q,
    subwoofer_eq_band2_gain,
    subwoofer_eq_band2_frequency,
    subwoofer_eq_band2_q,
    subwoofer_eq_band3_gain,
    subwoofer_eq_band3_frequency,
    subwoofer_eq_band3_q,
    subwoofer_eq_band4_gain,
    subwoofer_eq_band4_frequency,
    subwoofer_eq_band4_q,
    subwoofer_eq_band5_type,
    subwoofer_eq_band5_gain,
    subwoofer_eq_band5_frequency,
    subwoofer_eq_band5_q,
    subwoofer_eq_hpf_type,
    subwoofer_eq_hpf_frequency,
    subwoofer_eq_lpf_type,
    subwoofer_eq_lpf_frequency,
    subwoofer_delay_time,
    subwoofer_phase,
    subwoofer_compressor_switch,
    subwoofer_compressor_attack,
    subwoofer_compressor_release,
    subwoofer_compressor_threshold,
    subwoofer_compressor_ratio,
    subwoofer_compressor_gain,
    output_end
} roland_output_cmd_e;


/*
 * (6)System Block
 * command min max default parameter
 * BF 00    00 00   00      Version Request
 * >> Return “BF mm nn”. (mm is the Major Version. nn is the Minor Version)
 *
 * BF 02    00 00   00      System Reset
 * BF 03    00 7F   00      Parameter Request
 * BF 04    00 00   00      Build No Request
 * >> Return “BF 44 bb”. (bb is the Build No.)
 *
 * BF 08    01 02
 * --Algorithm Type(01:MIC 7band PEQ, 02:MIC 15band GEQ)
 * Please send the “Algorithm Type” at first.
 *
 * BF 10    00 06   00      Parameter Save
 * BF 11    00 06   00      Parameter Load
*/
typedef enum roland_system_cmd
{
    version_request = 0x00,

    system_reset = 0x02,
    parameter_request,
    build_no_request,

    parameter_save = 0x10,
    parameter_load,
    system_end
} roland_system_cmd_e;


/***********************************************************************
*                                全局宏定义
***********************************************************************/

/***********************************************************************
*                                全局数据结构定义
***********************************************************************/
typedef struct dsp_setting_item
{
    const char *serial_cmd_name;  //寄存器的名字
    uint8_t cmd_default_value;    //寄存器默认值

    API_INFO_T * effect_api;      //会受到哪个wsd的API的改动
    int effective_arg_idx;        //在effect api的有效参数中的编号

    const char *profile_xml_name; //在profile xml中保存的属性名
    const char *profile_xml_node; //在profile xml中保存在哪个node下
    int profile_value_offset;     //ktv app所使用的值与寄存器值之间的差值
    const double * translate_table;
}DSP_SETTING_T;

extern DSP_SETTING_T mixer_settings[mixer_end];
extern DSP_SETTING_T mic_effects_settings[mic_effects_end];
extern DSP_SETTING_T delay_settings[delay_end];
extern DSP_SETTING_T music_effects_settings[music_effects_end];
extern DSP_SETTING_T output_settings[output_end];

extern const double gHighPass_Freq_Table[70];
extern const double gLowPass_Freq_Table[112];
extern const double gPEQ_Freq_Table[61];
extern const double gPEQ_Gain_Table[121];
extern const double gPEQ_Q_Table[32];
/***********************************************************************
*                                导出函数
***********************************************************************/
void roland_cmd_settings_table_init();
int roland_cmd_translate(char *arg_list[], int arg_cnt);
#endif
