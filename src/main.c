#include "main.h"

int main(int argc, char** argv){
    size_t *pString;
    char filename[20] = {}, string[100] = {}, c;
    FILE *p = NULL;

    /* Check for arguments... */
    if(argc < 2){
        printf("To start a program, add file name to arguments...\n");
        return ERR_NO_FILE;
    }

    p = fopen(argv[1], "r");

    /* If there is no such file... */
    if(NULL == p){
        printf("Argument read error\n");
        return ERR_NO_FILE;
    }

    

    return OK;
}

