#include "server_commands.h"
#include "music.h"

server_command cmd_list[] = {
    { "play", &play_music, 1 },
    { "pause", &pause_music, 0},
    { "list tracks", &list_tracks, 0},
    { "stop", &stop_music, 0},
    { "resume", &res_music, 0},
    { "close", &close_srv, 0}
};

void play_music(char* filename) {
    printf("playing: %s \n", filename);
    if(music_isPlaying()) {
        res_music();
        return;
    }
    music_openFile("/home/travis/Code/MusicServer/resources/test.wav");
    music_play();
    return;
}

void pause_music() {
    printf("pausing tune\n");
    music_pause();
    return;
}

void res_music() {
    printf("Resuming playback");
    music_resume();
    return;
}

void stop_music() {
    printf("Stopping playback \n");
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