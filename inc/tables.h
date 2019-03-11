#ifndef TABLES_H
#define TABLES_H

/* Includes */
#include <stdint.h>

/* Defines */
#define PACKET_LENGTH_BYTE              188
#define HEADER_LENGTH_BYTE              4
#define PAYLOAD_LENGTH_BYTE             (PACKET_LENGTH_BYTE - HEADER_LENGTH_BYTE)
#define HEADER_SYNC_BYTE                0x47
#define HEADER_SYBC_BYTE_POS            0
#define HEADER_INDS_PIDH_POS            1
#define HEADER_PIDL_POS                 2
#define HEADER_SERV_FLDS_POS            3
#define HEADER_TEI_MSK                  0x80
#define HEADER_PUSI_MSK                 0x40
#define HEADER_TP_MSK                   0x20
#define HEADER_PIDH_MSK                 0x1F
#define HEADER_TSC_MSK                  0xC0
#define HEADER_AFC_MSK                  0x30
#define HEADER_CONT_CNT_MSK             0xF
#define SECTION_SYNT_IND_MASK           0x80
#define SECTION_LENGTH_MASK             0xFFF
#define SECTION_LENGTH_MAX              1021
#define SECTION_LENGTH_FULL_MAX         (SECTION_LENGTH_MAX + 3)
#define DATA_POINTER_POS                0
#define DATA_DATA_ONLY_POS              0
#define DATA_PTR_PAYLD_POS              1
#define DATA_TAB_ID_POS                 1
#define DATA_SECT_LEN_H_POS             2
#define DATA_SECT_LEN_L_POS             3
#define SECTION_VERS_NUM_MASK           0x3E
#define SECTION_CURR_TEX_IND_MASK       0x1
#define SECTION_DSCR_LEN_MASK           0xFFF
#define DESCRIPTOR_POSSIBLE_LEN         256
#define NIT_TABLES_NUM                  2
#define DESCRIPTORS_POSSIBLE_NUM        256
#define TS_HEADER_SIZE_BYTE             6
#define DESCRIPTOR_HEADER_SIZE_BYTE     2
#define FREQUENCY_SYMBOLS_NUM           8
#define ORBPOS_SYMBOLS_NUM              4
#define SYMRATE_SYMBOLS_NUM             7
#define BCD_VAL_MSK                     0xF
#define EAST_WEST_MSK                   0x80
#define POLARIZATION_MSK                0x60
#define ROLL_OFF_MSK                    0x18
#define MOD_SYS_MSK                     0x4
#define MOD_TYPE_MSK                    0x3
#define FEC_INNER_MASK                  0xF
#define TABLE_SECTIONS_MAX_NUM          256
#define CONT_CNT_MAX_NUM                16

/* Typedefs */

/* Enums */
typedef enum{
    pidNit              = 0x10
}ePidVals_type;

typedef enum{
    tableNitActual      = 0x40,
    tableNitOther       = 0x41
}eTableIds_type;

typedef enum{
    dscrNetName         = 0x40,
    dscrServLst         = 0x41,
    dscrStuffng         = 0x42,
    dscrSatDlvrSys      = 0x43,
    dscrCablDlvrSys     = 0x44,
    dscrLnkg            = 0x4A,
    dscrTerrDlvrSys     = 0x5A,
    dscrMltLngNetName   = 0x5B,
    dscrPrvDatSpec      = 0x5F,
    dscrFreqList        = 0x62,
    dscrCellLst         = 0x6C,
    dscrCellFreqLink    = 0x6D,
    dscrDefAuth         = 0x73,
    dscrTimSlcId        = 0x77,
    dscrS2DlvrSys       = 0x79,
    dscrXaitLoc         = 0x7D,
    dscrFtaContMng      = 0x7E,
    dscrExt             = 0x7F,
    dscrDummy           = 0xFF
}eDscrTags_type;

/* Elementary packet format */
typedef struct{
    uint8_t             syncByte;
    uint8_t             trnspErrInd;
    uint8_t             pldUnitStrtInd;
    uint8_t             trnspPriority;
    uint16_t            packetID;
    uint8_t             trnspScrmblCntr;
    uint8_t             adaptFldContr;
    uint8_t             contntyCnt;
    uint8_t             data[PAYLOAD_LENGTH_BYTE];
}elmntryPckt_type;

