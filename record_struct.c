/*
record_struct.c file is based on the solution provided in assignment 1
*/



/* This file defines the internal structure of the shared 
    transfer data structure csvRecord used to hold a set of 
    read fields. This can be included in files which need 
    to use its internal structure. This approach preserves
    data encapsulation in a C-centric way.
*/
#include "record_struct.h"

struct csvRecord {
    int fieldCount;
    char **fields;
};
