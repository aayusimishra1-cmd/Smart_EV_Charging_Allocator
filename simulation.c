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
/* DATA FROM data.c                                      */
/* ===================================================== */

extern struct Vehicle vehicles[5];

extern struct Charger c1;

extern struct Charger c2;


/* ===================================================== */
/* FUNCTIONS FROM queue.c                                */
/* ===================================================== */

extern void enqueue(
    struct Vehicle waitingQueue[],
    int *waitingCount,
    struct Vehicle vehicle
);


extern struct Vehicle dequeue(
    struct Vehicle waitingQueue[],
    int *waitingCount
);


/* ===================================================== */
/* FUNCTIONS FROM heap.c                                 */
/* ===================================================== */

extern void insertHeap(
    struct Vehicle vehicle
);

extern struct Vehicle extractMin();

extern int isHeapEmpty();

extern void setHeapRule(int rule);

extern void resetHeap();


/* ===================================================== */
/* RESET CHARGERS                                        */
/* ===================================================== */

void resetChargers() {

    c1.busy = 0;

    c1.vehicleId = 0;

    c1.finishTime = 0;


    c2.busy = 0;

    c2.vehicleId = 0;

    c2.finishTime = 0;
}


/* ===================================================== */
/* FIND NEXT FINISH EVENT                                */
/* ===================================================== */

int getNextEventTime() {

    int nextTime = -1;


    /* ---------------- CHARGER 1 ---------------- */

    if (c1.busy == 1) {

        nextTime = c1.finishTime;
    }


    /* ---------------- CHARGER 2 ---------------- */

    if (c2.busy == 1) {

        if (
            nextTime == -1 ||
            c2.finishTime < nextTime
        ) {

            nextTime = c2.finishTime;
        }
    }


    return nextTime;
}


/* ===================================================== */
/* FINISH CHARGING                                      */
/* ===================================================== */

void finishCharging(int currentTime) {


    /* ---------------- CHARGER 1 ---------------- */

    if (
        c1.busy == 1 &&
        c1.finishTime == currentTime
    ) {

        printf(
            "V%d finished charging on Charger %d\n",
            c1.vehicleId,
            c1.id
        );


        c1.busy = 0;

        c1.vehicleId = 0;

        c1.finishTime = 0;
    }


    /* ---------------- CHARGER 2 ---------------- */

    if (
        c2.busy == 1 &&
        c2.finishTime == currentTime
    ) {

        printf(
            "V%d finished charging on Charger %d\n",
            c2.vehicleId,
            c2.id
        );


        c2.busy = 0;

        c2.vehicleId = 0;

        c2.finishTime = 0;
    }
}


/* ===================================================== */
/* ASSIGN VEHICLE TO CHARGER                            */
/* ===================================================== */

void assignVehicle(
    struct Charger *charger,
    struct Vehicle vehicle,
    int currentTime
) {

    charger->busy = 1;

    charger->vehicleId =
        vehicle.id;

    charger->finishTime =
        currentTime +
        vehicle.chargeDuration;


    printf(
        "V%d starts charging on Charger %d at time %d\n",
        vehicle.id,
        charger->id,
        currentTime
    );


    printf(
        "Expected finish time: %d\n",
        charger->finishTime
    );
}


/* ===================================================== */
/* RUN FIFO SIMULATION                                   */
/* ===================================================== */

