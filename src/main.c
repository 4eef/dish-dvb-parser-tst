#include "main.h"

int main(int argc, char** argv){
    elmntryPckt_type tstPacket = {};

    /* Check arguments... */
    if(argc < 2){
        printf("To start a program, add file name to arguments...\n");
        return NO_SUCH_FILE;
    }

    file_openFile(argv[1]);

    parser_getAnySection(&tstPacket, pidNit);

    file_closeFile();    

    return OK;
}

