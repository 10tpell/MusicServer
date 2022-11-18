#include "cmd.h"
#include "files.h"
#include "global_cfg.h"

int parse_args(int argc, char * argv[], arg_options * opts) {
    unsigned int i,x,y;
    music_list = malloc((1000 * (sizeof(char *) + sizeof(int))) + sizeof(int));
    music_list->capacity = 1000;
    music_list->len = 0;

    printf("Parsing...\n");
    for (i=1; i<argc; i++) {
        if(strcmp(argv[i], "-v") == 0) {
            printf("verbose mode enabled: %s \n", argv[i]);
            verbose_b = 1;
        } else if(strcmp(argv[i], "-f") == 0){
            strcpy(&(opts->file_locations[0]), argv[i+1]);
            files_parseFileList(argv[i+1], music_list);
            files_saveMusicListToFile(music_list, "/home/travis/Code/MusicServer/resources/music_list.txt");
            i++;
        } else if(strcmp(argv[i], "-fp") == 0) {
            files_searchDir(argv[i+1], music_list);
        } else if(strcmp(argv[i], "-p") == 0) {
            opts->port = atoi(argv[i+1]);
            i++;
        }
    }

    return 0;
}
