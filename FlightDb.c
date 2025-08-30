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
    Tree rootByTime;
    Tree rootByFlight;
    Tree rootByAirport;
};

static void NodeFree(Node n);

// COMPARE FUNCTIONS
// orders by flight number then by day, hour, minute
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
int TimeCompare(Record rec1, Record rec2) { // returns -neg if rec2 is longer or +pos if rec2 is shorter, != 0 if strcmp rec1 and rec2 is not equal
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
// orders by departure airport then day, hour and minute
int AirportCompare(Record rec1, Record rec2) {
    if (strcmp(RecordGetDepartureAirport(rec1), RecordGetDepartureAirport(rec2)) != 0) {
        return strcmp(RecordGetDepartureAirport(rec1), RecordGetDepartureAirport(rec2));
    }
    else if (RecordGetDepartureDay(rec1) != RecordGetDepartureDay(rec2)) {
        return RecordGetDepartureDay(rec1) - RecordGetDepartureDay(rec2);
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

    db->rootByTime = TreeNew(TimeCompare);
    db->rootByFlight = TreeNew(FlightCompare);
    db->rootByAirport = TreeNew(AirportCompare);
    return db;
}

void DbFree(FlightDb db) { // DONE
    TreeFree(db->rootByAirport, true);
    TreeFree(db->rootByFlight, true);
    TreeFree(db->rootByTime, true);
}

bool DbInsertRecord(FlightDb db, Record r) { // DONE
    return (TreeInsert(db->rootByFlight, r) && TreeInsert(db->rootByTime, r) && TreeInsert(db->rootByAirport, r));
}

List DbFindByFlightNumber(FlightDb db, char *flightNumber) { // DONE // Needs testing
    List l = ListNew();
    if (db == NULL || flightNumber == NULL) {
        return l;
    }

    // CollectMatches(db->rootByFlight, flightNumber, l);

    Record dummyLower = RecordNew(flightNumber, "", "", 0, 0, 0, 0);
    Record dummyHigher = RecordNew(flightNumber, "", "", 6, 23, 59, 0);
    
    List orderedFlights = TreeSearchBetween(db->rootByFlight, dummyLower, dummyHigher);

    return orderedFlights;

    // make a 2 temp records to search the tree with. (upper and lower)

    // use search between in order

    // append all available flight numbers to List
    
}


List DbFindByDepartureAirportDay(FlightDb db, char *departureAirport, int day) { // DONE // Needs testing
    List l = ListNew();
    if (db == NULL || departureAirport == NULL || day == NULL) {
        return l;
    }

    Record dummyLower = RecordNew("", departureAirport, "", day, 0, 0, 0);
    Record dummyHigher = RecordNew("", departureAirport, "", day, 23, 59, 0);

    l = TreeSearchBetween(db->rootByAirport, dummyLower, dummyHigher);
    
    return l;
}

List DbFindBetweenTimes(FlightDb db, 
                        int day1, int hour1, int min1, 
                        int day2, int hour2, int min2) { // DONE // Needs testing
    List l = ListNew();

    if (db == NULL || day1 == NULL || hour1 == NULL || min1 == NULL ||
    day2 == NULL || hour2 == NULL || min2 == NULL) {
        return l;
    }

    Record dummyLower = RecordNew("", "", "", day1, hour1, min1, 0);
    Record dummyHigher = RecordNew("","","", day2, hour2, min2, 0);

    l = TreeSearchBetween(db->rootByTime, dummyLower, dummyHigher);

    return l;
}

Record DbFindNextFlight(FlightDb db, char *flightNumber, 
                        int day, int hour, int min) {

    Record target = RecordNew(flightNumber, "", "", day, hour, min, 0);

    return TreeNext(db->rootByFlight, target);

    return NULL;
}

