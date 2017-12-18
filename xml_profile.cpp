#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

#include "debug_config.h"
#include "roland_cmd.h"
#include "wsd_api.h"
#include "xml_profile.h"
#include "pugixml/pugiconfig.hpp"
#include "pugixml/pugixml.hpp"

pugi::xml_document profileXml;

static bool xml_add_attribute_to_node(const char* node_name,const char* attribute,int value)
{
    pugi::xml_node root_node = profileXml.child(ROOT_NODE_NAME);
    pugi::xml_node current_node = root_node.child(node_name);
    if (attribute)
    {
        debug_print_info("attribute %s=%d\n",attribute,value);
        current_node.append_attribute(attribute) = value;
        return true;
    }
    else
    {
        debug_print_warn("attribute is NULL, could not add!\n");
        return false;
    }
}

bool xml_create_profile()
{
    profileXml.load_buffer("",0);

    profileXml.append_child(ROOT_NODE_NAME);
    pugi::xml_node root_node = profileXml.child(ROOT_NODE_NAME);
    root_node.append_attribute("VERSION") = PROFILE_VERSION;

    root_node.append_child(MUSICINPUT_NODE_NAME);
    root_node.append_child(MICINPUT_NODE_NAME);
    root_node.append_child(ECHO_NODE_NAME);
    root_node.append_child(REVERB_NODE_NAME);
    root_node.append_child(REAR_NODE_NAME);
    root_node.append_child(FRONT_NODE_NAME);
    root_node.append_child(SUBWOOFER_NODE_NAME);
    root_node.append_child(CENTER_NODE_NAME);

    return true;
}

bool xml_save_profile(const char* save_path)
{
    return profileXml.save_file(save_path,PUGIXML_TEXT("\t"),pugi::format_indent_attributes);
}

bool xml_profile_add(DSP_SETTING_T* attribute_name_table, unsigned long block_id, unsigned long cmd_idx, unsigned long value)
{
    switch(block_id)
    {
    case roland_mixer_block:
        switch(cmd_idx)
        {
        case mic1_input_level:
        case mic2_input_level:
        //case mic_input_level:
        //case mic_direct_level:
        case aux_to_mic:
            xml_add_attribute_to_node(MICINPUT_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        break;

        //case music_input_level:
        case aux_to_music:
            xml_add_attribute_to_node(MUSICINPUT_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        break;

        case front_mic_dry_level:
        case front_music_level:
        case front_output_level:
        case front_mic_delay_level:
        case front_mic_reverb_level:
            xml_add_attribute_to_node(FRONT_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        break;

        case rear_output_level:
        case rear_mic_dry_level:
        case rear_music_level:
        case rear_mic_delay_level:
        case rear_mic_reverb_level:
            xml_add_attribute_to_node(REAR_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        break;

        case subwoofer_output_level:
        case subwoofer_mic_dry_level:
        case subwoofer_music_level:
        case subwoofer_mic_delay_level:
        case subwoofer_mic_reverb_level:
            xml_add_attribute_to_node(SUBWOOFER_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        break;

        case center_output_level:
        case center_mic_level:
        case center_music_level:
        case center_mic_delay_level:
        case center_mic_reverb_level:
//        case center_subwoofer_switch_stereo:
            xml_add_attribute_to_node(CENTER_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        break;

        default:
            debug_print_warn("ignore %s(0x%lx) in block_id 0x%lx!\n",attribute_name_table[cmd_idx].serial_cmd_name,cmd_idx,block_id);
        break;
        }
        break;


    case roland_mic_effects_block:
        xml_add_attribute_to_node(MICINPUT_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        break;

    case roland_echo_block:
        if (cmd_idx==delay_to_reverb_level)
        {
            xml_add_attribute_to_node(REVERB_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        }
        else if (cmd_idx<=delay_phase)
        {
            xml_add_attribute_to_node(ECHO_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        }
        else if(cmd_idx<=reverb_phase)
        {
            xml_add_attribute_to_node(REVERB_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        }
        else if(cmd_idx==mic_dry_phase)
        {
            xml_add_attribute_to_node(MICINPUT_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        }
        else if(cmd_idx<=delay_eq_lpf_frequency)
        {
            xml_add_attribute_to_node(ECHO_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        }
        else if(cmd_idx<=reverb_eq_lpf_frequency)
        {
            xml_add_attribute_to_node(REVERB_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        }
        else
        {
            debug_print_warn("ignore %s(0x%lx) in block_id 0x%lx!\n",attribute_name_table[cmd_idx].serial_cmd_name,cmd_idx,block_id);
        }
        break;

    case roland_music_effects_block:
        xml_add_attribute_to_node(MUSICINPUT_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        break;

    case roland_output_block:
        if (cmd_idx<=front_phase)
        {
            xml_add_attribute_to_node(FRONT_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        }
        else if (cmd_idx<=rear_phase)
        {
            xml_add_attribute_to_node(REAR_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        }
        else if (cmd_idx<=center_phase)
        {
            xml_add_attribute_to_node(CENTER_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        }
        else if (cmd_idx<=subwoofer_compressor_gain)
        {
            xml_add_attribute_to_node(SUBWOOFER_NODE_NAME, attribute_name_table[cmd_idx].serial_cmd_name, value);
        }
        else
        {
            debug_print_warn("ignore %s(0x%lx) in block_id 0x%lx!\n",attribute_name_table[cmd_idx].serial_cmd_name,cmd_idx,block_id);
        }
        break;

    default:
        debug_print_warn("unknown block_id 0x%lx!\n",block_id);
        return false;
        break;
    }
    return true;
}
