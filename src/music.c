#include "music.h"
#include "global_cfg.h"
#include "errors.h"
#include <fcntl.h>
#include <mpg123.h>

static int driver_id = 0;
static int music_play_b = 0;
static int paused_b = 0;
static int decoder_lib = MUSIC_DECODER_UNDEFINED;

static SNDFILE * snd_file = NULL;
static mpg123_handle * mh = NULL;
static ao_device * music_device = NULL;
static pthread_t player_thread = (pthread_t) NULL;

int music_init(void) {
    ao_initialize();
    driver_id = ao_default_driver_id();
    if(driver_id == -1) return RET_ERR_INIT;
    return RET_NO_ERR;
}

int music_isPlaying() {
    return music_play_b;
}

int music_openFile(char * fileName) {
    SF_INFO audio_info = {};
    ao_sample_format format;
    int err;
    int channels, encoding;
    long rate;

    int fd = open(fileName, O_RDONLY);
    /* open file using libsnd so that we can read the audio info */
    snd_file = sf_open_fd(fd, SFM_READ, &audio_info, SF_TRUE);

    if(snd_file == (SNDFILE *) 0) {
        printf("Error with libsndfile opening file: %s\n", sf_strerror((SNDFILE *) NULL));
        printf("libsndfile version %s\n", sf_version_string());
        close(fd);
        if(sf_error((SNDFILE *) NULL) == SF_ERR_UNRECOGNISED_FORMAT) {
            /* libsndfile does not support this format so switching to libmpg123 */
            printf("Attempting to use libmpg123...\n");
            // TODO : check file extension (might not always be an mp3)

            /* initializations */
            mpg123_init();
            mh = mpg123_new(NULL, &err);

            if(mh == (mpg123_handle *) 0) return RET_ERR_INIT;

            /* open the file and get the decoding format */
            fd = open(fileName, O_RDONLY);
            if(MPG123_OK != mpg123_open_fd(mh, fd)) 
            {
                printf("libmpg123: unable to open file. \n");
                printf("libmpg123: %s\n", mpg123_strerror(mh));
                return RET_ERR_IO; /* libmpg123 isn't the right lib either */ 
            }
            
            if(MPG123_OK != mpg123_getformat(mh, &rate, &channels, &encoding)) {
                printf("libmpg123: unable to open file.\n");
                printf("libmpg123: %s\n", mpg123_strerror(mh));
                return RET_ERR_IO;
            }

            decoder_lib = MUSIC_DECODER_LIBMPG123; /* libmpg123 is able to read file */

            /* set the output format and open the output device */
            format.bits = mpg123_encsize(encoding) * MUSIC_BITS;
            format.rate = rate;
            format.channels = channels;
            format.byte_format = AO_FMT_NATIVE;
            format.matrix = 0;
        }
    } else {
        decoder_lib = MUSIC_DECODER_LIBSNDFILE;

        /* fill out all of the info libao requires */
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
    }

    /* open the music device ready for playback */
    music_device = ao_open_live(driver_id, &format, (ao_option *) NULL);
    if (music_device == (ao_device *) NULL) {
        printf("Error opening libao device...\n");
        return RET_ERR_IO;
    }

    if(verbose_b) printf("successfully created libao device... \n");
    return RET_NO_ERR;
}

void music_play(void) {
    /* make sure that a music player is not already open (music_play_b does not 100% represent
       whether the thread is running or not, but it's good enough)*/
    if(!music_play_b) {
        pthread_create(&player_thread, NULL, player_thread_hndl, NULL);
    }
    return;
}

void * player_thread_hndl(void * args) {
    (void) args;
    short * audio_buffer;
    int sf_read = 0, buffer_size = 0;
    size_t done;
    
    music_play_b = 1;

    if(decoder_lib == MUSIC_DECODER_LIBMPG123) {
        buffer_size = mpg123_outblock(mh);
        audio_buffer = malloc(buffer_size * sizeof(unsigned char));
        
    } else if (decoder_lib == MUSIC_DECODER_LIBSNDFILE) {
        audio_buffer = calloc(MUSIC_BUFFER_SIZE, sizeof(short));
    }

    while(music_play_b) {
        if(!paused_b) {
            /* read in music MUSIC_BUFFER_SIZE amount of bytes at a time, then play it */
            if(decoder_lib == MUSIC_DECODER_LIBSNDFILE) {
                sf_read = sf_read_short(snd_file, audio_buffer, MUSIC_BUFFER_SIZE);
                if(ao_play(music_device, (char *) audio_buffer, sf_read * sizeof(short)) == 0) {
                    break;
                }
            } else if (decoder_lib == MUSIC_DECODER_LIBMPG123) {
                if(mpg123_read(mh, audio_buffer, buffer_size, &done) != MPG123_OK) {
                    music_play_b = 0;
                    pthread_exit(NULL);
                }

                int play_e = ao_play(music_device, (char *) audio_buffer, done);
                if(play_e == 0) {
                    printf("finishing playback, error: %d\n", play_e);
                    music_play_b = 0;
                    break;
                }
            }
        }
    }
    free(audio_buffer);
    pthread_exit(NULL);
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

/* Need to clean up memory allocated for music libraries */
void music_close(void) {
    music_stop();
    pthread_join(player_thread, NULL);
    ao_close(music_device);
    if(decoder_lib == MUSIC_DECODER_LIBMPG123) {
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
    } else if (decoder_lib == MUSIC_DECODER_LIBSNDFILE) {
        sf_close(snd_file);
    }
    music_deinit();
    return;
}