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
/* HEAP                                                   */
/* ===================================================== */

#define MAX_HEAP_SIZE 20

struct Vehicle minHeap[MAX_HEAP_SIZE];

int heapSize = 0;


/*
   Priority rule:

   1 = Shortest Charge First
   2 = Lowest Battery First
   3 = Emergency + Aging
*/

int heapRule = 1;


/* ===================================================== */
/* SET HEAP RULE                                          */
/* ===================================================== */

void setHeapRule(int rule) {

    heapRule = rule;
}


/* ===================================================== */
/* COMPARE PRIORITY                                       */
/* ===================================================== */

int hasHigherPriority(
    struct Vehicle a,
    struct Vehicle b
) {

    /* ----------------------------------------- */
    /* RULE 1: SHORTEST CHARGE FIRST             */
    /* ----------------------------------------- */

    if (heapRule == 1) {

        if (
            a.chargeDuration <
            b.chargeDuration
        ) {

            return 1;
        }

        if (
            a.chargeDuration >
            b.chargeDuration
        ) {

            return 0;
        }

        /* Tie → smaller vehicle ID */

        return a.id < b.id;
    }


    /* ----------------------------------------- */
    /* RULE 2: LOWEST BATTERY FIRST              */
    /* ----------------------------------------- */

    if (heapRule == 2) {

        if (
            a.currentBattery <
            b.currentBattery
        ) {

            return 1;
        }

        if (
            a.currentBattery >
            b.currentBattery
        ) {

            return 0;
        }

        /* Tie → earlier arrival */

        if (
            a.arrivalTime <
            b.arrivalTime
        ) {

            return 1;
        }

        if (
            a.arrivalTime >
            b.arrivalTime
        ) {

            return 0;
        }

        /* Final tie → smaller ID */

        return a.id < b.id;
    }


    /* ----------------------------------------- */
    /* RULE 3: EMERGENCY + AGING                 */
    /* ----------------------------------------- */

    if (heapRule == 3) {

        /*
           Emergency vehicles get priority.
        */

        if (
            a.emergency >
            b.emergency
        ) {

            return 1;
        }

        if (
            a.emergency <
            b.emergency
        ) {

            return 0;
        }


        /*
           Larger waitTime means
           more waiting → higher priority.
        */

        if (
            a.waitTime >
            b.waitTime
        ) {

            return 1;
        }

        if (
            a.waitTime <
            b.waitTime
        ) {

            return 0;
        }


        /* Final tie → smaller ID */

        return a.id < b.id;
    }


    return 0;
}


/* ===================================================== */
/* SWAP TWO VEHICLES                                     */
/* ===================================================== */

void swapVehicles(
    struct Vehicle *a,
    struct Vehicle *b
) {

    struct Vehicle temp;

    temp = *a;

    *a = *b;

    *b = temp;
}


/* ===================================================== */
/* HEAPIFY UP                                             */
/* ===================================================== */

void heapifyUp(int index) {

    while (index > 0) {

        int parent =
            (index - 1) / 2;


        if (
            !hasHigherPriority(
                minHeap[index],
                minHeap[parent]
            )
        ) {

            break;
        }


        swapVehicles(
            &minHeap[index],
            &minHeap[parent]
        );


        index = parent;
    }
}


/* ===================================================== */
/* HEAPIFY DOWN                                           */
/* ===================================================== */

void heapifyDown(int index) {

    while (1) {

        int left =
            2 * index + 1;

        int right =
            2 * index + 2;

        int highest = index;


        if (
            left < heapSize &&
            hasHigherPriority(
                minHeap[left],
                minHeap[highest]
            )
        ) {

            highest = left;
        }


        if (
            right < heapSize &&
            hasHigherPriority(
                minHeap[right],
                minHeap[highest]
            )
        ) {

            highest = right;
        }


        if (highest == index) {

            break;
        }


        swapVehicles(
            &minHeap[index],
            &minHeap[highest]
        );


        index = highest;
    }
}


/* ===================================================== */
/* INSERT INTO HEAP                                      */
/* ===================================================== */

void insertHeap(
    struct Vehicle vehicle
) {

    if (
        heapSize >= MAX_HEAP_SIZE
    ) {

        printf("Heap is full!\n");

        return;
    }


    minHeap[heapSize] =
        vehicle;

    heapSize++;


    heapifyUp(
        heapSize - 1
    );
}


/* ===================================================== */
/* EXTRACT HIGHEST PRIORITY                              */
/* ===================================================== */

struct Vehicle extractMin() {

    struct Vehicle selected;


    selected =
        minHeap[0];


    minHeap[0] =
        minHeap[heapSize - 1];

    heapSize--;


    if (heapSize > 0) {

        heapifyDown(0);
    }


    return selected;
}


/* ===================================================== */
/* CHECK EMPTY                                           */
/* ===================================================== */

int isHeapEmpty() {

    return heapSize == 0;
}


/* ===================================================== */
/* RESET HEAP                                             */
/* ===================================================== */

void resetHeap() {

    heapSize = 0;
}


/* ===================================================== */
/* DISPLAY HEAP                                           */
/* ===================================================== */

void displayHeap() {

    printf("\nHeap:\n");


    for (
        int i = 0;
        i < heapSize;
        i++
    ) {

        printf(
            "V%d ",
            minHeap[i].id
        );
    }


    printf("\n");
}