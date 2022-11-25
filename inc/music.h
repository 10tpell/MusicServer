#ifndef MUSIC_H
#define MUSIC_H

#include <ao/ao.h>
#include <sndfile.h>
#include <pthread.h>

#define MUSIC_BUFFER_SIZE 8192
#define MUSIC_DECODER_LIBSNDFILE 0
#define MUSIC_DECODER_LIBMPG123 1
#define MUSIC_DECODER_UNDEFINED 255

#define MUSIC_BITS 8


void * player_thread_hndl(void *);

int music_init();
void music_deinit();
void music_stop();
void music_resume();
void music_pause();
void music_play();
void music_close();
int music_openFile();
int music_isPlaying();

#endif