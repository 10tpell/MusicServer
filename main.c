#include <stdlib.h>
#include "cmd.h"
#include "server.h"


int main(int argc, char *argv[]) {
    arg_options options = { "" };
    parse_args(argc, argv, &options);
    //printf("Options Struct:\nFile Location: %s\nPort: %d\n", options.file_locations, options.port);
    server_start(options.port);
    exit(0);
}