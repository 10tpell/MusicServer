#include "sqlite3.h"
#include "errors.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static sqlite3 * db = 0;

/* TODO: Implement Callback */
static int callback (void * a, int b, char ** c, char ** d) {
    return RET_NO_ERR;
}

static int db_execQuery(char * query, char * err) {
    if (db == 0) return RET_ERR_INIT;

    sqlite3_exec(db, query, callback, 0, &err);

    return RET_NO_ERR;
}

int db_init() {
    if (sqlite3_initialize() != SQLITE_OK) return RET_ERR_INIT;  /* initialise sqlite3 */
    if (sqlite3_open("./db/MusicServer.db", &db) == 1) return RET_ERR_IO; /* open or create database */
    return RET_NO_ERR;  
}

int db_writeTrack(char * title, char * artist) {
    int len = 48;
    char * query;
    char * err;

    len += strlen(title) + strlen(artist);

    query = malloc(len * sizeof(char));
    if(query < (char *) 1) return RET_ERR_MALLOC;
    
    sprintf(query, "INSERT INTO Tracks (Title, Artist) Values ( %s, %s);", title, artist);
    printf("Executing query: %s\n", query);
    return db_execQuery(query, err);
}