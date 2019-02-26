#ifndef TABLES_H
#define TABLES_H

/* Includes */
#include <stdint.h>

/* Defines */
#define PACKET_LENGTH_BYTE              188
#define HEADER_LENGTH_BYTE              4
#define PAYLOAD_LENGTH_BYTE             (PACKET_LENGTH_BYTE - HEADER_LENGTH_BYTE)
#define HEADER_SYNC_BYTE                0x47

/* Typedefs */
typedef struct{
    union{
        uint32_t	header;
        uint32_t        sync_byte       :8,
                        trnspErrInd     :1,
                        pldUnStrtInd    :1,
                        trnspPrior      :1,
                        packetID        :13,
                        trnspScrCntr    :2,
                        adFldContr      :2,
                        contCnt         :4;
    };
    uint8_t             data[PAYLOAD_LENGTH_BYTE];
}elmntryPckt_type;

typedef struct{
    uint16_t            trnspStrID;
    uint16_t            origNetID;
    uint16_t            reserved0       :4,
                        trnspDscrLngth  :12;
    size_t              trnspDscr;
}trnspStream_type;

typedef struct{
    uint8_t             tableID;
    uint16_t            sectSyntInd     :1,
                        reserved0       :3,
                        sectLngth       :12;
    uint16_t            netID;
    uint16_t            reserved1       :2,
                        versionNum      :5,
                        currNextInd     :1,
                        sectNum         :8;
    uint8_t             lastSectNum;
    uint16_t            reserved2       :4,
                        netDscrLngth    :12;
    size_t              *pNetDscr;
    uint16_t            reserved3       :4,
                        trnspStrLpLngth :12;
    trnspStream_type    pTrnspStream;
    uint32_t            CRC32;
}netInfoTable_type;

#endif /* TABLES_H */

