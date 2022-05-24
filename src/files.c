#include "files.h"

int files_parseFileList(char * filePath, char * * fileArray, int * lenX, int * lenY) {
    FILE * file_ptr;
    char ch;
    char * file_str;
    int tmp_line_len, tmp_x_idx;
    struct stat stat_str;

    char n = '\n';
    int i = 0;

    if(stat(filePath, &stat_str) == -1) {
        printf("FAILED TO LOAD FILE \n");
        return -1;
    }
    
    file_ptr = fopen(filePath, "r");
    if (NULL == file_ptr) {
        printf("FAILED TO LOAD FILE\n");
        return -1;
    } 

    file_str = malloc(stat_str.st_size);

    *lenX = 0;
    *lenY = 0;
    tmp_line_len = 0;

    while(ch != EOF) {
        ch = fgetc(file_ptr);
        
        if(ch == n) {
            (*lenY)++;
            if(*lenX < tmp_line_len) *lenX = tmp_line_len;
            tmp_line_len = 0;
        } else {
            tmp_line_len++;
        }

        file_str[i] = ch;
        i++;
    }

    fclose(file_ptr);

    fileArray = malloc(sizeof(char) * (*lenY + 1));

    tmp_line_len = 0;
    tmp_x_idx = 0;
    int total = 0;
    for (int j=0; j<*lenY; j++) {
        fileArray[j] = malloc(sizeof(char) * (*lenX)); /* Todo: optimise this */
        for (int k=0; k<*lenX; k++) {
            total++;
            if(file_str[total] == n) {
                break;
            }
            fileArray[j][k] = file_str[total];
        }
    }

    free(file_str);

    return 0;
}