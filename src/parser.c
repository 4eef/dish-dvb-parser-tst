#include "parser.h"

/* Memory */

/* Inner mechanisms function prototypes */
void doSomething(void);


/* Interface functions */

/* Reads input file to get a single 188-byte packet */
uint8_t parser_getAnyPacket(elmntryPckt_type *pElmntryPckt){
    uint8_t status, readBuff[PACKET_LENGTH_BYTE] = {};
    uint16_t i = 0;
    /* Check argument pointer */
    if(NULL == pElmntryPckt){
        printf("Error with packet pointer!\n");
        return POINTER_ERR;
    }
    /* Find the synchronization byte */
    while(1){
        status = file_readToBuff(&readBuff[0], 1);
        i++;
        /* Error */
        if(status != NO_ERROR){
            // printf("Something happend with file reading.\n");
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
    pElmntryPckt->syncByte = readBuff[0];
    pElmntryPckt->trnspErrInd = (readBuff[1] & 0x80) >> 7;
    pElmntryPckt->pldUnitStrtInd = (readBuff[1] & 0x40) >> 6;
    pElmntryPckt->trnspPriority = (readBuff[1] & 0x20) >> 5;
    pElmntryPckt->packetID = (((readBuff[1] & 0x1F) << 8) | readBuff[2]);
    pElmntryPckt->trnspScrmblCntr = (readBuff[3] & 0xC0) >> 6;
    pElmntryPckt->adaptFldContr = (readBuff[3] & 0x30) >> 4;
    pElmntryPckt->contntyCnt = readBuff[3] & 0xF;
    memcpy(&pElmntryPckt->data[0], &readBuff[4], PAYLOAD_LENGTH_BYTE);
    return PACKET_GET_SUCCESS;
}

/* Get table section by table PID */
uint8_t parser_getAnySection(void *pRxBuff, ePidVals_type pidValue){
    bool pcktRxing = false;
    uint8_t rxStat = 0;
    uint16_t sctnLngth = 0;//, packetID = 0;
    uint32_t pcktsSkipped = 0;
    elmntryPckt_type rxPckt = {0};
    /* Check argument pointer */
    if(NULL == pRxBuff){
        printf("Error with packet pointer!\n");
        return POINTER_ERR;
    }
    while(1){
        /* Get single packet */
        rxStat = parser_getAnyPacket(&rxPckt);
        /* Check for errors */
        if(PACKET_READ_ERROR == rxStat){
            // printf("Done reading file.\n");
            return PACKET_READ_ERROR;
        }
        /* Filter it to needed identifier */
//        packetID = (rxPckt.packetIdH << 8) | rxPckt.packetIdL;
        if(rxPckt.packetID == pidValue){
            /* Start control */
//            if((rxPckt.pldUnitStrtInd == 1)){//) && (pcktRxing == false)){
                pcktRxing = true;
                /* Allocate memory to copy needed amount of data */
//                sctnLngth = ((rxPckt.data[(rxPckt.data[0] + 1)] << 8) | rxPckt.data[(rxPckt.data[0] + 2)]) & SECTION_LENGTH_MASK;
//                printf("Section length is %i\n%i packets skipped.\n", sctnLngth, pcktsSkipped);
                printf("PID: %i, TEI: %i, PUSI: %i, TSpr: %i, TSC: %i, adaFiCo: %i, cnt: %i\n", rxPckt.packetID, rxPckt.trnspErrInd, rxPckt.pldUnitStrtInd, rxPckt.trnspPriority, rxPckt.trnspScrmblCntr, rxPckt.adaptFldContr, rxPckt.contntyCnt);
                pcktsSkipped = 0;
//                return SECTION_GET_READY;
//            }
            /* Add data to  */
            switch(pidValue){
                case pidNit:
                    break;
                default:
                    break;
            }
            /* End control */
//            if((rxPckt.pldUnitStrtInd == 1) && (pcktRxing == true)){
//                pcktRxing = false;
//                /* Done copying data */
//                break;
//            }
        }
//        packetID = 0;
        pcktsSkipped++;
    }
    return SECTION_GET_READY;
}


/* Inner functions */

/* Do something */
void doSomething(void){

}

