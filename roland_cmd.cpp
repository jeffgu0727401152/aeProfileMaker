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

const double gHighPass_Freq_Table[70] =
{
	0.0,20.0,21.0,22.0,23.5,25.0,26.5,28.0,30.0,31.5,
	33.5,35.0,37.5,40.0,42.0,45.0,47.0,50.0,53.0,56.0,
	60.0,63.0,66.0,70.0,75.0,80.0,85.0,90.0,95.0,100.0,
	105.0,112.0,120.0,125.0,130.0,140.0,150.0,160.0,170.0,180.0,
	190.0,200.0,210.0,220.0,235.0,250.0,265.0,280.0,300.0,315.0,
	335.0,350.0,375.0,400.0,420.0,450.0,470.0,500.0,530.0,560.0,
	600.0,630.0,660.0,700.0,750.0,800.0,850.0,900.0,950.0,1000.0
};

const double gLowPass_Freq_Table[112] =
{
	35.0,37.5,40.0,42.0,45.0,47.0,50.0,53.0,56.0,60.0,
	63.0,66.0,70.0,75.0,80.0,85.0,90.0,95.0,100.0,105.0,
	112.0,120.0,125.0,130.0,140.0,150.0,160.0,170.0,180.0,190.0,
	200.0,210.0,220.0,235.0,250.0,265.0,280.0,300.0,315.0,335.0,
	350.0,375.0,400.0,420.0,450.0,470.0,500.0,530.0,560.0,600.0,
	630.0,660.0,700.0,750.0,800.0,850.0,900.0,950.0,1000.0,1050.0,
	1120.0,1200.0,1250.0,1300.0,1400.0,1500.0,1600.0,1700.0,1800.0,1900.0,
	2000.0,2100.0,2200.0,2350.0,2500.0,2650.0,2800.0,3000.0,3150.0,3350.0,
	3500.0,3750.0,4000.0,4200.0,4500.0,4700.0,5000.0,5300.0,5600.0,6000.0,
	6300.0,6600.0,7000.0,7500.0,8000.0,8500.0,9000.0,9500.0,10000.0,10500.0,
	11200.0,12000.0,12500.0,13000.0,14000.0,15000.0,16000.0,17000.0,18000.0,19000.0,
	20000.0,48000.0
};

const double gPEQ_Freq_Table[61] =
{
	20.0,22.0,25.0,28.0,32.0,36.0,40.0,45.0,50.0,56.0,
	63.0,71.0,80.0,90.0,100.0,112.0,125.0,140.0,160.0,180.0,
	200.0,224.0,250.0,280.0,315.0,355.0,400.0,450.0,500.0,560.0,
	630.0,710.0,800.0,900.0,1000.0,1120.0,1250.0,1400.0,1600.0,1800.0,
	2000.0,2240.0,2500.0,2800.0,3150.0,3550.0,4000.0,4500.0,5000.0,5600.0,
	6300.0,7100.0,8000.0,9000.0,10000.0,11200.0,12500.0,14000.0,16000.0,18000.0,20000.0
};

const double gPEQ_Gain_Table[121] =
{
	-12.0,-11.8,-11.6,-11.4,-11.2,
	-11.0,-10.8,-10.6,-10.4,-10.2,
	-10.0,-9.8,-9.6,-9.4,-9.2,
	-9.0,-8.8,-8.6,-8.4,-8.2,
	-8.0,-7.8,-7.6,-7.4,-7.2,
	-7.0,-6.8,-6.6,-6.4,-6.2,
	-6.0,-5.8,-5.6,-5.4,-5.2,
	-5.0,-4.8,-4.6,-4.4,-4.2,
	-4.0,-3.8,-3.6,-3.4,-3.2,
	-3.0,-2.8,-2.6,-2.4,-2.2,
	-2.0,-1.8,-1.6,-1.4,-1.2,
	-1.0,-0.8,-0.6,-0.4,-0.2,
	0.0,0.2,0.4,0.6,0.8,
	1.0,1.2,1.4,1.6,1.8,
	2.0,2.2,2.4,2.6,2.8,
	3.0,3.2,3.4,3.6,3.8,
	4.0,4.2,4.4,4.6,4.8,
	5.0,5.2,5.4,5.6,5.8,
	6.0,6.2,6.4,6.6,6.8,
	7.0,7.2,7.4,7.6,7.8,
	8.0,8.2,8.4,8.6,8.8,
	9.0,9.2,9.4,9.6,9.8,
	10.0,10.2,10.4,10.6,10.8,
	11.0,11.2,11.4,11.6,11.8,
	12.0
};

const double gPEQ_Q_Table[32] =
{
	0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,
	1.4,1.6,1.8,2.0,2.2,2.5,2.8,3.1,3.5,4.0,
	4.5,5.0,5.6,6.3,7.1,8.0,9.0,10.0,11.2,12.5,
	14.0,16.0
};


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
    mixer_settings[mic1_input_level].profile_xml_name = "Mic1Gain";
    mixer_settings[mic1_input_level].profile_value_offset = 0;
    mixer_settings[mic1_input_level].translate_table = NULL;
    mixer_settings[mic1_input_level].profile_xml_node = MICINPUT_NODE_NAME;

    mixer_settings[mic2_input_level].serial_cmd_name = "mic2_input_level";
    mixer_settings[mic2_input_level].effect_api = &(api_table[WSD_AL_AE_MixerMicLRLevelSetup]);
    mixer_settings[mic2_input_level].cmd_default_value = 0x40;
    mixer_settings[mic2_input_level].effective_arg_idx = 1;
    mixer_settings[mic2_input_level].profile_xml_name = "Mic2Gain";
    mixer_settings[mic2_input_level].profile_value_offset = 0;
    mixer_settings[mic2_input_level].translate_table = NULL;
    mixer_settings[mic2_input_level].profile_xml_node = MICINPUT_NODE_NAME;

    mixer_settings[mic_input_level].serial_cmd_name = "mic_input_level";
    mixer_settings[mic_input_level].effect_api = &(api_table[WSD_AL_AE_MixerMicInputLevelSetup]);
    mixer_settings[mic_input_level].cmd_default_value = 0x40;
    mixer_settings[mic_input_level].effective_arg_idx = 0;
    mixer_settings[mic_input_level].profile_xml_name = "mic_input_level";
    mixer_settings[mic_input_level].profile_value_offset = 0;
    mixer_settings[mic_input_level].translate_table = NULL;
    mixer_settings[mic_input_level].profile_xml_node = NULL;

    mixer_settings[mic_direct_level].serial_cmd_name = "mic_direct_level";
    mixer_settings[mic_direct_level].effect_api = &(api_table[WSD_AL_AE_DryOutputSetup]);
    mixer_settings[mic_direct_level].cmd_default_value = 0x7f;
    mixer_settings[mic_direct_level].effective_arg_idx = 0;
    mixer_settings[mic_direct_level].profile_xml_name = "mic_direct_level";
    mixer_settings[mic_direct_level].profile_value_offset = 0;
    mixer_settings[mic_direct_level].translate_table = NULL;
    mixer_settings[mic_direct_level].profile_xml_node = NULL;

    mixer_settings[front_mic_dry_level].serial_cmd_name = "front_mic_dry_level";
    mixer_settings[front_mic_dry_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_front]);
    mixer_settings[front_mic_dry_level].cmd_default_value = 0x7f;
    mixer_settings[front_mic_dry_level].effective_arg_idx = 0;
    mixer_settings[front_mic_dry_level].profile_xml_name = "MicGain";
    mixer_settings[front_mic_dry_level].profile_value_offset = 0;
    mixer_settings[front_mic_dry_level].translate_table = NULL;
    mixer_settings[front_mic_dry_level].profile_xml_node = FRONT_NODE_NAME;

    mixer_settings[music_input_level].serial_cmd_name = "music_input_level";
    mixer_settings[music_input_level].effect_api = &(api_table[WSD_AL_AE_MixerMusicInputLevelSetup]);
    mixer_settings[music_input_level].cmd_default_value = 0x40;
    mixer_settings[music_input_level].effective_arg_idx = 0;
    mixer_settings[music_input_level].profile_xml_name = "music_input_level";
    mixer_settings[music_input_level].profile_value_offset = 0;
    mixer_settings[music_input_level].translate_table = NULL;
    mixer_settings[music_input_level].profile_xml_node = NULL;

    mixer_settings[front_music_level].serial_cmd_name = "front_music_level";
    mixer_settings[front_music_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_front]);
    mixer_settings[front_music_level].cmd_default_value = 0x7f;
    mixer_settings[front_music_level].effective_arg_idx = 3;
    mixer_settings[front_music_level].profile_xml_name = "MusicGain";
    mixer_settings[front_music_level].profile_value_offset = 0;
    mixer_settings[front_music_level].translate_table = NULL;
    mixer_settings[front_music_level].profile_xml_node = FRONT_NODE_NAME;

    mixer_settings[rear_output_level].serial_cmd_name = "rear_output_level";
    mixer_settings[rear_output_level].effect_api = &(api_table[WSD_AL_AE_OutputSetup_rear]);
    mixer_settings[rear_output_level].cmd_default_value = 0x7f;
    mixer_settings[rear_output_level].effective_arg_idx = 0;
    mixer_settings[rear_output_level].profile_xml_name = "OutputGain";
    mixer_settings[rear_output_level].profile_value_offset = 0;
    mixer_settings[rear_output_level].translate_table = NULL;
    mixer_settings[rear_output_level].profile_xml_node = REAR_NODE_NAME;

    mixer_settings[front_output_level].serial_cmd_name = "front_output_level";
    mixer_settings[front_output_level].effect_api = &(api_table[WSD_AL_AE_OutputSetup_front]);
    mixer_settings[front_output_level].cmd_default_value = 0x7f;
    mixer_settings[front_output_level].effective_arg_idx = 0;
    mixer_settings[front_output_level].profile_xml_name = "OutputGain";
    mixer_settings[front_output_level].profile_value_offset = 0;
    mixer_settings[front_output_level].translate_table = NULL;
    mixer_settings[front_output_level].profile_xml_node = FRONT_NODE_NAME;

    mixer_settings[subwoofer_output_level].serial_cmd_name = "subwoofer_output_level";
    mixer_settings[subwoofer_output_level].effect_api = &(api_table[WSD_AL_AE_OutputSetup_subwoofer]);
    mixer_settings[subwoofer_output_level].cmd_default_value = 0x7f;
    mixer_settings[subwoofer_output_level].effective_arg_idx = 0;
    mixer_settings[subwoofer_output_level].profile_xml_name = "OutputGain";
    mixer_settings[subwoofer_output_level].profile_value_offset = 0;
    mixer_settings[subwoofer_output_level].translate_table = NULL;
    mixer_settings[subwoofer_output_level].profile_xml_node = SUBWOOFER_NODE_NAME;

    mixer_settings[aux_to_mic].serial_cmd_name = "aux_to_mic";
    mixer_settings[aux_to_mic].effect_api = &(api_table[WSD_AL_AE_MixerAuxLevelSetup]);
    mixer_settings[aux_to_mic].cmd_default_value = 0x00;
    mixer_settings[aux_to_mic].effective_arg_idx = 0;
    mixer_settings[aux_to_mic].profile_xml_name = "aux_to_mic";
    mixer_settings[aux_to_mic].profile_value_offset = 0;
    mixer_settings[aux_to_mic].translate_table = NULL;
    mixer_settings[aux_to_mic].profile_xml_node = NULL;

    mixer_settings[aux_to_music].serial_cmd_name = "aux_to_music";
    mixer_settings[aux_to_music].effect_api = &(api_table[WSD_AL_AE_MixerAuxLevelSetup]);
    mixer_settings[aux_to_music].cmd_default_value = 0x00;
    mixer_settings[aux_to_music].effective_arg_idx = 1;
    mixer_settings[aux_to_music].profile_xml_name = "aux_to_music";
    mixer_settings[aux_to_music].profile_value_offset = 0;
    mixer_settings[aux_to_music].translate_table = NULL;
    mixer_settings[aux_to_music].profile_xml_node = NULL;

    mixer_settings[center_output_level].serial_cmd_name = "center_output_level";
    mixer_settings[center_output_level].effect_api = &(api_table[WSD_AL_AE_OutputSetup_center]);
    mixer_settings[center_output_level].cmd_default_value = 0x7f;
    mixer_settings[center_output_level].effective_arg_idx = 0;
    mixer_settings[center_output_level].profile_xml_name = "OutputGain";
    mixer_settings[center_output_level].profile_value_offset = 0;
    mixer_settings[center_output_level].translate_table = NULL;
    mixer_settings[center_output_level].profile_xml_node = CENTER_NODE_NAME;

    mixer_settings[center_mic_level].serial_cmd_name = "center_mic_level";
    mixer_settings[center_mic_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_center]);
    mixer_settings[center_mic_level].cmd_default_value = 0x7f;
    mixer_settings[center_mic_level].effective_arg_idx = 0;
    mixer_settings[center_mic_level].profile_xml_name = "MicGain";
    mixer_settings[center_mic_level].profile_value_offset = 0;
    mixer_settings[center_mic_level].translate_table = NULL;
    mixer_settings[center_mic_level].profile_xml_node = CENTER_NODE_NAME;

    mixer_settings[center_music_level].serial_cmd_name = "center_music_level";
    mixer_settings[center_music_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_center]);
    mixer_settings[center_music_level].cmd_default_value = 0x00;
    mixer_settings[center_music_level].effective_arg_idx = 3;
    mixer_settings[center_music_level].profile_xml_name = "MusicGain";
    mixer_settings[center_music_level].profile_value_offset = 0;
    mixer_settings[center_music_level].translate_table = NULL;
    mixer_settings[center_music_level].profile_xml_node = CENTER_NODE_NAME;

    mixer_settings[front_mic_delay_level].serial_cmd_name = "front_mic_delay_level";
    mixer_settings[front_mic_delay_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_front]);
    mixer_settings[front_mic_delay_level].cmd_default_value = 0x7f;
    mixer_settings[front_mic_delay_level].effective_arg_idx = 1;
    mixer_settings[front_mic_delay_level].profile_xml_name = "EchoGain";
    mixer_settings[front_mic_delay_level].profile_value_offset = 0;
    mixer_settings[front_mic_delay_level].translate_table = NULL;
    mixer_settings[front_mic_delay_level].profile_xml_node = FRONT_NODE_NAME;

    mixer_settings[front_mic_reverb_level].serial_cmd_name = "front_mic_reverb_level";
    mixer_settings[front_mic_reverb_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_front]);
    mixer_settings[front_mic_reverb_level].cmd_default_value = 0x7f;
    mixer_settings[front_mic_reverb_level].effective_arg_idx = 2;
    mixer_settings[front_mic_reverb_level].profile_xml_name = "ReverbGain";
    mixer_settings[front_mic_reverb_level].profile_value_offset = 0;
    mixer_settings[front_mic_reverb_level].translate_table = NULL;
    mixer_settings[front_mic_reverb_level].profile_xml_node = FRONT_NODE_NAME;

    mixer_settings[subwoofer_mic_dry_level].serial_cmd_name = "subwoofer_mic_dry_level";
    mixer_settings[subwoofer_mic_dry_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_subwoofer]);
    mixer_settings[subwoofer_mic_dry_level].cmd_default_value = 0x7f;
    mixer_settings[subwoofer_mic_dry_level].effective_arg_idx = 0;
    mixer_settings[subwoofer_mic_dry_level].profile_xml_name = "MicGain";
    mixer_settings[subwoofer_mic_dry_level].profile_value_offset = 0;
    mixer_settings[subwoofer_mic_dry_level].translate_table = NULL;
    mixer_settings[subwoofer_mic_dry_level].profile_xml_node = SUBWOOFER_NODE_NAME;

    mixer_settings[subwoofer_music_level].serial_cmd_name = "subwoofer_music_level";
    mixer_settings[subwoofer_music_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_subwoofer]);
    mixer_settings[subwoofer_music_level].cmd_default_value = 0x7f;
    mixer_settings[subwoofer_music_level].effective_arg_idx = 3;
    mixer_settings[subwoofer_music_level].profile_xml_name = "MusicGain";
    mixer_settings[subwoofer_music_level].profile_value_offset = 0;
    mixer_settings[subwoofer_music_level].translate_table = NULL;
    mixer_settings[subwoofer_music_level].profile_xml_node = SUBWOOFER_NODE_NAME;

    mixer_settings[subwoofer_mic_delay_level].serial_cmd_name = "subwoofer_mic_delay_level";
    mixer_settings[subwoofer_mic_delay_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_subwoofer]);
    mixer_settings[subwoofer_mic_delay_level].cmd_default_value = 0x7f;
    mixer_settings[subwoofer_mic_delay_level].effective_arg_idx = 1;
    mixer_settings[subwoofer_mic_delay_level].profile_xml_name = "EchoGain";
    mixer_settings[subwoofer_mic_delay_level].profile_value_offset = 0;
    mixer_settings[subwoofer_mic_delay_level].translate_table = NULL;
    mixer_settings[subwoofer_mic_delay_level].profile_xml_node = SUBWOOFER_NODE_NAME;

    mixer_settings[subwoofer_mic_reverb_level].serial_cmd_name = "subwoofer_mic_reverb_level";
    mixer_settings[subwoofer_mic_reverb_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_subwoofer]);
    mixer_settings[subwoofer_mic_reverb_level].cmd_default_value = 0x7f;
    mixer_settings[subwoofer_mic_reverb_level].effective_arg_idx = 2;
    mixer_settings[subwoofer_mic_reverb_level].profile_xml_name = "ReverbGain";
    mixer_settings[subwoofer_mic_reverb_level].profile_value_offset = 0;
    mixer_settings[subwoofer_mic_reverb_level].translate_table = NULL;
    mixer_settings[subwoofer_mic_reverb_level].profile_xml_node = SUBWOOFER_NODE_NAME;

    mixer_settings[rear_mic_dry_level].serial_cmd_name = "rear_mic_dry_level";
    mixer_settings[rear_mic_dry_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_rear]);
    mixer_settings[rear_mic_dry_level].cmd_default_value = 0x7f;
    mixer_settings[rear_mic_dry_level].effective_arg_idx = 0;
    mixer_settings[rear_mic_dry_level].profile_xml_name = "MicGain";
    mixer_settings[rear_mic_dry_level].profile_value_offset = 0;
    mixer_settings[rear_mic_dry_level].translate_table = NULL;
    mixer_settings[rear_mic_dry_level].profile_xml_node = REAR_NODE_NAME;

    mixer_settings[rear_music_level].serial_cmd_name = "rear_music_level";
    mixer_settings[rear_music_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_rear]);
    mixer_settings[rear_music_level].cmd_default_value = 0x7f;
    mixer_settings[rear_music_level].effective_arg_idx = 3;
    mixer_settings[rear_music_level].profile_xml_name = "MusicGain";
    mixer_settings[rear_music_level].profile_value_offset = 0;
    mixer_settings[rear_music_level].translate_table = NULL;
    mixer_settings[rear_music_level].profile_xml_node = REAR_NODE_NAME;

    mixer_settings[rear_mic_delay_level].serial_cmd_name = "rear_mic_delay_level";
    mixer_settings[rear_mic_delay_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_rear]);
    mixer_settings[rear_mic_delay_level].cmd_default_value = 0x7f;
    mixer_settings[rear_mic_delay_level].effective_arg_idx = 1;
    mixer_settings[rear_mic_delay_level].profile_xml_name = "EchoGain";
    mixer_settings[rear_mic_delay_level].profile_value_offset = 0;
    mixer_settings[rear_mic_delay_level].translate_table = NULL;
    mixer_settings[rear_mic_delay_level].profile_xml_node = REAR_NODE_NAME;

    mixer_settings[rear_mic_reverb_level].serial_cmd_name = "rear_mic_reverb_level";
    mixer_settings[rear_mic_reverb_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_rear]);
    mixer_settings[rear_mic_reverb_level].cmd_default_value = 0x7f;
    mixer_settings[rear_mic_reverb_level].effective_arg_idx = 2;
    mixer_settings[rear_mic_reverb_level].profile_xml_name = "ReverbGain";
    mixer_settings[rear_mic_reverb_level].profile_value_offset = 0;
    mixer_settings[rear_mic_reverb_level].translate_table = NULL;
    mixer_settings[rear_mic_reverb_level].profile_xml_node = REAR_NODE_NAME;

    mixer_settings[center_mic_delay_level].serial_cmd_name = "center_mic_delay_level";
    mixer_settings[center_mic_delay_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_center]);
    mixer_settings[center_mic_delay_level].cmd_default_value = 0x7f;
    mixer_settings[center_mic_delay_level].effective_arg_idx = 1;
    mixer_settings[center_mic_delay_level].profile_xml_name = "EchoGain";
    mixer_settings[center_mic_delay_level].profile_value_offset = 0;
    mixer_settings[center_mic_delay_level].translate_table = NULL;
    mixer_settings[center_mic_delay_level].profile_xml_node = CENTER_NODE_NAME;

    mixer_settings[center_mic_reverb_level].serial_cmd_name = "center_mic_reverb_level";
    mixer_settings[center_mic_reverb_level].effect_api = &(api_table[WSD_AL_AE_MixerMixSetup_center]);
    mixer_settings[center_mic_reverb_level].cmd_default_value = 0x7f;
    mixer_settings[center_mic_reverb_level].effective_arg_idx = 2;
    mixer_settings[center_mic_reverb_level].profile_xml_name = "ReverbGain";
    mixer_settings[center_mic_reverb_level].profile_value_offset = 0;
    mixer_settings[center_mic_reverb_level].translate_table = NULL;
    mixer_settings[center_mic_reverb_level].profile_xml_node = CENTER_NODE_NAME;

    mixer_settings[center_subwoofer_switch_stereo].serial_cmd_name = "center_subwoofer_switch_stereo";
    mixer_settings[center_subwoofer_switch_stereo].effect_api = &(api_table[WSD_AL_AE_MixerCenterSubwooferSwitchStereo]);
    mixer_settings[center_subwoofer_switch_stereo].cmd_default_value = 0x0;
    mixer_settings[center_subwoofer_switch_stereo].effective_arg_idx = 0;
    mixer_settings[center_subwoofer_switch_stereo].profile_xml_name = "center_subwoofer_switch_stereo";
    mixer_settings[center_subwoofer_switch_stereo].profile_value_offset = 0;
    mixer_settings[center_subwoofer_switch_stereo].translate_table = NULL;
    mixer_settings[center_subwoofer_switch_stereo].profile_xml_node = NULL;
}

