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
    Node rootByAirport;
};

// COMPARE FUNCTIONS
// orders by flight number then by day, hour minute
int FlightCompare(Record rec1, Record rec2) {
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
// orders by day then hour, minute
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
// orders by departure airport then hour, minute, flight number
int AirportCompare(Record rec1, Record rec2) {
    if (strcmp(RecordGetDepartureAirport(rec1), RecordGetDepartureAirport(rec2)) != 0) {
        return strcmp(RecordGetDepartureAirport(rec1), RecordGetDepartureAirport(rec2));
    }
    else if (RecordGetDepartureDay(rec1) != RecordGetDepartureDay(rec2)) {
        returnRecordGetDepartureDay(rec1) - RecordGetDepartureDay(rec2);
    }
    else if (RecordGetDepartureHour(rec1) != RecordGetDepartureHour(rec2)) {
        return RecordGetDepartureHour(rec1) - RecordGetDepartureHour(rec2);
    }
    
    return RecordGetDepartureMinute(rec1) - RecordGetDepartureMinute(rec2);
}

////////////////////////////////////////////////////////////////////////////////

FlightDb DbNew(void) { // DONE
    FlightDb db = malloc(sizeof(db));

    if (db == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    db->rootByTime = TreeNew(LexCompare);
    db->rootByFlight = TreeNew(FlightCompare);
    db->rootByAirport = TreeNew(AirportCompare);
    return db;
}

void DbFree(FlightDb db) { // DONE
    NodeFree(db->rootByFlight);
    NodeFree(db->rootByTime);
}

// helper function
static void NodeFree(Node n) { // DONE
    if (n != NULL) {
        NodeFree(n->left);
        NodeFree(n->right);
        RecordFree(n->rec); // frees records
        free(n); // frees node
    }
}

bool DbInsertRecord(FlightDb db, Record r) { // DONE
    return (TreeInsert(db->rootByFlight, r) && TreeInsert(db->rootByTime, r) && TreeInsert(db->rootByAirport, r));
}

List DbFindByFlightNumber(FlightDb db, char *flightNumber) {
    List l = ListNew();
    if (db == NULL || flightNumber == NULL) {
        return l;
    }

    CollectMatches(db->rootByFlight, flightNumber, l);

    for (Node n = l; n != NULL; n = n->)

    // make a temp record to search the tree with.

    // append all available flight numbers to List

    // sort flight numbers by time
    
}

// helper functions
static CompareFlight(char *flightNumber, Record rec2) {
    return strcmp(flightNumber, RecordGetFlightNumber(rec2));
}

static void CollectMatches(Node n, const char *flightNumber, List l) {
    if (n == NULL) {
        return;
    }

    int cmp = CompareFlight(flightNumber, n->rec);
    if (cmp > 0) {
        CollectMatches(n->left, flightNumber, l);
    }
    else if (cmp < 0) {
        CollectMatches(n->right, flightNumber, l);
    }
    else {
        ListAppend(l, n->rec);
        CollectMatches(n->left, flightNumber, l);
        CollectMatches(n->right, flightNumber, l);
    }
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

