#include "files.h"
#include "global_cfg.h"

int files_parseFileList(char * filePath, files_music_list * cfg) {
    FILE * file_ptr;
    char ch;
    char * file_str;
    char music_id_tmp[5];
    int tmp_x_idx, line_num;
    struct stat stat_str;
    music_cfg_enum current_cfg_param = MUSIC_ID;

    char n = '\n';
    char c = ',';
    char s = ' ';

    if (verbose_b) printf("LOADING FILE...\n");

    if(stat(filePath, &stat_str) == -1) {
        printf("FAILED TO LOAD FILE \n");
        return -1;
    }
    
    file_ptr = fopen(filePath, "r");
    if (NULL == file_ptr) {
        printf("FAILED TO LOAD FILE\n");
        return -1;
    } 

    file_str = malloc(FILE_PATH_MAX_LEN);
    cfg->music_list = malloc(cfg->len * sizeof(files_music_cfg));

    line_num = 0;
    tmp_x_idx = 0;

    ch = getc(file_ptr);
    while(ch != EOF) {
        if (ch == n) {
            if (current_cfg_param == MUSIC_FILEPATH) {
                cfg->music_list[line_num].filepath = malloc(tmp_x_idx);
                memcpy(cfg->music_list[line_num].filepath, file_str, tmp_x_idx);
                tmp_x_idx = 0;
            }
            line_num++;
            current_cfg_param = MUSIC_ID;
            ch = fgetc(file_ptr);
            continue;
        } else if (ch == c) {
            if (current_cfg_param == MUSIC_ID) {
                cfg->music_list[line_num].id = atoi(music_id_tmp);
                tmp_x_idx = 0;
            }
            current_cfg_param++;
            ch = fgetc(file_ptr);
        } 

        if (current_cfg_param == MUSIC_ID && ch != s) {
            music_id_tmp[tmp_x_idx] = ch;
        } else if (current_cfg_param == MUSIC_FILEPATH) {
            file_str[tmp_x_idx] = ch;
        }
        tmp_x_idx++;
        ch = fgetc(file_ptr);
    }
    free(file_str);
    cfg->music_list = realloc(cfg->music_list, line_num * sizeof(files_music_cfg));
    cfg->len = line_num;

    return 0;
}

int files_saveMusicListToFile(files_music_list * cfg, char * filePath) {
    FILE * file_ptr;

    file_ptr = fopen(filePath, "w");
    for(int i=0; i<cfg->len; i++) {
        fprintf(file_ptr, "%d,%s\n", cfg->music_list[i].id, cfg->music_list[i].filepath);
    }
    fclose(file_ptr);
    return 0;
}
