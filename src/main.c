#include "main.h"

int main(int argc, char** argv){
    uint8_t rawData[SECTION_LENGTH_FULL_MAX] = {0};
    uint16_t sectionLength = 0, i;

    /* Check arguments... */
    if(argc < 2){
        printf("To start a program, add file name to arguments...\n");
        return NO_SUCH_FILE;
    }

    file_openFile(argv[1]);

    parser_getSection(&rawData, pidNit);
    parser_parseSection(&rawData, pidNit);
    printf("Table ID: %i\nSection length: %i\nNetwork ID: %i\nVersion: %i\nSection: %i/%i\n",
            nit.tableID, nit.sectnLngth, nit.netID, nit.versionNum, nit.sectnNum, nit.lastSectnNum);
//    printf("Network dscr len: %i\nTransport dscr loop len: %i\nTransport dscr len: %i\nDscr tag: %i\nDscr len: %i\n",
//           nit.netDscrLngth, nit.trnspStrLpLngth, nit.trnspStream.trnspDscrLngth, nit.trnspStream.dscrTag, nit.trnspStream.dscrLen);
    printf("loop pointers: %i\n", nit.dscrLoopPtrsNum);

    file_closeFile();    

    return OK;
}

