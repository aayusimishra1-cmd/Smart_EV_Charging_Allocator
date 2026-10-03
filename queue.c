#include <stdio.h>
/* ---------------- VEHICLE STRUCTURE ---------------- */
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
/* ---------------- ENQUEUE ---------------- */
/*
   Adds a vehicle to the end
   of the waiting queue.
*/
void enqueue(
    struct Vehicle waitingQueue[],
    int *waitingCount,
    struct Vehicle vehicle
) {

    waitingQueue[*waitingCount] = vehicle;

    (*waitingCount)++;
}
/* ---------------- DEQUEUE ---------------- */

/*
   Removes the first vehicle
   from the waiting queue.
*/
struct Vehicle dequeue(
    struct Vehicle waitingQueue[],
    int *waitingCount
) {
    struct Vehicle vehicle;

    vehicle = waitingQueue[0];

    /* Shift remaining vehicles left */

    for (
        int i = 0;
        i < *waitingCount - 1;
        i++
    ) {

        waitingQueue[i] =
            waitingQueue[i + 1];
    }
    (*waitingCount)--;

    return vehicle;
}