void runSimulation() {

    struct Vehicle waitingQueue[10];

    int waitingCount = 0;

    int currentTime = 0;

    int nextArrivalIndex = 0;


    resetChargers();


    printf("\n");

    printf("==============================\n");

    printf("FIFO SIMULATION\n");

    printf("==============================\n");


    while (

        nextArrivalIndex < 5 ||
        waitingCount > 0 ||
        c1.busy == 1 ||
        c2.busy == 1

    ) {


        /* ========================================= */
        /* ADD ARRIVED VEHICLES                      */
        /* ========================================= */

        while (

            nextArrivalIndex < 5 &&
            vehicles[nextArrivalIndex].arrivalTime
                <= currentTime

        ) {

            printf(
                "\nTime %d: V%d arrived\n",
                currentTime,
                vehicles[nextArrivalIndex].id
            );


            enqueue(
                waitingQueue,
                &waitingCount,
                vehicles[nextArrivalIndex]
            );


            nextArrivalIndex++;
        }


        /* ========================================= */
        /* FINISH CHARGING                           */
        /* ========================================= */

        finishCharging(currentTime);


        /* ========================================= */
        /* CHARGER 1                                 */
        /* ========================================= */

        if (

            c1.busy == 0 &&
            waitingCount > 0

        ) {

            struct Vehicle nextVehicle;


            nextVehicle =
                dequeue(
                    waitingQueue,
                    &waitingCount
                );


            assignVehicle(
                &c1,
                nextVehicle,
                currentTime
            );
        }


        /* ========================================= */
        /* CHARGER 2                                 */
        /* ========================================= */

        if (

            c2.busy == 0 &&
            waitingCount > 0

        ) {

            struct Vehicle nextVehicle;


            nextVehicle =
                dequeue(
                    waitingQueue,
                    &waitingCount
                );


            assignVehicle(
                &c2,
                nextVehicle,
                currentTime
            );
        }


        /* ========================================= */
        /* NEXT EVENT                                */
        /* ========================================= */

        int nextEventTime;


        nextEventTime =
            getNextEventTime();


        if (nextArrivalIndex < 5) {

            int nextArrivalTime;


            nextArrivalTime =
                vehicles[nextArrivalIndex].arrivalTime;


            if (

                nextEventTime == -1 ||
                nextArrivalTime < nextEventTime

            ) {

                currentTime =
                    nextArrivalTime;
            }

            else {

                currentTime =
                    nextEventTime;
            }
        }

        else {

            if (nextEventTime != -1) {

                currentTime =
                    nextEventTime;
            }
        }
    }


    printf("\n");

    printf("==============================\n");

    printf("FIFO SIMULATION COMPLETE\n");

    printf("==============================\n");
}


/* ===================================================== */
/* RUN SHORTEST CHARGE FIRST SIMULATION                 */
/* ===================================================== */

void runShortestFirst() {

    int currentTime = 0;

    int nextArrivalIndex = 0;


    resetChargers();

    resetHeap();

    setHeapRule(1);


    printf("\n");

    printf("==============================\n");

    printf("SHORTEST CHARGE FIRST\n");

    printf("==============================\n");


    while (

        nextArrivalIndex < 5 ||
        !isHeapEmpty() ||
        c1.busy == 1 ||
        c2.busy == 1

    ) {


        /* ========================================= */
        /* ADD ARRIVED VEHICLES TO HEAP             */
        /* ========================================= */

        while (

            nextArrivalIndex < 5 &&
            vehicles[nextArrivalIndex].arrivalTime
                <= currentTime

        ) {

            printf(
                "\nTime %d: V%d arrived\n",
                currentTime,
                vehicles[nextArrivalIndex].id
            );


            insertHeap(
                vehicles[nextArrivalIndex]
            );


            nextArrivalIndex++;
        }


        /* ========================================= */
        /* FINISH COMPLETED CHARGING                 */
        /* ========================================= */

        finishCharging(currentTime);


        /* ========================================= */
        /* ASSIGN CHARGER 1                         */
        /* ========================================= */

        if (

            c1.busy == 0 &&
            !isHeapEmpty()

        ) {

            struct Vehicle nextVehicle;


            nextVehicle =
                extractMin();


            assignVehicle(
                &c1,
                nextVehicle,
                currentTime
            );
        }


        /* ========================================= */
        /* ASSIGN CHARGER 2                         */
        /* ========================================= */

        if (

            c2.busy == 0 &&
            !isHeapEmpty()

        ) {

            struct Vehicle nextVehicle;


            nextVehicle =
                extractMin();


            assignVehicle(
                &c2,
                nextVehicle,
                currentTime
            );
        }


        /* ========================================= */
        /* FIND NEXT EVENT                           */
        /* ========================================= */

        int nextEventTime;


        nextEventTime =
            getNextEventTime();


        /* ========================================= */
        /* JUMP TO NEXT EVENT                        */
        /* ========================================= */

        if (nextArrivalIndex < 5) {

            int nextArrivalTime;


            nextArrivalTime =
                vehicles[nextArrivalIndex].arrivalTime;


            if (

                nextEventTime == -1 ||
                nextArrivalTime < nextEventTime

            ) {

                currentTime =
                    nextArrivalTime;
            }

            else {

                currentTime =
                    nextEventTime;
            }
        }

        else {

            if (nextEventTime != -1) {

                currentTime =
                    nextEventTime;
            }
        }
    }


    printf("\n");

    printf("==============================\n");

    printf("SHORTEST CHARGE FIRST COMPLETE\n");

    printf("==============================\n");
}


