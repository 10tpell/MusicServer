#include "server_commands.h"
#include "music.h"
#include "global_cfg.h"

server_command cmd_list[] = {
    { "play filepath", &play_music_path, 1 },
    { "play id", &play_music_id, 1},
    { "pause", &pause_music, 0},
    { "list tracks", &list_tracks, 0},
    { "stop", &stop_music, 0},
    { "resume", &res_music, 0},
    { "close", &close_srv, 0}
};

void play_music_path(void * filename) {
    if(verbose_b) printf("playing: %s \n", (char *) filename);
    if(music_isPlaying()) {
        res_music();
        return;
    }
    music_openFile("/home/travis/Code/MusicServer/resources/test.wav");
    music_play();
    return;
}

void play_music_id(void * fileId) {
    char * filePath;

    int id = atoi((char *)fileId);
    if(cfg_getFilePathFromId(&filePath, id) == 0) {
        play_music_path(filePath);  
    }
    free(filePath);
    return;  
}

void pause_music() {
    if(verbose_b) printf("pausing tune\n");
    music_pause();
    return;
}

void res_music() {
    if(verbose_b) printf("Resuming playback");
    music_resume();
    return;
}

void stop_music() {
    if(verbose_b) printf("Stopping playback \n");
    music_stop();
    return;
}

void list_tracks() {
    return;
}

void close_srv() {
    music_close();
    exit(0);
}