static void mic_effects_settings_default()
{
    memset(mic_effects_settings,0,sizeof(DSP_SETTING_T)*mic_effects_end);

    mic_effects_settings[bass_cut_switch].serial_cmd_name = "bass_cut_switch";
    mic_effects_settings[bass_cut_switch].effect_api = &(api_table[WSD_AL_AE_MicBasscutEnable]);
    mic_effects_settings[bass_cut_switch].cmd_default_value = 0x00;
    mic_effects_settings[bass_cut_switch].effective_arg_idx = 0;
    mic_effects_settings[bass_cut_switch].profile_xml_name = "BasscutEnable";
    mic_effects_settings[bass_cut_switch].profile_value_offset = 0;
    mic_effects_settings[bass_cut_switch].translate_table = NULL;
    mic_effects_settings[bass_cut_switch].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[bass_cut_frequency].serial_cmd_name = "bass_cut_frequency";
    mic_effects_settings[bass_cut_frequency].effect_api = &(api_table[WSD_AL_AE_MicBasscutFrequencySetup]);
    mic_effects_settings[bass_cut_frequency].cmd_default_value = 0x01;
    mic_effects_settings[bass_cut_frequency].effective_arg_idx = 0;
    mic_effects_settings[bass_cut_frequency].profile_xml_name = "Basscut";
    mic_effects_settings[bass_cut_frequency].profile_value_offset = -1;
    mic_effects_settings[bass_cut_frequency].translate_table = NULL;
    mic_effects_settings[bass_cut_frequency].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[noise_supp_switch].serial_cmd_name = "noise_supp_switch";
    mic_effects_settings[noise_supp_switch].effect_api = &(api_table[WSD_AL_AE_MicNoiseSuppEnable]);
    mic_effects_settings[noise_supp_switch].cmd_default_value = 0x00;
    mic_effects_settings[noise_supp_switch].effective_arg_idx = 0;
    mic_effects_settings[noise_supp_switch].profile_xml_name = "NoiseThresholdEnable";
    mic_effects_settings[noise_supp_switch].profile_value_offset = 0;
    mic_effects_settings[noise_supp_switch].translate_table = NULL;
    mic_effects_settings[noise_supp_switch].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[noise_supp_threshold].serial_cmd_name = "noise_supp_threshold";
    mic_effects_settings[noise_supp_threshold].effect_api = &(api_table[WSD_AL_AE_MicNoiseThresholdSetup]);
    mic_effects_settings[noise_supp_threshold].cmd_default_value = 0x03;
    mic_effects_settings[noise_supp_threshold].effective_arg_idx = 0;
    mic_effects_settings[noise_supp_threshold].profile_xml_name = "NoiseThreshold";
    mic_effects_settings[noise_supp_threshold].profile_value_offset = 0;
    mic_effects_settings[noise_supp_threshold].translate_table = NULL;
    mic_effects_settings[noise_supp_threshold].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[compressor_switch].serial_cmd_name = "compressor_switch";
    mic_effects_settings[compressor_switch].effect_api = &(api_table[WSD_AL_AE_MicCompressorEnable]);
    mic_effects_settings[compressor_switch].cmd_default_value = 0x00;
    mic_effects_settings[compressor_switch].effective_arg_idx = 0;
    mic_effects_settings[compressor_switch].profile_xml_name = "CompressorEnable";
    mic_effects_settings[compressor_switch].profile_value_offset = 0;
    mic_effects_settings[compressor_switch].translate_table = NULL;
    mic_effects_settings[compressor_switch].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[compressor_sustain].serial_cmd_name = "compressor_sustain";
    mic_effects_settings[compressor_sustain].effect_api = &(api_table[WSD_AL_AE_MicCompressorSetup]);
    mic_effects_settings[compressor_sustain].cmd_default_value = 0x07;
    mic_effects_settings[compressor_sustain].effective_arg_idx = 0;
    mic_effects_settings[compressor_sustain].profile_xml_name = "CompressorSustain";
    mic_effects_settings[compressor_sustain].profile_value_offset = 0;
    mic_effects_settings[compressor_sustain].translate_table = NULL;
    mic_effects_settings[compressor_sustain].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[compressor_out_level].serial_cmd_name = "compressor_out_level";
    mic_effects_settings[compressor_out_level].effect_api = &(api_table[WSD_AL_AE_MicCompressorSetup]);
    mic_effects_settings[compressor_out_level].cmd_default_value = 0x20;
    mic_effects_settings[compressor_out_level].effective_arg_idx = 1;
    mic_effects_settings[compressor_out_level].profile_xml_name = "CompressorLevel";
    mic_effects_settings[compressor_out_level].profile_value_offset = 0;
    mic_effects_settings[compressor_out_level].translate_table = NULL;
    mic_effects_settings[compressor_out_level].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[frequency_shift_value].serial_cmd_name = "frequency_shift_value";
    mic_effects_settings[frequency_shift_value].effect_api = &(api_table[WSD_AL_AE_MicFrequencyShiftSetup]);
    mic_effects_settings[frequency_shift_value].cmd_default_value = 0x40;
    mic_effects_settings[frequency_shift_value].effective_arg_idx = 0;
    mic_effects_settings[frequency_shift_value].profile_xml_name = "FreqShift";
    mic_effects_settings[frequency_shift_value].profile_value_offset = -54;
    mic_effects_settings[frequency_shift_value].translate_table = NULL;
    mic_effects_settings[frequency_shift_value].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_switch].serial_cmd_name = "peq_switch";
    mic_effects_settings[peq_switch].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqEnable]);
    mic_effects_settings[peq_switch].cmd_default_value = 0x00;
    mic_effects_settings[peq_switch].effective_arg_idx = 0;
    mic_effects_settings[peq_switch].profile_xml_name = "EQEnable";
    mic_effects_settings[peq_switch].profile_value_offset = 0;
    mic_effects_settings[peq_switch].translate_table = NULL;
    mic_effects_settings[peq_switch].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band1_type].serial_cmd_name = "peq_band1_type";
    mic_effects_settings[peq_band1_type].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqEdgeSetup]);
    mic_effects_settings[peq_band1_type].cmd_default_value = 0x00;
    mic_effects_settings[peq_band1_type].effective_arg_idx = 0;
    mic_effects_settings[peq_band1_type].profile_xml_name = "PEQLeftEdge";
    mic_effects_settings[peq_band1_type].profile_value_offset = 0;
    mic_effects_settings[peq_band1_type].translate_table = NULL;
    mic_effects_settings[peq_band1_type].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band1_gain].serial_cmd_name = "peq_band1_gain";
    mic_effects_settings[peq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band1]);
    mic_effects_settings[peq_band1_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band1_gain].effective_arg_idx = 2;
    mic_effects_settings[peq_band1_gain].profile_xml_name = "EQData1_Gain";
    mic_effects_settings[peq_band1_gain].profile_value_offset = -4;
    mic_effects_settings[peq_band1_gain].translate_table = gPEQ_Gain_Table;
    mic_effects_settings[peq_band1_gain].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band1_frequency].serial_cmd_name = "peq_band1_frequency";
    mic_effects_settings[peq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band1]);
    mic_effects_settings[peq_band1_frequency].cmd_default_value = 0x0c;
    mic_effects_settings[peq_band1_frequency].effective_arg_idx = 0;
    mic_effects_settings[peq_band1_frequency].profile_xml_name = "EQData1_Freq";
    mic_effects_settings[peq_band1_frequency].profile_value_offset = -2;
    mic_effects_settings[peq_band1_frequency].translate_table = gPEQ_Freq_Table;
    mic_effects_settings[peq_band1_frequency].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band1_q].serial_cmd_name = "peq_band1_q";
    mic_effects_settings[peq_band1_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band1]);
    mic_effects_settings[peq_band1_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band1_q].effective_arg_idx = 1;
    mic_effects_settings[peq_band1_q].profile_xml_name = "EQData1_Q";
    mic_effects_settings[peq_band1_q].profile_value_offset = 0;
    mic_effects_settings[peq_band1_q].translate_table = gPEQ_Q_Table;
    mic_effects_settings[peq_band1_q].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band2_gain].serial_cmd_name = "peq_band2_gain";
    mic_effects_settings[peq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band2]);
    mic_effects_settings[peq_band2_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band2_gain].effective_arg_idx = 2;
    mic_effects_settings[peq_band2_gain].profile_xml_name = "EQData2_Gain";
    mic_effects_settings[peq_band2_gain].profile_value_offset = -4;
    mic_effects_settings[peq_band2_gain].translate_table = gPEQ_Gain_Table;
    mic_effects_settings[peq_band2_gain].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band2_frequency].serial_cmd_name = "peq_band2_frequency";
    mic_effects_settings[peq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band2]);
    mic_effects_settings[peq_band2_frequency].cmd_default_value = 0x12;
    mic_effects_settings[peq_band2_frequency].effective_arg_idx = 0;
    mic_effects_settings[peq_band2_frequency].profile_xml_name = "EQData2_Freq";
    mic_effects_settings[peq_band2_frequency].profile_value_offset = -2;
    mic_effects_settings[peq_band2_frequency].translate_table = gPEQ_Freq_Table;
    mic_effects_settings[peq_band2_frequency].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band2_q].serial_cmd_name = "peq_band2_q";
    mic_effects_settings[peq_band2_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band2]);
    mic_effects_settings[peq_band2_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band2_q].effective_arg_idx = 1;
    mic_effects_settings[peq_band2_q].profile_xml_name = "EQData2_Q";
    mic_effects_settings[peq_band2_q].profile_value_offset = 0;
    mic_effects_settings[peq_band2_q].translate_table = gPEQ_Q_Table;
    mic_effects_settings[peq_band2_q].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band3_gain].serial_cmd_name = "peq_band3_gain";
    mic_effects_settings[peq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band3]);
    mic_effects_settings[peq_band3_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band3_gain].effective_arg_idx = 2;
    mic_effects_settings[peq_band3_gain].profile_xml_name = "EQData3_Gain";
    mic_effects_settings[peq_band3_gain].profile_value_offset = -4;
    mic_effects_settings[peq_band3_gain].translate_table = gPEQ_Gain_Table;
    mic_effects_settings[peq_band3_gain].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band3_frequency].serial_cmd_name = "peq_band3_frequency";
    mic_effects_settings[peq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band3]);
    mic_effects_settings[peq_band3_frequency].cmd_default_value = 0x18;
    mic_effects_settings[peq_band3_frequency].effective_arg_idx = 0;
    mic_effects_settings[peq_band3_frequency].profile_xml_name = "EQData3_Freq";
    mic_effects_settings[peq_band3_frequency].profile_value_offset = -2;
    mic_effects_settings[peq_band3_frequency].translate_table = gPEQ_Freq_Table;
    mic_effects_settings[peq_band3_frequency].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band3_q].serial_cmd_name = "peq_band3_q";
    mic_effects_settings[peq_band3_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band3]);
    mic_effects_settings[peq_band3_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band3_q].effective_arg_idx = 1;
    mic_effects_settings[peq_band3_q].profile_xml_name = "EQData3_Q";
    mic_effects_settings[peq_band3_q].profile_value_offset = 0;
    mic_effects_settings[peq_band3_q].translate_table = gPEQ_Q_Table;
    mic_effects_settings[peq_band3_q].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band4_gain].serial_cmd_name = "peq_band4_gain";
    mic_effects_settings[peq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band4]);
    mic_effects_settings[peq_band4_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band4_gain].effective_arg_idx = 2;
    mic_effects_settings[peq_band4_gain].profile_xml_name = "EQData4_Gain";
    mic_effects_settings[peq_band4_gain].profile_value_offset = -4;
    mic_effects_settings[peq_band4_gain].translate_table = gPEQ_Gain_Table;
    mic_effects_settings[peq_band4_gain].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band4_frequency].serial_cmd_name = "peq_band4_frequency";
    mic_effects_settings[peq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band4]);
    mic_effects_settings[peq_band4_frequency].cmd_default_value = 0x1e;
    mic_effects_settings[peq_band4_frequency].effective_arg_idx = 0;
    mic_effects_settings[peq_band4_frequency].profile_xml_name = "EQData4_Freq";
    mic_effects_settings[peq_band4_frequency].profile_value_offset = -2;
    mic_effects_settings[peq_band4_frequency].translate_table = gPEQ_Freq_Table;
    mic_effects_settings[peq_band4_frequency].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band4_q].serial_cmd_name = "peq_band4_q";
    mic_effects_settings[peq_band4_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band4]);
    mic_effects_settings[peq_band4_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band4_q].effective_arg_idx = 1;
    mic_effects_settings[peq_band4_q].profile_xml_name = "EQData4_Q";
    mic_effects_settings[peq_band4_q].profile_value_offset = 0;
    mic_effects_settings[peq_band4_q].translate_table = gPEQ_Q_Table;
    mic_effects_settings[peq_band4_q].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band5_gain].serial_cmd_name = "peq_band5_gain";
    mic_effects_settings[peq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band5]);
    mic_effects_settings[peq_band5_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band5_gain].effective_arg_idx = 2;
    mic_effects_settings[peq_band5_gain].profile_xml_name = "EQData5_Gain";
    mic_effects_settings[peq_band5_gain].profile_value_offset = -4;
    mic_effects_settings[peq_band5_gain].translate_table = gPEQ_Gain_Table;
    mic_effects_settings[peq_band5_gain].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band5_frequency].serial_cmd_name = "peq_band5_frequency";
    mic_effects_settings[peq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band5]);
    mic_effects_settings[peq_band5_frequency].cmd_default_value = 0x24;
    mic_effects_settings[peq_band5_frequency].effective_arg_idx = 0;
    mic_effects_settings[peq_band5_frequency].profile_xml_name = "EQData5_Freq";
    mic_effects_settings[peq_band5_frequency].profile_value_offset = -2;
    mic_effects_settings[peq_band5_frequency].translate_table = gPEQ_Freq_Table;
    mic_effects_settings[peq_band5_frequency].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band5_q].serial_cmd_name = "peq_band5_q";
    mic_effects_settings[peq_band5_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band5]);
    mic_effects_settings[peq_band5_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band5_q].effective_arg_idx = 1;
    mic_effects_settings[peq_band5_q].profile_xml_name = "EQData5_Q";
    mic_effects_settings[peq_band5_q].profile_value_offset = 0;
    mic_effects_settings[peq_band5_q].translate_table = gPEQ_Q_Table;
    mic_effects_settings[peq_band5_q].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band6_gain].serial_cmd_name = "peq_band6_gain";
    mic_effects_settings[peq_band6_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band6]);
    mic_effects_settings[peq_band6_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band6_gain].effective_arg_idx = 2;
    mic_effects_settings[peq_band6_gain].profile_xml_name = "EQData6_Gain";
    mic_effects_settings[peq_band6_gain].profile_value_offset = -4;
    mic_effects_settings[peq_band6_gain].translate_table = gPEQ_Gain_Table;
    mic_effects_settings[peq_band6_gain].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band6_frequency].serial_cmd_name = "peq_band6_frequency";
    mic_effects_settings[peq_band6_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band6]);
    mic_effects_settings[peq_band6_frequency].cmd_default_value = 0x2a;
    mic_effects_settings[peq_band6_frequency].effective_arg_idx = 0;
    mic_effects_settings[peq_band6_frequency].profile_xml_name = "EQData6_Freq";
    mic_effects_settings[peq_band6_frequency].profile_value_offset = -2;
    mic_effects_settings[peq_band6_frequency].translate_table = gPEQ_Freq_Table;
    mic_effects_settings[peq_band6_frequency].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band6_q].serial_cmd_name = "peq_band6_q";
    mic_effects_settings[peq_band6_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band6]);
    mic_effects_settings[peq_band6_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band6_q].effective_arg_idx = 1;
    mic_effects_settings[peq_band6_q].profile_xml_name = "EQData6_Q";
    mic_effects_settings[peq_band6_q].profile_value_offset = 0;
    mic_effects_settings[peq_band6_q].translate_table = gPEQ_Q_Table;
    mic_effects_settings[peq_band6_q].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band7_type].serial_cmd_name = "peq_band7_type";
    mic_effects_settings[peq_band7_type].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqEdgeSetup]);
    mic_effects_settings[peq_band7_type].cmd_default_value = 0x00;
    mic_effects_settings[peq_band7_type].effective_arg_idx = 1;
    mic_effects_settings[peq_band7_type].profile_xml_name = "PEQRightEdge";
    mic_effects_settings[peq_band7_type].profile_value_offset = 0;
    mic_effects_settings[peq_band7_type].translate_table = NULL;
    mic_effects_settings[peq_band7_type].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band7_gain].serial_cmd_name = "peq_band7_gain";
    mic_effects_settings[peq_band7_gain].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band7]);
    mic_effects_settings[peq_band7_gain].cmd_default_value = 0x40;
    mic_effects_settings[peq_band7_gain].effective_arg_idx = 2;
    mic_effects_settings[peq_band7_gain].profile_xml_name = "EQData7_Gain";
    mic_effects_settings[peq_band7_gain].profile_value_offset = -4;
    mic_effects_settings[peq_band7_gain].translate_table = gPEQ_Gain_Table;
    mic_effects_settings[peq_band7_gain].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band7_frequency].serial_cmd_name = "peq_band7_frequency";
    mic_effects_settings[peq_band7_frequency].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band7]);
    mic_effects_settings[peq_band7_frequency].cmd_default_value = 0x30;
    mic_effects_settings[peq_band7_frequency].effective_arg_idx = 0;
    mic_effects_settings[peq_band7_frequency].profile_xml_name = "EQData7_Freq";
    mic_effects_settings[peq_band7_frequency].profile_value_offset = -2;
    mic_effects_settings[peq_band7_frequency].translate_table = gPEQ_Freq_Table;
    mic_effects_settings[peq_band7_frequency].profile_xml_node = MICINPUT_NODE_NAME;

    mic_effects_settings[peq_band7_q].serial_cmd_name = "peq_band7_q";
    mic_effects_settings[peq_band7_q].effect_api = &(api_table[WSD_AL_AE_Mic7bandPeqSetup_band7]);
    mic_effects_settings[peq_band7_q].cmd_default_value = 0x07;
    mic_effects_settings[peq_band7_q].effective_arg_idx = 1;
    mic_effects_settings[peq_band7_q].profile_xml_name = "EQData7_Q";
    mic_effects_settings[peq_band7_q].profile_value_offset = 0;
    mic_effects_settings[peq_band7_q].translate_table = gPEQ_Q_Table;
    mic_effects_settings[peq_band7_q].profile_xml_node = MICINPUT_NODE_NAME;
}

