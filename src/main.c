#include "main.h"

int main(int argc, char** argv){
    elmntryPckt_type tstPacket = {};
    uint8_t tstSection[2048] = {};
    uint16_t sectionLength = 0, fullSectionLength = 0;

    /* Check arguments... */
    if(argc < 2){
        printf("To start a program, add file name to arguments...\n");
        return NO_SUCH_FILE;
    }

    file_openFile(argv[1]);

    parser_getAnyPacket(&tstPacket);
    parser_getAnyPacket(&tstPacket);
    parser_getAnyPacket(&tstPacket);
    parser_getAnyPacket(&tstPacket);
    parser_getAnyPacket(&tstPacket);
/*
    parser_getAnyPacket(&tstPacket);
    printf("1 byte %i\n", tstPacket.syncByte);
    printf("2 byte %i\n", tstPacket.raw0);
    printf("3 byte %i\n", tstPacket.packetIdL);
    printf("4 byte %i\n", tstPacket.raw1);
    printf("PID is %i\n", ((tstPacket.packetIdH << 8) | tstPacket.packetIdL));
    parser_getAnyPacket(&tstPacket);
    printf("1 byte %i\n", tstPacket.syncByte);
    printf("2 byte %i\n", tstPacket.raw0);
    printf("3 byte %i\n", tstPacket.packetIdL);
    printf("4 byte %i\n", tstPacket.raw1);
    printf("PID is %i\n", ((tstPacket.packetIdH << 8) | tstPacket.packetIdL));
*/
    parser_getSection(&tstSection, pidNit);
    sectionLength = (tstSection[1] << 8) | tstSection[2];
    fullSectionLength = strlen(&tstSection[0]);
    printf("Table ID: %i, Section length: %i, Est. section length: %i\n", tstSection[0], sectionLength, fullSectionLength);

    file_closeFile();    

    return OK;
}

