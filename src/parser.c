#include "parser.h"

/* Memory */


/* Reads input file to get a single 188-byte packet */
uint8_t parser_getPacket(elmntryPckt_type *pElmntryPckt){
    uint8_t status, readBuff[PACKET_LENGTH_BYTE] = {};
    uint16_t i = 0;
    /* Find the synchronization byte */
    while(1){
        status = file_readToBuff(&readBuff[0], 1);
        i++;
        /* Error */
        if(status != NO_ERROR){
            printf("Something happend with file reading. Closing...\n");
            file_closeFile();
            return PACKET_READ_ERROR;
        }
        /* Out of reading attempts */
        if(i >= BYTES_TO_READ){
            printf("Can't find packet synchronization byte!\n");
            return PACKET_NO_SYNC_BYTE;
        }
        /* Check for synchronization byte */
        if(readBuff[0] == HEADER_SYNC_BYTE){
            file_readToBuff(&readBuff[1], (PACKET_LENGTH_BYTE - 1));
            // printf("Found one, bro! On %i byte.\n", i);
            break;
        }
    }
    memcpy(pElmntryPckt, &readBuff[0], PACKET_LENGTH_BYTE);
    return PACKET_GET_SUCCESS;
}

