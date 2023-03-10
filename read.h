/*
reead.h file is based on the solution provided in assignment 1
*/



/* 
    Data structure declarations and prototypes for a 
    csv parsing module intended to convert from CSV files
    into C strings.
*/
#ifndef _READ_H_
#define _READ_H_

#include "record_struct.h"
#include <stdio.h>

/* Returns a list of CSV records. */
struct csvRecord **readCSV(FILE *csvFile, int *n);

/* Read a line of input from the given file. */
char *getQuery(FILE *f);

/* Free a set of records. */
void freeCSV(struct csvRecord **dataset, int n);

#endif
