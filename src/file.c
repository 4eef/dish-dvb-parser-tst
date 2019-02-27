#include "file.h"

/* Memory */
static int fileDscr;
static uint32_t readCnt;

/* Open a file to read... */
uint8_t file_openFile(char *pFilePath){
    /* Perform file open */
    fileDscr = open(pFilePath, O_RDONLY);
    /* Check the success */
    if(fileDscr == -1){
        printf("Error with opening file...\n");
        return NO_SUCH_FILE;
    }else{
        printf("File is opened, reading...\n");
    }
    return NO_ERROR;
}

/* Close a file... */
uint8_t file_closeFile(void){
    int status;
    /* Check file descriptor */
    if(fileDscr <= 0){
        printf("There is nothing to close!\n");
        return NO_FILE_TO_CLOSE;
    }
    /* Close file */
    status = close(fileDscr);
    if(status != 0){
        printf("Error closing file!\n");
        return ERROR_CLOSING_FILE;
    }
    printf("File is closed.\n");
    return NO_ERROR;
}

/* Read portion of data from file and write into buffer */
uint8_t file_readToBuff(void *pBuff, uint16_t cnt){
    /* Check for success */
    if(fileDscr <= 0){
        printf("Nothing to read. Open a file first!\n");
        return NO_SUCH_FILE;
    }else if(NULL == pBuff){
        printf("Error with file read buffer pointer...\n");
        return BUFFER_PTR_ERROR;
    }
    /* Perform file read */
    readCnt += read(fileDscr, pBuff, cnt);
    //readCnt += cnt;
    printf("Read bytes: %i\n", readCnt);
    return NO_ERROR;
}

/* File read test */
void file_testRead(void){
    uint8_t num;
    char txtStr[50];

    num = read(fileDscr, &txtStr[0], 1);

    printf("first char is %c, num data is %i\n", txtStr[0], num);
}

