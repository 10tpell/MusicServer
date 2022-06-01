#include "cmd.h"
#include "files.h"
#include "global_cfg.h"

int verbose_b = 0;

int parse_args(int argc, char * argv[], arg_options * opts) {
    unsigned int i,x,y;
    files_music_list * cfg = malloc((1000 * (sizeof(char *) + sizeof(int))) + sizeof(int));
    cfg->len = 1000;
    
    printf("Parsing...\n");
    for (i=1; i<argc; i++) {
        if(strcmp(argv[i], "-v") == 0) {
            printf("verbose mode enabled: %s \n", argv[i]);
            verbose_b = 1;
        } else if(strcmp(argv[i], "-f") == 0){
            strcpy(&(opts->file_locations[0]), argv[i+1]);
            files_parseFileList(argv[i+1], cfg);
            files_saveMusicListToFile(cfg, "/home/travis/Code/MusicServer/resources/music_list.txt");
            i++;
        } else if(strcmp(argv[i], "-p") == 0) {
            opts->port = atoi(argv[i+1]);
            i++;
        }
    }

    return 0;
}