static void delay_settings_default()
{
    memset(delay_settings,0,sizeof(DSP_SETTING_T)*delay_end);

    delay_settings[delay_input_level].serial_cmd_name = "delay_input_level";
    delay_settings[delay_input_level].effect_api = &(api_table[WSD_AL_AE_DelayInputLevelSetup]);
    delay_settings[delay_input_level].cmd_default_value = 0x00;
    delay_settings[delay_input_level].effective_arg_idx = 0;
    delay_settings[delay_input_level].profile_xml_name = "InputGain";
    delay_settings[delay_input_level].profile_value_offset = 0;
    delay_settings[delay_input_level].translate_table = NULL;
    delay_settings[delay_input_level].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_tone_gain].serial_cmd_name = "delay_tone_gain";
    delay_settings[delay_tone_gain].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_tone_gain].cmd_default_value = 0x40;
    delay_settings[delay_tone_gain].effective_arg_idx = 3;
    delay_settings[delay_tone_gain].profile_xml_name = "ToneLevel";
    delay_settings[delay_tone_gain].profile_value_offset = -57;
    delay_settings[delay_tone_gain].translate_table = NULL;
    delay_settings[delay_tone_gain].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_time].serial_cmd_name = "delay_time";
    delay_settings[delay_time].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_time].cmd_default_value = 0x14;
    delay_settings[delay_time].effective_arg_idx = 0;
    delay_settings[delay_time].profile_xml_name = "EchoTime";
    delay_settings[delay_time].profile_value_offset = 0;
    delay_settings[delay_time].translate_table = NULL;
    delay_settings[delay_time].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_feedback_level].serial_cmd_name = "delay_feedback_level";
    delay_settings[delay_feedback_level].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_feedback_level].cmd_default_value = 0x32;
    delay_settings[delay_feedback_level].effective_arg_idx = 4;
    delay_settings[delay_feedback_level].profile_xml_name = "FeedbackLevel";
    delay_settings[delay_feedback_level].profile_value_offset = 0;
    delay_settings[delay_feedback_level].translate_table = NULL;
    delay_settings[delay_feedback_level].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_pan].serial_cmd_name = "delay_pan";
    delay_settings[delay_pan].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_pan].cmd_default_value = 0x40;
    delay_settings[delay_pan].effective_arg_idx = 2;
    delay_settings[delay_pan].profile_xml_name = "EchoPan";
    delay_settings[delay_pan].profile_value_offset = 0;
    delay_settings[delay_pan].translate_table = NULL;
    delay_settings[delay_pan].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_level].serial_cmd_name = "delay_level";
    delay_settings[delay_level].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_level].cmd_default_value = 0x7f;
    delay_settings[delay_level].effective_arg_idx = 1;
    delay_settings[delay_level].profile_xml_name = "EchoLevel";
    delay_settings[delay_level].profile_value_offset = 0;
    delay_settings[delay_level].translate_table = NULL;
    delay_settings[delay_level].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_tap1_level].serial_cmd_name = "delay_tap1_level";
    delay_settings[delay_tap1_level].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap1]);
    delay_settings[delay_tap1_level].cmd_default_value = 0x0;
    delay_settings[delay_tap1_level].effective_arg_idx = 1;
    delay_settings[delay_tap1_level].profile_xml_name = "Tap1Level";
    delay_settings[delay_tap1_level].profile_value_offset = 0;
    delay_settings[delay_tap1_level].translate_table = NULL;
    delay_settings[delay_tap1_level].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_tap2_level].serial_cmd_name = "delay_tap2_level";
    delay_settings[delay_tap2_level].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap2]);
    delay_settings[delay_tap2_level].cmd_default_value = 0x0;
    delay_settings[delay_tap2_level].effective_arg_idx = 1;
    delay_settings[delay_tap2_level].profile_xml_name = "Tap2Level";
    delay_settings[delay_tap2_level].profile_value_offset = 0;
    delay_settings[delay_tap2_level].translate_table = NULL;
    delay_settings[delay_tap2_level].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_tap3_level].serial_cmd_name = "delay_tap3_level";
    delay_settings[delay_tap3_level].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap3]);
    delay_settings[delay_tap3_level].cmd_default_value = 0x0;
    delay_settings[delay_tap3_level].effective_arg_idx = 1;
    delay_settings[delay_tap3_level].profile_xml_name = "Tap3Level";
    delay_settings[delay_tap3_level].profile_value_offset = 0;
    delay_settings[delay_tap3_level].translate_table = NULL;
    delay_settings[delay_tap3_level].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_tap1_time].serial_cmd_name = "delay_tap1_time";
    delay_settings[delay_tap1_time].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap1]);
    delay_settings[delay_tap1_time].cmd_default_value = 0x3f;
    delay_settings[delay_tap1_time].effective_arg_idx = 0;
    delay_settings[delay_tap1_time].profile_xml_name = "Tap1Time";
    delay_settings[delay_tap1_time].profile_value_offset = 0;
    delay_settings[delay_tap1_time].translate_table = NULL;
    delay_settings[delay_tap1_time].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_tap2_time].serial_cmd_name = "delay_tap2_time";
    delay_settings[delay_tap2_time].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap2]);
    delay_settings[delay_tap2_time].cmd_default_value = 0x3f;
    delay_settings[delay_tap2_time].effective_arg_idx = 0;
    delay_settings[delay_tap2_time].profile_xml_name = "Tap2Time";
    delay_settings[delay_tap2_time].profile_value_offset = 0;
    delay_settings[delay_tap2_time].translate_table = NULL;
    delay_settings[delay_tap2_time].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_tap3_time].serial_cmd_name = "delay_tap3_time";
    delay_settings[delay_tap3_time].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap3]);
    delay_settings[delay_tap3_time].cmd_default_value = 0x3f;
    delay_settings[delay_tap3_time].effective_arg_idx = 0;
    delay_settings[delay_tap3_time].profile_xml_name = "Tap3Time";
    delay_settings[delay_tap3_time].profile_value_offset = 0;
    delay_settings[delay_tap3_time].translate_table = NULL;
    delay_settings[delay_tap3_time].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_tap1_pan].serial_cmd_name = "delay_tap1_pan";
    delay_settings[delay_tap1_pan].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap1]);
    delay_settings[delay_tap1_pan].cmd_default_value = 0x40;
    delay_settings[delay_tap1_pan].effective_arg_idx = 2;
    delay_settings[delay_tap1_pan].profile_xml_name = "Tap1Pan";
    delay_settings[delay_tap1_pan].profile_value_offset = 0;
    delay_settings[delay_tap1_pan].translate_table = NULL;
    delay_settings[delay_tap1_pan].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_tap2_pan].serial_cmd_name = "delay_tap2_pan";
    delay_settings[delay_tap2_pan].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap2]);
    delay_settings[delay_tap2_pan].cmd_default_value = 0x40;
    delay_settings[delay_tap2_pan].effective_arg_idx = 2;
    delay_settings[delay_tap2_pan].profile_xml_name = "Tap2Pan";
    delay_settings[delay_tap2_pan].profile_value_offset = 0;
    delay_settings[delay_tap2_pan].translate_table = NULL;
    delay_settings[delay_tap2_pan].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_tap3_pan].serial_cmd_name = "delay_tap3_pan";
    delay_settings[delay_tap3_pan].effect_api = &(api_table[WSD_AL_AE_DelayTapEffectsSetup_tap3]);
    delay_settings[delay_tap3_pan].cmd_default_value = 0x40;
    delay_settings[delay_tap3_pan].effective_arg_idx = 2;
    delay_settings[delay_tap3_pan].profile_xml_name = "Tap3Pan";
    delay_settings[delay_tap3_pan].profile_value_offset = 0;
    delay_settings[delay_tap3_pan].translate_table = NULL;
    delay_settings[delay_tap3_pan].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_master_level].serial_cmd_name = "delay_master_level";
    delay_settings[delay_master_level].effect_api = &(api_table[WSD_AL_AE_DelayOutputSetup]);
    delay_settings[delay_master_level].cmd_default_value = 0x40;
    delay_settings[delay_master_level].effective_arg_idx = 0;
    delay_settings[delay_master_level].profile_xml_name = "OutputGain";
    delay_settings[delay_master_level].profile_value_offset = 0;
    delay_settings[delay_master_level].translate_table = NULL;
    delay_settings[delay_master_level].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_to_reverb_level].serial_cmd_name = "delay_to_reverb_level";
    delay_settings[delay_to_reverb_level].effect_api = &(api_table[WSD_AL_AE_DelayToReverbLevelSetup]);
    delay_settings[delay_to_reverb_level].cmd_default_value = 0x00;
    delay_settings[delay_to_reverb_level].effective_arg_idx = 0;
    delay_settings[delay_to_reverb_level].profile_xml_name = "EchoToReverb";
    delay_settings[delay_to_reverb_level].profile_value_offset = 0;
    delay_settings[delay_to_reverb_level].translate_table = NULL;
    delay_settings[delay_to_reverb_level].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[delay_pre_delay_time].serial_cmd_name = "delay_pre_delay_time";
    delay_settings[delay_pre_delay_time].effect_api = &(api_table[WSD_AL_AE_DelayMasterEffectSetup]);
    delay_settings[delay_pre_delay_time].cmd_default_value = 0x00;
    delay_settings[delay_pre_delay_time].effective_arg_idx = 5;
    delay_settings[delay_pre_delay_time].profile_xml_name = "Predelay";
    delay_settings[delay_pre_delay_time].profile_value_offset = 0;
    delay_settings[delay_pre_delay_time].translate_table = NULL;
    delay_settings[delay_pre_delay_time].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_phase].serial_cmd_name = "delay_phase";
    delay_settings[delay_phase].effect_api = &(api_table[WSD_AL_AE_DelayOutputSetup]);
    delay_settings[delay_phase].cmd_default_value = 0x00;
    delay_settings[delay_phase].effective_arg_idx = 1;
    delay_settings[delay_phase].profile_xml_name = "PhaseInvert";
    delay_settings[delay_phase].profile_value_offset = 0;
    delay_settings[delay_phase].translate_table = NULL;
    delay_settings[delay_phase].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[reverb_input_level].serial_cmd_name = "reverb_input_level";
    delay_settings[reverb_input_level].effect_api = &(api_table[WSD_AL_AE_ReverbInputLevelSetup]);
    delay_settings[reverb_input_level].cmd_default_value = 0x00;
    delay_settings[reverb_input_level].effective_arg_idx = 0;
    delay_settings[reverb_input_level].profile_xml_name = "InputGain";
    delay_settings[reverb_input_level].profile_value_offset = 0;
    delay_settings[reverb_input_level].translate_table = NULL;
    delay_settings[reverb_input_level].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_type].serial_cmd_name = "reverb_type";
    delay_settings[reverb_type].effect_api = &(api_table[WSD_AL_AE_ReverbEffectsSetup]);
    delay_settings[reverb_type].cmd_default_value = 0x02;
    delay_settings[reverb_type].effective_arg_idx = 0;
    delay_settings[reverb_type].profile_xml_name = "ReverbType";
    delay_settings[reverb_type].profile_value_offset = 0;
    delay_settings[reverb_type].translate_table = NULL;
    delay_settings[reverb_type].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_tone_gain].serial_cmd_name = "reverb_tone_gain";
    delay_settings[reverb_tone_gain].effect_api = &(api_table[WSD_AL_AE_ReverbEffectsSetup]);
    delay_settings[reverb_tone_gain].cmd_default_value = 0x40;
    delay_settings[reverb_tone_gain].effective_arg_idx = 2;
    delay_settings[reverb_tone_gain].profile_xml_name = "ToneLevel";
    delay_settings[reverb_tone_gain].profile_value_offset = 0;
    delay_settings[reverb_tone_gain].translate_table = NULL;
    delay_settings[reverb_tone_gain].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_time].serial_cmd_name = "reverb_time";
    delay_settings[reverb_time].effect_api = &(api_table[WSD_AL_AE_ReverbEffectsSetup]);
    delay_settings[reverb_time].cmd_default_value = 0x0a;
    delay_settings[reverb_time].effective_arg_idx = 1;
    delay_settings[reverb_time].profile_xml_name = "ReverbTime";
    delay_settings[reverb_time].profile_value_offset = 0;
    delay_settings[reverb_time].translate_table = NULL;
    delay_settings[reverb_time].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_pre_delay_time].serial_cmd_name = "reverb_pre_delay_time";
    delay_settings[reverb_pre_delay_time].effect_api = &(api_table[WSD_AL_AE_ReverbEffectsSetup]);
    delay_settings[reverb_pre_delay_time].cmd_default_value = 0x00;
    delay_settings[reverb_pre_delay_time].effective_arg_idx = 3;
    delay_settings[reverb_pre_delay_time].profile_xml_name = "Predelay";
    delay_settings[reverb_pre_delay_time].profile_value_offset = 0;
    delay_settings[reverb_pre_delay_time].translate_table = NULL;
    delay_settings[reverb_pre_delay_time].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_output_level].serial_cmd_name = "reverb_output_level";
    delay_settings[reverb_output_level].effect_api = &(api_table[WSD_AL_AE_ReverbOutputSetup]);
    delay_settings[reverb_output_level].cmd_default_value = 0x40;
    delay_settings[reverb_output_level].effective_arg_idx = 0;
    delay_settings[reverb_output_level].profile_xml_name = "OutputGain";
    delay_settings[reverb_output_level].profile_value_offset = 0;
    delay_settings[reverb_output_level].translate_table = NULL;
    delay_settings[reverb_output_level].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_phase].serial_cmd_name = "reverb_phase";
    delay_settings[reverb_phase].effect_api = &(api_table[WSD_AL_AE_ReverbOutputSetup]);
    delay_settings[reverb_phase].cmd_default_value = 0x00;
    delay_settings[reverb_phase].effective_arg_idx = 1;
    delay_settings[reverb_phase].profile_xml_name = "PhaseInvert";
    delay_settings[reverb_phase].profile_value_offset = 0;
    delay_settings[reverb_phase].translate_table = NULL;
    delay_settings[reverb_phase].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[mic_dry_phase].serial_cmd_name = "mic_dry_phase";
    delay_settings[mic_dry_phase].effect_api = &(api_table[WSD_AL_AE_DryOutputSetup]);
    delay_settings[mic_dry_phase].cmd_default_value = 0x00;
    delay_settings[mic_dry_phase].effective_arg_idx = 1;
    delay_settings[mic_dry_phase].profile_xml_name = "PhaseInvert";
    delay_settings[mic_dry_phase].profile_value_offset = 0;
    delay_settings[mic_dry_phase].translate_table = NULL;
    delay_settings[mic_dry_phase].profile_xml_node = MICINPUT_NODE_NAME;

    delay_settings[delay_eq_switch].serial_cmd_name = "delay_eq_switch";
    delay_settings[delay_eq_switch].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqEnable]);
    delay_settings[delay_eq_switch].cmd_default_value = 0x00;
    delay_settings[delay_eq_switch].effective_arg_idx = 0;
    delay_settings[delay_eq_switch].profile_xml_name = "EQEnable";
    delay_settings[delay_eq_switch].profile_value_offset = 0;
    delay_settings[delay_eq_switch].translate_table = NULL;
    delay_settings[delay_eq_switch].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band1_type].serial_cmd_name = "delay_eq_band1_type";
    delay_settings[delay_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqEdgeSetup]);
    delay_settings[delay_eq_band1_type].cmd_default_value = 0x00;
    delay_settings[delay_eq_band1_type].effective_arg_idx = 0;
    delay_settings[delay_eq_band1_type].profile_xml_name = "PEQLeftEdge";
    delay_settings[delay_eq_band1_type].profile_value_offset = 0;
    delay_settings[delay_eq_band1_type].translate_table = NULL;
    delay_settings[delay_eq_band1_type].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band1_gain].serial_cmd_name = "delay_eq_band1_gain";
    delay_settings[delay_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band1]);
    delay_settings[delay_eq_band1_gain].cmd_default_value = 0x40;
    delay_settings[delay_eq_band1_gain].effective_arg_idx = 2;
    delay_settings[delay_eq_band1_gain].profile_xml_name = "EQData1_Gain";
    delay_settings[delay_eq_band1_gain].profile_value_offset = -4;
    delay_settings[delay_eq_band1_gain].translate_table = gPEQ_Gain_Table;
    delay_settings[delay_eq_band1_gain].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band1_frequency].serial_cmd_name = "delay_eq_band1_frequency";
    delay_settings[delay_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band1]);
    delay_settings[delay_eq_band1_frequency].cmd_default_value = 0x0c;
    delay_settings[delay_eq_band1_frequency].effective_arg_idx = 0;
    delay_settings[delay_eq_band1_frequency].profile_xml_name = "EQData1_Freq";
    delay_settings[delay_eq_band1_frequency].profile_value_offset = -2;
    delay_settings[delay_eq_band1_frequency].translate_table = gPEQ_Freq_Table;
    delay_settings[delay_eq_band1_frequency].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band1_q].serial_cmd_name = "delay_eq_band1_q";
    delay_settings[delay_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band1]);
    delay_settings[delay_eq_band1_q].cmd_default_value = 0x07;
    delay_settings[delay_eq_band1_q].effective_arg_idx = 1;
    delay_settings[delay_eq_band1_q].profile_xml_name = "EQData1_Q";
    delay_settings[delay_eq_band1_q].profile_value_offset = 0;
    delay_settings[delay_eq_band1_q].translate_table = gPEQ_Q_Table;
    delay_settings[delay_eq_band1_q].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band2_gain].serial_cmd_name = "delay_eq_band2_gain";
    delay_settings[delay_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band2]);
    delay_settings[delay_eq_band2_gain].cmd_default_value = 0x40;
    delay_settings[delay_eq_band2_gain].effective_arg_idx = 2;
    delay_settings[delay_eq_band2_gain].profile_xml_name = "EQData2_Gain";
    delay_settings[delay_eq_band2_gain].profile_value_offset = -4;
    delay_settings[delay_eq_band2_gain].translate_table = gPEQ_Gain_Table;
    delay_settings[delay_eq_band2_gain].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band2_frequency].serial_cmd_name = "delay_eq_band2_frequency";
    delay_settings[delay_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band2]);
    delay_settings[delay_eq_band2_frequency].cmd_default_value = 0x12;
    delay_settings[delay_eq_band2_frequency].effective_arg_idx = 0;
    delay_settings[delay_eq_band2_frequency].profile_xml_name = "EQData2_Freq";
    delay_settings[delay_eq_band2_frequency].profile_value_offset = -2;
    delay_settings[delay_eq_band2_frequency].translate_table = gPEQ_Freq_Table;
    delay_settings[delay_eq_band2_frequency].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band2_q].serial_cmd_name = "delay_eq_band2_q";
    delay_settings[delay_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band2]);
    delay_settings[delay_eq_band2_q].cmd_default_value = 0x07;
    delay_settings[delay_eq_band2_q].effective_arg_idx = 1;
    delay_settings[delay_eq_band2_q].profile_xml_name = "EQData2_Q";
    delay_settings[delay_eq_band2_q].profile_value_offset = 0;
    delay_settings[delay_eq_band2_q].translate_table = gPEQ_Q_Table;
    delay_settings[delay_eq_band2_q].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band3_gain].serial_cmd_name = "delay_eq_band3_gain";
    delay_settings[delay_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band3]);
    delay_settings[delay_eq_band3_gain].cmd_default_value = 0x40;
    delay_settings[delay_eq_band3_gain].effective_arg_idx = 2;
    delay_settings[delay_eq_band3_gain].profile_xml_name = "EQData3_Gain";
    delay_settings[delay_eq_band3_gain].profile_value_offset = -4;
    delay_settings[delay_eq_band3_gain].translate_table = gPEQ_Gain_Table;
    delay_settings[delay_eq_band3_gain].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band3_frequency].serial_cmd_name = "delay_eq_band3_frequency";
    delay_settings[delay_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band3]);
    delay_settings[delay_eq_band3_frequency].cmd_default_value = 0x18;
    delay_settings[delay_eq_band3_frequency].effective_arg_idx = 0;
    delay_settings[delay_eq_band3_frequency].profile_xml_name = "EQData3_Freq";
    delay_settings[delay_eq_band3_frequency].profile_value_offset = -2;
    delay_settings[delay_eq_band3_frequency].translate_table = gPEQ_Freq_Table;
    delay_settings[delay_eq_band3_frequency].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band3_q].serial_cmd_name = "delay_eq_band3_q";
    delay_settings[delay_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band3]);
    delay_settings[delay_eq_band3_q].cmd_default_value = 0x07;
    delay_settings[delay_eq_band3_q].effective_arg_idx = 1;
    delay_settings[delay_eq_band3_q].profile_xml_name = "EQData3_Q";
    delay_settings[delay_eq_band3_q].profile_value_offset = 0;
    delay_settings[delay_eq_band3_q].translate_table = gPEQ_Q_Table;
    delay_settings[delay_eq_band3_q].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band4_gain].serial_cmd_name = "delay_eq_band4_gain";
    delay_settings[delay_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band4]);
    delay_settings[delay_eq_band4_gain].cmd_default_value = 0x40;
    delay_settings[delay_eq_band4_gain].effective_arg_idx = 2;
    delay_settings[delay_eq_band4_gain].profile_xml_name = "EQData4_Gain";
    delay_settings[delay_eq_band4_gain].profile_value_offset = -4;
    delay_settings[delay_eq_band4_gain].translate_table = gPEQ_Gain_Table;
    delay_settings[delay_eq_band4_gain].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band4_frequency].serial_cmd_name = "delay_eq_band4_frequency";
    delay_settings[delay_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band4]);
    delay_settings[delay_eq_band4_frequency].cmd_default_value = 0x1e;
    delay_settings[delay_eq_band4_frequency].effective_arg_idx = 0;
    delay_settings[delay_eq_band4_frequency].profile_xml_name = "EQData4_Freq";
    delay_settings[delay_eq_band4_frequency].profile_value_offset = -2;
    delay_settings[delay_eq_band4_frequency].translate_table = gPEQ_Freq_Table;
    delay_settings[delay_eq_band4_frequency].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band4_q].serial_cmd_name = "delay_eq_band4_q";
    delay_settings[delay_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band4]);
    delay_settings[delay_eq_band4_q].cmd_default_value = 0x07;
    delay_settings[delay_eq_band4_q].effective_arg_idx = 1;
    delay_settings[delay_eq_band4_q].profile_xml_name = "EQData4_Q";
    delay_settings[delay_eq_band4_q].profile_value_offset = 0;
    delay_settings[delay_eq_band4_q].translate_table = gPEQ_Q_Table;
    delay_settings[delay_eq_band4_q].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band5_type].serial_cmd_name = "delay_eq_band5_type";
    delay_settings[delay_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqEdgeSetup]);
    delay_settings[delay_eq_band5_type].cmd_default_value = 0x00;
    delay_settings[delay_eq_band5_type].effective_arg_idx = 1;
    delay_settings[delay_eq_band5_type].profile_xml_name = "PEQRightEdge";
    delay_settings[delay_eq_band5_type].profile_value_offset = 0;
    delay_settings[delay_eq_band5_type].translate_table = NULL;
    delay_settings[delay_eq_band5_type].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band5_gain].serial_cmd_name = "delay_eq_band5_gain";
    delay_settings[delay_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band5]);
    delay_settings[delay_eq_band5_gain].cmd_default_value = 0x40;
    delay_settings[delay_eq_band5_gain].effective_arg_idx = 2;
    delay_settings[delay_eq_band5_gain].profile_xml_name = "EQData5_Gain";
    delay_settings[delay_eq_band5_gain].profile_value_offset = -4;
    delay_settings[delay_eq_band5_gain].translate_table = gPEQ_Gain_Table;
    delay_settings[delay_eq_band5_gain].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band5_frequency].serial_cmd_name = "delay_eq_band5_frequency";
    delay_settings[delay_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band5]);
    delay_settings[delay_eq_band5_frequency].cmd_default_value = 0x24;
    delay_settings[delay_eq_band5_frequency].effective_arg_idx = 0;
    delay_settings[delay_eq_band5_frequency].profile_xml_name = "EQData5_Freq";
    delay_settings[delay_eq_band5_frequency].profile_value_offset = -2;
    delay_settings[delay_eq_band5_frequency].translate_table = gPEQ_Freq_Table;
    delay_settings[delay_eq_band5_frequency].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_band5_q].serial_cmd_name = "delay_eq_band5_q";
    delay_settings[delay_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band5]);
    delay_settings[delay_eq_band5_q].cmd_default_value = 0x07;
    delay_settings[delay_eq_band5_q].effective_arg_idx = 1;
    delay_settings[delay_eq_band5_q].profile_xml_name = "EQData5_Q";
    delay_settings[delay_eq_band5_q].profile_value_offset = 0;
    delay_settings[delay_eq_band5_q].translate_table = gPEQ_Q_Table;
    delay_settings[delay_eq_band5_q].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_hpf_type].serial_cmd_name = "delay_eq_hpf_type";
    delay_settings[delay_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_DelayInputBandpassfilterSetup]);
    delay_settings[delay_eq_hpf_type].cmd_default_value = 0x06;
    delay_settings[delay_eq_hpf_type].effective_arg_idx = 0;
    delay_settings[delay_eq_hpf_type].profile_xml_name = "EQData0_Slope";
    delay_settings[delay_eq_hpf_type].profile_value_offset = 0;
    delay_settings[delay_eq_hpf_type].translate_table = NULL;
    delay_settings[delay_eq_hpf_type].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_hpf_frequency].serial_cmd_name = "delay_eq_hpf_frequency";
    delay_settings[delay_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInputBandpassfilterSetup]);
    delay_settings[delay_eq_hpf_frequency].cmd_default_value = 0x00;
    delay_settings[delay_eq_hpf_frequency].effective_arg_idx = 1;
    delay_settings[delay_eq_hpf_frequency].profile_xml_name = "EQData0_Freq";
    delay_settings[delay_eq_hpf_frequency].profile_value_offset = 0;
    delay_settings[delay_eq_hpf_frequency].translate_table = gHighPass_Freq_Table;
    delay_settings[delay_eq_hpf_frequency].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_lpf_type].serial_cmd_name = "delay_eq_lpf_type";
    delay_settings[delay_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_DelayInputBandpassfilterSetup]);
    delay_settings[delay_eq_lpf_type].cmd_default_value = 0x06;
    delay_settings[delay_eq_lpf_type].effective_arg_idx = 2;
    delay_settings[delay_eq_lpf_type].profile_xml_name = "EQData6_Slope";
    delay_settings[delay_eq_lpf_type].profile_value_offset = 0;
    delay_settings[delay_eq_lpf_type].translate_table = NULL;
    delay_settings[delay_eq_lpf_type].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[delay_eq_lpf_frequency].serial_cmd_name = "delay_eq_lpf_frequency";
    delay_settings[delay_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_DelayInputBandpassfilterSetup]);
    delay_settings[delay_eq_lpf_frequency].cmd_default_value = 0x6f;
    delay_settings[delay_eq_lpf_frequency].effective_arg_idx = 3;
    delay_settings[delay_eq_lpf_frequency].profile_xml_name = "EQData6_Freq";
    delay_settings[delay_eq_lpf_frequency].profile_value_offset = 0;
    delay_settings[delay_eq_lpf_frequency].translate_table = gLowPass_Freq_Table;
    delay_settings[delay_eq_lpf_frequency].profile_xml_node = ECHO_NODE_NAME;

    delay_settings[reverb_eq_switch].serial_cmd_name = "reverb_eq_switch";
    delay_settings[reverb_eq_switch].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqEnable]);
    delay_settings[reverb_eq_switch].cmd_default_value = 0x00;
    delay_settings[reverb_eq_switch].effective_arg_idx = 0;
    delay_settings[reverb_eq_switch].profile_xml_name = "EQEnable";
    delay_settings[reverb_eq_switch].profile_value_offset = 0;
    delay_settings[reverb_eq_switch].translate_table = NULL;
    delay_settings[reverb_eq_switch].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band1_type].serial_cmd_name = "reverb_eq_band1_type";
    delay_settings[reverb_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqEdgeSetup]);
    delay_settings[reverb_eq_band1_type].cmd_default_value = 0x00;
    delay_settings[reverb_eq_band1_type].effective_arg_idx = 0;
    delay_settings[reverb_eq_band1_type].profile_xml_name = "PEQLeftEdge";
    delay_settings[reverb_eq_band1_type].profile_value_offset = 0;
    delay_settings[reverb_eq_band1_type].translate_table = NULL;
    delay_settings[reverb_eq_band1_type].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band1_gain].serial_cmd_name = "reverb_eq_band1_gain";
    delay_settings[reverb_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band1]);
    delay_settings[reverb_eq_band1_gain].cmd_default_value = 0x40;
    delay_settings[reverb_eq_band1_gain].effective_arg_idx = 2;
    delay_settings[reverb_eq_band1_gain].profile_xml_name = "EQData1_Gain";
    delay_settings[reverb_eq_band1_gain].profile_value_offset = -4;
    delay_settings[reverb_eq_band1_gain].translate_table = gPEQ_Gain_Table;
    delay_settings[reverb_eq_band1_gain].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band1_frequency].serial_cmd_name = "reverb_eq_band1_frequency";
    delay_settings[reverb_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band1]);
    delay_settings[reverb_eq_band1_frequency].cmd_default_value = 0x0c;
    delay_settings[reverb_eq_band1_frequency].effective_arg_idx = 0;
    delay_settings[reverb_eq_band1_frequency].profile_xml_name = "EQData1_Freq";
    delay_settings[reverb_eq_band1_frequency].profile_value_offset = -2;
    delay_settings[reverb_eq_band1_frequency].translate_table = gPEQ_Freq_Table;
    delay_settings[reverb_eq_band1_frequency].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band1_q].serial_cmd_name = "reverb_eq_band1_q";
    delay_settings[reverb_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band1]);
    delay_settings[reverb_eq_band1_q].cmd_default_value = 0x07;
    delay_settings[reverb_eq_band1_q].effective_arg_idx = 1;
    delay_settings[reverb_eq_band1_q].profile_xml_name = "EQData1_Q";
    delay_settings[reverb_eq_band1_q].profile_value_offset = 0;
    delay_settings[reverb_eq_band1_q].translate_table = gPEQ_Q_Table;
    delay_settings[reverb_eq_band1_q].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band2_gain].serial_cmd_name = "reverb_eq_band2_gain";
    delay_settings[reverb_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band2]);
    delay_settings[reverb_eq_band2_gain].cmd_default_value = 0x40;
    delay_settings[reverb_eq_band2_gain].effective_arg_idx = 2;
    delay_settings[reverb_eq_band2_gain].profile_xml_name = "EQData2_Gain";
    delay_settings[reverb_eq_band2_gain].profile_value_offset = -4;
    delay_settings[reverb_eq_band2_gain].translate_table = gPEQ_Gain_Table;
    delay_settings[reverb_eq_band2_gain].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band2_frequency].serial_cmd_name = "reverb_eq_band2_frequency";
    delay_settings[reverb_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band2]);
    delay_settings[reverb_eq_band2_frequency].cmd_default_value = 0x12;
    delay_settings[reverb_eq_band2_frequency].effective_arg_idx = 0;
    delay_settings[reverb_eq_band2_frequency].profile_xml_name = "EQData2_Freq";
    delay_settings[reverb_eq_band2_frequency].profile_value_offset = -2;
    delay_settings[reverb_eq_band2_frequency].translate_table = gPEQ_Freq_Table;
    delay_settings[reverb_eq_band2_frequency].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band2_q].serial_cmd_name = "reverb_eq_band2_q";
    delay_settings[reverb_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band2]);
    delay_settings[reverb_eq_band2_q].cmd_default_value = 0x07;
    delay_settings[reverb_eq_band2_q].effective_arg_idx = 1;
    delay_settings[reverb_eq_band2_q].profile_xml_name = "EQData2_Q";
    delay_settings[reverb_eq_band2_q].profile_value_offset = 0;
    delay_settings[reverb_eq_band2_q].translate_table = gPEQ_Q_Table;
    delay_settings[reverb_eq_band2_q].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band3_gain].serial_cmd_name = "reverb_eq_band3_gain";
    delay_settings[reverb_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band3]);
    delay_settings[reverb_eq_band3_gain].cmd_default_value = 0x40;
    delay_settings[reverb_eq_band3_gain].effective_arg_idx = 2;
    delay_settings[reverb_eq_band3_gain].profile_xml_name = "EQData3_Gain";
    delay_settings[reverb_eq_band3_gain].profile_value_offset = -4;
    delay_settings[reverb_eq_band3_gain].translate_table = gPEQ_Gain_Table;
    delay_settings[reverb_eq_band3_gain].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band3_frequency].serial_cmd_name = "reverb_eq_band3_frequency";
    delay_settings[reverb_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band3]);
    delay_settings[reverb_eq_band3_frequency].cmd_default_value = 0x18;
    delay_settings[reverb_eq_band3_frequency].effective_arg_idx = 0;
    delay_settings[reverb_eq_band3_frequency].profile_xml_name = "EQData3_Freq";
    delay_settings[reverb_eq_band3_frequency].profile_value_offset = -2;
    delay_settings[reverb_eq_band3_frequency].translate_table = gPEQ_Freq_Table;
    delay_settings[reverb_eq_band3_frequency].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band3_q].serial_cmd_name = "reverb_eq_band3_q";
    delay_settings[reverb_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band3]);
    delay_settings[reverb_eq_band3_q].cmd_default_value = 0x07;
    delay_settings[reverb_eq_band3_q].effective_arg_idx = 1;
    delay_settings[reverb_eq_band3_q].profile_xml_name = "EQData3_Q";
    delay_settings[reverb_eq_band3_q].profile_value_offset = 0;
    delay_settings[reverb_eq_band3_q].translate_table = gPEQ_Q_Table;
    delay_settings[reverb_eq_band3_q].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band4_gain].serial_cmd_name = "reverb_eq_band4_gain";
    delay_settings[reverb_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band4]);
    delay_settings[reverb_eq_band4_gain].cmd_default_value = 0x40;
    delay_settings[reverb_eq_band4_gain].effective_arg_idx = 2;
    delay_settings[reverb_eq_band4_gain].profile_xml_name = "EQData4_Gain";
    delay_settings[reverb_eq_band4_gain].profile_value_offset = -4;
    delay_settings[reverb_eq_band4_gain].translate_table = gPEQ_Gain_Table;
    delay_settings[reverb_eq_band4_gain].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band4_frequency].serial_cmd_name = "reverb_eq_band4_frequency";
    delay_settings[reverb_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band4]);
    delay_settings[reverb_eq_band4_frequency].cmd_default_value = 0x1e;
    delay_settings[reverb_eq_band4_frequency].effective_arg_idx = 0;
    delay_settings[reverb_eq_band4_frequency].profile_xml_name = "EQData4_Freq";
    delay_settings[reverb_eq_band4_frequency].profile_value_offset = -2;
    delay_settings[reverb_eq_band4_frequency].translate_table = gPEQ_Freq_Table;
    delay_settings[reverb_eq_band4_frequency].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band4_q].serial_cmd_name = "reverb_eq_band4_q";
    delay_settings[reverb_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band4]);
    delay_settings[reverb_eq_band4_q].cmd_default_value = 0x07;
    delay_settings[reverb_eq_band4_q].effective_arg_idx = 1;
    delay_settings[reverb_eq_band4_q].profile_xml_name = "EQData4_Q";
    delay_settings[reverb_eq_band4_q].profile_value_offset = 0;
    delay_settings[reverb_eq_band4_q].translate_table = gPEQ_Q_Table;
    delay_settings[reverb_eq_band4_q].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band5_type].serial_cmd_name = "reverb_eq_band5_type";
    delay_settings[reverb_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqEdgeSetup]);
    delay_settings[reverb_eq_band5_type].cmd_default_value = 0x00;
    delay_settings[reverb_eq_band5_type].effective_arg_idx = 1;
    delay_settings[reverb_eq_band5_type].profile_xml_name = "PEQRightEdge";
    delay_settings[reverb_eq_band5_type].profile_value_offset = 0;
    delay_settings[reverb_eq_band5_type].translate_table = NULL;
    delay_settings[reverb_eq_band5_type].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band5_gain].serial_cmd_name = "reverb_eq_band5_gain";
    delay_settings[reverb_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band5]);
    delay_settings[reverb_eq_band5_gain].cmd_default_value = 0x40;
    delay_settings[reverb_eq_band5_gain].effective_arg_idx = 2;
    delay_settings[reverb_eq_band5_gain].profile_xml_name = "EQData5_Gain";
    delay_settings[reverb_eq_band5_gain].profile_value_offset = -4;
    delay_settings[reverb_eq_band5_gain].translate_table = gPEQ_Gain_Table;
    delay_settings[reverb_eq_band5_gain].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band5_frequency].serial_cmd_name = "reverb_eq_band5_frequency";
    delay_settings[reverb_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band5]);
    delay_settings[reverb_eq_band5_frequency].cmd_default_value = 0x24;
    delay_settings[reverb_eq_band5_frequency].effective_arg_idx = 0;
    delay_settings[reverb_eq_band5_frequency].profile_xml_name = "EQData5_Freq";
    delay_settings[reverb_eq_band5_frequency].profile_value_offset = -2;
    delay_settings[reverb_eq_band5_frequency].translate_table = gPEQ_Freq_Table;
    delay_settings[reverb_eq_band5_frequency].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_band5_q].serial_cmd_name = "reverb_eq_band5_q";
    delay_settings[reverb_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band5]);
    delay_settings[reverb_eq_band5_q].cmd_default_value = 0x07;
    delay_settings[reverb_eq_band5_q].effective_arg_idx = 1;
    delay_settings[reverb_eq_band5_q].profile_xml_name = "EQData5_Q";
    delay_settings[reverb_eq_band5_q].profile_value_offset = 0;
    delay_settings[reverb_eq_band5_q].translate_table = gPEQ_Q_Table;
    delay_settings[reverb_eq_band5_q].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_hpf_type].serial_cmd_name = "reverb_eq_hpf_type";
    delay_settings[reverb_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_ReverbInputBandpassfilterSetup]);
    delay_settings[reverb_eq_hpf_type].cmd_default_value = 0x06;
    delay_settings[reverb_eq_hpf_type].effective_arg_idx = 0;
    delay_settings[reverb_eq_hpf_type].profile_xml_name = "EQData0_Slope";
    delay_settings[reverb_eq_hpf_type].profile_value_offset = 0;
    delay_settings[reverb_eq_hpf_type].translate_table = NULL;
    delay_settings[reverb_eq_hpf_type].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_hpf_frequency].serial_cmd_name = "reverb_eq_hpf_frequency";
    delay_settings[reverb_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInputBandpassfilterSetup]);
    delay_settings[reverb_eq_hpf_frequency].cmd_default_value = 0x00;
    delay_settings[reverb_eq_hpf_frequency].effective_arg_idx = 1;
    delay_settings[reverb_eq_hpf_frequency].profile_xml_name = "EQData0_Freq";
    delay_settings[reverb_eq_hpf_frequency].profile_value_offset = 0;
    delay_settings[reverb_eq_hpf_frequency].translate_table = gHighPass_Freq_Table;
    delay_settings[reverb_eq_hpf_frequency].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_lpf_type].serial_cmd_name = "reverb_eq_lpf_type";
    delay_settings[reverb_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_ReverbInputBandpassfilterSetup]);
    delay_settings[reverb_eq_lpf_type].cmd_default_value = 0x06;
    delay_settings[reverb_eq_lpf_type].effective_arg_idx = 2;
    delay_settings[reverb_eq_lpf_type].profile_xml_name = "EQData6_Slope";
    delay_settings[reverb_eq_lpf_type].profile_value_offset = 0;
    delay_settings[reverb_eq_lpf_type].translate_table = NULL;
    delay_settings[reverb_eq_lpf_type].profile_xml_node = REVERB_NODE_NAME;

    delay_settings[reverb_eq_lpf_frequency].serial_cmd_name = "reverb_eq_lpf_frequency";
    delay_settings[reverb_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_ReverbInputBandpassfilterSetup]);
    delay_settings[reverb_eq_lpf_frequency].cmd_default_value = 0x6f;
    delay_settings[reverb_eq_lpf_frequency].effective_arg_idx = 3;
    delay_settings[reverb_eq_lpf_frequency].profile_xml_name = "EQData6_Freq";
    delay_settings[reverb_eq_lpf_frequency].profile_value_offset = 0;
    delay_settings[reverb_eq_lpf_frequency].translate_table = gLowPass_Freq_Table;
    delay_settings[reverb_eq_lpf_frequency].profile_xml_node = REVERB_NODE_NAME;
}

