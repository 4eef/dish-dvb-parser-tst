#ifndef PARSER_H
#define PARSER_H

/* Includes */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "file.h"
#include "tables.h"

/* Defines */
#define PACKETS_TO_READ                 5
#define BYTES_TO_READ                   (PACKETS_TO_READ * PACKET_LENGTH_BYTE)
#define PACKET_GET_SUCCESS              0
#define PACKET_READ_ERROR               1
#define PACKET_NO_SYNC_BYTE             2

/* Typedefs */

/* Function prototypes */
uint8_t parser_getPacket(elmntryPckt_type *pElmntryPckt);

#endif /* PARSER_H */
