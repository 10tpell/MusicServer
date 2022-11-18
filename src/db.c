#include "sqlite3.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static sqlite3 * db = 0;

/* TODO: Implement Callback */
static int callback (void * a, int b, char ** c, char ** d) {
    return 0;
}

static int db_execQuery(char * query, char * err) {
    if (db == 0) return -1;

    sqlite3_exec(db, query, callback, 0, err);

    return 0;
}

int db_init() {
    if (sqlite3_initialize() != SQLITE_OK) return -1;  /* initialise sqlite3 */
    if (sqlite3_open("./db/MusicServer.db", &db) == 1) return -1; /* open or create database */
    return 0;  
}

int db_writeTrack(char * title, char * artist) {
    int len = 48;
    char * query;
    char * err;

    len += strlen(title) + strlen(artist);
    query = malloc(len * sizeof(char));
    
    sprintf(query, "INSERT INTO Tracks (Title, Artist) Values ( %s, %s);", title, artist);
    printf("Executing query: %s\n", query);
    return db_execQuery(query, err);
}