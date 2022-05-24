#ifndef SERVER_CMDS_H
#define SERVER_CMDS_H

#include <stdio.h>

typedef struct server_command {
    char * cmd_name;
    void (*cmd_hndl) (void *);
    int n_params;
} server_command;

void play_music(char* filename);
void res_music();
void pause_music();
void stop_music();
void list_tracks();

#endif /* SERVER_CMDS_H */