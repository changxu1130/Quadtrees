/*
dictionary.h file is based on the solution provided in assignment 1
*/



/*
    Data structure declarations and prototypes for a 
    dictionary module intended to allow lookup
    for particular values.
*/
#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include "record_struct.h"
#include <stdio.h>

#define LOOKUPSTAGE 1
#define PROXIMITYSTAGE 2


struct queryResult {
    char *searchString;
    int numRecords;
    double closestValue;
    struct data **records;
};

/* CSV records. */
struct data;
struct data {
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    double deltaz;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char *segside;
    int statusid;
    int streetid;
    int street_group;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
};

/* A node in the dictionary, used to allow quick lookup. */
struct dictionaryNode;

struct dictionaryNode{
    struct data *record;
    struct dictionaryNode *next;
};

struct index;

struct index {
    int valueCount;
    struct dictionaryNode **nodes;
};

/* Dictionary - concrete data structure is linked list. */
struct dictionary {
    struct dictionaryNode *head;
    struct dictionaryNode *tail;
    struct index **indices;
};


/* Result of a query. */
struct queryResult;

/* Dictionary. */
struct dictionary;

/* Returns an empty dictionary. */
struct dictionary *newDict();

/* Prints the relevant field. */
void printField(FILE *f, struct data *record, int fieldIndex);

/* Insert a given record into the dictionary. */
void insertRecord(struct dictionary *dict, struct csvRecord *record);

/* Search for a given key in the dictionary. */
struct queryResult *lookupRecord(struct dictionary *dict, char *query);

/* Search for the closest record in the dictionary to the query string in the given
    field index. Assumes the field selected is double type. */
struct queryResult *searchClosestDouble(struct dictionary *dict, char *query, 
    int fieldIndex);

/* Output the given query result. 
    If stage is LOOKUPSTAGE, information retrieved by the lookupRecord
        function can be printed.
    
    If stage is PROXIMITYSTAGE, information retrieved by the searchClosestDouble
        function can be printed.
    
    The formats of these print outs is described in the COMP20003 Assignment 1 
        specification where summaryFile refers to stdout and outputFile refers 
        to the output file.
*/
void printQueryResult(struct queryResult *r, FILE *summaryFile, 
    FILE *outputFile, int stage);

/* Free the given query result. */
void freeQueryResult(struct queryResult *r);

/* Free a given dictionary. */
void freeDict(struct dictionary *dict);

#endif

