#include "music.h"

static int driver_id = 0;
static int music_play_b = 0;
static int thread_ex = 0;
static int paused_b = 0;

static SNDFILE * snd_file = NULL;
static ao_device * music_device = NULL;
static pthread_t * player_thread = NULL;

int music_init(void) {
    ao_initialize();
    driver_id = ao_default_driver_id();
    if(driver_id == -1) return -1;
    return 0;
}

int music_isPlaying() {
    return music_play_b;
}

int music_openFile(char * fileName) {
    SF_INFO audio_info = {};
    ao_sample_format format;

    snd_file = sf_open(fileName, SFM_READ, &audio_info);
    printf("FILE POINTER: %d \n", snd_file);
    printf("FILE FORMAT: %x \n", audio_info.format);

    format.channels = audio_info.channels;
    format.rate = audio_info.samplerate;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    
    switch(audio_info.format & SF_FORMAT_SUBMASK) {
        case SF_FORMAT_PCM_16:
            format.bits = 16;
            break;
        case SF_FORMAT_PCM_24:
            format.bits = 24;
            break;
        case SF_FORMAT_PCM_32:
            format.bits = 32;
            break;
        case SF_FORMAT_PCM_S8:
            format.bits = 8;
            break;
        case SF_FORMAT_PCM_U8:
            format.bits = 8;
            break;
        default:
            format.bits = 16;
            break;  
    }

    music_device = ao_open_live(driver_id, &format, (ao_option *) NULL);

    return 0;
}

void music_play(void) {
    if(thread_ex) {
        return;
    }
    pthread_create(&player_thread, NULL, player_thread_hndl, NULL);
}

void player_thread_hndl(void) {
    short * audio_buffer;

    audio_buffer = calloc(MUSIC_BUFFER_SIZE, sizeof(short));
    music_play_b = 1;

    while(music_play_b) {
        if(!paused_b) {
            int sf_read = sf_read_short(snd_file, audio_buffer, MUSIC_BUFFER_SIZE);
            if(ao_play(music_device, audio_buffer, sf_read * sizeof(short)) == 0) {
                break;
            }
        }
    }
    free(audio_buffer);
    return;
}

void music_deinit(void) {
    ao_shutdown();
    return;
}

void music_stop(void) {
    music_play_b = 0;
    return;
}

void music_pause(void) {
    paused_b = 1;
    return;
}

void music_resume(void) {
    paused_b = 0;
    return;
}

void music_close(void) {
    music_stop();
    pthread_join(*player_thread, NULL);
    ao_close(music_device);
    sf_close(snd_file);
    music_deinit();
    return;
}