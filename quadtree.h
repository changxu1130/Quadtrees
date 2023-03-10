/** ***********************
* C Programming Style for Algorithms and Data Structures (COMP20003_2022_SM2)
* Created by Chang Xu (xucm@student.unimelb.edu.au) 9/09/2022
*/


#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "dictionary.h"
#include "result_struct.h"

//constant defnitions


//data structure definition
struct point2D {
	long double x;
    long double y;
};
typedef struct point2D point2D_t;

struct rectangle2D {
	point2D_t* bottomL;
    point2D_t* upperR;
};
typedef struct rectangle2D rectangle2D_t;

typedef struct quadtreeNode quadtreeNode_t;
struct quadtreeNode {
    rectangle2D_t *currRect;
    point2D_t *point; 
    struct data **footpaths; ////////////////////////////////////////////
    int *nFootpath;
    int available; //flag, set to 1 if footpath can be inserted, 0 if not
    quadtreeNode_t *childSW; //SW = 0
    quadtreeNode_t *childNW; //NW = 1
    quadtreeNode_t *childNE; //NE = 2
    quadtreeNode_t *childSE; //SE = 3
}; 

struct rangeSearchResult {
    struct data** footpathsInRange;
    int* nInRange;
    int* quadrantArr;
    int* quadrantArrSize;
};
typedef struct rangeSearchResult rangeSearchResult_t;



//function prototypes


/*
this function takes a pointer to the node (root node in the first case or the head node ow)
and a pointer to the new point that need to be inserted into the quadtree. 
this function will return 1 if insert successfully, 0 if not. 
*/
rangeResult_t* pointRangeSearch(quadtreeNode_t* node, rangeResult_t* searchResults, rectangle2D_t* searchRange, char* query);

/*
this function takes a pointer to the node (root node in the first case or the head node ow)
and a pointer to the new point that need to be inserted into the quadtree. 
this function will return 1 if insert successfully, 0 if not. 
*/
quadtreeNode_t* pointRegionSearch(quadtreeNode_t* node, int* quadrantArr, 
                                int* quadrantArrSize, point2D_t* searchPoint, char* query);

/* 
printResult print the given query result. 
*/
void printResult(char* query, struct data** footpathArr, int footpathArrSize, FILE *outputFile, int stage);

/*
this function add quadtant being searched (represent as int) into the array of 
all the quadrant(s) being search
0=SW, 1=NW, 2=NE, 3=SE
*/
int* addQuadrantToArray(int* arr, int* size, int quadrant);

/*
this function takes int* arr, int* size, point2D_t* searchPoint, as input and print them in 
the formatting required to the stdout 
*/
void printQuadrantsToStdout(int* arr, int* size, char* query);


/*
arrSortm is an insertion sort alogorithm that sort the array of 
pointer to each footpath based on its attribute, footpathID
*/
void footpathsArrSort(struct data** arr, int n);


/*
this function malloc enough space for a point2D and return a pointer to that 
*/
point2D_t* createPoint(long double x, long double y);
/*
this funtion malloc enough space for a rectangle and return a pointer to that
*/
rectangle2D_t* createRect(point2D_t* bottomL, point2D_t* upperR);

/*
this function check whether the input point is in the rectangle object, return
1 if true, 0 if false
0=SW, 1=NW, 2=NE, 3=SE
*/
int inRectangle(rectangle2D_t* rect, point2D_t* point, int quadrant);

/*
this function create children node for a pre-created node
*/
quadtreeNode_t* createChildren(quadtreeNode_t* node, point2D_t* bottomL, point2D_t* upperR);

/*
this function takes a pointer to a quadtreeNode and free all the memory allocated to it
*/
int freeQuadtreeNode(quadtreeNode_t* node);

/*
this function free rectangle given its pointer 
*/
void freeCurreRect(rectangle2D_t* currRect);

/*
this function free point given its pointer
*/
void freePoint(point2D_t* point);

/*
this function takes a pointer to the footpath data struct and a root node, calls
the recusive insert point function to insert each footpath's (start_lon, start_lat) 
and (end_lon, end_lat) pairs into the quadtree.
*/
quadtreeNode_t* insertFootpath(quadtreeNode_t* node, struct data* footpath);

/*
this function add footpath into the array of footpaths stored in node
*/
struct data ** addFootpathToArray(struct data ** arrFootpaths, int* size, struct data* footpath);

/*
this function takes a pointer to the node (root node in the first case or the head node ow)
and a pointer to the new point that need to be inserted into the quadtree. 
this function will return 1 if insert successfully, 0 if not. 
*/
int insertPoint(quadtreeNode_t* node, point2D_t* point, struct data *footpath);

/*
this function compare two long double and check whether they are the same 
*/
int compareLongDouble(long double x, long double y);

long double absoluteValue(long double ld);

/*
this function takes 2 rectangeles, return 1 if the 2 overlap, 0 otherwise
*/
int rectangleOverlap(rectangle2D_t* rec1, rectangle2D_t* rect2);




/*
this function add footpath found in range into the array of all 
the footpaths found 
*/
struct data** addFoundFootpathToArray(struct data** footpathsInRange, int* nInRange, struct data* footpath);

/*
partition is a helper function that responsible to partoition the 
array for quicksort
*/
int partition(struct data** arr, int low, int high);

/*
quickSort call partition function to sort the array of 
pointers to footpath based on its id
*/
void quickSort(struct data** arr, int low, int high);


/*
swap is a helper function for the quickSort function, which swap two elements
*/
void swap(struct data** arr, int i, int j);

/*
removeDuplicate responsible to remove duplicated pointers that point 
to the same footpath if exsisted. It returns a ppointed to the updated
array free of duplication. 
*/
struct data** removeDuplicate(struct data** arr, int* n);

#endif
