#include <stdlib.h>
#include <signal.h>
#include "cmd.h"
#include "server.h"
#include "music.h"
#include "files.h"
#include "global_cfg.h"

/************************************************************************************************
 *  Function Declarations
************************************************************************************************/
void handle_int(int sig);


/************************************************************************************************
 *  Function Definitions
************************************************************************************************/
int main(int argc, char *argv[]) {
    arg_options options = { "" };
    parse_args(argc, argv, &options);
    
    signal(SIGINT, handle_int);

    music_init();
    server_start(options.port);
    music_close();
    server_close();
    //TODO: files_clean();
    exit(0);
}

void handle_int(int sig) {
    if(sig != SIGINT) {
        return;
    }
    //TODO: files_clean();
    music_close();
    server_close();
    exit(0);
}