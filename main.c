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
/* ---------------- CHARGER STRUCTURE ---------------- */
struct Charger {

    int id;
    int power;
    int busy;
    int vehicleId;
    int finishTime;

};
/* ---------------- DATA FROM data.c ---------------- */

extern struct Vehicle vehicles[5];
extern struct Charger c1;
extern struct Charger c2;
extern int calculateChargeDuration(
    int currentBattery,
    int targetBattery,
    int power
);

/* ---------------- SIMULATION FROM simulation.c ---------------- */

extern void runSimulation();
extern void runShortestFirst();
extern void runLowestBatteryFirst();
extern void insertHeap(struct Vehicle vehicle);

extern struct Vehicle extractMin();

extern void displayHeap();

/* MAIN FUNCTION                                         */

int main() {


    /* PROJECT TITLE                            */

    printf(
        "Smart EV Charging Allocator\n"
    );

    /* DISPLAY VEHICLE INFORMATION               */

    printf("\nVehicles:\n");


    for (int i = 0; i < 5; i++) {

        printf(

            "V%d: Arrival=%d, Battery=%d%%, Target=%d%%\n",

            vehicles[i].id,

            vehicles[i].arrivalTime,

            vehicles[i].currentBattery,

            vehicles[i].targetBattery
        );
    }

    /* CALCULATE CHARGING DURATION               */

    for (int i = 0; i < 5; i++) {

        vehicles[i].chargeDuration =

            calculateChargeDuration(

                vehicles[i].currentBattery,

                vehicles[i].targetBattery,

                c1.power
            );
    }

    /* DISPLAY CHARGING DURATION                 */
    
    printf("\nCharging Duration:\n");
    for (int i = 0; i < 5; i++) {

        printf(

            "V%d: %d time units\n",

            vehicles[i].id,

            vehicles[i].chargeDuration
        );
    }
    /* ----------------------------------------- */
/* TEST MIN-HEAP                             */
/* ----------------------------------------- */

    /* START SIMULATION                          */
    runSimulation();

runShortestFirst();
runLowestBatteryFirst();
    /* END PROGRAM                               */

    return 0;
}