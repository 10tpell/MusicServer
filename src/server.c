#include "server.h"
#include "server_commands.h"
#include "global_cfg.h"
#include "errors.h"

static int sock_desc;

int server_start(int port) {
    int ret, new_sock, * sock_new;
    struct sockaddr_in server, client;
    socklen_t cl_size;
    pthread_t sock_thread;
    
    /* open up socket for server to listen on */
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    ret = sock_desc;
    if(ret >= -1) {

        /* configure port that socket should bind to and then attempt to bind */
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(port);
        if(verbose_b) printf("Opened Socket: %d\n", sock_desc);
        ret = bind(sock_desc, (const struct sockaddr *) &server, (socklen_t) sizeof(server));
        if (ret >= 0) {
            if(verbose_b) printf("Binded to port: %d, status: %d, socket: %d\n", port, ret, sock_desc);
            
            /* Bound to socket, now listen for messages */
            ret = listen(sock_desc, 10);
            if (ret >= 0) {
                if(verbose_b) printf("Listening.... status: %d\n", ret);

                /* wait for a new connection from client, upon new connection start a new thread to handle connection */
                cl_size = (socklen_t) sizeof(client);
                while(new_sock = accept(sock_desc, (struct sockaddr *) &client, &cl_size)) {
                    printf("New connection accepted...\n");

                    sock_new = malloc(1);
                    if(sock_new < (int *) 1) return RET_ERR_MALLOC;

                    *sock_new = new_sock;
                    if(verbose_b) printf("socket created\n");
                    if(pthread_create(&sock_thread, NULL, connection_hndl, (void *) sock_new) < 0) {
                        printf("ERROR: FAILED TO START THREAD\n");
                        return RET_ERR_THREAD;
                    }
                }
            }
        } else {
            printf("Error binding to socket: %s (%d)", strerror(errno), errno);
        }
    }

    return ret;
}

void server_close() {
    close(sock_desc);
}

void * connection_hndl(void * sock) {
    char * msg, msg_buf[SERVER_MESSAGE_MAX_LEN];
    int msg_len;

    int con_sock_desc = *(int *)sock;
    int cmd_valid = 0;

    msg = "Connected to music server!\n";
    write(con_sock_desc, msg, strlen(msg));

    /* receive messages from client and handle them according to config */
    while(msg_len = recv(con_sock_desc, msg_buf, SERVER_MESSAGE_MAX_LEN, 0)) {
        if(msg_len <= 0) break; /* check that client hasn't closed connection */


        cmd_valid = 0; /* used to determine later if the command supplied is valid */
        if(verbose_b) printf("Recieved msg: %s (size: %d)\n", msg_buf, (int) strlen(msg_buf));

        /* loop through list of possible commands */
        for (int i=0; i<sizeof(cmd_list)/sizeof(server_command); i++) {
            if(memcmp(msg_buf, cmd_list[i].cmd_name, strlen(cmd_list[i].cmd_name)) == 0) {
                /* user entered a valid command, now it must be handled */
                if(verbose_b) printf("Valid command.\n");
                if(cmd_list[i].n_params == 0) {
                    cmd_list[i].cmd_hndl(NULL);
                } else {
                    cmd_list[i].cmd_hndl(((char *)msg_buf + strlen(cmd_list[i].cmd_name)+1));
                }

                /* set buffer back to 0 otherwise remnants of previous commands stay in the buffer
                not necessary but doesn't look good if printing out */
                memset(msg_buf, 0, SERVER_MESSAGE_MAX_LEN);
                
                cmd_valid = 1;
            }
        }

        if(!cmd_valid) {
            msg = "Not a valid command. \n";
            write(con_sock_desc, msg, strlen(msg));
        }
    }

    /* handle closing of socket connection */
    if (msg_len == 0) {
        printf("Connection closed\n");
    } else {
        msg = "Closing connection due to error";
        write(con_sock_desc, msg, sizeof(msg));
        printf("ERROR: %s (%d)\n", strerror(errno), errno);
    }
    close(con_sock_desc);
    free(sock);
    return RET_NO_ERR;
}