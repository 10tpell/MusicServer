#define _XOPEN_SOURCE 500

#include "files.h"
#include "global_cfg.h"
#include "errors.h"
#include <ftw.h>


int ntfw_callback(const char *filepath, const struct stat *info,
                const int typeflag, struct FTW *pathinfo) {
    int ret = RET_NO_ERR;

    char * fileExt = malloc(8*sizeof(char));
    if(fileExt < (char *) 1) return RET_ERR_MALLOC;

    if(FTW_D == typeflag) {
        printf("Directory %s\n", filepath);
    } else if (FTW_F == typeflag ) {
        files_getFileExtension(filepath, fileExt);
        for (int i=0; i<NUM_ALLOWED_EXTENSIONS; i++) {
            if (strcmp(fileExt, glb_cfg.allowed_extensions[i]) == 0) {
                cfg_addFile(filepath);
            }
        }
    } else {
        ret = RET_ERR_IO;
    }

    free(fileExt);
    return ret;
}

int files_searchDir(char * dirPath, files_music_list * cfg) {

    return nftw(dirPath, ntfw_callback, 200, 0);
}

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
        return RET_ERR_IO;
    }
    
    file_ptr = fopen(filePath, "r");
    if (NULL == file_ptr) {
        printf("FAILED TO LOAD FILE\n");
        return RET_ERR_IO;
    } 

    file_str = malloc(FILE_PATH_MAX_LEN * sizeof(char));

    if (cfg->music_list == (files_music_cfg *) 0) {
        cfg->music_list = malloc(cfg->capacity * sizeof(files_music_cfg)); /* TODO : Clean up */
        if((file_str < (char*) 1) || (cfg->music_list < (files_music_cfg * ) 1)) return RET_ERR_MALLOC;
    }

    line_num = 0;
    tmp_x_idx = 0;

    ch = getc(file_ptr);
    while(ch != EOF) {
        if (ch == n) {
            if (current_cfg_param == MUSIC_FILEPATH) {
                /* TODO: work out where this should be freed */
                cfg->music_list[cfg->len].filepath = malloc(tmp_x_idx+1);
                if(cfg->music_list[cfg->len].filepath < (char*) 1) return RET_ERR_MALLOC;

                memcpy(cfg->music_list[cfg->len].filepath, file_str, tmp_x_idx);
                
                /* adding a zero at the end of the filepath string so that we can use strcpy later on */
                *(cfg->music_list[cfg->len].filepath + tmp_x_idx+1) = 0;

                tmp_x_idx = 0;
                cfg->len++;
            }
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
    cfg->music_list = realloc(cfg->music_list, cfg->len * sizeof(files_music_cfg));

    return RET_NO_ERR;
}

int files_saveMusicListToFile(files_music_list * cfg, char * filePath) {
    FILE * file_ptr;

    file_ptr = fopen(filePath, "w");
    for(int i=0; i<cfg->len; i++) {
        fprintf(file_ptr, "%d,%s\n", cfg->music_list[i].id, cfg->music_list[i].filepath);
    }
    fclose(file_ptr);
    return RET_NO_ERR;
}

int files_isDir(char * dirPath) {
    struct stat st;

    if (stat(dirPath, &st) != 0) {
        return RET_NO_ERR;
    }
    return S_ISDIR(st.st_mode);
}

int files_getFileExtension(const char * filePath, char * extension) {
    int i;

    for (i=0; i<strlen(filePath); i++) {
        if (filePath[i] == 46) break;
    }
    
    if (i == strlen(filePath) - 1) return RET_ERR_PARAM;
    strcpy(extension, (char *) (filePath+(i+1)));
    return RET_NO_ERR;
}