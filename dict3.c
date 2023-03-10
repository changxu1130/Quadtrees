/** ***********************
* C Programming Style for Algorithms and Data Structures (COMP20003_2022_SM2)
* Created by Chang Xu (xucm@student.unimelb.edu.au) 9/09/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "read.h"
#include "record_struct.h"
#include "dictionary.h"
#include "quadtree.h"

#define MINARGS 8
#define EXPECTED_STAGE "3"
#define STAGE (LOOKUPSTAGE) ////////////////////////////////////////////////


int main(int argc, char **argv){

    //return an error message if Insufficient arguments being entered
    if(argc < MINARGS){
        fprintf(stderr, "Insufficient arguments\n");
        exit(EXIT_FAILURE);
    }

    //incorrect stage call
    if (strcmp(EXPECTED_STAGE, argv[1]) != 0){
        fprintf(stderr, "Expected stage was %s, received %s\n", EXPECTED_STAGE, argv[1]);
        exit(EXIT_FAILURE);
    }

    //reading files 
    char *inputCSVName = argv[2];
    char *outputFileName = argv[3];

    FILE *csvFile = fopen(inputCSVName, "r");
    assert(csvFile);
    FILE *outputFile = fopen(outputFileName, "w");
    assert(outputFile);

    point2D_t* iniBL = createPoint(strtold(argv[4], NULL), strtold(argv[5], NULL)); 
    point2D_t* iniUR = createPoint(strtold(argv[6], NULL), strtold(argv[7], NULL)); 

    //initialise the root node 
    quadtreeNode_t *root = (quadtreeNode_t *)malloc(sizeof(quadtreeNode_t));
    root->currRect = createRect(iniBL, iniUR);
    root->footpaths = NULL;
    root->point = NULL;
    root->available = 1; 
    root->nFootpath = (int*)malloc(sizeof(int));
    *(root->nFootpath)= 0;
    
    int n;
    struct csvRecord **dataset = readCSV(csvFile, &n);
    struct dictionary *dict = newDict();

    for(int i = 0; i < n; i++){
        insertRecord(dict, dataset[i]); 
    }
    freeCSV(dataset, n);
    

    struct dictionaryNode* currNode = dict->head;
    assert(currNode);

    while(1){
        
        insertFootpath(root, currNode->record);
        if((currNode->next)== NULL){
            break;
        }
        currNode = currNode->next;
    }
   
    assert(root);

    char *query = NULL;

    //accept an address 
    while((query = getQuery(stdin))){
        assert(query);
        point2D_t* searchPoint = (point2D_t*)malloc(sizeof(point2D_t)); 
        sscanf(query, "%Lf %Lf", &(searchPoint->x), &(searchPoint->y)); 
        
        int* quadrantArrSize = (int*)malloc(sizeof(int));
        *quadrantArrSize = 0;

        quadtreeNode_t* foundLeafNode = pointRegionSearch(root, NULL, quadrantArrSize, searchPoint, query);
        assert(foundLeafNode);
        
        //sort the array of footpath based on its ID before printing 
        quickSort(foundLeafNode->footpaths, 0, *(foundLeafNode->nFootpath) - 1);

        //print out the search result 
        printResult(query, foundLeafNode->footpaths, *(foundLeafNode->nFootpath), outputFile, 3);

        free(query);
        freePoint(searchPoint);
    }

    //free all malloced data structure
    freeQuadtreeNode(root);
    freeDict(dict);
    dict = NULL;

    fclose(csvFile);
    fclose(outputFile);

    return EXIT_SUCCESS;
}
