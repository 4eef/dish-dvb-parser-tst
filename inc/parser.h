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
#define SECTION_LENGTH_ERR              5
#define SECTION_CONTINUITY_ERR          6
#define SECTION_PARSED                  7
#define SECTION_PARSE_FAIL              8

/* Function prototypes */
uint8_t parser_getAnyPacket(elmntryPckt_type *pElmntryPckt);
uint8_t parser_getSection(void *pRawSctn, ePidVals_type pidValue);
uint8_t parser_parseSection(void *pTableSctn, void *pRawSctn, ePidVals_type pidValue);

#endif /* PARSER_H */
