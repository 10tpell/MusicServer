#include "cmd.h"

int parse_args(int argc, char * argv[], arg_options * opts) {
    unsigned int i;
    printf("Parsing...\n");
    for (i=1; i<argc; i++) {
        if(strcmp(argv[i], "-v") == 0) {
            printf("verbose mode enabled: %s \n", argv[i]);
        } else if(strcmp(argv[i], "-f") == 0){
            strcpy(&(opts->file_locations[0]), argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "-p") == 0) {
            opts->port = atoi(argv[i+1]);
            i++;
        }
    }

    return 0;
}
