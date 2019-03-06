#include "parser.h"

/* Memory */
netInfoTable_type nit;
dummyDscr_type dummyDscr;
srvcLstDscr_type srvcLstDscr;
freqListDscr_type freqListDscr;
satDlvrSysDscr_type satDlvrSysDscr;
terrDlvrSysDscr_type terrDlvrSysDscr;


/* Inner mechanisms function prototypes */
void nitInit(void);
uint8_t parseNit(void *pRawSctn);
void parseNetNameDscr(void *pRawSctn);
uint8_t parseTrnspLoop(uint16_t loopLngth, void *pRawSctn);
uint8_t parseSrvcLstDscr(void *pRawSctn);
uint8_t parseFreqListDscr(void *pRawSctn);
uint8_t parseSatDlvrSysDscr(void *pRawSctn);
uint8_t parseTerrDlvrSysDscr(void *pRawSctn);


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
uint8_t parser_getSection(void *pRawSctn, ePidVals_type pidValue){
    bool pcktRxing = false;
    uint8_t rxStat = 0, numToCpy = 0, ptrOffs = 0, contCnt = 0, *pRxBuffCopy = NULL;
    uint16_t i, sctnLngth = 0, fullSctnLngth = 0;
    elmntryPckt_type rxPckt = {0};

    /* Check argument pointer */
    if(NULL == pRawSctn){
        printf("Error with packet pointer!\n");
        return POINTER_ERR;
    }
    pRxBuffCopy = pRawSctn;
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
            /* Counter control */
            numToCpy = PAYLOAD_LENGTH_BYTE;
            ptrOffs = 0;
            /* Start & end control */
            if((rxPckt.pldUnitStrtInd != 0) && (pcktRxing == false)){
                /* Start control */
                pcktRxing = true;
                /* Retreive section length and check it */
                sctnLngth = ((rxPckt.data[(rxPckt.data[DATA_POINTER_POS] + DATA_SECT_LEN_H_POS)] << 8) |
                              rxPckt.data[(rxPckt.data[DATA_POINTER_POS] + DATA_SECT_LEN_L_POS)]) & SECTION_LENGTH_MASK;
                if(sctnLngth > SECTION_LENGTH_MAX){
                    printf("Section is too large!\n");
                    return SECTION_LENGTH_ERR;
                }
                /* Calculate full section length */
                fullSctnLngth = sctnLngth + DATA_SECT_LEN_L_POS;
                /* Calculate offsets */
                numToCpy = PAYLOAD_LENGTH_BYTE - rxPckt.data[DATA_POINTER_POS] - 1;
                ptrOffs = rxPckt.data[DATA_POINTER_POS] + DATA_PTR_PAYLD_POS;
                /* Copy continuity counter */
                contCnt = rxPckt.contntyCnt;
            }
            /* Receive the packet and copy data */
            if(pcktRxing == true){
                /* Continuity control */
                if(contCnt != rxPckt.contntyCnt){
                    pcktRxing = false;
                    printf("Section continuity check failed!\n");
                    return SECTION_CONTINUITY_ERR;
                }
                contCnt++;
                /* for() used instead of memcpy() for purpose */
                for(i = 0; i < numToCpy; i++){
                    if(fullSctnLngth != 0){
                        *pRxBuffCopy = rxPckt.data[i + ptrOffs];
                        pRxBuffCopy++;
                        fullSctnLngth--;
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
uint8_t parser_parseSection(void *pRawSctn, ePidVals_type pidValue){
    uint8_t parseStatus = SECTION_PARSE_FAIL;

    /* Pointer check */
    if(NULL == pRawSctn){
        printf("Pointer to source raw section error!\n");
        return POINTER_ERR;
    }
    /* Parsers calls */
    switch(pidValue){
        case pidNit:
            parseStatus = parseNit(pRawSctn);
            break;
        default:
            break;
    }

    return parseStatus;
}


/* Inner functions */

/* Parse NIT */
uint8_t parseNit(void *pRawSctn){
    uint8_t status = SECTION_PARSE_FAIL, *pRawSctnCopy, i;
    pRawSctnCopy = pRawSctn;

    /* Fill the section */
    nit.tableID = *pRawSctnCopy;
    pRawSctnCopy++;
    nit.sectnSyntxInd = *pRawSctnCopy & SECTION_SYNT_IND_MASK;
    nit.sectnLngth = (*pRawSctnCopy << 8) & SECTION_LENGTH_MASK;
    pRawSctnCopy++;
    nit.sectnLngth |= *pRawSctnCopy;
    pRawSctnCopy++;
    nit.netID = *pRawSctnCopy << 8;
    pRawSctnCopy++;
    nit.netID |= *pRawSctnCopy;
    pRawSctnCopy++;
    nit.versionNum = (*pRawSctnCopy & SECTION_VERS_NUM_MASK) >> 1;
    nit.currNextInd = *pRawSctnCopy & SECTION_CURR_TEX_IND_MASK;
    pRawSctnCopy++;
    nit.sectnNum = *pRawSctnCopy;
    pRawSctnCopy++;
    nit.lastSectnNum = *pRawSctnCopy;
    pRawSctnCopy++;
    nit.netDscrLngth = (*pRawSctnCopy << 8) & SECTION_DSCR_LEN_MASK;
    pRawSctnCopy++;
    nit.netDscrLngth |= *pRawSctnCopy;
    pRawSctnCopy++;
    /* Check network name descriptor length */
    if(nit.netDscrLngth != 0){
        parseNetNameDscr(pRawSctnCopy);
        pRawSctnCopy += nit.netDscrLngth;
    }
    nit.trnspStrLpLngth = (*pRawSctnCopy << 8) & SECTION_DSCR_LEN_MASK;
    pRawSctnCopy++;
    nit.trnspStrLpLngth |= *pRawSctnCopy;
    pRawSctnCopy++;
    /* Check transport descriptors length */
    if(nit.trnspStrLpLngth != 0){
        nit.dscrLoopPtrsNum = parseTrnspLoop(nit.trnspStrLpLngth, pRawSctnCopy);
        pRawSctnCopy += nit.trnspStrLpLngth;
    }
    nit.CRC32 = *pRawSctnCopy << 24;
    pRawSctnCopy++;
    nit.CRC32 |= *pRawSctnCopy << 16;
    pRawSctnCopy++;
    nit.CRC32 |= *pRawSctnCopy << 8;
    pRawSctnCopy++;
    nit.CRC32 |= *pRawSctnCopy;
    /* Return parsed section table ID */
    status = nit.tableID;

    return status;
}

/* Network name parse function */
void parseNetNameDscr(void *pRawSctn){
    uint8_t i, *pRawSctnCopy;
    pRawSctnCopy = pRawSctn;

    /* Get network name descriptor details */
    nit.netDscr.dscrTag = *pRawSctnCopy;
    pRawSctnCopy++;
    nit.netDscr.dscrLngth = *pRawSctnCopy;
    pRawSctnCopy++;
    for(i = 0; i < nit.netDscr.dscrLngth; i++){
        nit.netDscr.name[i] = *pRawSctnCopy;
        pRawSctnCopy++;
    }
}

/* Transport dtream loop parse function */
uint8_t parseTrnspLoop(uint16_t loopLngth, void *pRawSctn){
    uint8_t i, *pRawSctnCopy, trnspNum = 0, dscrTag, numToCopy = 0;
    uint16_t loopLngthCopy = loopLngth, dscrsLngth = 0;
    pRawSctnCopy = pRawSctn;

    do{
        /* Get TS header */
        nit.pTrnspStream[trnspNum]->trnspStrID = *pRawSctnCopy << 8;
        pRawSctnCopy++;
        nit.pTrnspStream[trnspNum]->trnspStrID |= *pRawSctnCopy;
        pRawSctnCopy++;
        nit.pTrnspStream[trnspNum]->origNetID = *pRawSctnCopy << 8;
        pRawSctnCopy++;
        nit.pTrnspStream[trnspNum]->origNetID |= *pRawSctnCopy;
        pRawSctnCopy++;
        nit.pTrnspStream[trnspNum]->trnspDscrLngth = (*pRawSctnCopy << 8) & SECTION_DSCR_LEN_MASK;
        pRawSctnCopy++;
        nit.pTrnspStream[trnspNum]->trnspDscrLngth |= *pRawSctnCopy;
        pRawSctnCopy++;
        dscrsLngth = nit.pTrnspStream[trnspNum]->trnspDscrLngth;
        do{
            /* Get descriptor tag */
            dscrTag = *pRawSctnCopy;
            switch(dscrTag){
//                case dscrServLst:
//                    break;
//                case dscrSatDlvrSys:
//                    break;
//                case dscrTerrDlvrSys:
//                    break;
//                case dscrFreqList:
//                    break;
                default:
                    /* Get descriptor length */
                    pRawSctnCopy++;
                    numToCopy = *pRawSctnCopy;
                    pRawSctnCopy++;
                    /* Preset pointer to a next descriptor */
                    pRawSctnCopy += numToCopy;
                    dscrsLngth -= (DESCRIPTOR_HEADER_SIZE_BYTE + numToCopy);
                    /* Set pointer to a dummy descriptor */
                    nit.pTrnspStream[trnspNum]->pTrnspDscr[nit.pTrnspStream[trnspNum]->dscrPtrsNum] = &dummyDscr;
                    nit.pTrnspStream[trnspNum]->dscrPtrsNum++;
                    dummyDscr.dscrTag = dscrDummy;
                    break;
            }
            dscrsLngth -= (DESCRIPTOR_HEADER_SIZE_BYTE + numToCopy);
        }while(dscrsLngth != 0);
        trnspNum++;
        loopLngthCopy -= (TS_HEADER_SIZE_BYTE + dscrsLngth);
    }while(loopLngthCopy != 0);

    return trnspNum;
}

