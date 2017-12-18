/***********************************************************************
*                                包含头文件
************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

#include <getopt.h>
#include <dlfcn.h>

#include "debug_config.h"
#include "roland_cmd.h"
#include "wsd_api.h"
#include "xml_profile.h"

#define MAX_ARGC 10
#define MAX_LINE_LENTH 256

static char* program_name;

static void print_usage(FILE* stream, int exit_code)
{
    fprintf(stream, "\n");
    fprintf(stream, "Usage: %s [-options] [args...]\n", program_name);
    fprintf(stream, "\n");
    fprintf(stream, "where options include:\n");
    fprintf(stream,
            "  -i/--input [roland editor cmdline file]\n"
            "        use [roland editor cmdline file] to make [wsd api call table] & [ktv app audio effects profile]\n"
            "\n"
            "  -o/--output [output file name]\n"
            "        [wsd api call table] & [ktv app audio effects profile] will be output into [output file name].txt [output file name].xml\n"
            "\n"
            "  -v/--version\n"
            "        print the program version\n"
            "\n"
            "  -d/--debug\n"
            "        print more debug message in program running\n"
            "\n"
            "  -h/--help\n"
           );
    exit(exit_code);
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int version_flag = 0;
    int input_flag = 0;
    int output_flag = 0;

    char* output_txt_path = NULL;
    char* output_xml_path = NULL;

    char *priv_arg_list[MAX_ARGC];
    int priv_arg_cnt;

    int next_option;
    int i;
    //opterr = 0; //不输出错误信息

    //命令行参数解析
    const char* const short_options = "i:o:vhd?";
    program_name = argv[0];
    const struct option long_options[] =
    {
        { "input", 1, NULL, 'i' },
        { "output", 1, NULL, 'o' },
        { "version", 0, NULL, 'v' },
        { "help", 0, NULL, 'h' },
        { "debug", 0, NULL, 'd' },
        { NULL, 0, NULL, 0}
    };

    if(argc > MAX_ARGC || argc < 2)
    {
        print_usage(stderr, -1);
        return -1;
    }

    while((next_option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1)
    {
        switch(next_option)
        {
            case 'i':
                input_flag = 1;
                priv_arg_list[0] = optarg;
                break;
            case 'o':
                output_flag = 1;
                priv_arg_list[1] = optarg;
                break;
            case 'd':
                debug_flag = 1;
                break;
            case 'v':
                version_flag = 1;
                break;
            case 'h':
            case '?':
            default:
                print_usage(stderr, -1);
                return 0;
        }
    }

    priv_arg_cnt = argc - optind + 1;

    for(i = optind; i < argc; i++)
    {
        priv_arg_list[i - optind + 1] = argv[i];
    }

    if (version_flag)
    {
        printf("%s profile_format_version %s\n",program_name,PROFILE_VERSION);
        print_usage(stderr, 0);
    }
    else if (input_flag)
    {
        int line_num = 0;
        FILE *in_fp,*out_fp;
        char oneline[MAX_LINE_LENTH+1];
        char effective_line[MAX_LINE_LENTH+1];

        if((in_fp = fopen(priv_arg_list[0], "r")) == NULL)
        {
            debug_print_error("[%s] open error!\n",priv_arg_list[0]);
            return -1;
        }

        //初始化保存xml的profile
        xml_create_profile();
        //初始化串口cmd的table
        roland_cmd_settings_table_init();

        while ((fgets(oneline, MAX_LINE_LENTH, in_fp)) != NULL)
        {
            int origin_len = strlen(oneline);

            line_num++;

            debug_print_info("====parsing line(%d)====\n",line_num);
            debug_print_info("raw context=\"%s\", lenth=%lu \n",oneline,strlen(oneline));

            //去行尾换行符与空格
            while (origin_len>0 && 
                    (oneline[origin_len-1]=='\n' || oneline[origin_len-1]=='\r' || oneline[origin_len-1]==' '))
            {
                oneline[origin_len-1]='\0';
                origin_len--;
            }

            //滤除行首空白
            char *cursor = oneline;
            while (*cursor == ' ' ||*cursor == '\t')
            {
                cursor++;
            }
            //判断是否为空行,是否为注释的行，跳过不处理这种行
            if (strlen(cursor)==0 || *cursor=='#')
            {
                debug_print_info("ignore line with # in header\n");
                continue;
            }

            //判断是否为[TX]打头,跳过不处理这种行
            if ( strncmp("[RX]",cursor,4) && strncmp("[rx]",cursor,4)
                    && strncmp("[Rx]",cursor,4) && strncmp("[rX]",cursor,4))
            {
                debug_print_info("ignore line with [Rx] in header\n");
                continue;
            }

            debug_print_info("line context=\"%s\", lenth=%lu \n",cursor,strlen(cursor));

            memset(effective_line,0,MAX_LINE_LENTH+1);
            memcpy(effective_line,cursor,strlen(cursor)*sizeof(char));

            //运行到此处，此行一定是有效行，按照空格切分字符串
            priv_arg_cnt = i = 2;    //priv_arg_list[0] store the input file name, priv_arg_list[1] store the output file name
            priv_arg_list[i] = strtok(cursor, " ");
            while(priv_arg_list[i] != NULL)
            {
                i++;
                priv_arg_cnt = i;
                priv_arg_list[i] = strtok(NULL, " ");
            }

            if (priv_arg_cnt<6)
            {
                debug_print_warn("ignore line(%d), too less arg in this line!\n",line_num);
                continue;
            }

            //对有效行进行解析
            roland_cmd_translate(&(priv_arg_list[3]),priv_arg_cnt-3);
        }
        fclose(in_fp);

        //确定两个文件名
        if (output_flag)
        {
            output_txt_path = (char*)malloc(strlen(priv_arg_list[1]) + strlen(".txt") + 1);
            output_xml_path = (char*)malloc(strlen(priv_arg_list[1]) + strlen(".xml") + 1);
            strcpy(output_txt_path, priv_arg_list[1]);
            strcat(output_txt_path, ".txt"); 
            strcpy(output_xml_path, priv_arg_list[1]);
            strcat(output_xml_path, ".xml"); 
        }
        else
        {
            output_txt_path = (char*)API_OUTPUT_FILE_DEFAULT_PATH;
            output_xml_path = (char*)XML_OUTPUT_FILE_DEFAULT_PATH;
        }


        debug_print_info("output to file [%s] & [&=%s]\n",output_xml_path,output_txt_path);

        //保存xml配置文件
        xml_save_profile(output_xml_path);
        if (output_flag) free(output_xml_path);

        //打印api call
        if((out_fp = fopen(output_txt_path, "wr")) == NULL)
        {
            debug_print_error("[%s] open error!\n",output_txt_path);
            if (output_flag) free(output_txt_path);
            return -1;
        }

        print_wsd_api(out_fp);

        if (output_flag) free(output_txt_path);
        fclose(out_fp);
    }
    else
    {//无效的参数
        print_usage(stderr, -1);
    }
}
