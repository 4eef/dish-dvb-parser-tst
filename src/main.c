#include "main.h"

int main(int argc, char** argv){
    netInfoTable_type nit = {0};
    uint8_t tst[1024] = {0};
    uint16_t sectionLength = 0, i;

    /* Check arguments... */
    if(argc < 2){
        printf("To start a program, add file name to arguments...\n");
        return NO_SUCH_FILE;
    }

    file_openFile(argv[1]);

    parser_getSection(&tst, pidNit);
    parser_parseSection(&nit, &tst, pidNit);
    printf("Table ID: %i\nSection length: %i\nNetwork ID: %i\nVersion: %i\nSection: %i/%i\n",
            nit.tableID, nit.sectnLngth, nit.netID, nit.versionNum, nit.sectnNum, nit.lastSectnNum);
    printf("Net name length: %i\nNetwork name: %s\n", nit.netDscr.dscrLngth, &nit.netDscr.name[0]);

    file_closeFile();    

    return OK;
}

