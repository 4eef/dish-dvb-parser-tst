#ifndef MAIN_H
#define MAIN_H

/* Includes */
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "file.h"
#include "parser.h"
#include "tables.h"

/* Defines */
#define OK                              0

/* Exported variables */
extern netInfoTable_type nit;
extern srvcLstDscr_type srvcLstDscr;
extern freqListDscr_type freqListDscr;
extern satDlvrSysDscr_type satDlvrSysDscr;
extern terrDlvrSysDscr_type terrDlvrSysDscr;

/* Function prototypes */
int main(int argc, char *argv[]);

#endif /* MAIN_H */

