#include "global_cfg.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/************************************************************************************************
 *  Initialise Global Variables
************************************************************************************************/
int verbose_b = 0;
files_music_list * music_list;
global_cfg glb_cfg = {
    2,
    {"wav", "mp3"} /* TODO: Fix this (BROKEN) */
};

void cfg_addFile(char * filePath) {
    if (music_list->len < music_list->capacity) {
        strcpy(music_list->music_list[music_list->len].filepath, filePath);
        music_list->music_list[music_list->len].id = music_list->music_list[music_list->len-1].id + 1;
        music_list->len++;
    } else {
        /* TODO: handle increasing music capacity */
    }
}

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