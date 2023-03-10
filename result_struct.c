/** ***********************
* C Programming Style for Algorithms and Data Structures (COMP20003_2022_SM2)
* Created by Chang Xu (xucm@student.unimelb.edu.au) 9/09/2022
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>

#include "result_struct.h"


/*
createRangeSearchResult function takes no input but return 
*/
rangeResult_t* createRangeSearchResult(rangeResult_t* searchResult){
    
    searchResult->footpathsInRange = NULL;
    searchResult->nInRange = (int*)malloc(sizeof(int));
    *(searchResult->nInRange) = 0;
    searchResult->quadrantArr = NULL;
    searchResult->quadrantArrSize = (int*)malloc(sizeof(int));
    *(searchResult->quadrantArrSize) = 0;

    return searchResult;
}


