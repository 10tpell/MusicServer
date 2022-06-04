#include "global_cfg.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/************************************************************************************************
 *  Initialise Global Variables
************************************************************************************************/
int verbose_b = 0;
files_music_list * music_list;

int cfg_getFilePathFromId(char * * filePath, int id) {
    int ret = 1;
    for (int i = 0; i < music_list->len; i++) {
        printf("%d\n", i);
        if (music_list->music_list[i].id == id) {
            printf("correct id found\n");

            /* for some reason it requires a pointer to a pointer for this? */
            *filePath = malloc(strlen(music_list->music_list[i].filepath) * sizeof(char));
            strcpy(*filePath, music_list->music_list[i].filepath);

            /* break wasn't working here for some reason */
            ret = 0;
            i = music_list->len;
        }
    }
    return ret;
}