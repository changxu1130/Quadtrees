/** ***********************
* C Programming Style for Algorithms and Data Structures (COMP20003_2022_SM2)
* Created by Chang Xu (xucm@student.unimelb.edu.au) 9/09/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>

#include "quadtree.h"
#include "dictionary.h"
#include "result_struct.h"

#define SW 0
#define NW 1
#define NE 2
#define SE 3


/*
this function takes a pointer to the node (root node in the first case or 
the head node ow)
and a pointer to the new point that need to be inserted into the quadtree. 
this function will return 1 if insert successfully, 0 if not. 
*/
rangeResult_t* pointRangeSearch(quadtreeNode_t* node, 
            rangeResult_t* searchResults, 
            rectangle2D_t* searchRange, char* query){
    assert(searchRange);

    //case 1: transition state, 
    //check which child/rectangle the point needs to go
    //keep looping 
    if(((node->footpaths) == NULL) && (node->available == 0)){
        //check all 4 child
        if(rectangleOverlap(node->childSW->currRect, searchRange) 
                                        && node->childSW->available == 0){
            //add direction to quadrantArr
            searchResults->quadrantArr = addQuadrantToArray(
                                searchResults->quadrantArr, 
                                        searchResults->quadrantArrSize, SW);
            searchResults = pointRangeSearch(node->childSW, 
                                searchResults, searchRange, query);
        }
        if(rectangleOverlap(node->childNW->currRect, searchRange) 
                                            && node->childNW->available == 0){
            //add direction to quadrantArr
            searchResults->quadrantArr = addQuadrantToArray(
                            searchResults->quadrantArr, 
                            searchResults->quadrantArrSize, NW);
            searchResults = pointRangeSearch(node->childNW, 
                                            searchResults, searchRange, query);
        }
        if(rectangleOverlap(node->childNE->currRect, searchRange) 
                                            && node->childNE->available == 0){
            //add direction to quadrantArr
            searchResults->quadrantArr = 
                        addQuadrantToArray(searchResults->quadrantArr, 
                                            searchResults->quadrantArrSize, NE);
            searchResults = pointRangeSearch(node->childNE, 
                                            searchResults, searchRange, query);
        }
        if(rectangleOverlap(node->childSE->currRect, searchRange) 
                                            && node->childSE->available == 0){
            //add direction to quadrantArr
            searchResults->quadrantArr =
                        addQuadrantToArray(searchResults->quadrantArr, 
                                            searchResults->quadrantArrSize, SE);
            searchResults = pointRangeSearch(node->childSE, 
                                            searchResults, searchRange, query);
        }
    }

    //case 2: encountered a node with data/not available leaf node 
    else if((node->footpaths) && (node->available == 0)){
        assert(node->footpaths);

        for(int i = 0; i < *(node->nFootpath); i++){
            searchResults->footpathsInRange = 
                            addFootpathToArray(searchResults->footpathsInRange, 
                                   searchResults->nInRange, node->footpaths[i]);
        }       
    }

    return searchResults;
}

/*
this function add footpath found in range into the array of all 
the footpaths found 
*/
struct data** addFoundFootpathToArray(struct data** footpathsInRange, 
                                int* nInRange, struct data* footpath){
    *nInRange = (*nInRange) + 1;

    //size == 0 initialise the int array 
    if(*nInRange == 1){
        footpathsInRange = (struct data**)malloc(sizeof(struct data*));        
    }
    else {
        footpathsInRange = 
                realloc(footpathsInRange, sizeof(struct data*) * (*nInRange));
    }
    footpathsInRange[(*nInRange) - 1] = footpath;
    return footpathsInRange; 
}

