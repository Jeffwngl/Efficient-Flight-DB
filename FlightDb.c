// Implementation of the Flight DB ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FlightDb.h"
#include "List.h"
#include "Record.h"
#include "Tree.h"

typedef struct node *Node;
struct node {
    Record rec;
    Node   left;
    Node   right;
};
// flightDb is the user interface for the tree and calls functions from Tree.c
struct flightDb {
    Node rootByTime;
    Node rootByFlight;
};

// COMPARE FUNCTIONS

int FlightCompare(Record rec1, Record rec2) { // orders by flight number then by time
    if (strcmp(RecordGetFlightNumber(rec1), RecordGetFlightNumber(rec2)) != 0) {
        return (strcmp(RecordGetFlightNumber(rec1), RecordGetFlightNumber(rec2)));
    }
    else if (RecordGetDepartureDay(rec1) != RecordGetDepartureDay(rec2)) {
        return RecordGetDepartureDay(rec1) - RecordGetDepartureDay(rec2);
    }

    else if (RecordGetDepartureHour(rec1) != RecordGetDepartureHour(rec2)) {
        return RecordGetDepartureHour(rec1) - RecordGetDepartureHour(rec2);
    }

    return RecordGetDepartureMinute(rec1) - RecordGetDepartureMinute(rec2);
}

int LexCompare(Record rec1, Record rec2) { // returns -neg if rec2 is longer or +pos if rec2 is shorter, != 0 if strcmp rec1 and rec2 is not equal
    if (RecordGetDepartureDay(rec1) != RecordGetDepartureDay(rec2)) {
        return RecordGetDepartureDay(rec1) - RecordGetDepartureDay(rec2);
    }

    else if (RecordGetDepartureHour(rec1) != RecordGetDepartureHour(rec2)) {
        return RecordGetDepartureHour(rec1) - RecordGetDepartureHour(rec2);
    }

    else if (RecordGetDepartureMinute(rec1) != RecordGetDepartureMinute(rec2)) {
        return RecordGetDepartureMinute(rec1) - RecordGetDepartureMinute(rec2);
    }

    return strcmp(RecordGetFlightNumber(rec1), RecordGetFlightNumber(rec2));
}

////////////////////////////////////////////////////////////////////////////////

FlightDb DbNew(void) {
    FlightDb db = malloc(sizeof(db));

    if (db == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    db->rootByTime = TreeNew(LexCompare);
    db->rootByFlight = TreeNew(FlightCompare);
    return db;
}

void DbFree(FlightDb db) {
    NodeFree(db->rootByFlight);
    NodeFree(db->rootByTime);
}

// helper function
static void NodeFree(Node n) {
    if (n != NULL) {
        NodeFree(n->left);
        NodeFree(n->right);
        RecordFree(n->rec); // frees records
        free(n); // frees node
    }
}

bool DbInsertRecord(FlightDb db, Record r) {
    return (TreeInsert(db->rootByFlight, r) && TreeInsert(db->rootByTime, r));
}

List DbFindByFlightNumber(FlightDb db, char *flightNumber) {
    List l = ListNew();
    TreeSearch(db, flightNumber); // !!! // TreeSearch takes in Record rec
}

List DbFindByDepartureAirportDay(FlightDb db, char *departureAirport,
                                 int day) {
    return ListNew();
}

List DbFindBetweenTimes(FlightDb db, 
                        int day1, int hour1, int min1, 
                        int day2, int hour2, int min2) {
    return ListNew();
}

Record DbFindNextFlight(FlightDb db, char *flightNumber, 
                        int day, int hour, int min) {
    return NULL;
}

