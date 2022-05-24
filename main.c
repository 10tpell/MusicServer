#include <stdlib.h>
#include <signal.h>
#include "cmd.h"
#include "server.h"
#include "music.h"

void handle_int(int sig);

int main(int argc, char *argv[]) {
    arg_options options = { "" };
    parse_args(argc, argv, &options);
    
    signal(SIGINT, handle_int);

    music_init();
    server_start(options.port);
    music_deinit();
    exit(0);
}

void handle_int(int sig) {
    if(sig != SIGINT) {
        return;
    }
    music_close();
    exit(0);
}