/*
this function takes a pointer to the node (root node in the first case 
or the head node ow)
and a pointer to the new point that need to be inserted into the quadtree. 
this function will return 1 if insert successfully, 0 if not. 
*/
quadtreeNode_t* pointRegionSearch(quadtreeNode_t* node, int* quadrantArr, 
                    int* quadrantArrSize, point2D_t* searchPoint, char* query){
    assert(searchPoint);

    quadtreeNode_t* tempNode = NULL;

    //case 1: reach an empty node, not found 
    if(((node->footpaths) == NULL) && (node->available == 1)){
        return NULL;
    }

    //case 2: transition state, check which 
    //child/rectangle the point needs to go
    //keep looping 
    else if(((node->footpaths) == NULL) && (node->available == 0)){
        //check all 4 child 
        if(inRectangle(node->childSW->currRect, searchPoint, SW)){
            //add direction to quadrantArr
            quadrantArr = addQuadrantToArray(quadrantArr, quadrantArrSize, SW);
            tempNode = pointRegionSearch(node->childSW, 
                            quadrantArr, quadrantArrSize, searchPoint, query);
        }
        else if(inRectangle(node->childNW->currRect, searchPoint, NW)){
            //add direction to quadrantArr
            quadrantArr = addQuadrantToArray(quadrantArr, quadrantArrSize, NW);
            tempNode = pointRegionSearch(node->childNW,
                            quadrantArr, quadrantArrSize, searchPoint, query);
        }
        else if(inRectangle(node->childNE->currRect, searchPoint, NE)){
            //add direction to quadrantArr
            quadrantArr = addQuadrantToArray(quadrantArr, quadrantArrSize, NE);
            tempNode = pointRegionSearch(node->childNE, 
                            quadrantArr, quadrantArrSize, searchPoint, query);
        }
        else if(inRectangle(node->childSE->currRect, searchPoint, SE)){
            //add direction to quadrantArr
            quadrantArr = addQuadrantToArray(quadrantArr, quadrantArrSize, SE);
            tempNode = pointRegionSearch(node->childSE, 
                            quadrantArr, quadrantArrSize, searchPoint, query);
        }
    }

    //case 3: encountered a node with data/not available leaf node 
    else if((node->footpaths) && (node->available == 0)){
        assert(node->footpaths);

        //if the two points are the same => found the right point region 
        if(compareLongDouble(node->point->x, searchPoint->x)
                         && compareLongDouble(node->point->y, searchPoint->y)){

            //print all the quadrants vistied to stdout 
            printQuadrantsToStdout(quadrantArr, quadrantArrSize, query);
            free(quadrantArr);
            free(quadrantArrSize);
        }
        tempNode = node;    
    }

    return tempNode;
}

/*
this function takes int* arr, int* size,
 point2D_t* searchPoint, as input and print them in 
the formatting required to the stdout 
*/
void printQuadrantsToStdout(int* arr, int* size, char* query){
    //0=SW, 1=NW, 2=NE, 3=SE
    char *quadrantIntToChar[] = {"SW", "NW", "NE", "SE"};

    //print the initial header
    printf("%s --> ", query);
    for(int i = 0; i < (*size); i++){
        printf("%s ", quadrantIntToChar[(arr[i])]);
    }
    printf("\n");
}

/* 
printResult print the given query result.
*/
void printResult(char* query, struct data** footpathArr, 
                        int footpathArrSize, FILE *outputFile, int stage){
    /* Print summary. */
    int nField = 19;

    // Field names by index.
    static char *fieldNames[] = {"footpath_id", "address", "clue_sa", 
        "asset_type", "deltaz", "distance", "grade1in", "mcc_id", "mccid_int", 
        "rlmax", "rlmin", "segside", "statusid", "streetid", "street_group", 
        "start_lat", "start_lon", "end_lat", "end_lon"};

    //print the initial header line
    fprintf(outputFile, "%s\n", query);

    for(int i = 0; i < footpathArrSize; i++){
        fprintf(outputFile, "--> ");
        for(int j = 0; j < nField; j++){
            fprintf(outputFile, "%s: ", fieldNames[j]);
            printField(outputFile, footpathArr[i], j);
            fprintf(outputFile, " || ");
        }
        fprintf(outputFile, "\n");
    }

}

/*
removeDuplicate responsible to remove duplicated pointers that point 
to the same footpath if exsisted. It returns a ppointed to the updated
array free of duplication. 
*/
struct data** removeDuplicate(struct data** arr, int* n){
    struct data** tempArr = (struct data**)malloc(sizeof(struct data*) * (*n));
    
    int actualSize = 1;
    tempArr[0] = arr[0];

    //loop through the original array to 
    //final the size of unique footpath pointer
    for(int i = 1; i < (*n); i++){
        //if no duplicate, 
        if((arr[i-1]->footpath_id) != (arr[i])->footpath_id){
            tempArr[actualSize] = arr[i];
            actualSize++;
        } 
    }
    //free the old array 
    free(arr);

    struct data** finalArr =
             (struct data**)malloc(sizeof(struct data*) * actualSize);

    for(int k = 0; k < actualSize; k++){
        finalArr[k] = tempArr[k];
    }

    free(tempArr);

    //update the body variable that keep track of the array size 
    (*n) = actualSize;

