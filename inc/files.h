#ifndef FILES_H
#define FILES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int files_parseFileList(char * filePath, char * * fileArray, int * lenX, int * lenY);

#endif