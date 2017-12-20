#ifndef _XML_PROFILE_H_
#define _XML_PROFILE_H_

#include <stdint.h>
#include "roland_cmd.h"

#define XML_OUTPUT_FILE_DEFAULT_PATH         "CustomProfile.xml"

#define ROOT_NODE_NAME        "AEProfile"
#define PROFILE_VERSION       "AESETTING_0.6"
#define MUSICINPUT_NODE_NAME  "MusicInput"
#define MICINPUT_NODE_NAME    "MicInput"
#define ECHO_NODE_NAME        "Echo"
#define REVERB_NODE_NAME      "Reverb"
#define REAR_NODE_NAME        "RearOutput"
#define FRONT_NODE_NAME       "FrontOutput"
#define SUBWOOFER_NODE_NAME   "Subwoofer"
#define CENTER_NODE_NAME      "CenterOutput"

bool xml_create_profile();
bool xml_profile_add(DSP_SETTING_T* attribute_name_table, unsigned long block_id, unsigned long cmd_idx, unsigned long value);
bool xml_save_profile(const char* save_path);

#endif
