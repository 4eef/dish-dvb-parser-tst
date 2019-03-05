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
    pElmntryPckt->syncByte = readBuff[HEADER_SYBC_BYTE_POS];
    pElmntryPckt->trnspErrInd = (readBuff[HEADER_INDS_PIDH_POS] & HEADER_TEI_MSK) >> 7;
    pElmntryPckt->pldUnitStrtInd = (readBuff[HEADER_INDS_PIDH_POS] & HEADER_PUSI_MSK) >> 6;
    pElmntryPckt->trnspPriority = (readBuff[HEADER_INDS_PIDH_POS] & HEADER_TP_MSK) >> 5;
    pElmntryPckt->packetID = (((readBuff[HEADER_INDS_PIDH_POS] & HEADER_PIDH_MSK) << 8) | readBuff[HEADER_PIDL_POS]);
    pElmntryPckt->trnspScrmblCntr = (readBuff[HEADER_SERV_FLDS_POS] & HEADER_TSC_MSK) >> 6;
    pElmntryPckt->adaptFldContr = (readBuff[HEADER_SERV_FLDS_POS] & HEADER_AFC_MSK) >> 4;
    pElmntryPckt->contntyCnt = readBuff[HEADER_SERV_FLDS_POS] & HEADER_CONT_CNT_MSK;
    memcpy(&pElmntryPckt->data[0], &readBuff[4], PAYLOAD_LENGTH_BYTE);
    return PACKET_GET_SUCCESS;
}

/* Get table section by table PID */
uint8_t parser_getSection(void *pRxBuff, ePidVals_type pidValue){
    bool pcktRxing = false;
    uint8_t rxStat = 0, numToCpy = 0, ptrOffs = 0, *pRxBuffCopy = NULL;
    uint16_t i, sctnLngth = 0, fullSctnLngth = 0;
    elmntryPckt_type rxPckt = {0};
    /* Check argument pointer */
    if(NULL == pRxBuff){
        printf("Error with packet pointer!\n");
        return POINTER_ERR;
    }
    pRxBuffCopy = pRxBuff;
    while(1){
        /* Get single packet */
        rxStat = parser_getAnyPacket(&rxPckt);
        /* Check for errors */
        if(PACKET_READ_ERROR == rxStat){
            // printf("Done reading file.\n");
            return PACKET_READ_ERROR;
        }
        /* Filter it to needed identifier */
        if(rxPckt.packetID == pidValue){
            /* Add Counter control */
            numToCpy = PAYLOAD_LENGTH_BYTE;
            ptrOffs = 0;
            /* Start & end control */
            if((rxPckt.pldUnitStrtInd != 0) && (pcktRxing == false)){
                /* Start control */
                pcktRxing = true;
                /* Retreive section length and check it */
                sctnLngth = ((rxPckt.data[(rxPckt.data[DATA_POINTER_POS] + DATA_SECT_LEN_H_POS)] << 8) |
                              rxPckt.data[(rxPckt.data[DATA_POINTER_POS] + DATA_SECT_LEN_L_POS)]) & SECTION_LENGTH_MASK;
                if(sctnLngth >= SECTION_LENGTH_MAX){
                    printf("Section is too large!\n");
                    return SECTION_LENGTH_ERR;
                }
                /* Calculate full section length */
                fullSctnLngth = sctnLngth + DATA_SECT_LEN_L_POS;
                /* Calculate offsets */
                numToCpy = PAYLOAD_LENGTH_BYTE - rxPckt.data[DATA_POINTER_POS] - 1;
                ptrOffs = rxPckt.data[DATA_POINTER_POS] + DATA_PTR_PAYLD_POS;
            }
            /* Receive the packet */
            if(pcktRxing == true){
                /* for() used instead of memcpy() for purpose */
                for(i = 0; i < numToCpy; i++){
                    if(fullSctnLngth != 0){
                        fullSctnLngth--;
                        *pRxBuffCopy = rxPckt.data[i + ptrOffs];
                        pRxBuffCopy++;
                    }else{ /* Done copying */
                        break;
                    }
                }
                /* End control */
                if(fullSctnLngth == 0){
                    pcktRxing = false;
                    break;
                } 
            }
        }
    }
    return SECTION_GET_READY;
}

/* Parse section */
uint8_t parser_parseSection(void){
    return 0;
}


/* Inner functions */

/* Do something */
void doSomething(void){

}

