#ifndef SERVER_CMDS_H
#define SERVER_CMDS_H

#include <stdio.h>

typedef struct server_command {
    char * cmd_name;
    void (*cmd_hndl) (void *);
    int n_params;
} server_command;

void play_music_path(void * filename);
void play_music_id(void * fileId);
void res_music();
void pause_music();
void stop_music();
void list_tracks();
void close_srv();

extern server_command cmd_list[7];

#endif /* SERVER_CMDS_H */