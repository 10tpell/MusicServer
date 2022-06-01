#ifndef FILES_H
#define FILES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define FILE_PATH_MAX_LEN 4096

typedef enum music_cfg_elem {
    MUSIC_ID, MUSIC_FILEPATH
} music_cfg_enum;

typedef struct cfg_struct {
    int id;
    char * filepath;
} files_music_cfg;

typedef struct {
    files_music_cfg * music_list;
    int len;
} files_music_list;

int files_parseFileList(char * filePath, files_music_list * cfg);
int files_saveMusicListToFile(files_music_list * cfg, char * filePath);

#endif