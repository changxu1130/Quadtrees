/** ***********************
* C Programming Style for Algorithms and Data Structures (COMP20003_2022_SM2)
* Created by Chang Xu (xucm@student.unimelb.edu.au) 9/09/2022
*/

#ifndef _RESULT_STRUCT_H_
#define _RESULT_STRUCT_H_

#include "dictionary.h"

//data structure definition
struct rangeResult {
    struct data** footpathsInRange;
    int* nInRange;
    int* quadrantArr;
    int* quadrantArrSize;
};
typedef struct rangeResult rangeResult_t;


//function prototypes

/*
createRangeSearchResult function takes no input but return 
*/
rangeResult_t* createRangeSearchResult(rangeResult_t* searchResult);

#endif