/* ===================================================== */
/* RUN LOWEST BATTERY FIRST SIMULATION                  */
/* ===================================================== */

void runLowestBatteryFirst() {

    int currentTime = 0;

    int nextArrivalIndex = 0;


    resetChargers();

    resetHeap();

    setHeapRule(2);


    printf("\n");

    printf("==============================\n");

    printf("LOWEST BATTERY FIRST\n");

    printf("==============================\n");


    while (

        nextArrivalIndex < 5 ||
        !isHeapEmpty() ||
        c1.busy == 1 ||
        c2.busy == 1

    ) {


        /* ========================================= */
        /* ADD ARRIVED VEHICLES TO HEAP             */
        /* ========================================= */

        while (

            nextArrivalIndex < 5 &&
            vehicles[nextArrivalIndex].arrivalTime
                <= currentTime

        ) {

            printf(
                "\nTime %d: V%d arrived\n",
                currentTime,
                vehicles[nextArrivalIndex].id
            );


            insertHeap(
                vehicles[nextArrivalIndex]
            );


            nextArrivalIndex++;
        }


        /* ========================================= */
        /* FINISH CHARGING                          */
        /* ========================================= */

        finishCharging(currentTime);


        /* ========================================= */
        /* ASSIGN CHARGER 1                         */
        /* ========================================= */

        if (

            c1.busy == 0 &&
            !isHeapEmpty()

        ) {

            struct Vehicle nextVehicle;


            nextVehicle =
                extractMin();


            assignVehicle(
                &c1,
                nextVehicle,
                currentTime
            );
        }


        /* ========================================= */
        /* ASSIGN CHARGER 2                         */
        /* ========================================= */

        if (

            c2.busy == 0 &&
            !isHeapEmpty()

        ) {

            struct Vehicle nextVehicle;


            nextVehicle =
                extractMin();


            assignVehicle(
                &c2,
                nextVehicle,
                currentTime
            );
        }


        /* ========================================= */
        /* FIND NEXT EVENT                           */
        /* ========================================= */

        int nextEventTime;


        nextEventTime =
            getNextEventTime();


        /* ========================================= */
        /* JUMP TO NEXT EVENT                        */
        /* ========================================= */

        if (nextArrivalIndex < 5) {

            int nextArrivalTime;


            nextArrivalTime =
                vehicles[nextArrivalIndex].arrivalTime;


            if (

                nextEventTime == -1 ||
                nextArrivalTime < nextEventTime

            ) {

                currentTime =
                    nextArrivalTime;
            }

            else {

                currentTime =
                    nextEventTime;
            }
        }

        else {

            if (nextEventTime != -1) {

                currentTime =
                    nextEventTime;
            }
        }
    }


    printf("\n");

    printf("==============================\n");

    printf("LOWEST BATTERY FIRST COMPLETE\n");

    printf("==============================\n");
}