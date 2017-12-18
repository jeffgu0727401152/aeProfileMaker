/***********************************************************************
*                                包含头文件
************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "debug_config.h"
#include "roland_cmd.h"
#include "wsd_api.h"
#include "xml_profile.h"

DSP_SETTING_T mixer_settings[mixer_end];
DSP_SETTING_T mic_effects_settings[mic_effects_end];
DSP_SETTING_T delay_settings[delay_end];
DSP_SETTING_T music_effects_settings[music_effects_end];
DSP_SETTING_T output_settings[output_end];

static void mixer_settings_default()
{
    memset(mixer_settings,0,sizeof(DSP_SETTING_T)*mixer_end);

    mixer_settings[mic1_input_level].serial_cmd_name = "mic1_input_level";
    mixer_settings[mic1_input_level].effect_api = &(api_table[WSD_AL_AE_MixerMicLRLevelSetup]);
    mixer_settings[mic1_input_level].cmd_default_value = 0x40;
    mixer_settings[mic1_input_level].effective_arg_idx = 0;

    mixer_settings[mic2_input_level].serial_cmd_name = "mic2_input_level";
    mixer_settings[mic2_input_level].effect_api = &(api_table[WSD_AL_AE_MixerMicLRLevelSetup]);
    mixer_settings[mic2_input_level].cmd_default_value = 0x40;
    mixer_settings[mic2_input_level].effective_arg_idx = 1;

    mixer_settings[mic_input_level].serial_cmd_name = "mic_input_level";
    mixer_settings[mic_input_level].effect_api = &(api_table[WSD_AL_AE_MixerMicInputLevelSetup]);
    mixer_settings[mic_input_level].cmd_default_value = 0x40;
    mixer_settings[mic_input_level].effective_arg_idx = 0;

    mixer_settings[mic_direct_level].serial_cmd_name = "mic_direct_level";
    mixer_settings[mic_direct_level].effect_api = &(api_table[WSD_AL_AE_DryOutputSetup]);
    mixer_settings[mic_direct_level].cmd_default_value = 0x7f;
    mixer_settings[mic_direct_level].effective_arg_idx = 0;

    mixer_settings[front_mic_dry_level].serial_cmd_name = "front_mic_dry_level";
    mixer_settings[front_mic_dry_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_front]);
    mixer_settings[front_mic_dry_level].cmd_default_value = 0x7f;
    mixer_settings[front_mic_dry_level].effective_arg_idx = 0;

    mixer_settings[music_input_level].serial_cmd_name = "music_input_level";
    mixer_settings[music_input_level].effect_api = &(api_table[WSD_AL_AE_MixerMusicInputLevelSetup]);
    mixer_settings[music_input_level].cmd_default_value = 0x40;
    mixer_settings[music_input_level].effective_arg_idx = 0;

    mixer_settings[front_music_level].serial_cmd_name = "front_music_level";
    mixer_settings[front_music_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_front]);
    mixer_settings[front_music_level].cmd_default_value = 0x7f;
    mixer_settings[front_music_level].effective_arg_idx = 3;

    mixer_settings[rear_output_level].serial_cmd_name = "rear_output_level";
    mixer_settings[rear_output_level].effect_api = &(api_table[WSD_AL_AE_OutputSetup_rear]);
    mixer_settings[rear_output_level].cmd_default_value = 0x7f;
    mixer_settings[rear_output_level].effective_arg_idx = 0;

    mixer_settings[front_output_level].serial_cmd_name = "front_output_level";
    mixer_settings[front_output_level].effect_api = &(api_table[WSD_AL_AE_OutputSetup_front]);
    mixer_settings[front_output_level].cmd_default_value = 0x7f;
    mixer_settings[front_output_level].effective_arg_idx = 0;

    mixer_settings[subwoofer_output_level].serial_cmd_name = "subwoofer_output_level";
    mixer_settings[subwoofer_output_level].effect_api = &(api_table[WSD_AL_AE_OutputSetup_subwoofer]);
    mixer_settings[subwoofer_output_level].cmd_default_value = 0x7f;
    mixer_settings[subwoofer_output_level].effective_arg_idx = 0;

    mixer_settings[aux_to_mic].serial_cmd_name = "aux_to_mic";
    mixer_settings[aux_to_mic].effect_api = &(api_table[WSD_AL_AE_MixerAuxLevelSetup]);
    mixer_settings[aux_to_mic].cmd_default_value = 0x00;
    mixer_settings[aux_to_mic].effective_arg_idx = 0;

    mixer_settings[aux_to_music].serial_cmd_name = "aux_to_music";
    mixer_settings[aux_to_music].effect_api = &(api_table[WSD_AL_AE_MixerAuxLevelSetup]);
    mixer_settings[aux_to_music].cmd_default_value = 0x00;
    mixer_settings[aux_to_music].effective_arg_idx = 1;

    mixer_settings[center_output_level].serial_cmd_name = "center_output_level";
    mixer_settings[center_output_level].effect_api = &(api_table[WSD_AL_AE_OutputSetup_center]);
    mixer_settings[center_output_level].cmd_default_value = 0x7f;
    mixer_settings[center_output_level].effective_arg_idx = 0;

    mixer_settings[center_mic_level].serial_cmd_name = "center_mic_level";
    mixer_settings[center_mic_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_center]);
    mixer_settings[center_mic_level].cmd_default_value = 0x7f;
    mixer_settings[center_mic_level].effective_arg_idx = 0;

    mixer_settings[center_music_level].serial_cmd_name = "center_music_level";
    mixer_settings[center_music_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_center]);
    mixer_settings[center_music_level].cmd_default_value = 0x00;
    mixer_settings[center_music_level].effective_arg_idx = 3;

    mixer_settings[front_mic_delay_level].serial_cmd_name = "front_mic_delay_level";
    mixer_settings[front_mic_delay_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_front]);
    mixer_settings[front_mic_delay_level].cmd_default_value = 0x7f;
    mixer_settings[front_mic_delay_level].effective_arg_idx = 1;

    mixer_settings[front_mic_reverb_level].serial_cmd_name = "front_mic_reverb_level";
    mixer_settings[front_mic_reverb_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_front]);
    mixer_settings[front_mic_reverb_level].cmd_default_value = 0x7f;
    mixer_settings[front_mic_reverb_level].effective_arg_idx = 2;

    mixer_settings[subwoofer_mic_dry_level].serial_cmd_name = "subwoofer_mic_dry_level";
    mixer_settings[subwoofer_mic_dry_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_subwoofer]);
    mixer_settings[subwoofer_mic_dry_level].cmd_default_value = 0x7f;
    mixer_settings[subwoofer_mic_dry_level].effective_arg_idx = 0;

    mixer_settings[subwoofer_music_level].serial_cmd_name = "subwoofer_music_level";
    mixer_settings[subwoofer_music_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_subwoofer]);
    mixer_settings[subwoofer_music_level].cmd_default_value = 0x7f;
    mixer_settings[subwoofer_music_level].effective_arg_idx = 3;

    mixer_settings[subwoofer_mic_delay_level].serial_cmd_name = "subwoofer_mic_delay_level";
    mixer_settings[subwoofer_mic_delay_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_subwoofer]);
    mixer_settings[subwoofer_mic_delay_level].cmd_default_value = 0x7f;
    mixer_settings[subwoofer_mic_delay_level].effective_arg_idx = 1;

    mixer_settings[subwoofer_mic_reverb_level].serial_cmd_name = "subwoofer_mic_reverb_level";
    mixer_settings[subwoofer_mic_reverb_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_subwoofer]);
    mixer_settings[subwoofer_mic_reverb_level].cmd_default_value = 0x7f;
    mixer_settings[subwoofer_mic_reverb_level].effective_arg_idx = 2;

    mixer_settings[rear_mic_dry_level].serial_cmd_name = "rear_mic_dry_level";
    mixer_settings[rear_mic_dry_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_rear]);
    mixer_settings[rear_mic_dry_level].cmd_default_value = 0x7f;
    mixer_settings[rear_mic_dry_level].effective_arg_idx = 0;

    mixer_settings[rear_music_level].serial_cmd_name = "rear_music_level";
    mixer_settings[rear_music_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_rear]);
    mixer_settings[rear_music_level].cmd_default_value = 0x7f;
    mixer_settings[rear_music_level].effective_arg_idx = 3;

    mixer_settings[rear_mic_delay_level].serial_cmd_name = "rear_mic_delay_level";
    mixer_settings[rear_mic_delay_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_rear]);
    mixer_settings[rear_mic_delay_level].cmd_default_value = 0x7f;
    mixer_settings[rear_mic_delay_level].effective_arg_idx = 1;

    mixer_settings[rear_mic_reverb_level].serial_cmd_name = "rear_mic_reverb_level";
    mixer_settings[rear_mic_reverb_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_rear]);
    mixer_settings[rear_mic_reverb_level].cmd_default_value = 0x7f;
    mixer_settings[rear_mic_reverb_level].effective_arg_idx = 2;

    mixer_settings[center_mic_delay_level].serial_cmd_name = "center_mic_delay_level";
    mixer_settings[center_mic_delay_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_center]);
    mixer_settings[center_mic_delay_level].cmd_default_value = 0x7f;
    mixer_settings[center_mic_delay_level].effective_arg_idx = 1;

    mixer_settings[center_mic_reverb_level].serial_cmd_name = "center_mic_reverb_level";
    mixer_settings[center_mic_reverb_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_center]);
    mixer_settings[center_mic_reverb_level].cmd_default_value = 0x7f;
    mixer_settings[center_mic_reverb_level].effective_arg_idx = 2;

    mixer_settings[center_subwoofer_switch_stereo].serial_cmd_name = "center_subwoofer_switch_stereo";
    mixer_settings[center_subwoofer_switch_stereo].effect_api = &(api_table[WSD_AL_AE_MixerCenterSubwooferSwitchStereo]);
    mixer_settings[center_subwoofer_switch_stereo].cmd_default_value = 0x0;
    mixer_settings[center_subwoofer_switch_stereo].effective_arg_idx = 0;
}

static void mic_effects_settings_default()
{
    memset(mic_effects_settings,0,sizeof(DSP_SETTING_T)*mic_effects_end);

    mic_effects_settings[bass_cut_switch].serial_cmd_name = "bass_cut_switch";
    mic_effects_settings[bass_cut_switch].effect_api = &(api_table[WSD_AL_AE_MicBasscutEnable]);
    mic_effects_settings[bass_cut_switch].cmd_default_value = 0x00;
    mic_effects_settings[bass_cut_switch].effective_arg_idx = 0;

    mic_effects_settings[bass_cut_frequency].serial_cmd_name = "bass_cut_frequency";
    mic_effects_settings[bass_cut_frequency].effect_api = &(api_table[WSD_AL_AE_MicBasscutFrequencySetup]);
    mic_effects_settings[bass_cut_frequency].cmd_default_value = 0x01;
    mic_effects_settings[bass_cut_frequency].effective_arg_idx = 0;

    mic_effects_settings[noise_supp_switch].serial_cmd_name = "noise_supp_switch";
    mic_effects_settings[noise_supp_switch].effect_api = &(api_table[WSD_AL_AE_MicNoiseSuppEnable]);
    mic_effects_settings[noise_supp_switch].cmd_default_value = 0x00;
    mic_effects_settings[noise_supp_switch].effective_arg_idx = 0;

    mic_effects_settings[noise_supp_threshold].serial_cmd_name = "noise_supp_threshold";
    mic_effects_settings[noise_supp_threshold].effect_api = &(api_table[WSD_AL_AE_MicNoiseThresholdSetup]);
    mic_effects_settings[noise_supp_threshold].cmd_default_value = 0x03;
    mic_effects_settings[noise_supp_threshold].effective_arg_idx = 0;

    mic_effects_settings[compressor_switch].serial_cmd_name = "compressor_switch";
    mic_effects_settings[compressor_switch].effect_api = &(api_table[WSD_AL_AE_MicCompressorEnable]);
    mic_effects_settings[compressor_switch].cmd_default_value = 0x00;
    mic_effects_settings[compressor_switch].effective_arg_idx = 0;

    mic_effects_settings[compressor_sustain].serial_cmd_name = "compressor_sustain";
    mic_effects_settings[compressor_sustain].effect_api = &(api_table[WSD_AL_AE_MicCompressorSetup]);
    mic_effects_settings[compressor_sustain].cmd_default_value = 0x07;
    mic_effects_settings[compressor_sustain].effective_arg_idx = 0;

    mic_effects_settings[compressor_out_level].serial_cmd_name = "compressor_out_level";
    mic_effects_settings[compressor_out_level].effect_api = &(api_table[WSD_AL_AE_MicCompressorSetup]);
    mic_effects_settings[compressor_out_level].cmd_default_value = 0x20;
    mic_effects_settings[compressor_out_level].effective_arg_idx = 1;

    mic_effects_settings[frequency_shift_value].serial_cmd_name = "frequency_shift_value";
    mic_effects_settings[frequency_shift_value].effect_api = &(api_table[WSD_AL_AE_MicFrequencyShiftSetup]);
    mic_effects_settings[frequency_shift_value].cmd_default_value = 0x40;
    mic_effects_settings[frequency_shift_value].effective_arg_idx = 0;

    mic_effects_settings[peq_switch].serial_cmd_name = "peq_switch";
    mic_effects_settings[peq_switch].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqEnable]);
    mic_effects_settings[peq_switch].cmd_default_value = 0x00;
    mic_effects_settings[peq_switch].effective_arg_idx = 0;

    mic_effects_settings[peq_band1_type].serial_cmd_name = "peq_band1_type";
    mic_effects_settings[peq_band1_type].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqEdgeSetup]);
    mic_effects_settings[peq_band1_type].cmd_default_value = 0x00;
    mic_effects_settings[peq_band1_type].effective_arg_idx = 0;

    mic_effects_settings[peq_band1_gain].serial_cmd_name = "peq_band1_gain";
    mic_effects_settings[peq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band1]);
    mic_effects_settings[peq_band1_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band1_gain].effective_arg_idx = 2;

    mic_effects_settings[peq_band1_frequency].serial_cmd_name = "peq_band1_frequency";
    mic_effects_settings[peq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band1]);
    mic_effects_settings[peq_band1_frequency].cmd_default_value = 0x0c;
    mic_effects_settings[peq_band1_frequency].effective_arg_idx = 0;

    mic_effects_settings[peq_band1_q].serial_cmd_name = "peq_band1_q";
    mic_effects_settings[peq_band1_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band1]);
    mic_effects_settings[peq_band1_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band1_q].effective_arg_idx = 1;

    mic_effects_settings[peq_band2_gain].serial_cmd_name = "peq_band2_gain";
    mic_effects_settings[peq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band2]);
    mic_effects_settings[peq_band2_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band2_gain].effective_arg_idx = 2;

    mic_effects_settings[peq_band2_frequency].serial_cmd_name = "peq_band2_frequency";
    mic_effects_settings[peq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band2]);
    mic_effects_settings[peq_band2_frequency].cmd_default_value = 0x12;
    mic_effects_settings[peq_band2_frequency].effective_arg_idx = 0;

    mic_effects_settings[peq_band2_q].serial_cmd_name = "peq_band2_q";
    mic_effects_settings[peq_band2_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band2]);
    mic_effects_settings[peq_band2_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band2_q].effective_arg_idx = 1;

    mic_effects_settings[peq_band3_gain].serial_cmd_name = "peq_band3_gain";
    mic_effects_settings[peq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band3]);
    mic_effects_settings[peq_band3_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band3_gain].effective_arg_idx = 2;

    mic_effects_settings[peq_band3_frequency].serial_cmd_name = "peq_band3_frequency";
    mic_effects_settings[peq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band3]);
    mic_effects_settings[peq_band3_frequency].cmd_default_value = 0x18;
    mic_effects_settings[peq_band3_frequency].effective_arg_idx = 0;

    mic_effects_settings[peq_band3_q].serial_cmd_name = "peq_band3_q";
    mic_effects_settings[peq_band3_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band3]);
    mic_effects_settings[peq_band3_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band3_q].effective_arg_idx = 1;

    mic_effects_settings[peq_band4_gain].serial_cmd_name = "peq_band4_gain";
    mic_effects_settings[peq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band4]);
    mic_effects_settings[peq_band4_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band4_gain].effective_arg_idx = 2;

    mic_effects_settings[peq_band4_frequency].serial_cmd_name = "peq_band4_frequency";
    mic_effects_settings[peq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band4]);
    mic_effects_settings[peq_band4_frequency].cmd_default_value = 0x1e;
    mic_effects_settings[peq_band4_frequency].effective_arg_idx = 0;

    mic_effects_settings[peq_band4_q].serial_cmd_name = "peq_band4_q";
    mic_effects_settings[peq_band4_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band4]);
    mic_effects_settings[peq_band4_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band4_q].effective_arg_idx = 1;

    mic_effects_settings[peq_band5_gain].serial_cmd_name = "peq_band5_gain";
    mic_effects_settings[peq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band5]);
    mic_effects_settings[peq_band5_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band5_gain].effective_arg_idx = 2;

    mic_effects_settings[peq_band5_frequency].serial_cmd_name = "peq_band5_frequency";
    mic_effects_settings[peq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band5]);
    mic_effects_settings[peq_band5_frequency].cmd_default_value = 0x24;
    mic_effects_settings[peq_band5_frequency].effective_arg_idx = 0;

    mic_effects_settings[peq_band5_q].serial_cmd_name = "peq_band5_q";
    mic_effects_settings[peq_band5_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band5]);
    mic_effects_settings[peq_band5_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band5_q].effective_arg_idx = 1;

    mic_effects_settings[peq_band6_gain].serial_cmd_name = "peq_band6_gain";
    mic_effects_settings[peq_band6_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band6]);
    mic_effects_settings[peq_band6_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band6_gain].effective_arg_idx = 2;

    mic_effects_settings[peq_band6_frequency].serial_cmd_name = "peq_band6_frequency";
    mic_effects_settings[peq_band6_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band6]);
    mic_effects_settings[peq_band6_frequency].cmd_default_value = 0x2a;
    mic_effects_settings[peq_band6_frequency].effective_arg_idx = 0;

    mic_effects_settings[peq_band6_q].serial_cmd_name = "peq_band6_q";
    mic_effects_settings[peq_band6_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band6]);
    mic_effects_settings[peq_band6_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band6_q].effective_arg_idx = 1;

    mic_effects_settings[peq_band7_type].serial_cmd_name = "peq_band7_type";
    mic_effects_settings[peq_band7_type].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqEdgeSetup]);
    mic_effects_settings[peq_band7_type].cmd_default_value = 0x00;
    mic_effects_settings[peq_band7_type].effective_arg_idx = 1;

    mic_effects_settings[peq_band7_gain].serial_cmd_name = "peq_band7_gain";
    mic_effects_settings[peq_band7_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band7]);
    mic_effects_settings[peq_band7_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band7_gain].effective_arg_idx = 2;

    mic_effects_settings[peq_band7_frequency].serial_cmd_name = "peq_band7_frequency";
    mic_effects_settings[peq_band7_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band7]);
    mic_effects_settings[peq_band7_frequency].cmd_default_value = 0x30;
    mic_effects_settings[peq_band7_frequency].effective_arg_idx = 0;

    mic_effects_settings[peq_band7_q].serial_cmd_name = "peq_band7_q";
    mic_effects_settings[peq_band7_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band7]);
    mic_effects_settings[peq_band7_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band7_q].effective_arg_idx = 1;
}

static void delay_settings_default()
{
    memset(delay_settings,0,sizeof(DSP_SETTING_T)*delay_end);

    delay_settings[delay_input_level].serial_cmd_name = "delay_input_level";
    delay_settings[delay_input_level].effect_api = &(api_table[WSD_AL_AE_DelayInputLevelSetup]);
    delay_settings[delay_input_level].cmd_default_value = 0x00;
    delay_settings[delay_input_level].effective_arg_idx = 0;

    delay_settings[delay_tone_gain].serial_cmd_name = "delay_tone_gain";
    delay_settings[delay_tone_gain].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_tone_gain].cmd_default_value = 0x40;
    delay_settings[delay_tone_gain].effective_arg_idx = 3;

    delay_settings[delay_time].serial_cmd_name = "delay_time";
    delay_settings[delay_time].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_time].cmd_default_value = 0x14;
    delay_settings[delay_time].effective_arg_idx = 0;

    delay_settings[delay_feedback_level].serial_cmd_name = "delay_feedback_level";
    delay_settings[delay_feedback_level].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_feedback_level].cmd_default_value = 0x32;
    delay_settings[delay_feedback_level].effective_arg_idx = 4;

    delay_settings[delay_pan].serial_cmd_name = "delay_pan";
    delay_settings[delay_pan].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_pan].cmd_default_value = 0x40;
    delay_settings[delay_pan].effective_arg_idx = 2;

    delay_settings[delay_level].serial_cmd_name = "delay_level";
    delay_settings[delay_level].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_level].cmd_default_value = 0x7f;
    delay_settings[delay_level].effective_arg_idx = 1;

    delay_settings[delay_tap1_level].serial_cmd_name = "delay_tap1_level";
    delay_settings[delay_tap1_level].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap1]);
    delay_settings[delay_tap1_level].cmd_default_value = 0x0;
    delay_settings[delay_tap1_level].effective_arg_idx = 1;

    delay_settings[delay_tap2_level].serial_cmd_name = "delay_tap2_level";
    delay_settings[delay_tap2_level].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap2]);
    delay_settings[delay_tap2_level].cmd_default_value = 0x0;
    delay_settings[delay_tap2_level].effective_arg_idx = 1;

    delay_settings[delay_tap3_level].serial_cmd_name = "delay_tap3_level";
    delay_settings[delay_tap3_level].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap3]);
    delay_settings[delay_tap3_level].cmd_default_value = 0x0;
    delay_settings[delay_tap3_level].effective_arg_idx = 1;

    delay_settings[delay_tap1_time].serial_cmd_name = "delay_tap1_time";
    delay_settings[delay_tap1_time].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap1]);
    delay_settings[delay_tap1_time].cmd_default_value = 0x3f;
    delay_settings[delay_tap1_time].effective_arg_idx = 0;

    delay_settings[delay_tap2_time].serial_cmd_name = "delay_tap2_time";
    delay_settings[delay_tap2_time].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap2]);
    delay_settings[delay_tap2_time].cmd_default_value = 0x3f;
    delay_settings[delay_tap2_time].effective_arg_idx = 0;

    delay_settings[delay_tap3_time].serial_cmd_name = "delay_tap3_time";
    delay_settings[delay_tap3_time].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap3]);
    delay_settings[delay_tap3_time].cmd_default_value = 0x3f;
    delay_settings[delay_tap3_time].effective_arg_idx = 0;

    delay_settings[delay_tap1_pan].serial_cmd_name = "delay_tap1_pan";
    delay_settings[delay_tap1_pan].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap1]);
    delay_settings[delay_tap1_pan].cmd_default_value = 0x40;
    delay_settings[delay_tap1_pan].effective_arg_idx = 2;

    delay_settings[delay_tap2_pan].serial_cmd_name = "delay_tap2_pan";
    delay_settings[delay_tap2_pan].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap2]);
    delay_settings[delay_tap2_pan].cmd_default_value = 0x40;
    delay_settings[delay_tap2_pan].effective_arg_idx = 2;

    delay_settings[delay_tap3_pan].serial_cmd_name = "delay_tap3_pan";
    delay_settings[delay_tap3_pan].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap3]);
    delay_settings[delay_tap3_pan].cmd_default_value = 0x40;
    delay_settings[delay_tap3_pan].effective_arg_idx = 2;

    delay_settings[delay_master_level].serial_cmd_name = "delay_master_level";
    delay_settings[delay_master_level].effect_api = &(api_table[WSD_AL_AE_DelayOutputSetup]);
    delay_settings[delay_master_level].cmd_default_value = 0x40;
    delay_settings[delay_master_level].effective_arg_idx = 0;

    delay_settings[delay_to_reverb_level].serial_cmd_name = "delay_to_reverb_level";
    delay_settings[delay_to_reverb_level].effect_api = &(api_table[WSD_AL_AE_DelayToReverbLevelSetup]);
    delay_settings[delay_to_reverb_level].cmd_default_value = 0x00;
    delay_settings[delay_to_reverb_level].effective_arg_idx = 0;

    delay_settings[delay_pre_delay_time].serial_cmd_name = "delay_pre_delay_time";
    delay_settings[delay_pre_delay_time].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_pre_delay_time].cmd_default_value = 0x00;
    delay_settings[delay_pre_delay_time].effective_arg_idx = 5;

    delay_settings[delay_phase].serial_cmd_name = "delay_phase";
    delay_settings[delay_phase].effect_api = &(api_table[WSD_AL_AE_DelayOutputSetup]);
    delay_settings[delay_phase].cmd_default_value = 0x00;
    delay_settings[delay_phase].effective_arg_idx = 1;

    delay_settings[reverb_input_level].serial_cmd_name = "reverb_input_level";
    delay_settings[reverb_input_level].effect_api = &(api_table[WSD_AL_AE_ReverbInputLevelSetup]);
    delay_settings[reverb_input_level].cmd_default_value = 0x00;
    delay_settings[reverb_input_level].effective_arg_idx = 0;

    delay_settings[reverb_type].serial_cmd_name = "reverb_type";
    delay_settings[reverb_type].effect_api = &(api_table[WSD_AL_AE_ReverbEffectsSetup]);
    delay_settings[reverb_type].cmd_default_value = 0x02;
    delay_settings[reverb_type].effective_arg_idx = 0;

    delay_settings[reverb_tone_gain].serial_cmd_name = "reverb_tone_gain";
    delay_settings[reverb_tone_gain].effect_api = &(api_table[WSD_AL_AE_ReverbEffectsSetup]);
    delay_settings[reverb_tone_gain].cmd_default_value = 0x40;
    delay_settings[reverb_tone_gain].effective_arg_idx = 2;

    delay_settings[reverb_time].serial_cmd_name = "reverb_time";
    delay_settings[reverb_time].effect_api = &(api_table[WSD_AL_AE_ReverbEffectsSetup]);
    delay_settings[reverb_time].cmd_default_value = 0x0a;
    delay_settings[reverb_time].effective_arg_idx = 1;

    delay_settings[reverb_pre_delay_time].serial_cmd_name = "reverb_pre_delay_time";
    delay_settings[reverb_pre_delay_time].effect_api = &(api_table[WSD_AL_AE_ReverbEffectsSetup]);
    delay_settings[reverb_pre_delay_time].cmd_default_value = 0x00;
    delay_settings[reverb_pre_delay_time].effective_arg_idx = 3;

    delay_settings[reverb_output_level].serial_cmd_name = "reverb_output_level";
    delay_settings[reverb_output_level].effect_api = &(api_table[WSD_AL_AE_ReverbOutputSetup]);
    delay_settings[reverb_output_level].cmd_default_value = 0x40;
    delay_settings[reverb_output_level].effective_arg_idx = 0;

    delay_settings[reverb_phase].serial_cmd_name = "reverb_phase";
    delay_settings[reverb_phase].effect_api = &(api_table[WSD_AL_AE_ReverbOutputSetup]);
    delay_settings[reverb_phase].cmd_default_value = 0x00;
    delay_settings[reverb_phase].effective_arg_idx = 1;

    delay_settings[mic_dry_phase].serial_cmd_name = "mic_dry_phase";
    delay_settings[mic_dry_phase].effect_api = &(api_table[WSD_AL_AE_DryOutputSetup]);
    delay_settings[mic_dry_phase].cmd_default_value = 0x00;
    delay_settings[mic_dry_phase].effective_arg_idx = 1;

    delay_settings[delay_eq_switch].serial_cmd_name = "delay_eq_switch";
    delay_settings[delay_eq_switch].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqEnable]);
    delay_settings[delay_eq_switch].cmd_default_value = 0x00;
    delay_settings[delay_eq_switch].effective_arg_idx = 0;

    delay_settings[delay_eq_band1_type].serial_cmd_name = "delay_eq_band1_type";
    delay_settings[delay_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqEdgeSetup]);
    delay_settings[delay_eq_band1_type].cmd_default_value = 0x00;
    delay_settings[delay_eq_band1_type].effective_arg_idx = 0;

    delay_settings[delay_eq_band1_gain].serial_cmd_name = "delay_eq_band1_gain";
    delay_settings[delay_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band1]);
    delay_settings[delay_eq_band1_gain].cmd_default_value = 0x40;
    delay_settings[delay_eq_band1_gain].effective_arg_idx = 2;

    delay_settings[delay_eq_band1_frequency].serial_cmd_name = "delay_eq_band1_frequency";
    delay_settings[delay_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band1]);
    delay_settings[delay_eq_band1_frequency].cmd_default_value = 0x0c;
    delay_settings[delay_eq_band1_frequency].effective_arg_idx = 0;

    delay_settings[delay_eq_band1_q].serial_cmd_name = "delay_eq_band1_q";
    delay_settings[delay_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band1]);
    delay_settings[delay_eq_band1_q].cmd_default_value = 0x07;
    delay_settings[delay_eq_band1_q].effective_arg_idx = 1;

    delay_settings[delay_eq_band2_gain].serial_cmd_name = "delay_eq_band2_gain";
    delay_settings[delay_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band2]);
    delay_settings[delay_eq_band2_gain].cmd_default_value = 0x40;
    delay_settings[delay_eq_band2_gain].effective_arg_idx = 2;

    delay_settings[delay_eq_band2_frequency].serial_cmd_name = "delay_eq_band2_frequency";
    delay_settings[delay_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band2]);
    delay_settings[delay_eq_band2_frequency].cmd_default_value = 0x12;
    delay_settings[delay_eq_band2_frequency].effective_arg_idx = 0;

    delay_settings[delay_eq_band2_q].serial_cmd_name = "delay_eq_band2_q";
    delay_settings[delay_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band2]);
    delay_settings[delay_eq_band2_q].cmd_default_value = 0x07;
    delay_settings[delay_eq_band2_q].effective_arg_idx = 1;

    delay_settings[delay_eq_band3_gain].serial_cmd_name = "delay_eq_band3_gain";
    delay_settings[delay_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band3]);
    delay_settings[delay_eq_band3_gain].cmd_default_value = 0x40;
    delay_settings[delay_eq_band3_gain].effective_arg_idx = 2;

    delay_settings[delay_eq_band3_frequency].serial_cmd_name = "delay_eq_band3_frequency";
    delay_settings[delay_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band3]);
    delay_settings[delay_eq_band3_frequency].cmd_default_value = 0x18;
    delay_settings[delay_eq_band3_frequency].effective_arg_idx = 0;

    delay_settings[delay_eq_band3_q].serial_cmd_name = "delay_eq_band3_q";
    delay_settings[delay_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band3]);
    delay_settings[delay_eq_band3_q].cmd_default_value = 0x07;
    delay_settings[delay_eq_band3_q].effective_arg_idx = 1;

    delay_settings[delay_eq_band4_gain].serial_cmd_name = "delay_eq_band4_gain";
    delay_settings[delay_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band4]);
    delay_settings[delay_eq_band4_gain].cmd_default_value = 0x40;
    delay_settings[delay_eq_band4_gain].effective_arg_idx = 2;

    delay_settings[delay_eq_band4_frequency].serial_cmd_name = "delay_eq_band4_frequency";
    delay_settings[delay_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band4]);
    delay_settings[delay_eq_band4_frequency].cmd_default_value = 0x1e;
    delay_settings[delay_eq_band4_frequency].effective_arg_idx = 0;

    delay_settings[delay_eq_band4_q].serial_cmd_name = "delay_eq_band4_q";
    delay_settings[delay_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band4]);
    delay_settings[delay_eq_band4_q].cmd_default_value = 0x07;
    delay_settings[delay_eq_band4_q].effective_arg_idx = 1;

    delay_settings[delay_eq_band5_type].serial_cmd_name = "delay_eq_band5_type";
    delay_settings[delay_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqEdgeSetup]);
    delay_settings[delay_eq_band5_type].cmd_default_value = 0x00;
    delay_settings[delay_eq_band5_type].effective_arg_idx = 1;

    delay_settings[delay_eq_band5_gain].serial_cmd_name = "delay_eq_band5_gain";
    delay_settings[delay_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band5]);
    delay_settings[delay_eq_band5_gain].cmd_default_value = 0x40;
    delay_settings[delay_eq_band5_gain].effective_arg_idx = 2;

    delay_settings[delay_eq_band5_frequency].serial_cmd_name = "delay_eq_band5_frequency";
    delay_settings[delay_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band5]);
    delay_settings[delay_eq_band5_frequency].cmd_default_value = 0x24;
    delay_settings[delay_eq_band5_frequency].effective_arg_idx = 0;

    delay_settings[delay_eq_band5_q].serial_cmd_name = "delay_eq_band5_q";
    delay_settings[delay_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band5]);
    delay_settings[delay_eq_band5_q].cmd_default_value = 0x07;
    delay_settings[delay_eq_band5_q].effective_arg_idx = 1;

    delay_settings[delay_eq_hpf_type].serial_cmd_name = "delay_eq_hpf_type";
    delay_settings[delay_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_DelayInputBandpassfilterSetup]);
    delay_settings[delay_eq_hpf_type].cmd_default_value = 0x06;
    delay_settings[delay_eq_hpf_type].effective_arg_idx = 0;

    delay_settings[delay_eq_hpf_frequency].serial_cmd_name = "delay_eq_hpf_frequency";
    delay_settings[delay_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInputBandpassfilterSetup]);
    delay_settings[delay_eq_hpf_frequency].cmd_default_value = 0x00;
    delay_settings[delay_eq_hpf_frequency].effective_arg_idx = 1;

    delay_settings[delay_eq_lpf_type].serial_cmd_name = "delay_eq_lpf_type";
    delay_settings[delay_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_DelayInputBandpassfilterSetup]);
    delay_settings[delay_eq_lpf_type].cmd_default_value = 0x06;
    delay_settings[delay_eq_lpf_type].effective_arg_idx = 2;

    delay_settings[delay_eq_lpf_frequency].serial_cmd_name = "delay_eq_lpf_frequency";
    delay_settings[delay_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInputBandpassfilterSetup]);
    delay_settings[delay_eq_lpf_frequency].cmd_default_value = 0x6f;
    delay_settings[delay_eq_lpf_frequency].effective_arg_idx = 3;

    delay_settings[reverb_eq_switch].serial_cmd_name = "reverb_eq_switch";
    delay_settings[reverb_eq_switch].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqEnable]);
    delay_settings[reverb_eq_switch].cmd_default_value = 0x00;
    delay_settings[reverb_eq_switch].effective_arg_idx = 0;

    delay_settings[reverb_eq_band1_type].serial_cmd_name = "reverb_eq_band1_type";
    delay_settings[reverb_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqEdgeSetup]);
    delay_settings[reverb_eq_band1_type].cmd_default_value = 0x00;
    delay_settings[reverb_eq_band1_type].effective_arg_idx = 0;

    delay_settings[reverb_eq_band1_gain].serial_cmd_name = "reverb_eq_band1_gain";
    delay_settings[reverb_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band1]);
    delay_settings[reverb_eq_band1_gain].cmd_default_value = 0x40;
    delay_settings[reverb_eq_band1_gain].effective_arg_idx = 2;

    delay_settings[reverb_eq_band1_frequency].serial_cmd_name = "reverb_eq_band1_frequency";
    delay_settings[reverb_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band1]);
    delay_settings[reverb_eq_band1_frequency].cmd_default_value = 0x0c;
    delay_settings[reverb_eq_band1_frequency].effective_arg_idx = 0;

    delay_settings[reverb_eq_band1_q].serial_cmd_name = "reverb_eq_band1_q";
    delay_settings[reverb_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band1]);
    delay_settings[reverb_eq_band1_q].cmd_default_value = 0x07;
    delay_settings[reverb_eq_band1_q].effective_arg_idx = 1;

    delay_settings[reverb_eq_band2_gain].serial_cmd_name = "reverb_eq_band2_gain";
    delay_settings[reverb_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band2]);
    delay_settings[reverb_eq_band2_gain].cmd_default_value = 0x40;
    delay_settings[reverb_eq_band2_gain].effective_arg_idx = 2;

    delay_settings[reverb_eq_band2_frequency].serial_cmd_name = "reverb_eq_band2_frequency";
    delay_settings[reverb_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band2]);
    delay_settings[reverb_eq_band2_frequency].cmd_default_value = 0x12;
    delay_settings[reverb_eq_band2_frequency].effective_arg_idx = 0;

    delay_settings[reverb_eq_band2_q].serial_cmd_name = "reverb_eq_band2_q";
    delay_settings[reverb_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band2]);
    delay_settings[reverb_eq_band2_q].cmd_default_value = 0x07;
    delay_settings[reverb_eq_band2_q].effective_arg_idx = 1;

    delay_settings[reverb_eq_band3_gain].serial_cmd_name = "reverb_eq_band3_gain";
    delay_settings[reverb_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band3]);
    delay_settings[reverb_eq_band3_gain].cmd_default_value = 0x40;
    delay_settings[reverb_eq_band3_gain].effective_arg_idx = 2;

    delay_settings[reverb_eq_band3_frequency].serial_cmd_name = "reverb_eq_band3_frequency";
    delay_settings[reverb_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band3]);
    delay_settings[reverb_eq_band3_frequency].cmd_default_value = 0x18;
    delay_settings[reverb_eq_band3_frequency].effective_arg_idx = 0;

    delay_settings[reverb_eq_band3_q].serial_cmd_name = "reverb_eq_band3_q";
    delay_settings[reverb_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band3]);
    delay_settings[reverb_eq_band3_q].cmd_default_value = 0x07;
    delay_settings[reverb_eq_band3_q].effective_arg_idx = 1;

    delay_settings[reverb_eq_band4_gain].serial_cmd_name = "reverb_eq_band4_gain";
    delay_settings[reverb_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band4]);
    delay_settings[reverb_eq_band4_gain].cmd_default_value = 0x40;
    delay_settings[reverb_eq_band4_gain].effective_arg_idx = 2;

    delay_settings[reverb_eq_band4_frequency].serial_cmd_name = "reverb_eq_band4_frequency";
    delay_settings[reverb_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band4]);
    delay_settings[reverb_eq_band4_frequency].cmd_default_value = 0x1e;
    delay_settings[reverb_eq_band4_frequency].effective_arg_idx = 0;

    delay_settings[reverb_eq_band4_q].serial_cmd_name = "reverb_eq_band4_q";
    delay_settings[reverb_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band4]);
    delay_settings[reverb_eq_band4_q].cmd_default_value = 0x07;
    delay_settings[reverb_eq_band4_q].effective_arg_idx = 1;

    delay_settings[reverb_eq_band5_type].serial_cmd_name = "reverb_eq_band5_type";
    delay_settings[reverb_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqEdgeSetup]);
    delay_settings[reverb_eq_band5_type].cmd_default_value = 0x00;
    delay_settings[reverb_eq_band5_type].effective_arg_idx = 1;

    delay_settings[reverb_eq_band5_gain].serial_cmd_name = "reverb_eq_band5_gain";
    delay_settings[reverb_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band5]);
    delay_settings[reverb_eq_band5_gain].cmd_default_value = 0x40;
    delay_settings[reverb_eq_band5_gain].effective_arg_idx = 2;

    delay_settings[reverb_eq_band5_frequency].serial_cmd_name = "reverb_eq_band5_frequency";
    delay_settings[reverb_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band5]);
    delay_settings[reverb_eq_band5_frequency].cmd_default_value = 0x24;
    delay_settings[reverb_eq_band5_frequency].effective_arg_idx = 0;

    delay_settings[reverb_eq_band5_q].serial_cmd_name = "reverb_eq_band5_q";
    delay_settings[reverb_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band5]);
    delay_settings[reverb_eq_band5_q].cmd_default_value = 0x07;
    delay_settings[reverb_eq_band5_q].effective_arg_idx = 1;

    delay_settings[reverb_eq_hpf_type].serial_cmd_name = "reverb_eq_hpf_type";
    delay_settings[reverb_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_ReverbInputBandpassfilterSetup]);
    delay_settings[reverb_eq_hpf_type].cmd_default_value = 0x06;
    delay_settings[reverb_eq_hpf_type].effective_arg_idx = 0;

    delay_settings[reverb_eq_hpf_frequency].serial_cmd_name = "reverb_eq_hpf_frequency";
    delay_settings[reverb_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInputBandpassfilterSetup]);
    delay_settings[reverb_eq_hpf_frequency].cmd_default_value = 0x00;
    delay_settings[reverb_eq_hpf_frequency].effective_arg_idx = 1;

    delay_settings[reverb_eq_lpf_type].serial_cmd_name = "reverb_eq_lpf_type";
    delay_settings[reverb_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_ReverbInputBandpassfilterSetup]);
    delay_settings[reverb_eq_lpf_type].cmd_default_value = 0x06;
    delay_settings[reverb_eq_lpf_type].effective_arg_idx = 2;

    delay_settings[reverb_eq_lpf_frequency].serial_cmd_name = "reverb_eq_lpf_frequency";
    delay_settings[reverb_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInputBandpassfilterSetup]);
    delay_settings[reverb_eq_lpf_frequency].cmd_default_value = 0x6f;
    delay_settings[reverb_eq_lpf_frequency].effective_arg_idx = 3;
}

static void music_effects_settings_default()
{
    memset(music_effects_settings,0,sizeof(DSP_SETTING_T)*music_effects_end);

    music_effects_settings[key_control_switch].serial_cmd_name = "key_control_switch";
    music_effects_settings[key_control_switch].effect_api = &(api_table[WSD_AL_AE_MusicKeyctrlEnable]);
    music_effects_settings[key_control_switch].cmd_default_value = 0x00;
    music_effects_settings[key_control_switch].effective_arg_idx = 0;

    music_effects_settings[key_control_pitch].serial_cmd_name = "key_control_pitch";
    music_effects_settings[key_control_pitch].effect_api = &(api_table[WSD_AL_AE_MusicKeyctrlSetup]);
    music_effects_settings[key_control_pitch].cmd_default_value = 0x40;
    music_effects_settings[key_control_pitch].effective_arg_idx = 0;

    music_effects_settings[key_control_pitch_fine].serial_cmd_name = "key_control_pitch_fine";
    music_effects_settings[key_control_pitch_fine].effect_api = &(api_table[WSD_AL_AE_MusicKeyctrlSetup]);
    music_effects_settings[key_control_pitch_fine].cmd_default_value = 0x40;
    music_effects_settings[key_control_pitch_fine].effective_arg_idx = 1;

    music_effects_settings[eq_switch].serial_cmd_name = "eq_switch";
    music_effects_settings[eq_switch].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqEnable]);
    music_effects_settings[eq_switch].cmd_default_value = 0x00;
    music_effects_settings[eq_switch].effective_arg_idx = 0;

    music_effects_settings[eq_band1_gain].serial_cmd_name = "eq_band1_gain";
    music_effects_settings[eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band1]);
    music_effects_settings[eq_band1_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band1_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band2_gain].serial_cmd_name = "eq_band2_gain";
    music_effects_settings[eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band2]);
    music_effects_settings[eq_band2_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band2_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band3_gain].serial_cmd_name = "eq_band3_gain";
    music_effects_settings[eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band3]);
    music_effects_settings[eq_band3_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band3_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band4_gain].serial_cmd_name = "eq_band4_gain";
    music_effects_settings[eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band4]);
    music_effects_settings[eq_band4_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band4_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band5_gain].serial_cmd_name = "eq_band5_gain";
    music_effects_settings[eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band5]);
    music_effects_settings[eq_band5_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band5_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band6_gain].serial_cmd_name = "eq_band6_gain";
    music_effects_settings[eq_band6_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band6]);
    music_effects_settings[eq_band6_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band6_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band7_gain].serial_cmd_name = "eq_band7_gain";
    music_effects_settings[eq_band7_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band7]);
    music_effects_settings[eq_band7_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band7_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band8_gain].serial_cmd_name = "eq_band8_gain";
    music_effects_settings[eq_band8_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band8]);
    music_effects_settings[eq_band8_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band8_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band9_gain].serial_cmd_name = "eq_band9_gain";
    music_effects_settings[eq_band9_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band9]);
    music_effects_settings[eq_band9_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band9_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band10_gain].serial_cmd_name = "eq_band10_gain";
    music_effects_settings[eq_band10_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band10]);
    music_effects_settings[eq_band10_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band10_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band11_gain].serial_cmd_name = "eq_band11_gain";
    music_effects_settings[eq_band11_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band11]);
    music_effects_settings[eq_band11_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band11_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band12_gain].serial_cmd_name = "eq_band12_gain";
    music_effects_settings[eq_band12_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band12]);
    music_effects_settings[eq_band12_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band12_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band13_gain].serial_cmd_name = "eq_band13_gain";
    music_effects_settings[eq_band13_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band13]);
    music_effects_settings[eq_band13_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band13_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band14_gain].serial_cmd_name = "eq_band14_gain";
    music_effects_settings[eq_band14_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band14]);
    music_effects_settings[eq_band14_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band14_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band15_gain].serial_cmd_name = "eq_band15_gain";
    music_effects_settings[eq_band15_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band15]);
    music_effects_settings[eq_band15_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band15_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band16_gain].serial_cmd_name = "eq_band16_gain";
    music_effects_settings[eq_band16_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band16]);
    music_effects_settings[eq_band16_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band16_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band17_gain].serial_cmd_name = "eq_band17_gain";
    music_effects_settings[eq_band17_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band17]);
    music_effects_settings[eq_band17_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band17_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band18_gain].serial_cmd_name = "eq_band18_gain";
    music_effects_settings[eq_band18_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band18]);
    music_effects_settings[eq_band18_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band18_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band19_gain].serial_cmd_name = "eq_band19_gain";
    music_effects_settings[eq_band19_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band19]);
    music_effects_settings[eq_band19_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band19_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band20_gain].serial_cmd_name = "eq_band20_gain";
    music_effects_settings[eq_band20_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band20]);
    music_effects_settings[eq_band20_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band20_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band21_gain].serial_cmd_name = "eq_band21_gain";
    music_effects_settings[eq_band21_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band21]);
    music_effects_settings[eq_band21_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band21_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band22_gain].serial_cmd_name = "eq_band22_gain";
    music_effects_settings[eq_band22_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band22]);
    music_effects_settings[eq_band22_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band22_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band23_gain].serial_cmd_name = "eq_band23_gain";
    music_effects_settings[eq_band23_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band23]);
    music_effects_settings[eq_band23_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band23_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band24_gain].serial_cmd_name = "eq_band24_gain";
    music_effects_settings[eq_band24_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band24]);
    music_effects_settings[eq_band24_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band24_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band25_gain].serial_cmd_name = "eq_band25_gain";
    music_effects_settings[eq_band25_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band25]);
    music_effects_settings[eq_band25_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band25_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band26_gain].serial_cmd_name = "eq_band26_gain";
    music_effects_settings[eq_band26_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band26]);
    music_effects_settings[eq_band26_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band26_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band27_gain].serial_cmd_name = "eq_band27_gain";
    music_effects_settings[eq_band27_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band27]);
    music_effects_settings[eq_band27_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band27_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band28_gain].serial_cmd_name = "eq_band28_gain";
    music_effects_settings[eq_band28_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band28]);
    music_effects_settings[eq_band28_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band28_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band29_gain].serial_cmd_name = "eq_band29_gain";
    music_effects_settings[eq_band29_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band29]);
    music_effects_settings[eq_band29_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band29_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band30_gain].serial_cmd_name = "eq_band30_gain";
    music_effects_settings[eq_band30_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band30]);
    music_effects_settings[eq_band30_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band30_gain].effective_arg_idx = 0;

    music_effects_settings[eq_band31_gain].serial_cmd_name = "eq_band31_gain";
    music_effects_settings[eq_band31_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band31]);
    music_effects_settings[eq_band31_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band31_gain].effective_arg_idx = 0;

    music_effects_settings[music_phase].serial_cmd_name = "music_phase";
    music_effects_settings[music_phase].effect_api = &(api_table[WSD_AL_AE_MusicPhaseSetup]);
    music_effects_settings[music_phase].cmd_default_value = 0x00;
    music_effects_settings[music_phase].effective_arg_idx = 0;
}

static void output_settings_default()
{
    memset(output_settings,0,sizeof(DSP_SETTING_T)*output_end);

    output_settings[front_eq_switch].serial_cmd_name = "front_eq_switch";
    output_settings[front_eq_switch].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEnable_front]);
    output_settings[front_eq_switch].cmd_default_value = 0x00;
    output_settings[front_eq_switch].effective_arg_idx = 0;

    output_settings[front_eq_band1_type].serial_cmd_name = "front_eq_band1_type";
    output_settings[front_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_front]);
    output_settings[front_eq_band1_type].cmd_default_value = 0x00;
    output_settings[front_eq_band1_type].effective_arg_idx = 0;

    output_settings[front_eq_band1_gain].serial_cmd_name = "front_eq_band1_gain";
    output_settings[front_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band1]);
    output_settings[front_eq_band1_gain].cmd_default_value = 0x40;
    output_settings[front_eq_band1_gain].effective_arg_idx = 2;

    output_settings[front_eq_band1_frequency].serial_cmd_name = "front_eq_band1_frequency";
    output_settings[front_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band1]);
    output_settings[front_eq_band1_frequency].cmd_default_value = 0x0c;
    output_settings[front_eq_band1_frequency].effective_arg_idx = 0;

    output_settings[front_eq_band1_q].serial_cmd_name = "front_eq_band1_q";
    output_settings[front_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band1]);
    output_settings[front_eq_band1_q].cmd_default_value = 0x07;
    output_settings[front_eq_band1_q].effective_arg_idx = 1;

    output_settings[front_eq_band2_gain].serial_cmd_name = "front_eq_band2_gain";
    output_settings[front_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band2]);
    output_settings[front_eq_band2_gain].cmd_default_value = 0x40;
    output_settings[front_eq_band2_gain].effective_arg_idx = 2;

    output_settings[front_eq_band2_frequency].serial_cmd_name = "front_eq_band2_frequency";
    output_settings[front_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band2]);
    output_settings[front_eq_band2_frequency].cmd_default_value = 0x12;
    output_settings[front_eq_band2_frequency].effective_arg_idx = 0;

    output_settings[front_eq_band2_q].serial_cmd_name = "front_eq_band2_q";
    output_settings[front_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band2]);
    output_settings[front_eq_band2_q].cmd_default_value = 0x07;
    output_settings[front_eq_band2_q].effective_arg_idx = 1;

    output_settings[front_eq_band3_gain].serial_cmd_name = "front_eq_band3_gain";
    output_settings[front_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band3]);
    output_settings[front_eq_band3_gain].cmd_default_value = 0x40;
    output_settings[front_eq_band3_gain].effective_arg_idx = 2;

    output_settings[front_eq_band3_frequency].serial_cmd_name = "front_eq_band3_frequency";
    output_settings[front_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band3]);
    output_settings[front_eq_band3_frequency].cmd_default_value = 0x18;
    output_settings[front_eq_band3_frequency].effective_arg_idx = 0;

    output_settings[front_eq_band3_q].serial_cmd_name = "front_eq_band3_q";
    output_settings[front_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band3]);
    output_settings[front_eq_band3_q].cmd_default_value = 0x07;
    output_settings[front_eq_band3_q].effective_arg_idx = 1;

    output_settings[front_eq_band4_gain].serial_cmd_name = "front_eq_band4_gain";
    output_settings[front_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band4]);
    output_settings[front_eq_band4_gain].cmd_default_value = 0x40;
    output_settings[front_eq_band4_gain].effective_arg_idx = 2;

    output_settings[front_eq_band4_frequency].serial_cmd_name = "front_eq_band4_frequency";
    output_settings[front_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band4]);
    output_settings[front_eq_band4_frequency].cmd_default_value = 0x1e;
    output_settings[front_eq_band4_frequency].effective_arg_idx = 0;

    output_settings[front_eq_band4_q].serial_cmd_name = "front_eq_band4_q";
    output_settings[front_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band4]);
    output_settings[front_eq_band4_q].cmd_default_value = 0x07;
    output_settings[front_eq_band4_q].effective_arg_idx = 1;

    output_settings[front_eq_band5_type].serial_cmd_name = "front_eq_band5_type";
    output_settings[front_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_front]);
    output_settings[front_eq_band5_type].cmd_default_value = 0x00;
    output_settings[front_eq_band5_type].effective_arg_idx = 1;

    output_settings[front_eq_band5_gain].serial_cmd_name = "front_eq_band5_gain";
    output_settings[front_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band5]);
    output_settings[front_eq_band5_gain].cmd_default_value = 0x40;
    output_settings[front_eq_band5_gain].effective_arg_idx = 2;

    output_settings[front_eq_band5_frequency].serial_cmd_name = "front_eq_band5_frequency";
    output_settings[front_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band5]);
    output_settings[front_eq_band5_frequency].cmd_default_value = 0x24;
    output_settings[front_eq_band5_frequency].effective_arg_idx = 0;

    output_settings[front_eq_band5_q].serial_cmd_name = "front_eq_band5_q";
    output_settings[front_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band5]);
    output_settings[front_eq_band5_q].cmd_default_value = 0x07;
    output_settings[front_eq_band5_q].effective_arg_idx = 1;

    output_settings[front_eq_hpf_type].serial_cmd_name = "front_eq_hpf_type";
    output_settings[front_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_front]);
    output_settings[front_eq_hpf_type].cmd_default_value = 0x06;
    output_settings[front_eq_hpf_type].effective_arg_idx = 0;

    output_settings[front_eq_hpf_frequency].serial_cmd_name = "front_eq_hpf_frequency";
    output_settings[front_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_front]);
    output_settings[front_eq_hpf_frequency].cmd_default_value = 0x00;
    output_settings[front_eq_hpf_frequency].effective_arg_idx = 1;

    output_settings[front_eq_lpf_type].serial_cmd_name = "front_eq_lpf_type";
    output_settings[front_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_front]);
    output_settings[front_eq_lpf_type].cmd_default_value = 0x06;
    output_settings[front_eq_lpf_type].effective_arg_idx = 2;

    output_settings[front_eq_lpf_frequency].serial_cmd_name = "front_eq_lpf_frequency";
    output_settings[front_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_front]);
    output_settings[front_eq_lpf_frequency].cmd_default_value = 0x6f;
    output_settings[front_eq_lpf_frequency].effective_arg_idx = 3;

    output_settings[front_delay_time].serial_cmd_name = "front_delay_time";
    output_settings[front_delay_time].effect_api = &(api_table[WSD_AL_AE_OutputSetup_front]);
    output_settings[front_delay_time].cmd_default_value = 0x0;
    output_settings[front_delay_time].effective_arg_idx = 1;

    output_settings[front_phase].serial_cmd_name = "front_phase";
    output_settings[front_phase].effect_api = &(api_table[WSD_AL_AE_OutputSetup_front]);
    output_settings[front_phase].cmd_default_value = 0x0;
    output_settings[front_phase].effective_arg_idx = 2;

    output_settings[rear_eq_switch].serial_cmd_name = "rear_eq_switch";
    output_settings[rear_eq_switch].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEnable_rear]);
    output_settings[rear_eq_switch].cmd_default_value = 0x00;
    output_settings[rear_eq_switch].effective_arg_idx = 0;

    output_settings[rear_eq_band1_type].serial_cmd_name = "rear_eq_band1_type";
    output_settings[rear_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_rear]);
    output_settings[rear_eq_band1_type].cmd_default_value = 0x00;
    output_settings[rear_eq_band1_type].effective_arg_idx = 0;

    output_settings[rear_eq_band1_gain].serial_cmd_name = "rear_eq_band1_gain";
    output_settings[rear_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band1]);
    output_settings[rear_eq_band1_gain].cmd_default_value = 0x40;
    output_settings[rear_eq_band1_gain].effective_arg_idx = 2;

    output_settings[rear_eq_band1_frequency].serial_cmd_name = "rear_eq_band1_frequency";
    output_settings[rear_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band1]);
    output_settings[rear_eq_band1_frequency].cmd_default_value = 0x0c;
    output_settings[rear_eq_band1_frequency].effective_arg_idx = 0;

    output_settings[rear_eq_band1_q].serial_cmd_name = "rear_eq_band1_q";
    output_settings[rear_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band1]);
    output_settings[rear_eq_band1_q].cmd_default_value = 0x07;
    output_settings[rear_eq_band1_q].effective_arg_idx = 1;

    output_settings[rear_eq_band2_gain].serial_cmd_name = "rear_eq_band2_gain";
    output_settings[rear_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band2]);
    output_settings[rear_eq_band2_gain].cmd_default_value = 0x40;
    output_settings[rear_eq_band2_gain].effective_arg_idx = 2;

    output_settings[rear_eq_band2_frequency].serial_cmd_name = "rear_eq_band2_frequency";
    output_settings[rear_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band2]);
    output_settings[rear_eq_band2_frequency].cmd_default_value = 0x12;
    output_settings[rear_eq_band2_frequency].effective_arg_idx = 0;

    output_settings[rear_eq_band2_q].serial_cmd_name = "rear_eq_band2_q";
    output_settings[rear_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band2]);
    output_settings[rear_eq_band2_q].cmd_default_value = 0x07;
    output_settings[rear_eq_band2_q].effective_arg_idx = 1;

    output_settings[rear_eq_band3_gain].serial_cmd_name = "rear_eq_band3_gain";
    output_settings[rear_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band3]);
    output_settings[rear_eq_band3_gain].cmd_default_value = 0x40;
    output_settings[rear_eq_band3_gain].effective_arg_idx = 2;

    output_settings[rear_eq_band3_frequency].serial_cmd_name = "rear_eq_band3_frequency";
    output_settings[rear_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band3]);
    output_settings[rear_eq_band3_frequency].cmd_default_value = 0x18;
    output_settings[rear_eq_band3_frequency].effective_arg_idx = 0;

    output_settings[rear_eq_band3_q].serial_cmd_name = "rear_eq_band3_q";
    output_settings[rear_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band3]);
    output_settings[rear_eq_band3_q].cmd_default_value = 0x07;
    output_settings[rear_eq_band3_q].effective_arg_idx = 1;

    output_settings[rear_eq_band4_gain].serial_cmd_name = "rear_eq_band4_gain";
    output_settings[rear_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band4]);
    output_settings[rear_eq_band4_gain].cmd_default_value = 0x40;
    output_settings[rear_eq_band4_gain].effective_arg_idx = 2;

    output_settings[rear_eq_band4_frequency].serial_cmd_name = "rear_eq_band4_frequency";
    output_settings[rear_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band4]);
    output_settings[rear_eq_band4_frequency].cmd_default_value = 0x1e;
    output_settings[rear_eq_band4_frequency].effective_arg_idx = 0;

    output_settings[rear_eq_band4_q].serial_cmd_name = "rear_eq_band4_q";
    output_settings[rear_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band4]);
    output_settings[rear_eq_band4_q].cmd_default_value = 0x07;
    output_settings[rear_eq_band4_q].effective_arg_idx = 1;

    output_settings[rear_eq_band5_type].serial_cmd_name = "rear_eq_band5_type";
    output_settings[rear_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_rear]);
    output_settings[rear_eq_band5_type].cmd_default_value = 0x00;
    output_settings[rear_eq_band5_type].effective_arg_idx = 1;

    output_settings[rear_eq_band5_gain].serial_cmd_name = "rear_eq_band5_gain";
    output_settings[rear_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band5]);
    output_settings[rear_eq_band5_gain].cmd_default_value = 0x40;
    output_settings[rear_eq_band5_gain].effective_arg_idx = 2;

    output_settings[rear_eq_band5_frequency].serial_cmd_name = "rear_eq_band5_frequency";
    output_settings[rear_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band5]);
    output_settings[rear_eq_band5_frequency].cmd_default_value = 0x24;
    output_settings[rear_eq_band5_frequency].effective_arg_idx = 0;

    output_settings[rear_eq_band5_q].serial_cmd_name = "rear_eq_band5_q";
    output_settings[rear_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band5]);
    output_settings[rear_eq_band5_q].cmd_default_value = 0x07;
    output_settings[rear_eq_band5_q].effective_arg_idx = 1;

    output_settings[rear_eq_hpf_type].serial_cmd_name = "rear_eq_hpf_type";
    output_settings[rear_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_rear]);
    output_settings[rear_eq_hpf_type].cmd_default_value = 0x06;
    output_settings[rear_eq_hpf_type].effective_arg_idx = 0;

    output_settings[rear_eq_hpf_frequency].serial_cmd_name = "rear_eq_hpf_frequency";
    output_settings[rear_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_rear]);
    output_settings[rear_eq_hpf_frequency].cmd_default_value = 0x00;
    output_settings[rear_eq_hpf_frequency].effective_arg_idx = 1;

    output_settings[rear_eq_lpf_type].serial_cmd_name = "rear_eq_lpf_type";
    output_settings[rear_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_rear]);
    output_settings[rear_eq_lpf_type].cmd_default_value = 0x06;
    output_settings[rear_eq_lpf_type].effective_arg_idx = 2;

    output_settings[rear_eq_lpf_frequency].serial_cmd_name = "rear_eq_lpf_frequency";
    output_settings[rear_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_rear]);
    output_settings[rear_eq_lpf_frequency].cmd_default_value = 0x6f;
    output_settings[rear_eq_lpf_frequency].effective_arg_idx = 3;

    output_settings[rear_delay_time].serial_cmd_name = "rear_delay_time";
    output_settings[rear_delay_time].effect_api = &(api_table[WSD_AL_AE_OutputSetup_rear]);
    output_settings[rear_delay_time].cmd_default_value = 0x0;
    output_settings[rear_delay_time].effective_arg_idx = 1;

    output_settings[rear_phase].serial_cmd_name = "rear_phase";
    output_settings[rear_phase].effect_api = &(api_table[WSD_AL_AE_OutputSetup_rear]);
    output_settings[rear_phase].cmd_default_value = 0x0;
    output_settings[rear_phase].effective_arg_idx = 2;

    output_settings[center_eq_switch].serial_cmd_name = "center_eq_switch";
    output_settings[center_eq_switch].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEnable_center]);
    output_settings[center_eq_switch].cmd_default_value = 0x00;
    output_settings[center_eq_switch].effective_arg_idx = 0;

    output_settings[center_eq_band1_type].serial_cmd_name = "center_eq_band1_type";
    output_settings[center_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_center]);
    output_settings[center_eq_band1_type].cmd_default_value = 0x00;
    output_settings[center_eq_band1_type].effective_arg_idx = 0;

    output_settings[center_eq_band1_gain].serial_cmd_name = "center_eq_band1_gain";
    output_settings[center_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band1]);
    output_settings[center_eq_band1_gain].cmd_default_value = 0x40;
    output_settings[center_eq_band1_gain].effective_arg_idx = 2;

    output_settings[center_eq_band1_frequency].serial_cmd_name = "center_eq_band1_frequency";
    output_settings[center_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band1]);
    output_settings[center_eq_band1_frequency].cmd_default_value = 0x0c;
    output_settings[center_eq_band1_frequency].effective_arg_idx = 0;

    output_settings[center_eq_band1_q].serial_cmd_name = "center_eq_band1_q";
    output_settings[center_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band1]);
    output_settings[center_eq_band1_q].cmd_default_value = 0x07;
    output_settings[center_eq_band1_q].effective_arg_idx = 1;

    output_settings[center_eq_band2_gain].serial_cmd_name = "center_eq_band2_gain";
    output_settings[center_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band2]);
    output_settings[center_eq_band2_gain].cmd_default_value = 0x40;
    output_settings[center_eq_band2_gain].effective_arg_idx = 2;

    output_settings[center_eq_band2_frequency].serial_cmd_name = "center_eq_band2_frequency";
    output_settings[center_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band2]);
    output_settings[center_eq_band2_frequency].cmd_default_value = 0x12;
    output_settings[center_eq_band2_frequency].effective_arg_idx = 0;

    output_settings[center_eq_band2_q].serial_cmd_name = "center_eq_band2_q";
    output_settings[center_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band2]);
    output_settings[center_eq_band2_q].cmd_default_value = 0x07;
    output_settings[center_eq_band2_q].effective_arg_idx = 1;

    output_settings[center_eq_band3_gain].serial_cmd_name = "center_eq_band3_gain";
    output_settings[center_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band3]);
    output_settings[center_eq_band3_gain].cmd_default_value = 0x40;
    output_settings[center_eq_band3_gain].effective_arg_idx = 2;

    output_settings[center_eq_band3_frequency].serial_cmd_name = "center_eq_band3_frequency";
    output_settings[center_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band3]);
    output_settings[center_eq_band3_frequency].cmd_default_value = 0x18;
    output_settings[center_eq_band3_frequency].effective_arg_idx = 0;

    output_settings[center_eq_band3_q].serial_cmd_name = "center_eq_band3_q";
    output_settings[center_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band3]);
    output_settings[center_eq_band3_q].cmd_default_value = 0x07;
    output_settings[center_eq_band3_q].effective_arg_idx = 1;

    output_settings[center_eq_band4_gain].serial_cmd_name = "center_eq_band4_gain";
    output_settings[center_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band4]);
    output_settings[center_eq_band4_gain].cmd_default_value = 0x40;
    output_settings[center_eq_band4_gain].effective_arg_idx = 2;

    output_settings[center_eq_band4_frequency].serial_cmd_name = "center_eq_band4_frequency";
    output_settings[center_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band4]);
    output_settings[center_eq_band4_frequency].cmd_default_value = 0x1e;
    output_settings[center_eq_band4_frequency].effective_arg_idx = 0;

    output_settings[center_eq_band4_q].serial_cmd_name = "center_eq_band4_q";
    output_settings[center_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band4]);
    output_settings[center_eq_band4_q].cmd_default_value = 0x07;
    output_settings[center_eq_band4_q].effective_arg_idx = 1;

    output_settings[center_eq_band5_type].serial_cmd_name = "center_eq_band5_type";
    output_settings[center_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_center]);
    output_settings[center_eq_band5_type].cmd_default_value = 0x00;
    output_settings[center_eq_band5_type].effective_arg_idx = 1;

    output_settings[center_eq_band5_gain].serial_cmd_name = "center_eq_band5_gain";
    output_settings[center_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band5]);
    output_settings[center_eq_band5_gain].cmd_default_value = 0x40;
    output_settings[center_eq_band5_gain].effective_arg_idx = 2;

    output_settings[center_eq_band5_frequency].serial_cmd_name = "center_eq_band5_frequency";
    output_settings[center_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band5]);
    output_settings[center_eq_band5_frequency].cmd_default_value = 0x24;
    output_settings[center_eq_band5_frequency].effective_arg_idx = 0;

    output_settings[center_eq_band5_q].serial_cmd_name = "center_eq_band5_q";
    output_settings[center_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band5]);
    output_settings[center_eq_band5_q].cmd_default_value = 0x07;
    output_settings[center_eq_band5_q].effective_arg_idx = 1;

    output_settings[center_eq_hpf_type].serial_cmd_name = "center_eq_hpf_type";
    output_settings[center_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_center]);
    output_settings[center_eq_hpf_type].cmd_default_value = 0x06;
    output_settings[center_eq_hpf_type].effective_arg_idx = 0;

    output_settings[center_eq_hpf_frequency].serial_cmd_name = "center_eq_hpf_frequency";
    output_settings[center_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_center]);
    output_settings[center_eq_hpf_frequency].cmd_default_value = 0x00;
    output_settings[center_eq_hpf_frequency].effective_arg_idx = 1;

    output_settings[center_eq_lpf_type].serial_cmd_name = "center_eq_lpf_type";
    output_settings[center_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_center]);
    output_settings[center_eq_lpf_type].cmd_default_value = 0x06;
    output_settings[center_eq_lpf_type].effective_arg_idx = 2;

    output_settings[center_eq_lpf_frequency].serial_cmd_name = "center_eq_lpf_frequency";
    output_settings[center_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_center]);
    output_settings[center_eq_lpf_frequency].cmd_default_value = 0x6f;
    output_settings[center_eq_lpf_frequency].effective_arg_idx = 3;

    output_settings[center_delay_time].serial_cmd_name = "center_delay_time";
    output_settings[center_delay_time].effect_api = &(api_table[WSD_AL_AE_OutputSetup_center]);
    output_settings[center_delay_time].cmd_default_value = 0x0;
    output_settings[center_delay_time].effective_arg_idx = 1;

    output_settings[center_phase].serial_cmd_name = "center_phase";
    output_settings[center_phase].effect_api = &(api_table[WSD_AL_AE_OutputSetup_center]);
    output_settings[center_phase].cmd_default_value = 0x0;
    output_settings[center_phase].effective_arg_idx = 2;

    output_settings[subwoofer_eq_switch].serial_cmd_name = "subwoofer_eq_switch";
    output_settings[subwoofer_eq_switch].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEnable_subwoofer]);
    output_settings[subwoofer_eq_switch].cmd_default_value = 0x01;
    output_settings[subwoofer_eq_switch].effective_arg_idx = 0;

    output_settings[subwoofer_eq_band1_type].serial_cmd_name = "subwoofer_eq_band1_type";
    output_settings[subwoofer_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_subwoofer]);
    output_settings[subwoofer_eq_band1_type].cmd_default_value = 0x00;
    output_settings[subwoofer_eq_band1_type].effective_arg_idx = 0;

    output_settings[subwoofer_eq_band1_gain].serial_cmd_name = "subwoofer_eq_band1_gain";
    output_settings[subwoofer_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band1]);
    output_settings[subwoofer_eq_band1_gain].cmd_default_value = 0x40;
    output_settings[subwoofer_eq_band1_gain].effective_arg_idx = 2;

    output_settings[subwoofer_eq_band1_frequency].serial_cmd_name = "subwoofer_eq_band1_frequency";
    output_settings[subwoofer_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band1]);
    output_settings[subwoofer_eq_band1_frequency].cmd_default_value = 0x6;
    output_settings[subwoofer_eq_band1_frequency].effective_arg_idx = 0;

    output_settings[subwoofer_eq_band1_q].serial_cmd_name = "subwoofer_eq_band1_q";
    output_settings[subwoofer_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band1]);
    output_settings[subwoofer_eq_band1_q].cmd_default_value = 0x13;
    output_settings[subwoofer_eq_band1_q].effective_arg_idx = 1;

    output_settings[subwoofer_eq_band2_gain].serial_cmd_name = "subwoofer_eq_band2_gain";
    output_settings[subwoofer_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band2]);
    output_settings[subwoofer_eq_band2_gain].cmd_default_value = 0x40;
    output_settings[subwoofer_eq_band2_gain].effective_arg_idx = 2;

    output_settings[subwoofer_eq_band2_frequency].serial_cmd_name = "subwoofer_eq_band2_frequency";
    output_settings[subwoofer_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band2]);
    output_settings[subwoofer_eq_band2_frequency].cmd_default_value = 0x0a;
    output_settings[subwoofer_eq_band2_frequency].effective_arg_idx = 0;

    output_settings[subwoofer_eq_band2_q].serial_cmd_name = "subwoofer_eq_band2_q";
    output_settings[subwoofer_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band2]);
    output_settings[subwoofer_eq_band2_q].cmd_default_value = 0x13;
    output_settings[subwoofer_eq_band2_q].effective_arg_idx = 1;

    output_settings[subwoofer_eq_band3_gain].serial_cmd_name = "subwoofer_eq_band3_gain";
    output_settings[subwoofer_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band3]);
    output_settings[subwoofer_eq_band3_gain].cmd_default_value = 0x40;
    output_settings[subwoofer_eq_band3_gain].effective_arg_idx = 2;

    output_settings[subwoofer_eq_band3_frequency].serial_cmd_name = "subwoofer_eq_band3_frequency";
    output_settings[subwoofer_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band3]);
    output_settings[subwoofer_eq_band3_frequency].cmd_default_value = 0x0d;
    output_settings[subwoofer_eq_band3_frequency].effective_arg_idx = 0;

    output_settings[subwoofer_eq_band3_q].serial_cmd_name = "subwoofer_eq_band3_q";
    output_settings[subwoofer_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band3]);
    output_settings[subwoofer_eq_band3_q].cmd_default_value = 0x13;
    output_settings[subwoofer_eq_band3_q].effective_arg_idx = 1;

    output_settings[subwoofer_eq_band4_gain].serial_cmd_name = "subwoofer_eq_band4_gain";
    output_settings[subwoofer_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band4]);
    output_settings[subwoofer_eq_band4_gain].cmd_default_value = 0x40;
    output_settings[subwoofer_eq_band4_gain].effective_arg_idx = 2;

    output_settings[subwoofer_eq_band4_frequency].serial_cmd_name = "subwoofer_eq_band4_frequency";
    output_settings[subwoofer_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band4]);
    output_settings[subwoofer_eq_band4_frequency].cmd_default_value = 0x10;
    output_settings[subwoofer_eq_band4_frequency].effective_arg_idx = 0;

    output_settings[subwoofer_eq_band4_q].serial_cmd_name = "subwoofer_eq_band4_q";
    output_settings[subwoofer_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band4]);
    output_settings[subwoofer_eq_band4_q].cmd_default_value = 0x13;
    output_settings[subwoofer_eq_band4_q].effective_arg_idx = 1;

    output_settings[subwoofer_eq_band5_type].serial_cmd_name = "subwoofer_eq_band5_type";
    output_settings[subwoofer_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_subwoofer]);
    output_settings[subwoofer_eq_band5_type].cmd_default_value = 0x00;
    output_settings[subwoofer_eq_band5_type].effective_arg_idx = 1;

    output_settings[subwoofer_eq_band5_gain].serial_cmd_name = "subwoofer_eq_band5_gain";
    output_settings[subwoofer_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band5]);
    output_settings[subwoofer_eq_band5_gain].cmd_default_value = 0x40;
    output_settings[subwoofer_eq_band5_gain].effective_arg_idx = 2;

    output_settings[subwoofer_eq_band5_frequency].serial_cmd_name = "subwoofer_eq_band5_frequency";
    output_settings[subwoofer_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band5]);
    output_settings[subwoofer_eq_band5_frequency].cmd_default_value = 0x14;
    output_settings[subwoofer_eq_band5_frequency].effective_arg_idx = 0;

    output_settings[subwoofer_eq_band5_q].serial_cmd_name = "subwoofer_eq_band5_q";
    output_settings[subwoofer_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band5]);
    output_settings[subwoofer_eq_band5_q].cmd_default_value = 0x13;
    output_settings[subwoofer_eq_band5_q].effective_arg_idx = 1;

    output_settings[subwoofer_eq_hpf_type].serial_cmd_name = "subwoofer_eq_hpf_type";
    output_settings[subwoofer_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_subwoofer]);
    output_settings[subwoofer_eq_hpf_type].cmd_default_value = 0x06;
    output_settings[subwoofer_eq_hpf_type].effective_arg_idx = 0;

    output_settings[subwoofer_eq_hpf_frequency].serial_cmd_name = "subwoofer_eq_hpf_frequency";
    output_settings[subwoofer_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_subwoofer]);
    output_settings[subwoofer_eq_hpf_frequency].cmd_default_value = 0x05;
    output_settings[subwoofer_eq_hpf_frequency].effective_arg_idx = 1;

    output_settings[subwoofer_eq_lpf_type].serial_cmd_name = "subwoofer_eq_lpf_type";
    output_settings[subwoofer_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_subwoofer]);
    output_settings[subwoofer_eq_lpf_type].cmd_default_value = 0x06;
    output_settings[subwoofer_eq_lpf_type].effective_arg_idx = 2;

    output_settings[subwoofer_eq_lpf_frequency].serial_cmd_name = "subwoofer_eq_lpf_frequency";
    output_settings[subwoofer_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_subwoofer]);
    output_settings[subwoofer_eq_lpf_frequency].cmd_default_value = 0x16;
    output_settings[subwoofer_eq_lpf_frequency].effective_arg_idx = 3;

    output_settings[subwoofer_delay_time].serial_cmd_name = "subwoofer_delay_time";
    output_settings[subwoofer_delay_time].effect_api = &(api_table[WSD_AL_AE_OutputSetup_subwoofer]);
    output_settings[subwoofer_delay_time].cmd_default_value = 0x0;
    output_settings[subwoofer_delay_time].effective_arg_idx = 1;

    output_settings[subwoofer_phase].serial_cmd_name = "subwoofer_phase";
    output_settings[subwoofer_phase].effect_api = &(api_table[WSD_AL_AE_OutputSetup_subwoofer]);
    output_settings[subwoofer_phase].cmd_default_value = 0x0;
    output_settings[subwoofer_phase].effective_arg_idx = 2;

    output_settings[subwoofer_compressor_switch].serial_cmd_name = "subwoofer_compressor_switch";
    output_settings[subwoofer_compressor_switch].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorEnable]);
    output_settings[subwoofer_compressor_switch].cmd_default_value = 0x1;
    output_settings[subwoofer_compressor_switch].effective_arg_idx = 0;

    output_settings[subwoofer_compressor_attack].serial_cmd_name = "subwoofer_compressor_attack";
    output_settings[subwoofer_compressor_attack].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorSetup]);
    output_settings[subwoofer_compressor_attack].cmd_default_value = 0x50;
    output_settings[subwoofer_compressor_attack].effective_arg_idx = 0;

    output_settings[subwoofer_compressor_release].serial_cmd_name = "subwoofer_compressor_release";
    output_settings[subwoofer_compressor_release].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorSetup]);
    output_settings[subwoofer_compressor_release].cmd_default_value = 0x05;
    output_settings[subwoofer_compressor_release].effective_arg_idx = 1;

    output_settings[subwoofer_compressor_threshold].serial_cmd_name = "subwoofer_compressor_threshold";
    output_settings[subwoofer_compressor_threshold].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorSetup]);
    output_settings[subwoofer_compressor_threshold].cmd_default_value = 0x46;
    output_settings[subwoofer_compressor_threshold].effective_arg_idx = 2;

    output_settings[subwoofer_compressor_ratio].serial_cmd_name = "subwoofer_compressor_ratio";
    output_settings[subwoofer_compressor_ratio].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorSetup]);
    output_settings[subwoofer_compressor_ratio].cmd_default_value = 0x9;
    output_settings[subwoofer_compressor_ratio].effective_arg_idx = 3;

    output_settings[subwoofer_compressor_gain].serial_cmd_name = "subwoofer_compressor_gain";
    output_settings[subwoofer_compressor_gain].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorSetup]);
    output_settings[subwoofer_compressor_gain].cmd_default_value = 0x5;
    output_settings[subwoofer_compressor_gain].effective_arg_idx = 4;
}

void roland_cmd_settings_table_init()
{
    mixer_settings_default();
    mic_effects_settings_default();
    delay_settings_default();
    music_effects_settings_default();
    output_settings_default();
}

int roland_cmd_translate(char *arg_list[], int arg_cnt)
{
    unsigned long block_id,cmd_idx,value;
    DSP_SETTING_T* settings_table;
    int settings_table_size;

    block_id = strtoul(arg_list[0], NULL, 16);
    cmd_idx = strtoul(arg_list[1], NULL, 16);
    value = strtoul(arg_list[2], NULL, 16);

    switch(block_id)
    {
    case roland_mixer_block:
        debug_print_info("block_id 0x%lx is mixer block:\n",block_id);
        settings_table = mixer_settings;
        settings_table_size = mixer_end;
        break;

    case roland_mic_effects_block:
        debug_print_info("block_id 0x%lx is mic effect block:\n",block_id);
        settings_table = mic_effects_settings;
        settings_table_size = mic_effects_end;
        break;

    case roland_echo_block:
        debug_print_info("block_id 0x%lx is echo block:\n",block_id);
        settings_table = delay_settings;
        settings_table_size = delay_end;
        break;

    case roland_music_effects_block:
        debug_print_info("block_id 0x%lx is music effect block:\n",block_id);
        settings_table = music_effects_settings;
        settings_table_size = music_effects_end;
        break;

    case roland_output_block:
        debug_print_info("block_id 0x%lx is output block:\n",block_id);
        settings_table = output_settings;
        settings_table_size = output_end;
        break;

    default:
        debug_print_warn("block_id 0x%lx is unknown!!!\n",block_id);
        return -1;
        break;
    }

    if (cmd_idx >= settings_table_size)
    {
        debug_print_error("cmd_idx 0x%lx is larger than 0x%x,out of table size in block_id 0x%lx!!!\n",cmd_idx,settings_table_size,block_id);
        return -1;
    }

    //将此项配置保存成xml
    if (settings_table[cmd_idx].serial_cmd_name)
    {
        xml_profile_add(settings_table, block_id, cmd_idx, value);
    }

    //收集统计api信息
    if (settings_table[cmd_idx].cmd_default_value!=value)
    {
        debug_print_info("(%s) 0x%x->0x%lx\n",
                settings_table[cmd_idx].serial_cmd_name,settings_table[cmd_idx].cmd_default_value,value);
        if (settings_table[cmd_idx].effect_api)
        {
            int cmd_value_diff = (int)value - (int)(settings_table[cmd_idx].cmd_default_value);
            int cmd_arg_idx = settings_table[cmd_idx].effective_arg_idx;

            settings_table[cmd_idx].effect_api->is_active = true;
            settings_table[cmd_idx].effect_api->arg_value[cmd_arg_idx] = settings_table[cmd_idx].effect_api->arg_default_value[cmd_arg_idx] + cmd_value_diff;

            if (PRINT_DEBUG || debug_flag)
            {
                printf("effect api is %s",settings_table[cmd_idx].effect_api->api_name);
                for (int i=0; i<settings_table[cmd_idx].effect_api->effective_arg_cnt; i++)
                {
                    if (i==cmd_arg_idx)
                    {
                        printf("%d",settings_table[cmd_idx].effect_api->arg_default_value[cmd_arg_idx] + cmd_value_diff);
                    }
                    else
                    {
                        printf("0xff");
                    }

                    if (i!=settings_table[cmd_idx].effect_api->effective_arg_cnt-1)
                    {
                        printf(",");
                    }
                }
                printf(");\n");
            }
        }
        else
        {
            debug_print_warn("effect api is NULL, (0x%lx 0x%lx) is not a effective cmd!\n",block_id,cmd_idx);
        }
    }
    else
    {
        debug_print_info("(%s) = 0x%lx, no difference with default value!\n",
               settings_table[cmd_idx].serial_cmd_name,value);
    }
    return 0;
}