/* DESCRIPTORS */
typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
}dummyDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    char                name[DESCRIPTOR_POSSIBLE_LEN];
}netNameDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint8_t             srvcNum;
    uint16_t            srvcID[DESCRIPTOR_POSSIBLE_LEN];
    uint8_t             srvcType[DESCRIPTOR_POSSIBLE_LEN];
}srvcLstDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint8_t             frequency[FREQUENCY_SYMBOLS_NUM];
    uint8_t             orbPosition[ORBPOS_SYMBOLS_NUM];
    uint8_t             eastWest;
    uint8_t             polarization;
    uint8_t             rollOff_00;
    uint8_t             modSys;
    uint8_t             modType;
    uint8_t             symRate[SYMRATE_SYMBOLS_NUM];
    uint8_t             fecInner;
}satDlvrSysDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint8_t             centreFreq;
    uint8_t             bandwidth;
    uint8_t             priority;
    uint8_t             timSlicInd;
    uint8_t             mpeFecInd;
    uint8_t             constellation;
    uint8_t             hierarchyInfo;
    uint8_t             codeRateHpStr;
    uint8_t             codeRateLpStr;
    uint8_t             guardInterval;
    uint8_t             txMode;
    uint8_t             otherFreqFlag;
    uint32_t            reserved0;
}terrDlvrSysDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint8_t             codingType;
    uint8_t             freqsNum;
    uint32_t            centreFreq[FREQUENCY_SYMBOLS_NUM][DESCRIPTOR_POSSIBLE_LEN];
}freqListDscr_type;
/*
typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint8_t             *pStffngByte;
}stffngDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint32_t            frequency;
    uint16_t            reserved0       :12,
                        fecOuter        :4;
    uint8_t             modulation;
    uint32_t            symRate         :28,
                        fecInner        :4;
}cblDlvrSysDscr_type;

typedef struct{
    uint8_t             hndOvrType      :4,
                        reserved0       :3,
                        orgnType        :1;
    uint16_t            netID;
    uint16_t            initSrvcID;
}mobHndOvrInfo_type;

typedef struct{
    uint16_t            trgtEventID;
    uint8_t             trgtListed      :1,
                        eventSimulcast  :1,
                        reserved0       :6;
}eventLnkgInfo_type;

typedef struct{
    uint16_t            trgtEventID;
    uint8_t             trgtListed      :1,
                        eventSimulcast  :1,
                        linkType        :2,
                        trgtIdType      :2,
                        origNetIdFlag   :1,
                        srvcIdFlag      :1;
    uint16_t            userDefID;
    uint16_t            trgtTrnspStrID;
    uint16_t            trgtOrigNetID;
    uint16_t            trgtSrvcID;
}evLnkgInfo_type;

typedef struct{
    uint8_t             loopLength;
    evLnkgInfo_type     *pEvLnkgInfo;
}extEvLnkgInfo_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint16_t            trnspStrID;
    uint16_t            origNetID;
    uint16_t            srvcID;
    uint8_t             lnkgeType;
    void                *pInfo;
    uint8_t             *pPrivateData;
}linkageDscr_type;

typedef struct{
    uint32_t            ISO639LngCode   :24,
                        netNameLngth    :8;
    char                *pNetName;
}netName_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    netName_type        *pNetName;
}mLngNetNamDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint32_t            prvDataSpecifier;
}prvDatSpecDscr_type;

typedef struct{
    uint8_t             cellIdExt;
    uint16_t            sCellLat;
    uint16_t            sCellLong;
    uint8_t             sCellExtOfLatH;
    uint8_t             sCellExtOfLatL  :4,
                        sCellExtOfLongH :4;
    uint8_t             sCellExtOfLongL;
}sCellInfo_type;

typedef struct{
    uint16_t            cellID;
    uint16_t            cellLat;
    uint32_t            cellLong        :12,
                        cellExtOfLat    :12,
                        subcellInfLngth :8;
    sCellInfo_type      *pSubcellInfo;
}cellInfo_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    cellInfo_type       *pCellInfo;
}cellLstDscr_type;

typedef struct{
    uint8_t             cellIfExt;
    uint32_t            txFreq;
}sCellFreqInfo_type;

typedef struct{
    uint16_t            cellID;
    uint32_t            frequency;
    uint8_t             cubcellInfoLngth;
    sCellFreqInfo_type  *pSubcellFreqInfo;
}cellFreqInfo_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    cellFreqInfo_type   *pCellFreqInfo;
}cellFrqLnkDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint8_t             *pDefAuthByte;
}defAuthDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint16_t            timSlicng       :1,
                        mpeFec          :2,
                        reserved0       :2,
                        frameSize       :3,
                        maxBurstDura    :8,
                        maxAvgRate      :4,
                        timSlcFecID     :4;
    uint8_t             *pIdSelByte;
}timSlcFecDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint8_t             scrmSeqSel      :1,
                        mltInpStrFlag   :1,
                        backwCompInd    :1,
                        reserved0       :5;
    uint32_t            reserved1       :6,
                        scrmSeqInd      :18,
                        inpStrID        :8;
}S2DlvrSysDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint16_t            xaitOrigNetID;
    uint16_t            xaitSrvcID;
    uint8_t             xaitVerNum      :5,
                        xaitUpdPolicy   :3;
}xaitLocDscr_type;

typedef struct{
    uint8_t             dscrTag;
    uint8_t             dscrLngth;
    uint8_t             userDef         :1,
                        reserved0       :3,
                        doNotScramble   :1,
                        ctrrRemAcOvrInt :2,
                        doNotApplyRevoc :1;
}ftaContMngDscr_type;
*/
/* NIT Packet organization */
typedef struct{
    uint16_t            trnspStrID;
    uint16_t            origNetID;
    uint16_t            trnspDscrLngth;
    uint8_t             dscrPtrsNum;
    void                *pTrnspDscr[DESCRIPTORS_POSSIBLE_NUM];
}trnspStream_type;

typedef struct{
    uint8_t             tableID;
    uint8_t             sectnSyntxInd;
    uint16_t            sectnLngth;
    uint16_t            netID;
    uint8_t             versionNum;
    uint8_t             currNextInd;
    uint8_t             sectnNum;
    uint8_t             lastSectnNum;
    uint16_t            netDscrLngth;
    netNameDscr_type    netDscr;
    uint16_t            trnspStrLpLngth;
    uint8_t             tsNum;
    trnspStream_type    trnspStream[DESCRIPTORS_POSSIBLE_NUM];
    uint32_t            CRC32;
}netInfoTable_type;

#endif /* TABLES_H */

