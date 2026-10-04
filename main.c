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

extern int calculateChargeDuration(
    int currentBattery,
    int targetBattery,
    int power
);


/* ===================================================== */
/* SIMULATION FUNCTIONS                                  */
/* ===================================================== */

extern void runSimulation();

extern void runShortestFirst();

extern void runLowestBatteryFirst();

extern void runEmergencyAging();

extern void compareStrategies();


/* ===================================================== */
/* DISPLAY VEHICLES                                      */
/* ===================================================== */

void displayVehicles() {

    printf("\n");
    printf("========================================\n");
    printf("           VEHICLE INFORMATION\n");
    printf("========================================\n");

    for (int i = 0; i < 5; i++) {

        printf(
            "V%d: Arrival=%d, Battery=%d%%, Target=%d%%",
            vehicles[i].id,
            vehicles[i].arrivalTime,
            vehicles[i].currentBattery,
            vehicles[i].targetBattery
        );

        if (vehicles[i].emergency == 1) {

            printf(" [EMERGENCY]");
        }

        printf("\n");
    }

    printf("========================================\n");
}


/* ===================================================== */
/* CALCULATE CHARGING DURATION                           */
/* ===================================================== */
/* ===================================================== */
/* SEARCH VEHICLE                                        */
/* ===================================================== */

void searchVehicle() {

    int searchId;
    int found = 0;

    char input[50];

    printf("\n");
    printf("========================================\n");
    printf("             SEARCH VEHICLE\n");
    printf("========================================\n");

    printf("Enter Vehicle ID: ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) {

        printf("\nInput error.\n");

        return;
    }

    if (sscanf(input, "%d", &searchId) != 1) {

        printf("\nInvalid Vehicle ID.\n");

        return;
    }

    for (int i = 0; i < 5; i++) {

        if (vehicles[i].id == searchId) {

            printf("\nVehicle Found!\n");
            printf("----------------------------------------\n");

            printf(
                "Vehicle ID       : V%d\n",
                vehicles[i].id
            );

            printf(
                "Arrival Time     : %d\n",
                vehicles[i].arrivalTime
            );

            printf(
                "Current Battery  : %d%%\n",
                vehicles[i].currentBattery
            );

            printf(
                "Target Battery   : %d%%\n",
                vehicles[i].targetBattery
            );

            printf(
                "Charge Duration  : %d time units\n",
                vehicles[i].chargeDuration
            );

            if (vehicles[i].emergency == 1) {

                printf(
                    "Emergency        : Yes\n"
                );

            }
            else {

                printf(
                    "Emergency        : No\n"
                );
            }

            printf("----------------------------------------\n");

            found = 1;

            break;
        }
    }

    if (found == 0) {

        printf(
            "\nVehicle V%d not found.\n",
            searchId
        );
    }
}

/* ===================================================== */
/* SORT VEHICLES                                         */
/* ===================================================== */

void sortVehicles() {

    struct Vehicle temp;

    for (int i = 0; i < 5 - 1; i++) {

        for (int j = 0; j < 5 - i - 1; j++) {

            if (vehicles[j].id > vehicles[j + 1].id) {

                temp = vehicles[j];

                vehicles[j] = vehicles[j + 1];

                vehicles[j + 1] = temp;
            }
        }
    }

    printf("\n");
    printf("========================================\n");
    printf("          VEHICLES SORTED BY ID\n");
    printf("========================================\n");

    for (int i = 0; i < 5; i++) {

        printf(
            "V%d: Arrival=%d, Battery=%d%%, Target=%d%%",
            vehicles[i].id,
            vehicles[i].arrivalTime,
            vehicles[i].currentBattery,
            vehicles[i].targetBattery
        );

        if (vehicles[i].emergency == 1) {

            printf(" [EMERGENCY]");
        }

        printf("\n");
    }

    printf("========================================\n");
}

void calculateDurations() {

    for (int i = 0; i < 5; i++) {

        vehicles[i].chargeDuration =
            calculateChargeDuration(
                vehicles[i].currentBattery,
                vehicles[i].targetBattery,
                c1.power
            );
    }

}
/* ===================================================== */
/* PRESS ENTER TO CONTINUE                               */
/* ===================================================== */

void pressEnter() {

    char input[50];

    printf("\nPress ENTER to return to the main menu...");

    fflush(stdout);

    fgets(input, sizeof(input), stdin);
}

/* ===================================================== */
/* MAIN MENU                                             */
/* ===================================================== */

int main() {

    int choice;

    char input[50];

    calculateDurations();

    do {

        printf("\n");
        printf("========================================\n");
        printf("       SMART EV CHARGING ALLOCATOR\n");
        printf("========================================\n");

        printf("1. Display Vehicles\n");
        printf("2. Run FIFO\n");
        printf("3. Run Shortest Charge First\n");
        printf("4. Run Lowest Battery First\n");
        printf("5. Run Emergency + Aging\n");
        printf("6. Compare All Strategies\n");
        printf("7. Search Vehicle\n");
        printf("8. Sort Vehicles\n");
        printf("0. Exit\n");

        printf("----------------------------------------\n");
        printf("Enter your choice: ");

        fflush(stdout);


        /* ============================================= */
        /* READ MENU INPUT                                */
        /* ============================================= */

        if (fgets(input, sizeof(input), stdin) == NULL) {

            printf("\nInput error. Exiting...\n");

            break;
        }


        /* ============================================= */
        /* CONVERT INPUT TO INTEGER                      */
        /* ============================================= */

        if (sscanf(input, "%d", &choice) != 1) {

            printf("\nInvalid input. Please enter a number.\n");

            continue;
        }


        /* ============================================= */
        /* MENU OPTIONS                                   */
        /* ============================================= */

        switch (choice) {

            case 1:

                displayVehicles();

                pressEnter();

                break;


            case 2:

                runSimulation();

                pressEnter();

                break;


            case 3:

                runShortestFirst();

                pressEnter();

                break;


            case 4:

                runLowestBatteryFirst();

                pressEnter();

                break;


            case 5:

                runEmergencyAging();

                pressEnter();

                break;


            case 6:

                runSimulation();

                runShortestFirst();

                runLowestBatteryFirst();

                runEmergencyAging();

                compareStrategies();

                pressEnter();

                break;

            case 7:

                searchVehicle();

                pressEnter();

                break;

            case 8:

            sortVehicles();

            pressEnter();

            break;

            case 0:

                printf("\n");
                printf("Exiting Smart EV Charging Allocator...\n");

                break;


            default:

                printf("\n");
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 0);


    return 0;
}