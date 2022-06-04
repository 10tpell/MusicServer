#ifndef GLOBAL_H
#define GLOBAL_H

/************************************************************************************************
 *  Typedefs
************************************************************************************************/

/* music storage related data types */

typedef struct cfg_struct {
    int id;
    char * filepath;
} files_music_cfg;

typedef struct {
    files_music_cfg * music_list;
    int len;
} files_music_list;

/************************************************************************************************
 *  Global Variables
************************************************************************************************/

extern int verbose_b;
extern files_music_list * music_list;

int cfg_getFilePathFromId(char * * filePath, int id);

#endif