    return finalArr;
}

/*
partition is a helper function that responsible to partoition the 
array for quicksort
*/
int partition(struct data** arr, int low, int high){
    // pivot
    int pivot = arr[high]->footpath_id;
  
    //Index of smaller element and indicates the right  
    //position of pivot found so far
    int i = (low - 1);
  
    for (int j = low; j <= high - 1; j++)
    {
  
      //current element is smaller than the pivot
      if (arr[j]->footpath_id < pivot)
      {
  
        //incre,ment the index of the smaller element
        i++;
        swap(arr, i, j);
      }
    }
    swap(arr, i + 1, high);
    return (i + 1);
  }

/*
quickSort call partition function to sort the array of 
pointers to footpath based on its id
*/
void quickSort(struct data** arr, int low, int high){
    if (low < high)
    {
        int pivot = partition(arr, low, high);
        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

/*
swap is a helper function for the quickSort function, which swap two elements
*/
void swap(struct data** arr, int i, int j){
    struct data* temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }


/*
this function add quadtant being searched (represent as int) into the array of 
all the quadrant(s) being search
0=SW, 1=NW, 2=NE, 3=SE
*/
int* addQuadrantToArray(int* arr, int* size, int quadrant){
    *size = (*size) + 1;

    //size == 0 initialise the int array 
    if(*size == 1){
        arr = (int*)malloc(sizeof(int));        
    }
    else {
        arr = realloc(arr, sizeof(int) * (*size));
    }
    arr[(*size) - 1] = quadrant;
    return arr; 
}


/*
this function takes a pointer to the footpath data struct and a root node, calls
the recusive insert point function to insert each
 footpath's (start_lon, start_lat) 
and (end_lon, end_lat) pairs into the quadtree
*/
quadtreeNode_t* insertFootpath(quadtreeNode_t* node, struct data* footpath){
    //since both the start and the end point need to
    // be inseted, create point2D_t 
    //for each and insert separately 
    assert(node);
    assert(footpath);

    point2D_t* start = createPoint(footpath->start_lon, footpath->start_lat);
    insertPoint(node, start, footpath);

    //initialise point
    point2D_t* end = createPoint(footpath->end_lon, footpath->end_lat);
    insertPoint(node, end, footpath);
    return node;
}

/*
this function add footpath into the array of footpaths stored in node
*/
struct data ** addFootpathToArray(struct data ** arrFootpaths, 
                                int* size, struct data* footpath){
    int tempSize = *size;
    *size = tempSize + 1;

    //empty array, malloc the space for it
    if(arrFootpaths == NULL){
        arrFootpaths = (struct data **)malloc(sizeof(struct data*)); 
    }

    //else realloc 
    else{
        arrFootpaths = realloc(arrFootpaths, (*size) * sizeof(struct data*));  
    }

    arrFootpaths[(*size) - 1] = footpath;  

    return arrFootpaths;
}

/*
this function compare two long double and check whether they are the same 
*/
int compareLongDouble(long double x, long double y){
    long double epsilon = 10e-13;
    return (absoluteValue(x - y) <= epsilon);
}


long double absoluteValue(long double ld){
    return (ld > 0) ? ld : -ld;
}


/*
this function takes a pointer to the node (root node 
in the first case or the head node ow)
and a pointer to the new point that need to be inserted into the quadtree. 
this function will return 1 if insert successfully, 0 if not. 
*/
int insertPoint(quadtreeNode_t* node, point2D_t* point, struct data* footpath){
    assert(footpath);
    //case 1: if the node is empty (data is empty) 
    //and the node is available, insert (base case)
    if(((node->footpaths) == NULL) && (node->available == 1)){
        
        //create leaf
        node = createChildren(node, 
                    (node->currRect->bottomL), (node->currRect->upperR));
        node->footpaths = 
                addFootpathToArray(node->footpaths, node->nFootpath, footpath);
        node->point = point;

        //set available flag to false
        node->available = 0;
        
        return 1;
    }

    //case 2: transition state, 
    //check which child/rectangle the point needs to go
    if(((node->footpaths) == NULL) && (node->available == 0)){  
        //check all 4 child 0=SW, 1=NW, 2=NE, 3=SE
        if(inRectangle(node->childSW->currRect, point, SW)){
            insertPoint(node->childSW, point, footpath);
            return 1;
        }
        if(inRectangle(node->childNW->currRect, point, NW)){
            insertPoint(node->childNW, point, footpath);
            return 1;
        }
        if(inRectangle(node->childNE->currRect, point, NE)){
            insertPoint(node->childNE, point, footpath);
            return 1;
        }
        if(inRectangle(node->childSE->currRect, point, SE)){
            insertPoint(node->childSE, point, footpath);
            return 1;
        }
    }

    //case 3: encountered a node with data/not available
    if(node->footpaths){
        assert(node->footpaths);

        //check if the current data is the same as that stored in the quadtree
        if(compareLongDouble(node->point->x, point->x) 
                            && compareLongDouble(node->point->y, point->y)){
            node->footpaths = 
                addFootpathToArray(node->footpaths, node->nFootpath, footpath);
            return 1;
        }

        //set the footpath and point stored in current node to NULL
        struct data** tempFootpaths = node->footpaths;

        point2D_t* tempPoint = node->point;
        node->footpaths = NULL;
        node->point = NULL;
        node->available = 0;

        //insertPoint(quadtreeNode_t* node,
        // point2D_t* point, struct data *footpath)
        for(int k = 0; k < *(node->nFootpath); k++){
            //printf("call you second\n");
            insertPoint(node, tempPoint, tempFootpaths[k]);
        }

        //deal with the new point that was supposed to be inserted
        insertPoint(node, point, footpath);
        free(tempFootpaths);
        return 1;
    }

    //insert failure
    return 0;
}




/*
this function create children node for a pre-created node
*/
quadtreeNode_t* createChildren(quadtreeNode_t* node, 
                point2D_t* bottomL, point2D_t* upperR){

    long double a, b, c, d;

    a = bottomL->x;
    b = bottomL->y;
    c = upperR->x;
    d = upperR->y;

    //0=SW, 1=NW, 2=NE, 3=SE
    
    //initialise the 4 children
    node->childSW = (quadtreeNode_t *)malloc(sizeof(quadtreeNode_t));
    node->childSW->footpaths = NULL;
    node->childSW->point = NULL;
    node->childSW->available = 1; 
    node->childSW->nFootpath = (int *)malloc(sizeof(int));
    *(node->childSW->nFootpath) = 0;

    //subdivide the current rectangle 
    point2D_t* bLPointSW = createPoint(bottomL->x, bottomL->y);
    point2D_t* uRPointSW = createPoint(((a + c) / 2.0), ((b + d) / 2.0));
    node->childSW->currRect = createRect(bLPointSW, uRPointSW);

    node->childNW = (quadtreeNode_t *)malloc(sizeof(quadtreeNode_t));
    node->childNW->footpaths = NULL;
    node->childNW->point = NULL;
    node->childNW->available = 1; 
    node->childNW->nFootpath = (int *)malloc(sizeof(int));
    *(node->childNW->nFootpath) = 0;

    //subdivide the current rectangle 
    point2D_t* bLPointNW = createPoint(a, (b + d) / 2.0);
    point2D_t* uRPointNW = createPoint((a + c) / 2.0, d);
    node->childNW->currRect = createRect(bLPointNW, uRPointNW); 
    
    node->childNE = (quadtreeNode_t *)malloc(sizeof(quadtreeNode_t));
    node->childNE->footpaths = NULL;
    node->childNE->point = NULL;
    node->childNE->available = 1; 
    node->childNE->nFootpath = (int *)malloc(sizeof(int));
    *(node->childNE->nFootpath) = 0;

    //subdivide the current rectangle
    point2D_t* bLPointNE = createPoint(((a + c) / 2.0), ((b + d) / 2.0));
    point2D_t* uRPointNE = createPoint(upperR->x, upperR->y); 
    node->childNE->currRect = createRect(bLPointNE, uRPointNE); 

    node->childSE = (quadtreeNode_t *)malloc(sizeof(quadtreeNode_t));
    //printf("createChildren\n");
    node->childSE->footpaths = NULL;
    node->childSE->point = NULL;
    node->childSE->available = 1; 
    node->childSE->nFootpath = (int *)malloc(sizeof(int));
    *(node->childSE->nFootpath) = 0;

    //subdivide the current rectangle
    point2D_t* bLPointSE = createPoint((a + c) / 2.0, b);
    point2D_t* uRPointSE = createPoint(c, (b + d) / 2.0);
    node->childSE->currRect = createRect(bLPointSE, uRPointSE);

    return node;
}




/*
this function check whether the input point is in the rectangle object, return
1 if true, 0 if false
0=SW, 1=NW, 2=NE, 3=SE
*/
int inRectangle(rectangle2D_t* rect, point2D_t* point, int quadrant){

    long double a, b, c, d;

    a = rect->bottomL->x;
    b = rect->bottomL->y;
    c = rect->upperR->x;
    d = rect->upperR->y;

    if(quadrant == SW){
        if(((point->x >= a) && (point->x < c)) && ((point->y >= b) && (point->y < d))){
            return 1;
        }
    }

    if(quadrant == NW){
        if(((point->x >= a) && (point->x <= c)) && ((point->y >= b) && (point->y <= d))){
            return 1;
        }
    }

    if(quadrant == NE){
        if(((point->x > a) && (point->x <= c)) && ((point->y >= b) && (point->y <= d))){
            return 1;
        }
    }

    if(quadrant == SE){        
        if(((point->x > a) && (point->x <= c)) && ((point->y >= b) && (point->y < d))){
            return 1;
        }
    }

    return 0;
}


/*
this function takes 2 pointers to rectangele, return 1 if the 2 overlap, 0 otherwise
*/
int rectangleOverlap(rectangle2D_t* rec1, rectangle2D_t* rec2){
    long double epsilon = 10e-12; 
    long double a1 = rec1->bottomL->x;
    long double a2 = rec1->upperR->x;
    long double b1 = rec1->bottomL->y;
    long double b2 = rec1->upperR->y;

    long double x1 = rec2->bottomL->x;
    long double x2 = rec2->upperR->x;
    long double y1 = rec2->bottomL->y;
    long double y2 = rec2->upperR->y;
    

    long double xaDiff1 = a1 - x2;
    long double xaDiff2 = x1 - a2;
    long double ybDiff1 = y1 - b2;
    long double ybDiff2 = b1 - y2;

    if((xaDiff1 > epsilon) || (xaDiff2 > epsilon)){
        return 0;
    }

    if((ybDiff1 > epsilon) || (ybDiff2 > epsilon)){
        return 0;
    }

    return 1;
}

/* helper functions for creating, freeing and 
other checking regarding data structure */

/*
this function malloc enough space for a point2D and return a pointer to that 
*/
point2D_t* createPoint(long double x, long double y){
    point2D_t* point = (point2D_t*)malloc(sizeof(point2D_t));
    point->x = x;
    point->y = y;
    return point;
}

/*
this funtion malloc enough space for a rectangle and return a pointer to that
*/
rectangle2D_t* createRect(point2D_t* bottomL, point2D_t* upperR){
    rectangle2D_t *rect = (rectangle2D_t *)malloc(sizeof(rectangle2D_t)); 
    rect->bottomL = bottomL;
    rect->upperR = upperR;
    return rect;
}

/*
this function free point given its pointer
*/
void freePoint(point2D_t* point){
     free(point);
}

/*
this function free rectangle given its pointer 
*/
void freeCurreRect(rectangle2D_t* currRect){
    freePoint(currRect->bottomL);
    freePoint(currRect->upperR);
    free(currRect);
}

/*
this function takes a pointer to a quadtreeNode 
and free all the memory allocated to it
*/
int freeQuadtreeNode(quadtreeNode_t* node){
    assert(node);
    //case 1: a child node with no data
    if((node->footpaths == NULL) && ((node->available) == 1)){ 
        freeCurreRect(node->currRect);
        free(node->nFootpath);
        free(node);
        return 1; 
    }

    //case 2: leaf node with data stored 
    if((node->footpaths != NULL) && ((node->available) == 0)){
        //free all 4 children 0=SW, 1=NW, 2=NE, 3=SE
      
        freeQuadtreeNode(node->childSW);
        freeQuadtreeNode(node->childNW);
        freeQuadtreeNode(node->childNE);
        freeQuadtreeNode(node->childSE);

        //free itself
        freeCurreRect(node->currRect);
        free(node->footpaths);
        free(node->nFootpath);
        freePoint(node->point);
        free(node);
        return 1;
    }

    //case 3: internal node
    if((node->footpaths == NULL) && ((node->available) == 0)){
        //free all 4 child 0=SW, 1=NW, 2=NE, 3=SE
        freeQuadtreeNode(node->childSW);
        freeQuadtreeNode(node->childNW);
        freeQuadtreeNode(node->childNE);
        freeQuadtreeNode(node->childSE);
        
        //free itself
        freeCurreRect(node->currRect);
        free(node->nFootpath);
        //printf("free node 3\n");
        free(node);
        
        return 1;   
    }
    return 0;
}


