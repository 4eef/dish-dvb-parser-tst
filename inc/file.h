#ifndef FILE_H
#define FILE_H

/* Includes */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Defines */
#define NO_ERROR                        0
#define NO_SUCH_FILE                    1
#define NO_FILE_TO_CLOSE                2
#define ERROR_CLOSING_FILE              3
#define BUFFER_PTR_ERROR                4

/* Function prototypes */
uint8_t file_openFile(char *pFilePath);
uint8_t file_closeFile(void);
uint8_t file_readToBuff(void *pBuff, uint16_t cnt);
void file_testRead(void);

#endif /* FILE_H */

