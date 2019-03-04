#ifndef PARSER_H
#define PARSER_H

/* Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file.h"
#include "tables.h"

/* Defines */
#define PACKETS_TO_READ                 5
#define BYTES_TO_READ                   (PACKETS_TO_READ * PACKET_LENGTH_BYTE)
#define PACKET_GET_SUCCESS              0
#define PACKET_READ_ERROR               1
#define PACKET_NO_SYNC_BYTE             2
#define POINTER_ERR                     3
#define SECTION_GET_READY               4
#define ALLOC_ERR                       5

/* Function prototypes */
uint8_t parser_getAnyPacket(elmntryPckt_type *pElmntryPckt);
uint8_t parser_getSection(void *pRxBuff, ePidVals_type pidValue);

#endif /* PARSER_H */
