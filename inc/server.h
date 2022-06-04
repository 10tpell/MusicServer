#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <pthread.h>

#define SERVER_MESSAGE_MAX_LEN 1024

void * connection_hndl(void * sock);

int server_start(int port);
void server_close();

#endif /* SERVER_H */