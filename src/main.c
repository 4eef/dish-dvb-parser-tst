#include "main.h"

int main(int argc, char** argv){
    uint8_t i, rawData[SECTION_LENGTH_FULL_MAX] = {0};

    /* Check arguments... */
    if(argc < 2){
        printf("To start a program, add file name to arguments...\n");
        return NO_SUCH_FILE;
    }

    file_openFile(argv[1]);

    /* Get all sections */
    for(i = 0; i < TABLE_SECTIONS_MAX_NUM; i++){
        parser_getSection(&rawData, pidNit);
        parser_parseSection(&rawData, pidNit);
        /* Reduce number of parse iterations to number of sections */
        if(i >= nit.lastSectnNum){
            break;
        }
    }

    printf("Table ID: %i\nSection length: %i\nNetwork ID: %i\nVersion: %i\nSections: %i\n",
            nit.tableID, nit.sectnLngth, nit.netID, nit.versionNum, nit.lastSectnNum);
    printf("Freq list tag: %i\n", freqListDscr.dscrTag);
    printf("Service list tag: %i\n", srvcLstDscr.dscrTag);
    printf("Sat dlvr tag: %i\n", satDlvrSysDscr.dscrTag);
    printf("Sat frequency is: %i%i%i,%i%i%i%i%i GHz\n", satDlvrSysDscr.frequency[0], satDlvrSysDscr.frequency[1],
           satDlvrSysDscr.frequency[2], satDlvrSysDscr.frequency[3], satDlvrSysDscr.frequency[4], satDlvrSysDscr.frequency[5],
           satDlvrSysDscr.frequency[6], satDlvrSysDscr.frequency[7]);

    file_closeFile();    

    return OK;
}

