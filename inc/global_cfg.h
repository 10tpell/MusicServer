#ifndef GLOBAL_H
#define GLOBAL_H

#define NUM_ALLOWED_EXTENSIONS 2

#define MUSIC_LIST_DEFAULT_CAP 100
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
    int capacity;
} files_music_list;

/* General global config */
typedef struct {
    char ** allowed_extensions;
} global_cfg;
/************************************************************************************************
 *  Global Variables
************************************************************************************************/

extern int verbose_b;
extern files_music_list * music_list;
extern global_cfg glb_cfg;

int cfg_getFilePathFromId(char * * filePath, int id);
void cfg_addFile(const char * filePath);
int cfg_init();
void cfg_close();

#endif