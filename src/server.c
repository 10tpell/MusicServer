#include "server.h"

int server_start(int port) {
    int ret, sock_desc, new_sock, * sock_new;
    struct sockaddr_in server, client;
    socklen_t cl_size;
    pthread_t sock_thread;
    
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    ret = sock_desc;
    if(ret >= -1) {
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(port);
        printf("Opened Socket: %d\n", sock_desc);
        ret = bind(sock_desc, (const struct sockaddr *) &server, (socklen_t) sizeof(server));
        if (ret >= 0) {
            printf("Binded to port: %d, status: %d, socket: %d\n", port, ret, sock_desc);

            ret = listen(sock_desc, 10);
            if (ret >= 0) {
                printf("Listening.... status: %d\n", ret);
                cl_size = (socklen_t) sizeof(client);
                while(new_sock = accept(sock_desc, (struct sockaddr *) &client, &cl_size)) {
                    printf("Connection accepted...\n");

                    sock_new = malloc(1);
                    *sock_new = new_sock;
                    printf("socket created\n");
                    if(pthread_create(&sock_thread, NULL, connection_hndl, (void *) sock_new) < 0) {
                        printf("ERROR: FAILED TO START THREAD\n");
                        return -1;
                    }
                }
            }
        } else {
            printf("Error binding to socket: %s (%d)", strerror(errno), errno);
        }
    }

    return ret;
}

void * connection_hndl(void * sock) {
    char * msg, msg_buf[SERVER_MESSAGE_MAX_LEN];
    int msg_len;

    char * play_cmd = "play";
    int con_sock_desc = *(int *)sock;

    printf("in thread\n");

    msg = "Connected to music server!\n";
    write(con_sock_desc, msg, strlen(msg));

    while(msg_len = recv(con_sock_desc, msg_buf, SERVER_MESSAGE_MAX_LEN, 0)) {
        if(msg_len <= 0) break;
        printf("Recieved msg: %s (size: %d)\n", msg_buf, (int) strlen(msg_buf));
        if(memcmp(msg_buf, play_cmd, 1) == 0) {
            printf("playing: %s \n", ((char *)msg_buf + strlen(play_cmd)+1));
        }
    }

    if (msg_len == 0) {
        printf("Connection closed\n");
    } else {
        msg = "Closing connection due to error";
        write(con_sock_desc, msg, sizeof(msg));
        printf("ERROR: %s (%d)\n", strerror(errno), errno);
    }
    close(con_sock_desc);
    free(sock);
    return 0;
}