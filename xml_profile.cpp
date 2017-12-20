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

static bool xml_add_attribute_to_node(const char* node_name,const char* attribute,const int value)
{
    pugi::xml_node root_node = profileXml.child(ROOT_NODE_NAME);
    pugi::xml_node current_node = root_node.child(node_name);
    if (attribute)
    {
        debug_print_info("add int attribute %s=%d\n",attribute,value);
        current_node.append_attribute(attribute) = value;
        return true;
    }
    else
    {
        debug_print_warn("attribute is NULL, could not add!\n");
        return false;
    }
}

static bool xml_add_attribute_to_node(const char* node_name,const char* attribute,const double value)
{
    pugi::xml_node root_node = profileXml.child(ROOT_NODE_NAME);
    pugi::xml_node current_node = root_node.child(node_name);
    if (attribute)
    {
        debug_print_info("add double attribute %s=%lf\n",attribute,value);
        char cValue[16];
        sprintf(cValue, "%lf", value);
        current_node.append_attribute(attribute) = cValue;
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
    root_node.append_child(CENTER_NODE_NAME);
    root_node.append_child(SUBWOOFER_NODE_NAME);

    return true;
}

bool xml_save_profile(const char* save_path)
{
    return profileXml.save_file(save_path,PUGIXML_TEXT("\t"),pugi::format_indent_attributes);
}

bool xml_profile_add(DSP_SETTING_T* attribute_name_table, unsigned long block_id, unsigned long cmd_idx, unsigned long value)
{
    int store_double = 0;
    int attribute_value_int = value + attribute_name_table[cmd_idx].profile_value_offset;
    double attribute_value_double = 0.0f;

    debug_print_info("%s(0x%lx) in block_id(0x%lx),xml node is \"%s\"\n",
            attribute_name_table[cmd_idx].serial_cmd_name,cmd_idx,block_id,attribute_name_table[cmd_idx].profile_xml_node);

    if (attribute_name_table[cmd_idx].profile_xml_node==NULL)
    {
        debug_print_warn("no xml node name, ignore %s(0x%lx) in block_id(0x%lx)!\n",attribute_name_table[cmd_idx].serial_cmd_name,cmd_idx,block_id);
        return false;
    }

    if (attribute_name_table[cmd_idx].translate_table!=NULL)
    {
        unsigned int table_size = 0;
        store_double = 1;

        if (attribute_name_table[cmd_idx].translate_table == gHighPass_Freq_Table)
        {
            table_size = sizeof(gHighPass_Freq_Table)/sizeof(double);
        }
        else if (attribute_name_table[cmd_idx].translate_table == gLowPass_Freq_Table)
        {
            table_size = sizeof(gLowPass_Freq_Table)/sizeof(double);
        }
        else if (attribute_name_table[cmd_idx].translate_table == gPEQ_Freq_Table)
        {
            table_size = sizeof(gPEQ_Freq_Table)/sizeof(double);
        }
        else if (attribute_name_table[cmd_idx].translate_table == gPEQ_Gain_Table)
        {
            table_size = sizeof(gPEQ_Gain_Table)/sizeof(double);
        }
        else if (attribute_name_table[cmd_idx].translate_table == gPEQ_Q_Table)
        {
            table_size = sizeof(gPEQ_Q_Table)/sizeof(double);
        }
        else
        {
            debug_print_error("unknown table!!!\n");
            return false;
        }

        if ( attribute_value_int<0 || attribute_value_int>=table_size)
        {
            debug_print_error("attribute_value_int(%d) is not in translate_table size(%u)!!!\n",attribute_value_int,table_size);
            return false;
        }

        attribute_value_double = attribute_name_table[cmd_idx].translate_table[attribute_value_int];
    }

    if (store_double)
    {
        xml_add_attribute_to_node(attribute_name_table[cmd_idx].profile_xml_node, attribute_name_table[cmd_idx].profile_xml_name, attribute_value_double);
    }
    else
    {
        xml_add_attribute_to_node(attribute_name_table[cmd_idx].profile_xml_node, attribute_name_table[cmd_idx].profile_xml_name, attribute_value_int);
    }

    return true;
}
