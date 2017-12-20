#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

#include "debug_config.h"
#include "wsd_api.h"

API_INFO_T api_table[WSD_AL_AE_count] =
{
    //mixer
    {"WSD_AL_AE_MixerMicLRLevelSetup(",                  2, {0x40,0x40}, {0xff,0xff} },
    {"WSD_AL_AE_MixerMicInputLevelSetup(",               1, {0x40} },
    {"WSD_AL_AE_MixerMusicInputLevelSetup(",             1, {0x40} },
    {"WSD_AL_AE_MixerAuxLevelSetup(",                    2, {0x00,0x00}, {0xff,0xff} },
    {"WSD_AL_AE_MixerMixSetup(WSD_AE_REAR_OUTPUT,",      4, {0x7f,0x7f,0x7f,0x7f}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_MixerMixSetup(WSD_AE_FRONT_OUTPUT,",     4, {0x7f,0x7f,0x7f,0x7f}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_MixerMixSetup(WSD_AE_SUBWOOFER_OUTPUT,", 4, {0x7f,0x7f,0x7f,0x7f}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_MixerMixSetup(WSD_AE_CENTER_OUTPUT,",    4, {0x7f,0x7f,0x7f,0x00}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_MixerCenterSubwooferSwitchStereo(",      1, {0x00} },

    //mic
    {"WSD_AL_AE_MicBasscutEnable(",                      1, {0x00} },
    {"WSD_AL_AE_MicBasscutFrequencySetup(",              1, {0x01} },
    {"WSD_AL_AE_MicNoiseSuppEnable(",                    1, {0x00} },
    {"WSD_AL_AE_MicNoiseThresholdSetup(",                1, {0x03} },
    {"WSD_AL_AE_MicCompressorEnable(",                   1, {0x00} },
    {"WSD_AL_AE_MicCompressorSetup(",                    2, {0x07,0x20}, {0xff,0xff} },
    {"WSD_AL_AE_MicFrequencyShiftSetup(",                1, {0x00} },
    {"WSD_AL_AE_Mic7bandPeqEnable(",                     1, {0x00} },
    {"WSD_AL_AE_Mic7bandPeqEdgeSetup(",                  2, {0x01,0x01}, {0xff,0xff} },
    {"WSD_AL_AE_Mic7bandPeqSetup(1,",                    3, {0x0c,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Mic7bandPeqSetup(2,",                    3, {0x12,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Mic7bandPeqSetup(3,",                    3, {0x18,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Mic7bandPeqSetup(4,",                    3, {0x1e,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Mic7bandPeqSetup(5,",                    3, {0x24,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Mic7bandPeqSetup(6,",                    3, {0x2a,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Mic7bandPeqSetup(7,",                    3, {0x30,0x07,0x00}, {0xff,0xff,0x7f} },

    //echo
    {"WSD_AL_AE_DelayInputLevelSetup(",                  1, {0x00} },
    {"WSD_AL_AE_DelayInput5bandPeqEnable(",              1, {0x00} },
    {"WSD_AL_AE_DelayInput5bandPeqEdgeSetup(",           2, {0x01,0x01} },
    {"WSD_AL_AE_DelayInput5bandPeqSetup(1,",             3, {0x0c,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_DelayInput5bandPeqSetup(2,",             3, {0x12,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_DelayInput5bandPeqSetup(3,",             3, {0x18,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_DelayInput5bandPeqSetup(4,",             3, {0x1e,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_DelayInput5bandPeqSetup(5,",             3, {0x24,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_DelayInputBandpassfilterSetup(",         4, {0x06,0x00,0x06,0x6f}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_ReverbInputLevelSetup(",                 1, {0x00} },
    {"WSD_AL_AE_ReverbInput5bandPeqEnable(",             1, {0x00} },
    {"WSD_AL_AE_ReverbInput5bandPeqEdgeSetup(",          2, {0x01,0x01}, {0xff,0xff} },
    {"WSD_AL_AE_ReverbInput5bandPeqSetup(1,",            3, {0x0c,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_ReverbInput5bandPeqSetup(2,",            3, {0x0c,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_ReverbInput5bandPeqSetup(3,",            3, {0x0c,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_ReverbInput5bandPeqSetup(4,",            3, {0x0c,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_ReverbInput5bandPeqSetup(5,",            3, {0x0c,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_ReverbInputBandpassfilterSetup(",        4, {0x06,0x00,0x06,0x6f}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_DelayMasterEffectSetup(",                6, {0x14,0x7f,0x40,0x40,0x32,0x00}, {0xff,0xff,0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_DelayTapEffectsSetup(1,",                3, {0x3f,0x00,0x40}, {0xff,0xff,0xff} },
    {"WSD_AL_AE_DelayTapEffectsSetup(2,",                3, {0x3f,0x00,0x40}, {0xff,0xff,0xff} },
    {"WSD_AL_AE_DelayTapEffectsSetup(3,",                3, {0x3f,0x00,0x40}, {0xff,0xff,0xff} },
    {"WSD_AL_AE_ReverbEffectsSetup(",                    4, {0x02,0x0a,0x40,0x00}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_DelayToReverbLevelSetup(",               1, {0x00} },
    {"WSD_AL_AE_DryOutputSetup(",                        2, {0x7f,0x00}, {0xff,0xff} },
    {"WSD_AL_AE_DelayOutputSetup(",                      2, {0x40,0x00}, {0xff,0xff} },
    {"WSD_AL_AE_ReverbOutputSetup(",                     2, {0x40,0x00}, {0xff,0xff} },

    //music
    {"WSD_AL_AE_Music31bandGeqEnable(",                  1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(1,",                 1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(2,",                 1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(3,",                 1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(4,",                 1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(5,",                 1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(6,",                 1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(7,",                 1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(8,",                 1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(9,",                 1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(10,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(11,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(12,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(13,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(14,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(15,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(16,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(17,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(18,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(19,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(20,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(21,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(22,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(23,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(24,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(25,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(26,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(27,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(28,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(29,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(30,",                1, {0x00} },
    {"WSD_AL_AE_Music31bandGeqSetup(31,",                1, {0x00} },
    {"WSD_AL_AE_MusicKeyctrlEnable(",                    1, {0x00} },
    {"WSD_AL_AE_MusicKeyctrlSetup(",                     2, {0x00,0x00}, {0xff,0xff} },
    {"WSD_AL_AE_MusicPhaseSetup(",                       1, {0x00} },

    //output
    {"WSD_AL_AE_Output5bandPeqEnable(WSD_AE_REAR_OUTPUT,",          1, {0x00} },
    {"WSD_AL_AE_Output5bandPeqEnable(WSD_AE_FRONT_OUTPUT,",         1, {0x00} },
    {"WSD_AL_AE_Output5bandPeqEnable(WSD_AE_SUBWOOFER_OUTPUT,",     1, {0x01} },
    {"WSD_AL_AE_Output5bandPeqEnable(WSD_AE_CENTER_OUTPUT,",        1, {0x00} },
    {"WSD_AL_AE_Output5bandPeqEdgeSetup(WSD_AE_REAR_OUTPUT,",       2, {0x01,0x01}, {0xff,0xff} },
    {"WSD_AL_AE_Output5bandPeqEdgeSetup(WSD_AE_FRONT_OUTPUT,",      2, {0x01,0x01}, {0xff,0xff} },
    {"WSD_AL_AE_Output5bandPeqEdgeSetup(WSD_AE_SUBWOOFER_OUTPUT,",  2, {0x01,0x01}, {0xff,0xff} },
    {"WSD_AL_AE_Output5bandPeqEdgeSetup(WSD_AE_CENTER_OUTPUT,",     2, {0x01,0x01}, {0xff,0xff} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_REAR_OUTPUT,1,",         3, {0x0c,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_REAR_OUTPUT,2,",         3, {0x12,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_REAR_OUTPUT,3,",         3, {0x18,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_REAR_OUTPUT,4,",         3, {0x1e,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_REAR_OUTPUT,5,",         3, {0x24,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_FRONT_OUTPUT,1,",        3, {0x0c,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_FRONT_OUTPUT,2,",        3, {0x12,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_FRONT_OUTPUT,3,",        3, {0x18,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_FRONT_OUTPUT,4,",        3, {0x1e,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_FRONT_OUTPUT,5,",        3, {0x24,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_SUBWOOFER_OUTPUT,1,",    3, {0x06,0x13,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_SUBWOOFER_OUTPUT,2,",    3, {0x0a,0x13,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_SUBWOOFER_OUTPUT,3,",    3, {0x0d,0x13,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_SUBWOOFER_OUTPUT,4,",    3, {0x10,0x13,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_SUBWOOFER_OUTPUT,5,",    3, {0x14,0x13,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_CENTER_OUTPUT,1,",       3, {0x0c,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_CENTER_OUTPUT,2,",       3, {0x12,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_CENTER_OUTPUT,3,",       3, {0x18,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_CENTER_OUTPUT,4,",       3, {0x1e,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_Output5bandPeqSetup(WSD_AE_CENTER_OUTPUT,5,",       3, {0x24,0x07,0x00}, {0xff,0xff,0x7f} },
    {"WSD_AL_AE_OutputBandpassfilterSetup(WSD_AE_REAR_OUTPUT,",     4, {0x06,0x00,0x06,0x6f}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_OutputBandpassfilterSetup(WSD_AE_FRONT_OUTPUT,",    4, {0x06,0x00,0x06,0x6f}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_OutputBandpassfilterSetup(WSD_AE_SUBWOOFER_OUTPUT,",4, {0x06,0x05,0x06,0x16}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_OutputBandpassfilterSetup(WSD_AE_CENTER_OUTPUT,",   4, {0x06,0x00,0x06,0x6f}, {0xff,0xff,0xff,0xff} },
    {"WSD_AL_AE_OutputSetup(WSD_AE_REAR_OUTPUT,",                   3, {0x7f,0x00,0x00}, {0xff,0xff,0xff} },
    {"WSD_AL_AE_OutputSetup(WSD_AE_FRONT_OUTPUT,",                  3, {0x7f,0x00,0x00}, {0xff,0xff,0xff} },
    {"WSD_AL_AE_OutputSetup(WSD_AE_SUBWOOFER_OUTPUT,",              3, {0x7f,0x00,0x00}, {0xff,0xff,0xff} },
    {"WSD_AL_AE_OutputSetup(WSD_AE_CENTER_OUTPUT,",                 3, {0x7f,0x00,0x00}, {0xff,0xff,0xff} },
    {"WSD_AL_AE_OutputSubwooferCompressorEnable(",                  1, {0x01} },
    {"WSD_AL_AE_OutputSubwooferCompressorSetup(",                   5, {0x50,0x05,0x46,0x09,0x05}, {0xff,0xff,0xff,0xff,0xff} },
};

void print_wsd_api(FILE* stream)
{
    int i,j;

#if ENABLE_FILTER
    debug_print_warn("check every enable api to make sure all is_active api is useable!\n");
    if (api_table[WSD_AL_AE_MicBasscutEnable].is_active!=true)
    {
        api_table[WSD_AL_AE_MicBasscutFrequencySetup].is_active = false;
    }

    if (api_table[WSD_AL_AE_MicNoiseSuppEnable].is_active!=true)
    {
        api_table[WSD_AL_AE_MicNoiseThresholdSetup].is_active = false;
    }

    if (api_table[WSD_AL_AE_MicCompressorEnable].is_active!=true)
    {
        api_table[WSD_AL_AE_MicCompressorSetup].is_active = false;
    }

    if (api_table[WSD_AL_AE_Mic7bandPeqEnable].is_active!=true)
    {
        api_table[WSD_AL_AE_Mic7bandPeqEdgeSetup].is_active = false;
        api_table[WSD_AL_AE_Mic7bandPeqSetup_band1].is_active = false;
        api_table[WSD_AL_AE_Mic7bandPeqSetup_band2].is_active = false;
        api_table[WSD_AL_AE_Mic7bandPeqSetup_band3].is_active = false;
        api_table[WSD_AL_AE_Mic7bandPeqSetup_band4].is_active = false;
        api_table[WSD_AL_AE_Mic7bandPeqSetup_band5].is_active = false;
        api_table[WSD_AL_AE_Mic7bandPeqSetup_band6].is_active = false;
        api_table[WSD_AL_AE_Mic7bandPeqSetup_band7].is_active = false;
    }

    if (api_table[WSD_AL_AE_DelayInputLevelSetup].is_active!=true)
    {
        api_table[WSD_AL_AE_DelayInput5bandPeqEnable].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqEdgeSetup].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band1].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band2].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band3].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band4].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band5].is_active = false;
        api_table[WSD_AL_AE_DelayInputBandpassfilterSetup].is_active = false;
        api_table[WSD_AL_AE_DelayMasterEffectSetup].is_active = false;
        api_table[WSD_AL_AE_DelayTapEffectsSetup_tap1].is_active = false;
        api_table[WSD_AL_AE_DelayTapEffectsSetup_tap2].is_active = false;
        api_table[WSD_AL_AE_DelayTapEffectsSetup_tap3].is_active = false;
        api_table[WSD_AL_AE_DelayToReverbLevelSetup].is_active = false;
        api_table[WSD_AL_AE_DelayOutputSetup].is_active = false;
    }

    if (api_table[WSD_AL_AE_ReverbInputLevelSetup].is_active!=true)
    {
        api_table[WSD_AL_AE_ReverbInput5bandPeqEnable].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqEdgeSetup].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band1].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band2].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band3].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band4].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band5].is_active = false;
        api_table[WSD_AL_AE_ReverbInputBandpassfilterSetup].is_active = false;
        api_table[WSD_AL_AE_ReverbEffectsSetup].is_active = false;
        api_table[WSD_AL_AE_ReverbOutputSetup].is_active = false;
    }

    if (api_table[WSD_AL_AE_DelayInput5bandPeqEnable].is_active!=true)
    {
        api_table[WSD_AL_AE_DelayInput5bandPeqEdgeSetup].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band1].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band2].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band3].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band4].is_active = false;
        api_table[WSD_AL_AE_DelayInput5bandPeqSetup_band5].is_active = false;
        api_table[WSD_AL_AE_DelayInputBandpassfilterSetup].is_active = false;
    }

    if (api_table[WSD_AL_AE_ReverbInput5bandPeqEnable].is_active!=true)
    {
        api_table[WSD_AL_AE_ReverbInput5bandPeqEdgeSetup].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band1].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band2].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band3].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band4].is_active = false;
        api_table[WSD_AL_AE_ReverbInput5bandPeqSetup_band5].is_active = false;
        api_table[WSD_AL_AE_ReverbInputBandpassfilterSetup].is_active = false;
    }

    if (api_table[WSD_AL_AE_Music31bandGeqEnable].is_active!=true)
    {
        api_table[WSD_AL_AE_Music31bandGeqSetup_band1].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band2].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band3].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band4].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band5].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band6].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band7].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band8].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band9].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band10].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band11].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band12].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band13].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band14].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band15].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band16].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band17].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band18].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band19].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band20].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band21].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band22].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band23].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band24].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band25].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band26].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band27].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band28].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band29].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band30].is_active = false;
        api_table[WSD_AL_AE_Music31bandGeqSetup_band31].is_active = false;
    }

    if (api_table[WSD_AL_AE_MusicKeyctrlEnable].is_active!=true)
    {
        api_table[WSD_AL_AE_MusicKeyctrlSetup].is_active = false;
    }

    if (api_table[WSD_AL_AE_Output5bandPeqEnable_rear].is_active!=true)
    {
        api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_rear].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band1].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band2].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band3].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band4].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_rear_band5].is_active = false;
        api_table[WSD_AL_AE_OutputBandpassfilterSetup_rear].is_active = false;
    }

    if (api_table[WSD_AL_AE_Output5bandPeqEnable_front].is_active!=true)
    {
        api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_front].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_front_band1].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_front_band2].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_front_band3].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_front_band4].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_front_band5].is_active = false;
        api_table[WSD_AL_AE_OutputBandpassfilterSetup_front].is_active = false;
    }

    if (api_table[WSD_AL_AE_Output5bandPeqEnable_center].is_active!=true)
    {
        api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_center].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_center_band1].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_center_band2].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_center_band3].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_center_band4].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_center_band5].is_active = false;
        api_table[WSD_AL_AE_OutputBandpassfilterSetup_center].is_active = false;
    }

    if (api_table[WSD_AL_AE_Output5bandPeqEnable_subwoofer].is_active!=true)
    {
        api_table[WSD_AL_AE_Output5bandPeqEdgeSetup_subwoofer].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band1].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band2].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band3].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band4].is_active = false;
        api_table[WSD_AL_AE_Output5bandPeqSetup_subwoofer_band5].is_active = false;
        api_table[WSD_AL_AE_OutputBandpassfilterSetup_subwoofer].is_active = false;
    }

    if (api_table[WSD_AL_AE_OutputSubwooferCompressorEnable].is_active!=true)
    {
        api_table[WSD_AL_AE_OutputSubwooferCompressorSetup].is_active = false;
    }
#endif

    for (i=0; i<WSD_AL_AE_count; i++)
    {
        if (api_table[i].is_active==true)
        {
            fprintf(stream, "%s", api_table[i].api_name);
            for (j=0; j<api_table[i].effective_arg_cnt; j++)
            {
                fprintf(stream, "%d", api_table[i].arg_value[j]);
                if (j!=api_table[i].effective_arg_cnt-1)
                {
                    fprintf(stream, ",");
                }
            }
            fprintf(stream, ")\n");
        }
    }
}

