#ifndef _WSD_API_H_
#define _WSD_API_H_


#include <stdint.h>

#define API_OUTPUT_FILE_DEFAULT_PATH         "ApiMayCall.txt"

#define API_TABLE_MAX 256
#define API_ARGC_MAX 6

typedef enum wsd_api_idx
{
    //mixer
    WSD_AL_AE_MixerMicLRLevelSetup=0,
    WSD_AL_AE_MixerMicInputLevelSetup,
    WSD_AL_AE_MixerMusicInputLevelSetup,
    WSD_AL_AE_MixerAuxLevelSetup,
    WSD_AL_AE_MixerMixSetup_rear,
    WSD_AL_AE_MixerMixSetup_front,
    WSD_AL_AE_MixerMixSetup_subwoofer,
    WSD_AL_AE_MixerMixSetup_center,
    WSD_AL_AE_MixerCenterSubwooferSwitchStereo,

    //mic
    WSD_AL_AE_MicBasscutEnable,
    WSD_AL_AE_MicBasscutFrequencySetup,
    WSD_AL_AE_MicNoiseSuppEnable,
    WSD_AL_AE_MicNoiseThresholdSetup,
    WSD_AL_AE_MicCompressorEnable,
    WSD_AL_AE_MicCompressorSetup,
    WSD_AL_AE_MicFrequencyShiftSetup,
    WSD_AL_AE_Mic7bandPeqEnable,
    WSD_AL_AE_Mic7bandPeqEdgeSetup,
    WSD_AL_AE_Mic7bandPeqSetup_band1,
    WSD_AL_AE_Mic7bandPeqSetup_band2,
    WSD_AL_AE_Mic7bandPeqSetup_band3,
    WSD_AL_AE_Mic7bandPeqSetup_band4,
    WSD_AL_AE_Mic7bandPeqSetup_band5,
    WSD_AL_AE_Mic7bandPeqSetup_band6,
    WSD_AL_AE_Mic7bandPeqSetup_band7,
    

    //echo
    WSD_AL_AE_DelayInputLevelSetup,
    WSD_AL_AE_DelayInput5bandPeqEnable,
    WSD_AL_AE_DelayInput5bandPeqEdgeSetup,
    WSD_AL_AE_DelayInput5bandPeqSetup_band1,
    WSD_AL_AE_DelayInput5bandPeqSetup_band2,
    WSD_AL_AE_DelayInput5bandPeqSetup_band3,
    WSD_AL_AE_DelayInput5bandPeqSetup_band4,
    WSD_AL_AE_DelayInput5bandPeqSetup_band5,
    WSD_AL_AE_DelayInputBandpassfilterSetup,
    WSD_AL_AE_ReverbInputLevelSetup,
    WSD_AL_AE_ReverbInput5bandPeqEnable,
    WSD_AL_AE_ReverbInput5bandPeqEdgeSetup,
    WSD_AL_AE_ReverbInput5bandPeqSetup_band1,
    WSD_AL_AE_ReverbInput5bandPeqSetup_band2,
    WSD_AL_AE_ReverbInput5bandPeqSetup_band3,
    WSD_AL_AE_ReverbInput5bandPeqSetup_band4,
    WSD_AL_AE_ReverbInput5bandPeqSetup_band5,
    WSD_AL_AE_ReverbInputBandpassfilterSetup,
    WSD_AL_AE_DelayMasterEffectSetup,
    WSD_AL_AE_DelayTapEffectsSetup_tap1,
    WSD_AL_AE_DelayTapEffectsSetup_tap2,
    WSD_AL_AE_DelayTapEffectsSetup_tap3,
    WSD_AL_AE_ReverbEffectsSetup,
    WSD_AL_AE_DelayToReverbLevelSetup,
    WSD_AL_AE_DryOutputSetup,
    WSD_AL_AE_DelayOutputSetup,
    WSD_AL_AE_ReverbOutputSetup,

    //music
    WSD_AL_AE_Music31bandGeqEnable,
    WSD_AL_AE_Music31bandGeqSetup_band1,
    WSD_AL_AE_Music31bandGeqSetup_band2,
    WSD_AL_AE_Music31bandGeqSetup_band3,
    WSD_AL_AE_Music31bandGeqSetup_band4,
    WSD_AL_AE_Music31bandGeqSetup_band5,
    WSD_AL_AE_Music31bandGeqSetup_band6,
    WSD_AL_AE_Music31bandGeqSetup_band7,
    WSD_AL_AE_Music31bandGeqSetup_band8,
    WSD_AL_AE_Music31bandGeqSetup_band9,
    WSD_AL_AE_Music31bandGeqSetup_band10,
    WSD_AL_AE_Music31bandGeqSetup_band11,
    WSD_AL_AE_Music31bandGeqSetup_band12,
    WSD_AL_AE_Music31bandGeqSetup_band13,
    WSD_AL_AE_Music31bandGeqSetup_band14,
    WSD_AL_AE_Music31bandGeqSetup_band15,
    WSD_AL_AE_Music31bandGeqSetup_band16,
    WSD_AL_AE_Music31bandGeqSetup_band17,
    WSD_AL_AE_Music31bandGeqSetup_band18,
    WSD_AL_AE_Music31bandGeqSetup_band19,
    WSD_AL_AE_Music31bandGeqSetup_band20,
    WSD_AL_AE_Music31bandGeqSetup_band21,
    WSD_AL_AE_Music31bandGeqSetup_band22,
    WSD_AL_AE_Music31bandGeqSetup_band23,
    WSD_AL_AE_Music31bandGeqSetup_band24,
    WSD_AL_AE_Music31bandGeqSetup_band25,
    WSD_AL_AE_Music31bandGeqSetup_band26,
    WSD_AL_AE_Music31bandGeqSetup_band27,
    WSD_AL_AE_Music31bandGeqSetup_band28,
    WSD_AL_AE_Music31bandGeqSetup_band29,
    WSD_AL_AE_Music31bandGeqSetup_band30,
    WSD_AL_AE_Music31bandGeqSetup_band31,
    WSD_AL_AE_MusicKeyctrlEnable,
    WSD_AL_AE_MusicKeyctrlSetup,
    WSD_AL_AE_MusicPhaseSetup,

    //output
    WSD_AL_AE_Output5bandPeqEnable_rear,
    WSD_AL_AE_Output5bandPeqEnable_front,
    WSD_AL_AE_Output5bandPeqEnable_subwoofer,
    WSD_AL_AE_Output5bandPeqEnable_center,
    WSD_AL_AE_Output5bandPeqEdgeSetup_rear,
    WSD_AL_AE_Output5bandPeqEdgeSetup_front,
    WSD_AL_AE_Output5bandPeqEdgeSetup_subwoofer,
    WSD_AL_AE_Output5bandPeqEdgeSetup_center,
    WSD_AL_AE_Output5bandPeqSetup_rear_band1,
    WSD_AL_AE_Output5bandPeqSetup_rear_band2,
    WSD_AL_AE_Output5bandPeqSetup_rear_band3,
    WSD_AL_AE_Output5bandPeqSetup_rear_band4,
    WSD_AL_AE_Output5bandPeqSetup_rear_band5,
    WSD_AL_AE_Output5bandPeqSetup_front_band1,
    WSD_AL_AE_Output5bandPeqSetup_front_band2,
    WSD_AL_AE_Output5bandPeqSetup_front_band3,
    WSD_AL_AE_Output5bandPeqSetup_front_band4,
    WSD_AL_AE_Output5bandPeqSetup_front_band5,
    WSD_AL_AE_Output5bandPeqSetup_subwoofer_band1,
    WSD_AL_AE_Output5bandPeqSetup_subwoofer_band2,
    WSD_AL_AE_Output5bandPeqSetup_subwoofer_band3,
    WSD_AL_AE_Output5bandPeqSetup_subwoofer_band4,
    WSD_AL_AE_Output5bandPeqSetup_subwoofer_band5,
    WSD_AL_AE_Output5bandPeqSetup_center_band1,
    WSD_AL_AE_Output5bandPeqSetup_center_band2,
    WSD_AL_AE_Output5bandPeqSetup_center_band3,
    WSD_AL_AE_Output5bandPeqSetup_center_band4,
    WSD_AL_AE_Output5bandPeqSetup_center_band5,
    WSD_AL_AE_OutputBandpassfilterSetup_rear,
    WSD_AL_AE_OutputBandpassfilterSetup_front,
    WSD_AL_AE_OutputBandpassfilterSetup_subwoofer,
    WSD_AL_AE_OutputBandpassfilterSetup_center,
    WSD_AL_AE_OutputSetup_rear,
    WSD_AL_AE_OutputSetup_front,
    WSD_AL_AE_OutputSetup_subwoofer,
    WSD_AL_AE_OutputSetup_center,
    WSD_AL_AE_OutputSubwooferCompressorEnable,
    WSD_AL_AE_OutputSubwooferCompressorSetup,
    WSD_AL_AE_count,
} wsd_api_idx_e;

typedef struct api_info
{
    const char *api_name;
    int effective_arg_cnt;              //不算channel参数,eq的band参数
    int arg_default_value[API_ARGC_MAX];
    int arg_value[API_ARGC_MAX];
    bool is_active;
}API_INFO_T;

extern API_INFO_T api_table[WSD_AL_AE_count];

void print_wsd_api(FILE* stream);
#endif
