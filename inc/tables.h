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
        uint32_t        syncByte        :8,
                        trnspErrInd     :1,
                        pldUnitStrtInd  :1,
                        trnspPriority   :1,
                        packetID        :13,
                        trnspScrmblCntr :2,
                        adaptFldContr   :2,
                        contntyCnt      :4;
    };
    uint8_t             data[PAYLOAD_LENGTH_BYTE];
}elmntryPckt_type;

typedef struct{
    uint16_t            trnspStrID;
    uint16_t            origNetID;
    uint16_t            reserved0       :4,
                        trnspDscrLngth  :12;
    int                 *pTrnspDscr;
}trnspStream_type;

typedef struct{
    uint8_t             tableID;
    uint16_t            sectnSyntxInd   :1,
                        reserved0       :3,
                        sectnLngth      :12;
    uint16_t            netID;
    uint16_t            reserved1       :2,
                        versionNum      :5,
                        currNextInd     :1,
                        sectnNum        :8;
    uint8_t             lastSectnNum;
    uint16_t            reserved2       :4,
                        netDscrLngth    :12;
    int              	*pNetDscr;
    uint16_t            reserved3       :4,
                        trnspStrLpLngth :12;
    trnspStream_type    *pTrnspStream;
    uint32_t            CRC32;
}netInfoTable_type;

#endif /* TABLES_H */