static void music_effects_settings_default()
{
    memset(music_effects_settings,0,sizeof(DSP_SETTING_T)*music_effects_end);

    music_effects_settings[key_control_switch].serial_cmd_name = "key_control_switch";
    music_effects_settings[key_control_switch].effect_api = &(api_table[WSD_AL_AE_MusicKeyctrlEnable]);
    music_effects_settings[key_control_switch].cmd_default_value = 0x00;
    music_effects_settings[key_control_switch].effective_arg_idx = 0;
    music_effects_settings[key_control_switch].profile_xml_name = "KeyCtrlEnable";
    music_effects_settings[key_control_switch].profile_value_offset = 0;
    music_effects_settings[key_control_switch].translate_table = NULL;
    music_effects_settings[key_control_switch].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[key_control_pitch].serial_cmd_name = "key_control_pitch";
    music_effects_settings[key_control_pitch].effect_api = &(api_table[WSD_AL_AE_MusicKeyctrlSetup]);
    music_effects_settings[key_control_pitch].cmd_default_value = 0x40;
    music_effects_settings[key_control_pitch].effective_arg_idx = 0;
    music_effects_settings[key_control_pitch].profile_xml_name = "KeyCtrlPitch";
    music_effects_settings[key_control_pitch].profile_value_offset = -52;
    music_effects_settings[key_control_pitch].translate_table = NULL;
    music_effects_settings[key_control_pitch].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[key_control_pitch_fine].serial_cmd_name = "key_control_pitch_fine";
    music_effects_settings[key_control_pitch_fine].effect_api = &(api_table[WSD_AL_AE_MusicKeyctrlSetup]);
    music_effects_settings[key_control_pitch_fine].cmd_default_value = 0x40;
    music_effects_settings[key_control_pitch_fine].effective_arg_idx = 1;
    music_effects_settings[key_control_pitch_fine].profile_xml_name = "KeyCtrlFineTune";
    music_effects_settings[key_control_pitch_fine].profile_value_offset = -14;
    music_effects_settings[key_control_pitch_fine].translate_table = NULL;
    music_effects_settings[key_control_pitch_fine].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_switch].serial_cmd_name = "eq_switch";
    music_effects_settings[eq_switch].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqEnable]);
    music_effects_settings[eq_switch].cmd_default_value = 0x00;
    music_effects_settings[eq_switch].effective_arg_idx = 0;
    music_effects_settings[eq_switch].profile_xml_name = "GEQ31BandEnable";
    music_effects_settings[eq_switch].profile_value_offset = 0;
    music_effects_settings[eq_switch].translate_table = NULL;
    music_effects_settings[eq_switch].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band1_gain].serial_cmd_name = "eq_band1_gain";
    music_effects_settings[eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band1]);
    music_effects_settings[eq_band1_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band1_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band1_gain].profile_xml_name = "GEQ31Band1Gain";
    music_effects_settings[eq_band1_gain].profile_value_offset = -4;
    music_effects_settings[eq_band1_gain].translate_table = NULL;
    music_effects_settings[eq_band1_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band2_gain].serial_cmd_name = "eq_band2_gain";
    music_effects_settings[eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band2]);
    music_effects_settings[eq_band2_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band2_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band2_gain].profile_xml_name = "GEQ31Band2Gain";
    music_effects_settings[eq_band2_gain].profile_value_offset = -4;
    music_effects_settings[eq_band2_gain].translate_table = NULL;
    music_effects_settings[eq_band2_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band3_gain].serial_cmd_name = "eq_band3_gain";
    music_effects_settings[eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band3]);
    music_effects_settings[eq_band3_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band3_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band3_gain].profile_xml_name = "GEQ31Band3Gain";
    music_effects_settings[eq_band3_gain].profile_value_offset = -4;
    music_effects_settings[eq_band3_gain].translate_table = NULL;
    music_effects_settings[eq_band3_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band4_gain].serial_cmd_name = "eq_band4_gain";
    music_effects_settings[eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band4]);
    music_effects_settings[eq_band4_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band4_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band4_gain].profile_xml_name = "GEQ31Band4Gain";
    music_effects_settings[eq_band4_gain].profile_value_offset = -4;
    music_effects_settings[eq_band4_gain].translate_table = NULL;
    music_effects_settings[eq_band4_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band5_gain].serial_cmd_name = "eq_band5_gain";
    music_effects_settings[eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band5]);
    music_effects_settings[eq_band5_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band5_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band5_gain].profile_xml_name = "GEQ31Band5Gain";
    music_effects_settings[eq_band5_gain].profile_value_offset = -4;
    music_effects_settings[eq_band5_gain].translate_table = NULL;
    music_effects_settings[eq_band5_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band6_gain].serial_cmd_name = "eq_band6_gain";
    music_effects_settings[eq_band6_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band6]);
    music_effects_settings[eq_band6_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band6_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band6_gain].profile_xml_name = "GEQ31Band6Gain";
    music_effects_settings[eq_band6_gain].profile_value_offset = -4;
    music_effects_settings[eq_band6_gain].translate_table = NULL;
    music_effects_settings[eq_band6_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band7_gain].serial_cmd_name = "eq_band7_gain";
    music_effects_settings[eq_band7_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band7]);
    music_effects_settings[eq_band7_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band7_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band7_gain].profile_xml_name = "GEQ31Band7Gain";
    music_effects_settings[eq_band7_gain].profile_value_offset = -4;
    music_effects_settings[eq_band7_gain].translate_table = NULL;
    music_effects_settings[eq_band7_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band8_gain].serial_cmd_name = "eq_band8_gain";
    music_effects_settings[eq_band8_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band8]);
    music_effects_settings[eq_band8_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band8_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band8_gain].profile_xml_name = "GEQ31Band8Gain";
    music_effects_settings[eq_band8_gain].profile_value_offset = -4;
    music_effects_settings[eq_band8_gain].translate_table = NULL;
    music_effects_settings[eq_band8_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band9_gain].serial_cmd_name = "eq_band9_gain";
    music_effects_settings[eq_band9_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band9]);
    music_effects_settings[eq_band9_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band9_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band9_gain].profile_xml_name = "GEQ31Band9Gain";
    music_effects_settings[eq_band9_gain].profile_value_offset = -4;
    music_effects_settings[eq_band9_gain].translate_table = NULL;
    music_effects_settings[eq_band9_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band10_gain].serial_cmd_name = "eq_band10_gain";
    music_effects_settings[eq_band10_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band10]);
    music_effects_settings[eq_band10_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band10_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band10_gain].profile_xml_name = "GEQ31Band10Gain";
    music_effects_settings[eq_band10_gain].profile_value_offset = -4;
    music_effects_settings[eq_band10_gain].translate_table = NULL;
    music_effects_settings[eq_band10_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band11_gain].serial_cmd_name = "eq_band11_gain";
    music_effects_settings[eq_band11_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band11]);
    music_effects_settings[eq_band11_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band11_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band11_gain].profile_xml_name = "GEQ31Band11Gain";
    music_effects_settings[eq_band11_gain].profile_value_offset = -4;
    music_effects_settings[eq_band11_gain].translate_table = NULL;
    music_effects_settings[eq_band11_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band12_gain].serial_cmd_name = "eq_band12_gain";
    music_effects_settings[eq_band12_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band12]);
    music_effects_settings[eq_band12_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band12_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band12_gain].profile_xml_name = "GEQ31Band12Gain";
    music_effects_settings[eq_band12_gain].profile_value_offset = -4;
    music_effects_settings[eq_band12_gain].translate_table = NULL;
    music_effects_settings[eq_band12_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band13_gain].serial_cmd_name = "eq_band13_gain";
    music_effects_settings[eq_band13_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band13]);
    music_effects_settings[eq_band13_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band13_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band13_gain].profile_xml_name = "GEQ31Band13Gain";
    music_effects_settings[eq_band13_gain].profile_value_offset = -4;
    music_effects_settings[eq_band13_gain].translate_table = NULL;
    music_effects_settings[eq_band13_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band14_gain].serial_cmd_name = "eq_band14_gain";
    music_effects_settings[eq_band14_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band14]);
    music_effects_settings[eq_band14_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band14_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band14_gain].profile_xml_name = "GEQ31Band14Gain";
    music_effects_settings[eq_band14_gain].profile_value_offset = -4;
    music_effects_settings[eq_band14_gain].translate_table = NULL;
    music_effects_settings[eq_band14_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band15_gain].serial_cmd_name = "eq_band15_gain";
    music_effects_settings[eq_band15_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band15]);
    music_effects_settings[eq_band15_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band15_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band15_gain].profile_xml_name = "GEQ31Band15Gain";
    music_effects_settings[eq_band15_gain].profile_value_offset = -4;
    music_effects_settings[eq_band15_gain].translate_table = NULL;
    music_effects_settings[eq_band15_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band16_gain].serial_cmd_name = "eq_band16_gain";
    music_effects_settings[eq_band16_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band16]);
    music_effects_settings[eq_band16_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band16_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band16_gain].profile_xml_name = "GEQ31Band16Gain";
    music_effects_settings[eq_band16_gain].profile_value_offset = -4;
    music_effects_settings[eq_band16_gain].translate_table = NULL;
    music_effects_settings[eq_band16_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band17_gain].serial_cmd_name = "eq_band17_gain";
    music_effects_settings[eq_band17_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band17]);
    music_effects_settings[eq_band17_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band17_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band17_gain].profile_xml_name = "GEQ31Band17Gain";
    music_effects_settings[eq_band17_gain].profile_value_offset = -4;
    music_effects_settings[eq_band17_gain].translate_table = NULL;
    music_effects_settings[eq_band17_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band18_gain].serial_cmd_name = "eq_band18_gain";
    music_effects_settings[eq_band18_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band18]);
    music_effects_settings[eq_band18_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band18_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band18_gain].profile_xml_name = "GEQ31Band18Gain";
    music_effects_settings[eq_band18_gain].profile_value_offset = -4;
    music_effects_settings[eq_band18_gain].translate_table = NULL;
    music_effects_settings[eq_band18_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band19_gain].serial_cmd_name = "eq_band19_gain";
    music_effects_settings[eq_band19_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band19]);
    music_effects_settings[eq_band19_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band19_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band19_gain].profile_xml_name = "GEQ31Band19Gain";
    music_effects_settings[eq_band19_gain].profile_value_offset = -4;
    music_effects_settings[eq_band19_gain].translate_table = NULL;
    music_effects_settings[eq_band19_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band20_gain].serial_cmd_name = "eq_band20_gain";
    music_effects_settings[eq_band20_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band20]);
    music_effects_settings[eq_band20_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band20_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band20_gain].profile_xml_name = "GEQ31Band20Gain";
    music_effects_settings[eq_band20_gain].profile_value_offset = -4;
    music_effects_settings[eq_band20_gain].translate_table = NULL;
    music_effects_settings[eq_band20_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band21_gain].serial_cmd_name = "eq_band21_gain";
    music_effects_settings[eq_band21_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band21]);
    music_effects_settings[eq_band21_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band21_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band21_gain].profile_xml_name = "GEQ31Band21Gain";
    music_effects_settings[eq_band21_gain].profile_value_offset = -4;
    music_effects_settings[eq_band21_gain].translate_table = NULL;
    music_effects_settings[eq_band21_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band22_gain].serial_cmd_name = "eq_band22_gain";
    music_effects_settings[eq_band22_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band22]);
    music_effects_settings[eq_band22_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band22_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band22_gain].profile_xml_name = "GEQ31Band22Gain";
    music_effects_settings[eq_band22_gain].profile_value_offset = -4;
    music_effects_settings[eq_band22_gain].translate_table = NULL;
    music_effects_settings[eq_band22_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band23_gain].serial_cmd_name = "eq_band23_gain";
    music_effects_settings[eq_band23_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band23]);
    music_effects_settings[eq_band23_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band23_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band23_gain].profile_xml_name = "GEQ31Band23Gain";
    music_effects_settings[eq_band23_gain].profile_value_offset = -4;
    music_effects_settings[eq_band23_gain].translate_table = NULL;
    music_effects_settings[eq_band23_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band24_gain].serial_cmd_name = "eq_band24_gain";
    music_effects_settings[eq_band24_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band24]);
    music_effects_settings[eq_band24_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band24_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band24_gain].profile_xml_name = "GEQ31Band24Gain";
    music_effects_settings[eq_band24_gain].profile_value_offset = -4;
    music_effects_settings[eq_band24_gain].translate_table = NULL;
    music_effects_settings[eq_band24_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band25_gain].serial_cmd_name = "eq_band25_gain";
    music_effects_settings[eq_band25_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band25]);
    music_effects_settings[eq_band25_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band25_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band25_gain].profile_xml_name = "GEQ31Band25Gain";
    music_effects_settings[eq_band25_gain].profile_value_offset = -4;
    music_effects_settings[eq_band25_gain].translate_table = NULL;
    music_effects_settings[eq_band25_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band26_gain].serial_cmd_name = "eq_band26_gain";
    music_effects_settings[eq_band26_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band26]);
    music_effects_settings[eq_band26_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band26_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band26_gain].profile_xml_name = "GEQ31Band26Gain";
    music_effects_settings[eq_band26_gain].profile_value_offset = -4;
    music_effects_settings[eq_band26_gain].translate_table = NULL;
    music_effects_settings[eq_band26_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band27_gain].serial_cmd_name = "eq_band27_gain";
    music_effects_settings[eq_band27_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band27]);
    music_effects_settings[eq_band27_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band27_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band27_gain].profile_xml_name = "GEQ31Band27Gain";
    music_effects_settings[eq_band27_gain].profile_value_offset = -4;
    music_effects_settings[eq_band27_gain].translate_table = NULL;
    music_effects_settings[eq_band27_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band28_gain].serial_cmd_name = "eq_band28_gain";
    music_effects_settings[eq_band28_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band28]);
    music_effects_settings[eq_band28_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band28_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band28_gain].profile_xml_name = "GEQ31Band28Gain";
    music_effects_settings[eq_band28_gain].profile_value_offset = -4;
    music_effects_settings[eq_band28_gain].translate_table = NULL;
    music_effects_settings[eq_band28_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band29_gain].serial_cmd_name = "eq_band29_gain";
    music_effects_settings[eq_band29_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band29]);
    music_effects_settings[eq_band29_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band29_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band29_gain].profile_xml_name = "GEQ31Band29Gain";
    music_effects_settings[eq_band29_gain].profile_value_offset = -4;
    music_effects_settings[eq_band29_gain].translate_table = NULL;
    music_effects_settings[eq_band29_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band30_gain].serial_cmd_name = "eq_band30_gain";
    music_effects_settings[eq_band30_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band30]);
    music_effects_settings[eq_band30_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band30_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band30_gain].profile_xml_name = "GEQ31Band30Gain";
    music_effects_settings[eq_band30_gain].profile_value_offset = -4;
    music_effects_settings[eq_band30_gain].translate_table = NULL;
    music_effects_settings[eq_band30_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[eq_band31_gain].serial_cmd_name = "eq_band31_gain";
    music_effects_settings[eq_band31_gain].effect_api = &(api_table[WSD_AL_AE_Music31bandGeqSetup_band31]);
    music_effects_settings[eq_band31_gain].cmd_default_value = 0x40;
    music_effects_settings[eq_band31_gain].effective_arg_idx = 0;
    music_effects_settings[eq_band31_gain].profile_xml_name = "GEQ31Band31Gain";
    music_effects_settings[eq_band31_gain].profile_value_offset = -4;
    music_effects_settings[eq_band31_gain].translate_table = NULL;
    music_effects_settings[eq_band31_gain].profile_xml_node = MUSICINPUT_NODE_NAME;

    music_effects_settings[music_phase].serial_cmd_name = "music_phase";
    music_effects_settings[music_phase].effect_api = &(api_table[WSD_AL_AE_MusicPhaseSetup]);
    music_effects_settings[music_phase].cmd_default_value = 0x00;
    music_effects_settings[music_phase].effective_arg_idx = 0;
    music_effects_settings[music_phase].profile_xml_name = "PhaseInvert";
    music_effects_settings[music_phase].profile_value_offset = 0;
    music_effects_settings[music_phase].translate_table = NULL;
    music_effects_settings[music_phase].profile_xml_node = MUSICINPUT_NODE_NAME;
}

