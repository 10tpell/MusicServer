#ifndef CMD_H
#define CMD_H

#include <stdio.h>
#include <string.h>

#define MAX_FILE_PATH 256

typedef struct arg_options {
    char file_locations[MAX_FILE_PATH]; /* location of a file which contains locations to look for media - TODO: Support this through command line */
    int port;                           /* port which media server will bind to */
    
} arg_options;

int parse_args(int argc, char * argv[], arg_options * opts);

#endif /* CMD_H */