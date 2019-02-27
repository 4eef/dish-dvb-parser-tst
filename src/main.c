#include "main.h"

int main(int argc, char** argv){
    elmntryPckt_type tstPacket = {};

    /* Check arguments... */
    if(argc < 2){
        printf("To start a program, add file name to arguments...\n");
        return NO_SUCH_FILE;
    }

    file_openFile(argv[1]);

    parser_getPacket(&tstPacket);

    printf("Packet ID is %i\n", tstPacket.packetID);
    printf("Unit start indicator is %i\n", tstPacket.pldUnitStrtInd);
    printf("Continuity counter is %i\n", tstPacket.contntyCnt);

    memset(&tstPacket, 0, sizeof(elmntryPckt_type));

    parser_getPacket(&tstPacket);

    printf("Packet ID is %i\n", tstPacket.packetID);
    printf("Unit start indicator is %i\n", tstPacket.pldUnitStrtInd);
    printf("Continuity counter is %i\n", tstPacket.contntyCnt);

    file_closeFile();

    

    return OK;
}