static void output_settings_default()
{
    memset(output_settings,0,sizeof(DSP_SETTING_T)*output_end);

    output_settings[front_eq_switch].serial_cmd_name = "front_eq_switch";
    output_settings[front_eq_switch].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEnable_front]);
    output_settings[front_eq_switch].cmd_default_value = 0x00;
    output_settings[front_eq_switch].effective_arg_idx = 0;
    output_settings[front_eq_switch].profile_xml_name = "EQEnable";
    output_settings[front_eq_switch].profile_value_offset = 0;
    output_settings[front_eq_switch].translate_table = NULL;
    output_settings[front_eq_switch].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band1_type].serial_cmd_name = "front_eq_band1_type";
    output_settings[front_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_front]);
    output_settings[front_eq_band1_type].cmd_default_value = 0x00;
    output_settings[front_eq_band1_type].effective_arg_idx = 0;
    output_settings[front_eq_band1_type].profile_xml_name = "PEQLeftEdge";
    output_settings[front_eq_band1_type].profile_value_offset = 0;
    output_settings[front_eq_band1_type].translate_table = NULL;
    output_settings[front_eq_band1_type].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band1_gain].serial_cmd_name = "front_eq_band1_gain";
    output_settings[front_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band1]);
    output_settings[front_eq_band1_gain].cmd_default_value = 0x40;
    output_settings[front_eq_band1_gain].effective_arg_idx = 2;
    output_settings[front_eq_band1_gain].profile_xml_name = "EQData1_Gain";
    output_settings[front_eq_band1_gain].profile_value_offset = -4;
    output_settings[front_eq_band1_gain].translate_table = gPEQ_Gain_Table;
    output_settings[front_eq_band1_gain].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band1_frequency].serial_cmd_name = "front_eq_band1_frequency";
    output_settings[front_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band1]);
    output_settings[front_eq_band1_frequency].cmd_default_value = 0x0c;
    output_settings[front_eq_band1_frequency].effective_arg_idx = 0;
    output_settings[front_eq_band1_frequency].profile_xml_name = "EQData1_Freq";
    output_settings[front_eq_band1_frequency].profile_value_offset = -2;
    output_settings[front_eq_band1_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[front_eq_band1_frequency].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band1_q].serial_cmd_name = "front_eq_band1_q";
    output_settings[front_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band1]);
    output_settings[front_eq_band1_q].cmd_default_value = 0x07;
    output_settings[front_eq_band1_q].effective_arg_idx = 1;
    output_settings[front_eq_band1_q].profile_xml_name = "EQData1_Q";
    output_settings[front_eq_band1_q].profile_value_offset = 0;
    output_settings[front_eq_band1_q].translate_table = gPEQ_Q_Table;
    output_settings[front_eq_band1_q].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band2_gain].serial_cmd_name = "front_eq_band2_gain";
    output_settings[front_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band2]);
    output_settings[front_eq_band2_gain].cmd_default_value = 0x40;
    output_settings[front_eq_band2_gain].effective_arg_idx = 2;
    output_settings[front_eq_band2_gain].profile_xml_name = "EQData2_Gain";
    output_settings[front_eq_band2_gain].profile_value_offset = -4;
    output_settings[front_eq_band2_gain].translate_table = gPEQ_Gain_Table;
    output_settings[front_eq_band2_gain].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band2_frequency].serial_cmd_name = "front_eq_band2_frequency";
    output_settings[front_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band2]);
    output_settings[front_eq_band2_frequency].cmd_default_value = 0x12;
    output_settings[front_eq_band2_frequency].effective_arg_idx = 0;
    output_settings[front_eq_band2_frequency].profile_xml_name = "EQData2_Freq";
    output_settings[front_eq_band2_frequency].profile_value_offset = -2;
    output_settings[front_eq_band2_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[front_eq_band2_frequency].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band2_q].serial_cmd_name = "front_eq_band2_q";
    output_settings[front_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band2]);
    output_settings[front_eq_band2_q].cmd_default_value = 0x07;
    output_settings[front_eq_band2_q].effective_arg_idx = 1;
    output_settings[front_eq_band2_q].profile_xml_name = "EQData2_Q";
    output_settings[front_eq_band2_q].profile_value_offset = 0;
    output_settings[front_eq_band2_q].translate_table = gPEQ_Q_Table;
    output_settings[front_eq_band2_q].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band3_gain].serial_cmd_name = "front_eq_band3_gain";
    output_settings[front_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band3]);
    output_settings[front_eq_band3_gain].cmd_default_value = 0x40;
    output_settings[front_eq_band3_gain].effective_arg_idx = 2;
    output_settings[front_eq_band3_gain].profile_xml_name = "EQData3_Gain";
    output_settings[front_eq_band3_gain].profile_value_offset = -4;
    output_settings[front_eq_band3_gain].translate_table = gPEQ_Gain_Table;
    output_settings[front_eq_band3_gain].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band3_frequency].serial_cmd_name = "front_eq_band3_frequency";
    output_settings[front_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band3]);
    output_settings[front_eq_band3_frequency].cmd_default_value = 0x18;
    output_settings[front_eq_band3_frequency].effective_arg_idx = 0;
    output_settings[front_eq_band3_frequency].profile_xml_name = "EQData3_Freq";
    output_settings[front_eq_band3_frequency].profile_value_offset = -2;
    output_settings[front_eq_band3_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[front_eq_band3_frequency].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band3_q].serial_cmd_name = "front_eq_band3_q";
    output_settings[front_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band3]);
    output_settings[front_eq_band3_q].cmd_default_value = 0x07;
    output_settings[front_eq_band3_q].effective_arg_idx = 1;
    output_settings[front_eq_band3_q].profile_xml_name = "EQData3_Q";
    output_settings[front_eq_band3_q].profile_value_offset = 0;
    output_settings[front_eq_band3_q].translate_table = gPEQ_Q_Table;
    output_settings[front_eq_band3_q].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band4_gain].serial_cmd_name = "front_eq_band4_gain";
    output_settings[front_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band4]);
    output_settings[front_eq_band4_gain].cmd_default_value = 0x40;
    output_settings[front_eq_band4_gain].effective_arg_idx = 2;
    output_settings[front_eq_band4_gain].profile_xml_name = "EQData4_Gain";
    output_settings[front_eq_band4_gain].profile_value_offset = -4;
    output_settings[front_eq_band4_gain].translate_table = gPEQ_Gain_Table;
    output_settings[front_eq_band4_gain].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band4_frequency].serial_cmd_name = "front_eq_band4_frequency";
    output_settings[front_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band4]);
    output_settings[front_eq_band4_frequency].cmd_default_value = 0x1e;
    output_settings[front_eq_band4_frequency].effective_arg_idx = 0;
    output_settings[front_eq_band4_frequency].profile_xml_name = "EQData4_Freq";
    output_settings[front_eq_band4_frequency].profile_value_offset = -2;
    output_settings[front_eq_band4_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[front_eq_band4_frequency].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band4_q].serial_cmd_name = "front_eq_band4_q";
    output_settings[front_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band4]);
    output_settings[front_eq_band4_q].cmd_default_value = 0x07;
    output_settings[front_eq_band4_q].effective_arg_idx = 1;
    output_settings[front_eq_band4_q].profile_xml_name = "EQData4_Q";
    output_settings[front_eq_band4_q].profile_value_offset = 0;
    output_settings[front_eq_band4_q].translate_table = gPEQ_Q_Table;
    output_settings[front_eq_band4_q].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band5_type].serial_cmd_name = "front_eq_band5_type";
    output_settings[front_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_front]);
    output_settings[front_eq_band5_type].cmd_default_value = 0x00;
    output_settings[front_eq_band5_type].effective_arg_idx = 1;
    output_settings[front_eq_band5_type].profile_xml_name = "PEQRightEdge";
    output_settings[front_eq_band5_type].profile_value_offset = 0;
    output_settings[front_eq_band5_type].translate_table = NULL;
    output_settings[front_eq_band5_type].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band5_gain].serial_cmd_name = "front_eq_band5_gain";
    output_settings[front_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band5]);
    output_settings[front_eq_band5_gain].cmd_default_value = 0x40;
    output_settings[front_eq_band5_gain].effective_arg_idx = 2;
    output_settings[front_eq_band5_gain].profile_xml_name = "EQData5_Gain";
    output_settings[front_eq_band5_gain].profile_value_offset = -4;
    output_settings[front_eq_band5_gain].translate_table = gPEQ_Gain_Table;
    output_settings[front_eq_band5_gain].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band5_frequency].serial_cmd_name = "front_eq_band5_frequency";
    output_settings[front_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band5]);
    output_settings[front_eq_band5_frequency].cmd_default_value = 0x24;
    output_settings[front_eq_band5_frequency].effective_arg_idx = 0;
    output_settings[front_eq_band5_frequency].profile_xml_name = "EQData5_Freq";
    output_settings[front_eq_band5_frequency].profile_value_offset = -2;
    output_settings[front_eq_band5_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[front_eq_band5_frequency].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_band5_q].serial_cmd_name = "front_eq_band5_q";
    output_settings[front_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_front_band5]);
    output_settings[front_eq_band5_q].cmd_default_value = 0x07;
    output_settings[front_eq_band5_q].effective_arg_idx = 1;
    output_settings[front_eq_band5_q].profile_xml_name = "EQData5_Q";
    output_settings[front_eq_band5_q].profile_value_offset = 0;
    output_settings[front_eq_band5_q].translate_table = gPEQ_Q_Table;
    output_settings[front_eq_band5_q].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_hpf_type].serial_cmd_name = "front_eq_hpf_type";
    output_settings[front_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_front]);
    output_settings[front_eq_hpf_type].cmd_default_value = 0x06;
    output_settings[front_eq_hpf_type].effective_arg_idx = 0;
    output_settings[front_eq_hpf_type].profile_xml_name = "EQData0_Slope";
    output_settings[front_eq_hpf_type].profile_value_offset = 0;
    output_settings[front_eq_hpf_type].translate_table = NULL;
    output_settings[front_eq_hpf_type].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_hpf_frequency].serial_cmd_name = "front_eq_hpf_frequency";
    output_settings[front_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_front]);
    output_settings[front_eq_hpf_frequency].cmd_default_value = 0x00;
    output_settings[front_eq_hpf_frequency].effective_arg_idx = 1;
    output_settings[front_eq_hpf_frequency].profile_xml_name = "EQData0_Freq";
    output_settings[front_eq_hpf_frequency].profile_value_offset = 0;
    output_settings[front_eq_hpf_frequency].translate_table = gHighPass_Freq_Table;
    output_settings[front_eq_hpf_frequency].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_lpf_type].serial_cmd_name = "front_eq_lpf_type";
    output_settings[front_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_front]);
    output_settings[front_eq_lpf_type].cmd_default_value = 0x06;
    output_settings[front_eq_lpf_type].effective_arg_idx = 2;
    output_settings[front_eq_lpf_type].profile_xml_name = "EQData6_Slope";
    output_settings[front_eq_lpf_type].profile_value_offset = 0;
    output_settings[front_eq_lpf_type].translate_table = NULL;
    output_settings[front_eq_lpf_type].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_eq_lpf_frequency].serial_cmd_name = "front_eq_lpf_frequency";
    output_settings[front_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_front]);
    output_settings[front_eq_lpf_frequency].cmd_default_value = 0x6f;
    output_settings[front_eq_lpf_frequency].effective_arg_idx = 3;
    output_settings[front_eq_lpf_frequency].profile_xml_name = "EQData6_Freq";
    output_settings[front_eq_lpf_frequency].profile_value_offset = 0;
    output_settings[front_eq_lpf_frequency].translate_table = gLowPass_Freq_Table;
    output_settings[front_eq_lpf_frequency].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_delay_time].serial_cmd_name = "front_delay_time";
    output_settings[front_delay_time].effect_api = &(api_table[WSD_AL_AE_OutputSetup_front]);
    output_settings[front_delay_time].cmd_default_value = 0x0;
    output_settings[front_delay_time].effective_arg_idx = 1;
    output_settings[front_delay_time].profile_xml_name = "DelayTime";
    output_settings[front_delay_time].profile_value_offset = 0;
    output_settings[front_delay_time].translate_table = NULL;
    output_settings[front_delay_time].profile_xml_node = FRONT_NODE_NAME;

    output_settings[front_phase].serial_cmd_name = "front_phase";
    output_settings[front_phase].effect_api = &(api_table[WSD_AL_AE_OutputSetup_front]);
    output_settings[front_phase].cmd_default_value = 0x0;
    output_settings[front_phase].effective_arg_idx = 2;
    output_settings[front_phase].profile_xml_name = "PhaseInvert";
    output_settings[front_phase].profile_value_offset = 0;
    output_settings[front_phase].translate_table = NULL;
    output_settings[front_phase].profile_xml_node = FRONT_NODE_NAME;

    output_settings[rear_eq_switch].serial_cmd_name = "rear_eq_switch";
    output_settings[rear_eq_switch].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEnable_rear]);
    output_settings[rear_eq_switch].cmd_default_value = 0x00;
    output_settings[rear_eq_switch].effective_arg_idx = 0;
    output_settings[rear_eq_switch].profile_xml_name = "EQEnable";
    output_settings[rear_eq_switch].profile_value_offset = 0;
    output_settings[rear_eq_switch].translate_table = NULL;
    output_settings[rear_eq_switch].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band1_type].serial_cmd_name = "rear_eq_band1_type";
    output_settings[rear_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_rear]);
    output_settings[rear_eq_band1_type].cmd_default_value = 0x00;
    output_settings[rear_eq_band1_type].effective_arg_idx = 0;
    output_settings[rear_eq_band1_type].profile_xml_name = "PEQLeftEdge";
    output_settings[rear_eq_band1_type].profile_value_offset = 0;
    output_settings[rear_eq_band1_type].translate_table = NULL;
    output_settings[rear_eq_band1_type].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band1_gain].serial_cmd_name = "rear_eq_band1_gain";
    output_settings[rear_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band1]);
    output_settings[rear_eq_band1_gain].cmd_default_value = 0x40;
    output_settings[rear_eq_band1_gain].effective_arg_idx = 2;
    output_settings[rear_eq_band1_gain].profile_xml_name = "EQData1_Gain";
    output_settings[rear_eq_band1_gain].profile_value_offset = -4;
    output_settings[rear_eq_band1_gain].translate_table = gPEQ_Gain_Table;
    output_settings[rear_eq_band1_gain].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band1_frequency].serial_cmd_name = "rear_eq_band1_frequency";
    output_settings[rear_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band1]);
    output_settings[rear_eq_band1_frequency].cmd_default_value = 0x0c;
    output_settings[rear_eq_band1_frequency].effective_arg_idx = 0;
    output_settings[rear_eq_band1_frequency].profile_xml_name = "EQData1_Freq";
    output_settings[rear_eq_band1_frequency].profile_value_offset = -2;
    output_settings[rear_eq_band1_frequency].translate_table = NULL;
    output_settings[rear_eq_band1_frequency].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band1_q].serial_cmd_name = "rear_eq_band1_q";
    output_settings[rear_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band1]);
    output_settings[rear_eq_band1_q].cmd_default_value = 0x07;
    output_settings[rear_eq_band1_q].effective_arg_idx = 1;
    output_settings[rear_eq_band1_q].profile_xml_name = "EQData1_Q";
    output_settings[rear_eq_band1_q].profile_value_offset = 0;
    output_settings[rear_eq_band1_q].translate_table = gPEQ_Q_Table;
    output_settings[rear_eq_band1_q].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band2_gain].serial_cmd_name = "rear_eq_band2_gain";
    output_settings[rear_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band2]);
    output_settings[rear_eq_band2_gain].cmd_default_value = 0x40;
    output_settings[rear_eq_band2_gain].effective_arg_idx = 2;
    output_settings[rear_eq_band2_gain].profile_xml_name = "EQData2_Gain";
    output_settings[rear_eq_band2_gain].profile_value_offset = -4;
    output_settings[rear_eq_band2_gain].translate_table = gPEQ_Gain_Table;
    output_settings[rear_eq_band2_gain].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band2_frequency].serial_cmd_name = "rear_eq_band2_frequency";
    output_settings[rear_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band2]);
    output_settings[rear_eq_band2_frequency].cmd_default_value = 0x12;
    output_settings[rear_eq_band2_frequency].effective_arg_idx = 0;
    output_settings[rear_eq_band2_frequency].profile_xml_name = "EQData2_Freq";
    output_settings[rear_eq_band2_frequency].profile_value_offset = -2;
    output_settings[rear_eq_band2_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[rear_eq_band2_frequency].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band2_q].serial_cmd_name = "rear_eq_band2_q";
    output_settings[rear_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band2]);
    output_settings[rear_eq_band2_q].cmd_default_value = 0x07;
    output_settings[rear_eq_band2_q].effective_arg_idx = 1;
    output_settings[rear_eq_band2_q].profile_xml_name = "EQData2_Q";
    output_settings[rear_eq_band2_q].profile_value_offset = 0;
    output_settings[rear_eq_band2_q].translate_table = gPEQ_Q_Table;
    output_settings[rear_eq_band2_q].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band3_gain].serial_cmd_name = "rear_eq_band3_gain";
    output_settings[rear_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band3]);
    output_settings[rear_eq_band3_gain].cmd_default_value = 0x40;
    output_settings[rear_eq_band3_gain].effective_arg_idx = 2;
    output_settings[rear_eq_band3_gain].profile_xml_name = "EQData3_Gain";
    output_settings[rear_eq_band3_gain].profile_value_offset = -4;
    output_settings[rear_eq_band3_gain].translate_table = gPEQ_Gain_Table;
    output_settings[rear_eq_band3_gain].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band3_frequency].serial_cmd_name = "rear_eq_band3_frequency";
    output_settings[rear_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band3]);
    output_settings[rear_eq_band3_frequency].cmd_default_value = 0x18;
    output_settings[rear_eq_band3_frequency].effective_arg_idx = 0;
    output_settings[rear_eq_band3_frequency].profile_xml_name = "EQData3_Freq";
    output_settings[rear_eq_band3_frequency].profile_value_offset = -2;
    output_settings[rear_eq_band3_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[rear_eq_band3_frequency].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band3_q].serial_cmd_name = "rear_eq_band3_q";
    output_settings[rear_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band3]);
    output_settings[rear_eq_band3_q].cmd_default_value = 0x07;
    output_settings[rear_eq_band3_q].effective_arg_idx = 1;
    output_settings[rear_eq_band3_q].profile_xml_name = "EQData3_Q";
    output_settings[rear_eq_band3_q].profile_value_offset = 0;
    output_settings[rear_eq_band3_q].translate_table = gPEQ_Q_Table;
    output_settings[rear_eq_band3_q].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band4_gain].serial_cmd_name = "rear_eq_band4_gain";
    output_settings[rear_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band4]);
    output_settings[rear_eq_band4_gain].cmd_default_value = 0x40;
    output_settings[rear_eq_band4_gain].effective_arg_idx = 2;
    output_settings[rear_eq_band4_gain].profile_xml_name = "EQData4_Gain";
    output_settings[rear_eq_band4_gain].profile_value_offset = -4;
    output_settings[rear_eq_band4_gain].translate_table = gPEQ_Gain_Table;
    output_settings[rear_eq_band4_gain].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band4_frequency].serial_cmd_name = "rear_eq_band4_frequency";
    output_settings[rear_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band4]);
    output_settings[rear_eq_band4_frequency].cmd_default_value = 0x1e;
    output_settings[rear_eq_band4_frequency].effective_arg_idx = 0;
    output_settings[rear_eq_band4_frequency].profile_xml_name = "EQData4_Freq";
    output_settings[rear_eq_band4_frequency].profile_value_offset = -2;
    output_settings[rear_eq_band4_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[rear_eq_band4_frequency].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band4_q].serial_cmd_name = "rear_eq_band4_q";
    output_settings[rear_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band4]);
    output_settings[rear_eq_band4_q].cmd_default_value = 0x07;
    output_settings[rear_eq_band4_q].effective_arg_idx = 1;
    output_settings[rear_eq_band4_q].profile_xml_name = "EQData4_Q";
    output_settings[rear_eq_band4_q].profile_value_offset = 0;
    output_settings[rear_eq_band4_q].translate_table = gPEQ_Q_Table;
    output_settings[rear_eq_band4_q].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band5_type].serial_cmd_name = "rear_eq_band5_type";
    output_settings[rear_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_rear]);
    output_settings[rear_eq_band5_type].cmd_default_value = 0x00;
    output_settings[rear_eq_band5_type].effective_arg_idx = 1;
    output_settings[rear_eq_band5_type].profile_xml_name = "PEQRightEdge";
    output_settings[rear_eq_band5_type].profile_value_offset = 0;
    output_settings[rear_eq_band5_type].translate_table = NULL;
    output_settings[rear_eq_band5_type].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band5_gain].serial_cmd_name = "rear_eq_band5_gain";
    output_settings[rear_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band5]);
    output_settings[rear_eq_band5_gain].cmd_default_value = 0x40;
    output_settings[rear_eq_band5_gain].effective_arg_idx = 2;
    output_settings[rear_eq_band5_gain].profile_xml_name = "EQData5_Gain";
    output_settings[rear_eq_band5_gain].profile_value_offset = -4;
    output_settings[rear_eq_band5_gain].translate_table = gPEQ_Gain_Table;
    output_settings[rear_eq_band5_gain].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band5_frequency].serial_cmd_name = "rear_eq_band5_frequency";
    output_settings[rear_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band5]);
    output_settings[rear_eq_band5_frequency].cmd_default_value = 0x24;
    output_settings[rear_eq_band5_frequency].effective_arg_idx = 0;
    output_settings[rear_eq_band5_frequency].profile_xml_name = "EQData5_Freq";
    output_settings[rear_eq_band5_frequency].profile_value_offset = -2;
    output_settings[rear_eq_band5_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[rear_eq_band5_frequency].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_band5_q].serial_cmd_name = "rear_eq_band5_q";
    output_settings[rear_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band5]);
    output_settings[rear_eq_band5_q].cmd_default_value = 0x07;
    output_settings[rear_eq_band5_q].effective_arg_idx = 1;
    output_settings[rear_eq_band5_q].profile_xml_name = "EQData5_Q";
    output_settings[rear_eq_band5_q].profile_value_offset = 0;
    output_settings[rear_eq_band5_q].translate_table = gPEQ_Q_Table;
    output_settings[rear_eq_band5_q].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_hpf_type].serial_cmd_name = "rear_eq_hpf_type";
    output_settings[rear_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_rear]);
    output_settings[rear_eq_hpf_type].cmd_default_value = 0x06;
    output_settings[rear_eq_hpf_type].effective_arg_idx = 0;
    output_settings[rear_eq_hpf_type].profile_xml_name = "EQData0_Slope";
    output_settings[rear_eq_hpf_type].profile_value_offset = 0;
    output_settings[rear_eq_hpf_type].translate_table = NULL;
    output_settings[rear_eq_hpf_type].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_hpf_frequency].serial_cmd_name = "rear_eq_hpf_frequency";
    output_settings[rear_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_rear]);
    output_settings[rear_eq_hpf_frequency].cmd_default_value = 0x00;
    output_settings[rear_eq_hpf_frequency].effective_arg_idx = 1;
    output_settings[rear_eq_hpf_frequency].profile_xml_name = "EQData0_Freq";
    output_settings[rear_eq_hpf_frequency].profile_value_offset = 0;
    output_settings[rear_eq_hpf_frequency].translate_table = gHighPass_Freq_Table;
    output_settings[rear_eq_hpf_frequency].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_lpf_type].serial_cmd_name = "rear_eq_lpf_type";
    output_settings[rear_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_rear]);
    output_settings[rear_eq_lpf_type].cmd_default_value = 0x06;
    output_settings[rear_eq_lpf_type].effective_arg_idx = 2;
    output_settings[rear_eq_lpf_type].profile_xml_name = "EQData6_Slope";
    output_settings[rear_eq_lpf_type].profile_value_offset = 0;
    output_settings[rear_eq_lpf_type].translate_table = NULL;
    output_settings[rear_eq_lpf_type].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_eq_lpf_frequency].serial_cmd_name = "rear_eq_lpf_frequency";
    output_settings[rear_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_rear]);
    output_settings[rear_eq_lpf_frequency].cmd_default_value = 0x6f;
    output_settings[rear_eq_lpf_frequency].effective_arg_idx = 3;
    output_settings[rear_eq_lpf_frequency].profile_xml_name = "EQData6_Freq";
    output_settings[rear_eq_lpf_frequency].profile_value_offset = 0;
    output_settings[rear_eq_lpf_frequency].translate_table = gLowPass_Freq_Table;
    output_settings[rear_eq_lpf_frequency].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_delay_time].serial_cmd_name = "rear_delay_time";
    output_settings[rear_delay_time].effect_api = &(api_table[WSD_AL_AE_OutputSetup_rear]);
    output_settings[rear_delay_time].cmd_default_value = 0x0;
    output_settings[rear_delay_time].effective_arg_idx = 1;
    output_settings[rear_delay_time].profile_xml_name = "DelayTime";
    output_settings[rear_delay_time].profile_value_offset = 0;
    output_settings[rear_delay_time].translate_table = NULL;
    output_settings[rear_delay_time].profile_xml_node = REAR_NODE_NAME;

    output_settings[rear_phase].serial_cmd_name = "rear_phase";
    output_settings[rear_phase].effect_api = &(api_table[WSD_AL_AE_OutputSetup_rear]);
    output_settings[rear_phase].cmd_default_value = 0x0;
    output_settings[rear_phase].effective_arg_idx = 2;
    output_settings[rear_phase].profile_xml_name = "PhaseInvert";
    output_settings[rear_phase].profile_value_offset = 0;
    output_settings[rear_phase].translate_table = NULL;
    output_settings[rear_phase].profile_xml_node = REAR_NODE_NAME;

    output_settings[center_eq_switch].serial_cmd_name = "center_eq_switch";
    output_settings[center_eq_switch].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEnable_center]);
    output_settings[center_eq_switch].cmd_default_value = 0x00;
    output_settings[center_eq_switch].effective_arg_idx = 0;
    output_settings[center_eq_switch].profile_xml_name = "EQEnable";
    output_settings[center_eq_switch].profile_value_offset = 0;
    output_settings[center_eq_switch].translate_table = NULL;
    output_settings[center_eq_switch].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band1_type].serial_cmd_name = "center_eq_band1_type";
    output_settings[center_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_center]);
    output_settings[center_eq_band1_type].cmd_default_value = 0x00;
    output_settings[center_eq_band1_type].effective_arg_idx = 0;
    output_settings[center_eq_band1_type].profile_xml_name = "PEQLeftEdge";
    output_settings[center_eq_band1_type].profile_value_offset = 0;
    output_settings[center_eq_band1_type].translate_table = NULL;
    output_settings[center_eq_band1_type].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band1_gain].serial_cmd_name = "center_eq_band1_gain";
    output_settings[center_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band1]);
    output_settings[center_eq_band1_gain].cmd_default_value = 0x40;
    output_settings[center_eq_band1_gain].effective_arg_idx = 2;
    output_settings[center_eq_band1_gain].profile_xml_name = "EQData1_Gain";
    output_settings[center_eq_band1_gain].profile_value_offset = -4;
    output_settings[center_eq_band1_gain].translate_table = gPEQ_Gain_Table;
    output_settings[center_eq_band1_gain].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band1_frequency].serial_cmd_name = "center_eq_band1_frequency";
    output_settings[center_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band1]);
    output_settings[center_eq_band1_frequency].cmd_default_value = 0x0c;
    output_settings[center_eq_band1_frequency].effective_arg_idx = 0;
    output_settings[center_eq_band1_frequency].profile_xml_name = "EQData1_Freq";
    output_settings[center_eq_band1_frequency].profile_value_offset = -2;
    output_settings[center_eq_band1_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[center_eq_band1_frequency].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band1_q].serial_cmd_name = "center_eq_band1_q";
    output_settings[center_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band1]);
    output_settings[center_eq_band1_q].cmd_default_value = 0x07;
    output_settings[center_eq_band1_q].effective_arg_idx = 1;
    output_settings[center_eq_band1_q].profile_xml_name = "EQData1_Q";
    output_settings[center_eq_band1_q].profile_value_offset = 0;
    output_settings[center_eq_band1_q].translate_table = gPEQ_Q_Table;
    output_settings[center_eq_band1_q].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band2_gain].serial_cmd_name = "center_eq_band2_gain";
    output_settings[center_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band2]);
    output_settings[center_eq_band2_gain].cmd_default_value = 0x40;
    output_settings[center_eq_band2_gain].effective_arg_idx = 2;
    output_settings[center_eq_band2_gain].profile_xml_name = "EQData2_Gain";
    output_settings[center_eq_band2_gain].profile_value_offset = -4;
    output_settings[center_eq_band2_gain].translate_table = gPEQ_Gain_Table;
    output_settings[center_eq_band2_gain].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band2_frequency].serial_cmd_name = "center_eq_band2_frequency";
    output_settings[center_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band2]);
    output_settings[center_eq_band2_frequency].cmd_default_value = 0x12;
    output_settings[center_eq_band2_frequency].effective_arg_idx = 0;
    output_settings[center_eq_band2_frequency].profile_xml_name = "EQData2_Freq";
    output_settings[center_eq_band2_frequency].profile_value_offset = -2;
    output_settings[center_eq_band2_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[center_eq_band2_frequency].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band2_q].serial_cmd_name = "center_eq_band2_q";
    output_settings[center_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band2]);
    output_settings[center_eq_band2_q].cmd_default_value = 0x07;
    output_settings[center_eq_band2_q].effective_arg_idx = 1;
    output_settings[center_eq_band2_q].profile_xml_name = "EQData2_Q";
    output_settings[center_eq_band2_q].profile_value_offset = 0;
    output_settings[center_eq_band2_q].translate_table = gPEQ_Q_Table;
    output_settings[center_eq_band2_q].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band3_gain].serial_cmd_name = "center_eq_band3_gain";
    output_settings[center_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band3]);
    output_settings[center_eq_band3_gain].cmd_default_value = 0x40;
    output_settings[center_eq_band3_gain].effective_arg_idx = 2;
    output_settings[center_eq_band3_gain].profile_xml_name = "EQData3_Gain";
    output_settings[center_eq_band3_gain].profile_value_offset = -4;
    output_settings[center_eq_band3_gain].translate_table = gPEQ_Gain_Table;
    output_settings[center_eq_band3_gain].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band3_frequency].serial_cmd_name = "center_eq_band3_frequency";
    output_settings[center_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band3]);
    output_settings[center_eq_band3_frequency].cmd_default_value = 0x18;
    output_settings[center_eq_band3_frequency].effective_arg_idx = 0;
    output_settings[center_eq_band3_frequency].profile_xml_name = "EQData3_Freq";
    output_settings[center_eq_band3_frequency].profile_value_offset = -2;
    output_settings[center_eq_band3_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[center_eq_band3_frequency].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band3_q].serial_cmd_name = "center_eq_band3_q";
    output_settings[center_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band3]);
    output_settings[center_eq_band3_q].cmd_default_value = 0x07;
    output_settings[center_eq_band3_q].effective_arg_idx = 1;
    output_settings[center_eq_band3_q].profile_xml_name = "EQData3_Q";
    output_settings[center_eq_band3_q].profile_value_offset = 0;
    output_settings[center_eq_band3_q].translate_table = gPEQ_Q_Table;
    output_settings[center_eq_band3_q].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band4_gain].serial_cmd_name = "center_eq_band4_gain";
    output_settings[center_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band4]);
    output_settings[center_eq_band4_gain].cmd_default_value = 0x40;
    output_settings[center_eq_band4_gain].effective_arg_idx = 2;
    output_settings[center_eq_band4_gain].profile_xml_name = "EQData4_Gain";
    output_settings[center_eq_band4_gain].profile_value_offset = -4;
    output_settings[center_eq_band4_gain].translate_table = gPEQ_Gain_Table;
    output_settings[center_eq_band4_gain].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band4_frequency].serial_cmd_name = "center_eq_band4_frequency";
    output_settings[center_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band4]);
    output_settings[center_eq_band4_frequency].cmd_default_value = 0x1e;
    output_settings[center_eq_band4_frequency].effective_arg_idx = 0;
    output_settings[center_eq_band4_frequency].profile_xml_name = "EQData4_Freq";
    output_settings[center_eq_band4_frequency].profile_value_offset = -2;
    output_settings[center_eq_band4_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[center_eq_band4_frequency].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band4_q].serial_cmd_name = "center_eq_band4_q";
    output_settings[center_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band4]);
    output_settings[center_eq_band4_q].cmd_default_value = 0x07;
    output_settings[center_eq_band4_q].effective_arg_idx = 1;
    output_settings[center_eq_band4_q].profile_xml_name = "EQData4_Q";
    output_settings[center_eq_band4_q].profile_value_offset = 0;
    output_settings[center_eq_band4_q].translate_table = gPEQ_Q_Table;
    output_settings[center_eq_band4_q].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band5_type].serial_cmd_name = "center_eq_band5_type";
    output_settings[center_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_center]);
    output_settings[center_eq_band5_type].cmd_default_value = 0x00;
    output_settings[center_eq_band5_type].effective_arg_idx = 1;
    output_settings[center_eq_band5_type].profile_xml_name = "PEQRightEdge";
    output_settings[center_eq_band5_type].profile_value_offset = 0;
    output_settings[center_eq_band5_type].translate_table = NULL;
    output_settings[center_eq_band5_type].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band5_gain].serial_cmd_name = "center_eq_band5_gain";
    output_settings[center_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band5]);
    output_settings[center_eq_band5_gain].cmd_default_value = 0x40;
    output_settings[center_eq_band5_gain].effective_arg_idx = 2;
    output_settings[center_eq_band5_gain].profile_xml_name = "EQData5_Gain";
    output_settings[center_eq_band5_gain].profile_value_offset = -4;
    output_settings[center_eq_band5_gain].translate_table = gPEQ_Gain_Table;
    output_settings[center_eq_band5_gain].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band5_frequency].serial_cmd_name = "center_eq_band5_frequency";
    output_settings[center_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band5]);
    output_settings[center_eq_band5_frequency].cmd_default_value = 0x24;
    output_settings[center_eq_band5_frequency].effective_arg_idx = 0;
    output_settings[center_eq_band5_frequency].profile_xml_name = "EQData5_Freq";
    output_settings[center_eq_band5_frequency].profile_value_offset = -2;
    output_settings[center_eq_band5_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[center_eq_band5_frequency].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_band5_q].serial_cmd_name = "center_eq_band5_q";
    output_settings[center_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_center_band5]);
    output_settings[center_eq_band5_q].cmd_default_value = 0x07;
    output_settings[center_eq_band5_q].effective_arg_idx = 1;
    output_settings[center_eq_band5_q].profile_xml_name = "EQData5_Q";
    output_settings[center_eq_band5_q].profile_value_offset = 0;
    output_settings[center_eq_band5_q].translate_table = gPEQ_Q_Table;
    output_settings[center_eq_band5_q].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_hpf_type].serial_cmd_name = "center_eq_hpf_type";
    output_settings[center_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_center]);
    output_settings[center_eq_hpf_type].cmd_default_value = 0x06;
    output_settings[center_eq_hpf_type].effective_arg_idx = 0;
    output_settings[center_eq_hpf_type].profile_xml_name = "EQData0_Slope";
    output_settings[center_eq_hpf_type].profile_value_offset = 0;
    output_settings[center_eq_hpf_type].translate_table = NULL;
    output_settings[center_eq_hpf_type].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_hpf_frequency].serial_cmd_name = "center_eq_hpf_frequency";
    output_settings[center_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_center]);
    output_settings[center_eq_hpf_frequency].cmd_default_value = 0x00;
    output_settings[center_eq_hpf_frequency].effective_arg_idx = 1;
    output_settings[center_eq_hpf_frequency].profile_xml_name = "EQData0_Freq";
    output_settings[center_eq_hpf_frequency].profile_value_offset = 0;
    output_settings[center_eq_hpf_frequency].translate_table = gHighPass_Freq_Table;
    output_settings[center_eq_hpf_frequency].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_lpf_type].serial_cmd_name = "center_eq_lpf_type";
    output_settings[center_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_center]);
    output_settings[center_eq_lpf_type].cmd_default_value = 0x06;
    output_settings[center_eq_lpf_type].effective_arg_idx = 2;
    output_settings[center_eq_lpf_type].profile_xml_name = "EQData6_Slope";
    output_settings[center_eq_lpf_type].profile_value_offset = 0;
    output_settings[center_eq_lpf_type].translate_table = NULL;
    output_settings[center_eq_lpf_type].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_eq_lpf_frequency].serial_cmd_name = "center_eq_lpf_frequency";
    output_settings[center_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_center]);
    output_settings[center_eq_lpf_frequency].cmd_default_value = 0x6f;
    output_settings[center_eq_lpf_frequency].effective_arg_idx = 3;
    output_settings[center_eq_lpf_frequency].profile_xml_name = "EQData0_Freq";
    output_settings[center_eq_lpf_frequency].profile_value_offset = 0;
    output_settings[center_eq_lpf_frequency].translate_table = gLowPass_Freq_Table;
    output_settings[center_eq_lpf_frequency].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_delay_time].serial_cmd_name = "center_delay_time";
    output_settings[center_delay_time].effect_api = &(api_table[WSD_AL_AE_OutputSetup_center]);
    output_settings[center_delay_time].cmd_default_value = 0x0;
    output_settings[center_delay_time].effective_arg_idx = 1;
    output_settings[center_delay_time].profile_xml_name = "DelayTime";
    output_settings[center_delay_time].profile_value_offset = 0;
    output_settings[center_delay_time].translate_table = NULL;
    output_settings[center_delay_time].profile_xml_node = CENTER_NODE_NAME;

    output_settings[center_phase].serial_cmd_name = "center_phase";
    output_settings[center_phase].effect_api = &(api_table[WSD_AL_AE_OutputSetup_center]);
    output_settings[center_phase].cmd_default_value = 0x0;
    output_settings[center_phase].effective_arg_idx = 2;
    output_settings[center_phase].profile_xml_name = "PhaseInvert";
    output_settings[center_phase].profile_value_offset = 0;
    output_settings[center_phase].translate_table = NULL;
    output_settings[center_phase].profile_xml_node = CENTER_NODE_NAME;

    output_settings[subwoofer_eq_switch].serial_cmd_name = "subwoofer_eq_switch";
    output_settings[subwoofer_eq_switch].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEnable_subwoofer]);
    output_settings[subwoofer_eq_switch].cmd_default_value = 0x01;
    output_settings[subwoofer_eq_switch].effective_arg_idx = 0;
    output_settings[subwoofer_eq_switch].profile_xml_name = "EQEnable";
    output_settings[subwoofer_eq_switch].profile_value_offset = 0;
    output_settings[subwoofer_eq_switch].translate_table = NULL;
    output_settings[subwoofer_eq_switch].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band1_type].serial_cmd_name = "subwoofer_eq_band1_type";
    output_settings[subwoofer_eq_band1_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_subwoofer]);
    output_settings[subwoofer_eq_band1_type].cmd_default_value = 0x00;
    output_settings[subwoofer_eq_band1_type].effective_arg_idx = 0;
    output_settings[subwoofer_eq_band1_type].profile_xml_name = "PEQLeftEdge";
    output_settings[subwoofer_eq_band1_type].profile_value_offset = 0;
    output_settings[subwoofer_eq_band1_type].translate_table = NULL;
    output_settings[subwoofer_eq_band1_type].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band1_gain].serial_cmd_name = "subwoofer_eq_band1_gain";
    output_settings[subwoofer_eq_band1_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band1]);
    output_settings[subwoofer_eq_band1_gain].cmd_default_value = 0x40;
    output_settings[subwoofer_eq_band1_gain].effective_arg_idx = 2;
    output_settings[subwoofer_eq_band1_gain].profile_xml_name = "EQData1_Gain";
    output_settings[subwoofer_eq_band1_gain].profile_value_offset = -4;
    output_settings[subwoofer_eq_band1_gain].translate_table = gPEQ_Gain_Table;
    output_settings[subwoofer_eq_band1_gain].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band1_frequency].serial_cmd_name = "subwoofer_eq_band1_frequency";
    output_settings[subwoofer_eq_band1_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band1]);
    output_settings[subwoofer_eq_band1_frequency].cmd_default_value = 0x6;
    output_settings[subwoofer_eq_band1_frequency].effective_arg_idx = 0;
    output_settings[subwoofer_eq_band1_frequency].profile_xml_name = "EQData1_Freq";
    output_settings[subwoofer_eq_band1_frequency].profile_value_offset = -2;
    output_settings[subwoofer_eq_band1_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[subwoofer_eq_band1_frequency].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band1_q].serial_cmd_name = "subwoofer_eq_band1_q";
    output_settings[subwoofer_eq_band1_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band1]);
    output_settings[subwoofer_eq_band1_q].cmd_default_value = 0x13;
    output_settings[subwoofer_eq_band1_q].effective_arg_idx = 1;
    output_settings[subwoofer_eq_band1_q].profile_xml_name = "EQData1_Q";
    output_settings[subwoofer_eq_band1_q].profile_value_offset = 0;
    output_settings[subwoofer_eq_band1_q].translate_table = gPEQ_Q_Table;
    output_settings[subwoofer_eq_band1_q].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band2_gain].serial_cmd_name = "subwoofer_eq_band2_gain";
    output_settings[subwoofer_eq_band2_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band2]);
    output_settings[subwoofer_eq_band2_gain].cmd_default_value = 0x40;
    output_settings[subwoofer_eq_band2_gain].effective_arg_idx = 2;
    output_settings[subwoofer_eq_band2_gain].profile_xml_name = "EQData2_Gain";
    output_settings[subwoofer_eq_band2_gain].profile_value_offset = -4;
    output_settings[subwoofer_eq_band2_gain].translate_table = gPEQ_Gain_Table;
    output_settings[subwoofer_eq_band2_gain].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band2_frequency].serial_cmd_name = "subwoofer_eq_band2_frequency";
    output_settings[subwoofer_eq_band2_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band2]);
    output_settings[subwoofer_eq_band2_frequency].cmd_default_value = 0x0a;
    output_settings[subwoofer_eq_band2_frequency].effective_arg_idx = 0;
    output_settings[subwoofer_eq_band2_frequency].profile_xml_name = "EQData2_Freq";
    output_settings[subwoofer_eq_band2_frequency].profile_value_offset = -2;
    output_settings[subwoofer_eq_band2_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[subwoofer_eq_band2_frequency].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band2_q].serial_cmd_name = "subwoofer_eq_band2_q";
    output_settings[subwoofer_eq_band2_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band2]);
    output_settings[subwoofer_eq_band2_q].cmd_default_value = 0x13;
    output_settings[subwoofer_eq_band2_q].effective_arg_idx = 1;
    output_settings[subwoofer_eq_band2_q].profile_xml_name = "EQData2_Q";
    output_settings[subwoofer_eq_band2_q].profile_value_offset = 0;
    output_settings[subwoofer_eq_band2_q].translate_table = gPEQ_Q_Table;
    output_settings[subwoofer_eq_band2_q].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band3_gain].serial_cmd_name = "subwoofer_eq_band3_gain";
    output_settings[subwoofer_eq_band3_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band3]);
    output_settings[subwoofer_eq_band3_gain].cmd_default_value = 0x40;
    output_settings[subwoofer_eq_band3_gain].effective_arg_idx = 2;
    output_settings[subwoofer_eq_band3_gain].profile_xml_name = "EQData2_Gain";
    output_settings[subwoofer_eq_band3_gain].profile_value_offset = -4;
    output_settings[subwoofer_eq_band3_gain].translate_table = gPEQ_Gain_Table;
    output_settings[subwoofer_eq_band3_gain].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band3_frequency].serial_cmd_name = "subwoofer_eq_band3_frequency";
    output_settings[subwoofer_eq_band3_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band3]);
    output_settings[subwoofer_eq_band3_frequency].cmd_default_value = 0x0d;
    output_settings[subwoofer_eq_band3_frequency].effective_arg_idx = 0;
    output_settings[subwoofer_eq_band3_frequency].profile_xml_name = "EQData2_Freq";
    output_settings[subwoofer_eq_band3_frequency].profile_value_offset = -2;
    output_settings[subwoofer_eq_band3_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[subwoofer_eq_band3_frequency].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band3_q].serial_cmd_name = "subwoofer_eq_band3_q";
    output_settings[subwoofer_eq_band3_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band3]);
    output_settings[subwoofer_eq_band3_q].cmd_default_value = 0x13;
    output_settings[subwoofer_eq_band3_q].effective_arg_idx = 1;
    output_settings[subwoofer_eq_band3_q].profile_xml_name = "EQData3_Q";
    output_settings[subwoofer_eq_band3_q].profile_value_offset = 0;
    output_settings[subwoofer_eq_band3_q].translate_table = gPEQ_Q_Table;
    output_settings[subwoofer_eq_band3_q].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band4_gain].serial_cmd_name = "subwoofer_eq_band4_gain";
    output_settings[subwoofer_eq_band4_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band4]);
    output_settings[subwoofer_eq_band4_gain].cmd_default_value = 0x40;
    output_settings[subwoofer_eq_band4_gain].effective_arg_idx = 2;
    output_settings[subwoofer_eq_band4_gain].profile_xml_name = "EQData4_Gain";
    output_settings[subwoofer_eq_band4_gain].profile_value_offset = -4;
    output_settings[subwoofer_eq_band4_gain].translate_table = gPEQ_Gain_Table;
    output_settings[subwoofer_eq_band4_gain].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band4_frequency].serial_cmd_name = "subwoofer_eq_band4_frequency";
    output_settings[subwoofer_eq_band4_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band4]);
    output_settings[subwoofer_eq_band4_frequency].cmd_default_value = 0x10;
    output_settings[subwoofer_eq_band4_frequency].effective_arg_idx = 0;
    output_settings[subwoofer_eq_band4_frequency].profile_xml_name = "EQData4_Freq";
    output_settings[subwoofer_eq_band4_frequency].profile_value_offset = -2;
    output_settings[subwoofer_eq_band4_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[subwoofer_eq_band4_frequency].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band4_q].serial_cmd_name = "subwoofer_eq_band4_q";
    output_settings[subwoofer_eq_band4_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band4]);
    output_settings[subwoofer_eq_band4_q].cmd_default_value = 0x13;
    output_settings[subwoofer_eq_band4_q].effective_arg_idx = 1;
    output_settings[subwoofer_eq_band4_q].profile_xml_name = "EQData4_Q";
    output_settings[subwoofer_eq_band4_q].profile_value_offset = 0;
    output_settings[subwoofer_eq_band4_q].translate_table = gPEQ_Q_Table;
    output_settings[subwoofer_eq_band4_q].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band5_type].serial_cmd_name = "subwoofer_eq_band5_type";
    output_settings[subwoofer_eq_band5_type].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_subwoofer]);
    output_settings[subwoofer_eq_band5_type].cmd_default_value = 0x00;
    output_settings[subwoofer_eq_band5_type].effective_arg_idx = 1;
    output_settings[subwoofer_eq_band5_type].profile_xml_name = "PEQRightEdge";
    output_settings[subwoofer_eq_band5_type].profile_value_offset = 0;
    output_settings[subwoofer_eq_band5_type].translate_table = NULL;
    output_settings[subwoofer_eq_band5_type].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band5_gain].serial_cmd_name = "subwoofer_eq_band5_gain";
    output_settings[subwoofer_eq_band5_gain].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band5]);
    output_settings[subwoofer_eq_band5_gain].cmd_default_value = 0x40;
    output_settings[subwoofer_eq_band5_gain].effective_arg_idx = 2;
    output_settings[subwoofer_eq_band5_gain].profile_xml_name = "EQData5_Gain";
    output_settings[subwoofer_eq_band5_gain].profile_value_offset = -4;
    output_settings[subwoofer_eq_band5_gain].translate_table = gPEQ_Gain_Table;
    output_settings[subwoofer_eq_band5_gain].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band5_frequency].serial_cmd_name = "subwoofer_eq_band5_frequency";
    output_settings[subwoofer_eq_band5_frequency].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band5]);
    output_settings[subwoofer_eq_band5_frequency].cmd_default_value = 0x14;
    output_settings[subwoofer_eq_band5_frequency].effective_arg_idx = 0;
    output_settings[subwoofer_eq_band5_frequency].profile_xml_name = "EQData5_Freq";
    output_settings[subwoofer_eq_band5_frequency].profile_value_offset = -2;
    output_settings[subwoofer_eq_band5_frequency].translate_table = gPEQ_Freq_Table;
    output_settings[subwoofer_eq_band5_frequency].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_band5_q].serial_cmd_name = "subwoofer_eq_band5_q";
    output_settings[subwoofer_eq_band5_q].effect_api = &(api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band5]);
    output_settings[subwoofer_eq_band5_q].cmd_default_value = 0x13;
    output_settings[subwoofer_eq_band5_q].effective_arg_idx = 1;
    output_settings[subwoofer_eq_band5_q].profile_xml_name = "EQData5_Q";
    output_settings[subwoofer_eq_band5_q].profile_value_offset = 0;
    output_settings[subwoofer_eq_band5_q].translate_table = gPEQ_Q_Table;
    output_settings[subwoofer_eq_band5_q].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_hpf_type].serial_cmd_name = "subwoofer_eq_hpf_type";
    output_settings[subwoofer_eq_hpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_subwoofer]);
    output_settings[subwoofer_eq_hpf_type].cmd_default_value = 0x06;
    output_settings[subwoofer_eq_hpf_type].effective_arg_idx = 0;
    output_settings[subwoofer_eq_hpf_type].profile_xml_name = "EQData0_Slope";
    output_settings[subwoofer_eq_hpf_type].profile_value_offset = 0;
    output_settings[subwoofer_eq_hpf_type].translate_table = NULL;
    output_settings[subwoofer_eq_hpf_type].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_hpf_frequency].serial_cmd_name = "subwoofer_eq_hpf_frequency";
    output_settings[subwoofer_eq_hpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_subwoofer]);
    output_settings[subwoofer_eq_hpf_frequency].cmd_default_value = 0x05;
    output_settings[subwoofer_eq_hpf_frequency].effective_arg_idx = 1;
    output_settings[subwoofer_eq_hpf_frequency].profile_xml_name = "EQData0_Freq";
    output_settings[subwoofer_eq_hpf_frequency].profile_value_offset = 0;
    output_settings[subwoofer_eq_hpf_frequency].translate_table = gHighPass_Freq_Table;
    output_settings[subwoofer_eq_hpf_frequency].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_lpf_type].serial_cmd_name = "subwoofer_eq_lpf_type";
    output_settings[subwoofer_eq_lpf_type].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_subwoofer]);
    output_settings[subwoofer_eq_lpf_type].cmd_default_value = 0x06;
    output_settings[subwoofer_eq_lpf_type].effective_arg_idx = 2;
    output_settings[subwoofer_eq_lpf_type].profile_xml_name = "EQData6_Slope";
    output_settings[subwoofer_eq_lpf_type].profile_value_offset = 0;
    output_settings[subwoofer_eq_lpf_type].translate_table = NULL;
    output_settings[subwoofer_eq_lpf_type].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_eq_lpf_frequency].serial_cmd_name = "subwoofer_eq_lpf_frequency";
    output_settings[subwoofer_eq_lpf_frequency].effect_api = &(api_table[WSD_AL_AE_OutputBandpassfilterSetup_subwoofer]);
    output_settings[subwoofer_eq_lpf_frequency].cmd_default_value = 0x16;
    output_settings[subwoofer_eq_lpf_frequency].effective_arg_idx = 3;
    output_settings[subwoofer_eq_lpf_frequency].profile_xml_name = "EQData6_Freq";
    output_settings[subwoofer_eq_lpf_frequency].profile_value_offset = 0;
    output_settings[subwoofer_eq_lpf_frequency].translate_table = gLowPass_Freq_Table;
    output_settings[subwoofer_eq_lpf_frequency].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_delay_time].serial_cmd_name = "subwoofer_delay_time";
    output_settings[subwoofer_delay_time].effect_api = &(api_table[WSD_AL_AE_OutputSetup_subwoofer]);
    output_settings[subwoofer_delay_time].cmd_default_value = 0x0;
    output_settings[subwoofer_delay_time].effective_arg_idx = 1;
    output_settings[subwoofer_delay_time].profile_xml_name = "DelayTime";
    output_settings[subwoofer_delay_time].profile_value_offset = 0;
    output_settings[subwoofer_delay_time].translate_table = NULL;
    output_settings[subwoofer_delay_time].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_phase].serial_cmd_name = "subwoofer_phase";
    output_settings[subwoofer_phase].effect_api = &(api_table[WSD_AL_AE_OutputSetup_subwoofer]);
    output_settings[subwoofer_phase].cmd_default_value = 0x0;
    output_settings[subwoofer_phase].effective_arg_idx = 2;
    output_settings[subwoofer_phase].profile_xml_name = "PhaseInvert";
    output_settings[subwoofer_phase].profile_value_offset = 0;
    output_settings[subwoofer_phase].translate_table = NULL;
    output_settings[subwoofer_phase].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_compressor_switch].serial_cmd_name = "subwoofer_compressor_switch";
    output_settings[subwoofer_compressor_switch].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorEnable]);
    output_settings[subwoofer_compressor_switch].cmd_default_value = 0x1;
    output_settings[subwoofer_compressor_switch].effective_arg_idx = 0;
    output_settings[subwoofer_compressor_switch].profile_xml_name = "CompressorEnable";
    output_settings[subwoofer_compressor_switch].profile_value_offset = 0;
    output_settings[subwoofer_compressor_switch].translate_table = NULL;
    output_settings[subwoofer_compressor_switch].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_compressor_attack].serial_cmd_name = "subwoofer_compressor_attack";
    output_settings[subwoofer_compressor_attack].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorSetup]);
    output_settings[subwoofer_compressor_attack].cmd_default_value = 0x50;
    output_settings[subwoofer_compressor_attack].effective_arg_idx = 0;
    output_settings[subwoofer_compressor_attack].profile_xml_name = "CompressorAttackTime";
    output_settings[subwoofer_compressor_attack].profile_value_offset = 0;
    output_settings[subwoofer_compressor_attack].translate_table = NULL;
    output_settings[subwoofer_compressor_attack].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_compressor_release].serial_cmd_name = "subwoofer_compressor_release";
    output_settings[subwoofer_compressor_release].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorSetup]);
    output_settings[subwoofer_compressor_release].cmd_default_value = 0x05;
    output_settings[subwoofer_compressor_release].effective_arg_idx = 1;
    output_settings[subwoofer_compressor_release].profile_xml_name = "CompressorReleaseTime";
    output_settings[subwoofer_compressor_release].profile_value_offset = 0;
    output_settings[subwoofer_compressor_release].translate_table = NULL;
    output_settings[subwoofer_compressor_release].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_compressor_threshold].serial_cmd_name = "subwoofer_compressor_threshold";
    output_settings[subwoofer_compressor_threshold].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorSetup]);
    output_settings[subwoofer_compressor_threshold].cmd_default_value = 0x46;
    output_settings[subwoofer_compressor_threshold].effective_arg_idx = 2;
    output_settings[subwoofer_compressor_threshold].profile_xml_name = "CompressorThresHold";
    output_settings[subwoofer_compressor_threshold].profile_value_offset = 0;
    output_settings[subwoofer_compressor_threshold].translate_table = NULL;
    output_settings[subwoofer_compressor_threshold].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_compressor_ratio].serial_cmd_name = "subwoofer_compressor_ratio";
    output_settings[subwoofer_compressor_ratio].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorSetup]);
    output_settings[subwoofer_compressor_ratio].cmd_default_value = 0x9;
    output_settings[subwoofer_compressor_ratio].effective_arg_idx = 3;
    output_settings[subwoofer_compressor_ratio].profile_xml_name = "CompressorRatio";
    output_settings[subwoofer_compressor_ratio].profile_value_offset = 0;
    output_settings[subwoofer_compressor_ratio].translate_table = NULL;
    output_settings[subwoofer_compressor_ratio].profile_xml_node = SUBWOOFER_NODE_NAME;

    output_settings[subwoofer_compressor_gain].serial_cmd_name = "subwoofer_compressor_gain";
    output_settings[subwoofer_compressor_gain].effect_api = &(api_table[WSD_AL_AE_OutputSubwooferCompressorSetup]);
    output_settings[subwoofer_compressor_gain].cmd_default_value = 0x5;
    output_settings[subwoofer_compressor_gain].effective_arg_idx = 4;
    output_settings[subwoofer_compressor_gain].profile_xml_name = "CompressorGain";
    output_settings[subwoofer_compressor_gain].profile_value_offset = 0;
    output_settings[subwoofer_compressor_gain].translate_table = NULL;
    output_settings[subwoofer_compressor_gain].profile_xml_node = SUBWOOFER_NODE_NAME;
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
        debug_print_info("block_id(0x%lx) is \"mixer block\"\n",block_id);
        settings_table = mixer_settings;
        settings_table_size = mixer_end;
        break;

    case roland_mic_effects_block:
        debug_print_info("block_id(0x%lx) is \"mic effect block\"\n",block_id);
        settings_table = mic_effects_settings;
        settings_table_size = mic_effects_end;
        break;

    case roland_echo_block:
        debug_print_info("block_id(0x%lx) is \"echo block\"\n",block_id);
        settings_table = delay_settings;
        settings_table_size = delay_end;
        break;

    case roland_music_effects_block:
        debug_print_info("block_id(0x%lx) is \"music effect block\"\n",block_id);
        settings_table = music_effects_settings;
        settings_table_size = music_effects_end;
        break;

    case roland_output_block:
        debug_print_info("block_id(0x%lx) is \"output block\"\n",block_id);
        settings_table = output_settings;
        settings_table_size = output_end;
        break;

    default:
        debug_print_warn("block_id(0x%lx) is unknown!!!\n",block_id);
        return -1;
        break;
    }

    if (cmd_idx >= settings_table_size)
    {
        debug_print_error("cmd_idx(0x%lx) is larger than 0x%x,out of table size of block_id(0x%lx)!!!\n",cmd_idx,settings_table_size,block_id);
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
        debug_print_info("\"%s\" 0x%x->0x%lx\n",
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
        debug_print_info("\"%s\" is 0x%lx, no difference with default value!\n",
               settings_table[cmd_idx].serial_cmd_name,value);
    }
    return 0;
}
