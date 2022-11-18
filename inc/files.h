#ifndef FILES_H
#define FILES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "global_cfg.h"

#define FILE_PATH_MAX_LEN 4096

typedef enum music_cfg_elem {
    MUSIC_ID, MUSIC_FILEPATH
} music_cfg_enum;

int files_searchDir(char * dirPath, files_music_list * cfg);
int files_parseFileList(char * filePath, files_music_list * cfg);
int files_saveMusicListToFile(files_music_list * cfg, char * filePath);
int files_isDir(char * dirPath);
void files_clean();

#endif