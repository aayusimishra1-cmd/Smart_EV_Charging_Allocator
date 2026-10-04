#include <stdio.h>


/* ===================================================== */
/* VEHICLE STRUCTURE                                     */
/* ===================================================== */

struct Vehicle {

    int id;
    int arrivalTime;
    int currentBattery;
    int targetBattery;
    int chargeDuration;
    int emergency;
    int startTime;
    int waitTime;

};


/* ===================================================== */
/* CHARGER STRUCTURE                                     */
/* ===================================================== */

struct Charger {

    int id;
    int power;
    int busy;
    int vehicleId;
    int finishTime;

};


/* ===================================================== */
/* VEHICLE DATA                                          */
/* ===================================================== */

struct Vehicle vehicles[5] = {

    /* ID  Arrival  Battery  Target  Duration  Emergency  Start  Wait */

    {1,    0,       20,      80,     0,        0,         -1,    0},

    {2,    0,       40,      80,     0,        0,         -1,    0},

    {3,    1,       30,      80,     0,        0,         -1,    0},

    {4,    2,       50,      80,     0,        1,         -1,    0},

    {5,    2,       60,      80,     0,        0,         -1,    0}

};


/* ===================================================== */
/* CHARGER DATA                                          */
/* ===================================================== */

struct Charger c1 = {

    1,
    50,
    0,
    0,
    0

};


struct Charger c2 = {

    2,
    50,
    0,
    0,
    0

};


/* ===================================================== */
/* CALCULATE CHARGING DURATION                           */
/* ===================================================== */

int calculateChargeDuration(

    int currentBattery,

    int targetBattery,

    int power

) {

    int batteryDifference;

    int k;

    int duration;


    batteryDifference =
        targetBattery - currentBattery;


    k = 10;


    duration =
        (batteryDifference * k) / power;


    return duration;
}