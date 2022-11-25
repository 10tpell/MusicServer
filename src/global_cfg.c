#include "global_cfg.h"
#include "errors.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/************************************************************************************************
 *  Initialise Global Variables
************************************************************************************************/
int verbose_b = 0;
files_music_list * music_list;

global_cfg glb_cfg;
static const char allowedExt[NUM_ALLOWED_EXTENSIONS][8] = {"wav", "mp3"};
//     {NUM_ALLOWED_EXTENSIONS,
//     .allowed_extensions = {"wav", "mp3"} /* TODO: Fix this (BROKEN) */
// };

int cfg_init() {
    glb_cfg.allowed_extensions = malloc(NUM_ALLOWED_EXTENSIONS * sizeof(char *));

    if(glb_cfg.allowed_extensions < (char **) 1) return RET_ERR_MALLOC;

    for(unsigned char i = 0; i < NUM_ALLOWED_EXTENSIONS; i++) {
        glb_cfg.allowed_extensions[i] = malloc(8 * sizeof(char));
        if(glb_cfg.allowed_extensions[i] < (char *) 1) return RET_ERR_MALLOC;

        strcpy(glb_cfg.allowed_extensions[i], allowedExt[i]);
    }
}

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
    int ret = RET_ERR_PARAM;
    for (int i = 0; i < music_list->len; i++) {
        printf("%d\n", i);
        if (music_list->music_list[i].id == id) {
            printf("correct id found\n");

            /* for some reason it requires a pointer to a pointer for this? */
            *filePath = malloc(strlen(music_list->music_list[i].filepath) * sizeof(char));
            if(*filePath < 1) return RET_ERR_MALLOC;

            strcpy(*filePath, music_list->music_list[i].filepath);

            ret = RET_NO_ERR;

            /* break wasn't working here for some reason (so terminated for loop by changing i) */
            i = music_list->len;
        }
    }
    return ret;
}