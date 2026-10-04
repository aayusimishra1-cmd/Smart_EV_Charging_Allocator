#include <stdio.h> 
 
#define MAX_VEHICLES 5 
#define MAX_QUEUE 10 
#define MAX_PATIENCE 10 
 
/* ============================= */ 
/* STRATEGY RESULTS              */ 
/* ============================= */ 
 
int strategyServed[5]; 
int strategyLost[5]; 
 
float strategyAverageWait[5]; 
int strategyMaxWait[5]; 
 
float strategyUtilization[5]; 
 
/* ============================= */ 
/* STRUCTURES                    */ 
/* ============================= */ 
 
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
 
struct Charger { 
    int id; 
    int power; 
    int busy; 
    int vehicleId; 
    int finishTime; 
}; 
 
/* ============================= */ 
/* DATA FROM data.c              */ 
/* ============================= */ 
 
extern struct Vehicle vehicles[MAX_VEHICLES]; 
 
extern struct Charger c1; 
extern struct Charger c2; 
 
extern int calculateChargeDuration( 
    int currentBattery, 
    int targetBattery, 
    int power 
); 
 
/* ============================= */ 
/* QUEUE FUNCTIONS               */ 
/* ============================= */ 
 
extern void enqueue( 
    struct Vehicle waitingQueue[], 
    int *waitingCount, 
    struct Vehicle vehicle 
); 
 
extern struct Vehicle dequeue( 
    struct Vehicle waitingQueue[], 
    int *waitingCount 
); 
 
/* ============================= */ 
/* HEAP FUNCTIONS                */ 
/* ============================= */ 
 
extern void insertHeap(struct Vehicle vehicle); 
extern struct Vehicle extractMin(); 
extern int isHeapEmpty(); 
extern void setHeapRule(int rule); 
extern void resetHeap(); 
 
/* ============================= */ 
/* RESET CHARGERS                */ 
/* ============================= */ 
 
void resetChargers() { 
 
    c1.busy = 0; 
    c1.vehicleId = 0; 
    c1.finishTime = 0; 
 
    c2.busy = 0; 
    c2.vehicleId = 0; 
    c2.finishTime = 0; 
} 
 
/* ============================= */ 
/* NEXT FINISH EVENT             */ 
/* ============================= */ 
 
int getNextEventTime() { 
 
    int nextTime = -1; 
 
    if (c1.busy == 1) { 
        nextTime = c1.finishTime; 
    } 
 
    if (c2.busy == 1) { 
 
        if (nextTime == -1 || 
            c2.finishTime < nextTime) { 
 
            nextTime = c2.finishTime; 
        } 
    } 
 
    return nextTime; 
} 
 
/* ============================= */ 
/* FINISH CHARGING               */ 
/* ============================= */ 
 
void finishCharging(int currentTime) { 
 
    if (c1.busy == 1 && 
        c1.finishTime == currentTime) { 
 
        printf( 
            "V%d finished charging on Charger %d\n", 
            c1.vehicleId, 
            c1.id 
        ); 
 
        c1.busy = 0; 
        c1.vehicleId = 0; 
        c1.finishTime = 0; 
    } 
 
    if (c2.busy == 1 && 
        c2.finishTime == currentTime) { 
 
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
 
/* ============================= */ 
/* ASSIGN VEHICLE                */ 
/* ============================= */ 
 
void assignVehicle( 
    struct Charger *charger, 
    struct Vehicle vehicle, 
    int currentTime 
) { 
 
    charger->busy = 1; 
    charger->vehicleId = vehicle.id; 
 
    charger->finishTime = 
        currentTime + vehicle.chargeDuration; 
 
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
 
/* ============================= */ 
/* PRINT STRATEGY NAME           */ 
/* ============================= */ 
 
void printStrategyName(int strategy) { 
 
    printf("\n"); 
    printf("==============================\n"); 
 
    if (strategy == 1) { 
        printf("FIFO SIMULATION\n"); 
    } 
    else if (strategy == 2) { 
        printf("SHORTEST CHARGE FIRST\n"); 
    } 
    else if (strategy == 3) { 
        printf("LOWEST BATTERY FIRST\n"); 
    } 
    else { 
        printf("EMERGENCY + AGING\n"); 
    } 
 
    printf("==============================\n"); 
} 
 
/* ============================= */ 
/* SELECT VEHICLE FROM QUEUE     */ 
/* ============================= */ 
 
int getFIFO( 
    struct Vehicle waitingQueue[], 
    int *waitingCount, 
    int currentTime, 
    struct Vehicle *selected, 
    int *lost 
) { 
 
    while (*waitingCount > 0) { 
 
        *selected = 
            dequeue( 
                waitingQueue, 
                waitingCount 
            ); 
 
        selected->waitTime = 
            currentTime - selected->arrivalTime; 
 
        /* 
            Vehicle leaves if patience 
            limit has been exceeded. 
        */ 
 
        if (selected->waitTime > MAX_PATIENCE) { 
 
            printf( 
                "V%d waited too long and left.\n", 
                selected->id 
            ); 
 
            (*lost)++; 
        } 
        else { 
            return 1; 
        } 
    } 
 
    return 0; 
} 
 
/* ============================= */ 
/* SELECT VEHICLE FROM HEAP      */ 
/* ============================= */ 
 
int getPriorityVehicle( 
    int currentTime, 
    struct Vehicle *selected, 
    int *lost 
) { 
 
    while (!isHeapEmpty()) { 
 
        *selected = extractMin(); 
 
        selected->waitTime = 
            currentTime - selected->arrivalTime; 
 
        if (selected->waitTime > MAX_PATIENCE) { 
 
            printf( 
                "V%d waited too long and left.\n", 
                selected->id 
            ); 
 
            (*lost)++; 
        } 
        else { 
            return 1; 
        } 
    } 
 
    return 0; 
} 
 
/* ============================= */ 
/* RUN COMMON SIMULATION         */ 
/* ============================= */ 
 
void compareStrategies(); 
 
void runStrategy(int strategy) { 
 
    struct Vehicle waitingQueue[MAX_QUEUE]; 
 
    int waitingCount = 0; 
 
    int currentTime = 0; 
    int nextArrivalIndex = 0; 
 
    int served = 0; 
    int lost = 0; 
 
    int totalWait = 0; 
    int maxWait = 0; 
 
    int totalChargeTime = 0; 
 
    int useHeap = 0; 
 
/* ------------------------- */ 
/* CALCULATE CHARGE DURATIONS */ 
/* ------------------------- */ 
 
for (int i = 0; i < MAX_VEHICLES; i++) { 
 
    vehicles[i].chargeDuration = 
        calculateChargeDuration( 
            vehicles[i].currentBattery, 
            vehicles[i].targetBattery, 
            c1.power 
        ); 
} 
 
/* ------------------------- */ 
/* RESET SYSTEM               */ 
/* ------------------------- */ 
 
resetChargers(); 
resetHeap(); 
 
    /* ------------------------- */ 
    /* SELECT DATA STRUCTURE     */ 
    /* ------------------------- */ 
 
    if (strategy == 1) { 
        useHeap = 0; 
    } 
    else { 
        useHeap = 1; 
 
        /* 
            1 = Shortest Charge 
            2 = Lowest Battery 
            3 = Emergency + Aging 
        */ 
 
        setHeapRule(strategy - 1); 
    } 
 
    printStrategyName(strategy); 
 
    /* ========================= */ 
    /* MAIN EVENT LOOP            */ 
    /* ========================= */ 
 
    while ( 
        nextArrivalIndex < MAX_VEHICLES || 
        waitingCount > 0 || 
        !isHeapEmpty() || 
        c1.busy == 1 || 
        c2.busy == 1 
    ) { 
 
        /* --------------------- */ 
        /* ADD ARRIVING VEHICLES  */ 
        /* --------------------- */ 
 
        while ( 
            nextArrivalIndex < MAX_VEHICLES && 
            vehicles[nextArrivalIndex].arrivalTime 
                <= currentTime 
        ) { 
 
            printf( 
                "\nTime %d: V%d arrived", 
                currentTime, 
                vehicles[nextArrivalIndex].id 
            ); 
 
            if ( 
                vehicles[nextArrivalIndex].emergency == 1 
            ) { 
                printf(" [EMERGENCY]"); 
            } 
 
            printf("\n"); 
 
            if (useHeap == 1) { 
 
                insertHeap( 
                    vehicles[nextArrivalIndex] 
                ); 
            } 
            else { 
 
                enqueue( 
                    waitingQueue, 
                    &waitingCount, 
                    vehicles[nextArrivalIndex] 
                ); 
            } 
 
            nextArrivalIndex++; 
        } 
 
        /* --------------------- */ 
        /* FINISH CHARGING       */ 
        /* --------------------- */ 
 
        finishCharging(currentTime); 
 
        /* ===================== */ 
        /* CHARGER 1             */ 
        /* ===================== */ 
 
        if (c1.busy == 0) { 
 
            struct Vehicle nextVehicle; 
            int found = 0; 
 
            if (useHeap == 1) { 
 
                found = 
                    getPriorityVehicle( 
                        currentTime, 
                        &nextVehicle, 
                        &lost 
                    ); 
            } 
            else { 
 
                found = 
                    getFIFO( 
                        waitingQueue, 
                        &waitingCount, 
                        currentTime, 
                        &nextVehicle, 
                        &lost 
                    ); 
            } 
 
            if (found == 1) { 
 
                nextVehicle.startTime = 
                    currentTime; 
 
                nextVehicle.waitTime = 
                    currentTime - 
                    nextVehicle.arrivalTime; 
 
                printf( 
                    "V%d waited %d time units\n", 
                    nextVehicle.id, 
                    nextVehicle.waitTime 
                ); 
 
                assignVehicle( 
                    &c1, 
                    nextVehicle, 
                    currentTime 
                ); 
 
                served++; 
 
                totalWait += 
                    nextVehicle.waitTime; 
 
                if ( 
                    nextVehicle.waitTime > 
                    maxWait 
                ) { 
                    maxWait = 
                        nextVehicle.waitTime; 
                } 
 
                totalChargeTime += 
                    nextVehicle.chargeDuration; 
            } 
        } 
 
        /* ===================== */ 
        /* CHARGER 2             */ 
        /* ===================== */ 
 
        if (c2.busy == 0) { 
 
            struct Vehicle nextVehicle; 
            int found = 0; 
 
            if (useHeap == 1) { 
 
                found = 
                    getPriorityVehicle( 
                        currentTime, 
                        &nextVehicle, 
                        &lost 
                    ); 
            } 
            else { 
 
                found = 
                    getFIFO( 
                        waitingQueue, 
                        &waitingCount, 
                        currentTime, 
                        &nextVehicle, 
                        &lost 
                    ); 
            } 
 
            if (found == 1) { 
 
                nextVehicle.startTime = 
                    currentTime; 
 
                nextVehicle.waitTime = 
                    currentTime - 
                    nextVehicle.arrivalTime; 
 
                printf( 
                    "V%d waited %d time units\n", 
                    nextVehicle.id, 
                    nextVehicle.waitTime 
                ); 
 
                assignVehicle( 
                    &c2, 
                    nextVehicle, 
                    currentTime 
                ); 
 
                served++; 
 
                totalWait += 
                    nextVehicle.waitTime; 
 
                if ( 
                    nextVehicle.waitTime > 
                    maxWait 
                ) { 
                    maxWait = 
                        nextVehicle.waitTime; 
                } 
 
                totalChargeTime += 
                    nextVehicle.chargeDuration; 
            } 
        } 
 
        /* ===================== */ 
        /* FIND NEXT EVENT        */ 
        /* ===================== */ 
 
        { 
            int nextEventTime; 
            int nextArrivalTime = -1; 
 
            nextEventTime = 
                getNextEventTime(); 
 
            if ( 
                nextArrivalIndex < MAX_VEHICLES 
            ) { 
                nextArrivalTime = 
                    vehicles[nextArrivalIndex] 
                        .arrivalTime; 
            } 
 
            /* 
                Jump directly to the next 
                arrival or charging finish. 
            */ 
 
            if (nextArrivalTime != -1 && 
                nextEventTime != -1) { 
 
                if ( 
                    nextArrivalTime < 
                    nextEventTime 
                ) { 
                    currentTime = 
                        nextArrivalTime; 
                } 
                else { 
                    currentTime = 
                        nextEventTime; 
                } 
            } 
            else if (nextArrivalTime != -1) { 
 
                currentTime = 
                    nextArrivalTime; 
            } 
            else if (nextEventTime != -1) { 
 
                currentTime = 
                    nextEventTime; 
            } 
            else { 
 
                break; 
            } 
        } 
    } 
 
    /* ========================= */ 
    /* RESULTS                   */ 
    /* ========================= */ 
 
    printf("\n"); 
    printf("------------------------------\n"); 
    printf("RESULTS\n"); 
    printf("------------------------------\n"); 
 
    printf( 
        "Vehicles Served   : %d\n", 
        served 
    ); 
 
    printf( 
        "Vehicles Lost     : %d\n", 
        lost 
    ); 
 
    if (served > 0) { 
 
        printf( 
            "Average Wait      : %.2f\n", 
            (float)totalWait / served 
        ); 
    } 
    else { 
 
        printf( 
            "Average Wait      : 0.00\n" 
        ); 
    } 
 
    printf( 
        "Maximum Wait      : %d\n", 
        maxWait 
    ); 
 
if (currentTime > 0) { 
 
    float utilization; 
 
    utilization = 
        ((float)totalChargeTime / 
        (2 * currentTime)) * 100; 
 
    if (utilization > 100) { 
        utilization = 100; 
    } 
 
    printf( 
        "Charger Utilization: %.2f%%\n", 
        utilization 
    ); 
 
    strategyUtilization[strategy] = 
        utilization; 
} 
else { 
 
    printf( 
        "Charger Utilization: 0.00%%\n" 
    ); 
 
    strategyUtilization[strategy] = 
        0.0; 
} 
 
 
/* ----------------------------- */ 
/* SAVE STRATEGY RESULTS         */ 
/* ----------------------------- */ 
 
strategyServed[strategy] = served; 
strategyLost[strategy] = lost; 
 
if (served > 0) { 
 
    strategyAverageWait[strategy] = 
        (float)totalWait / served; 
} 
else { 
 
    strategyAverageWait[strategy] = 
        0.0; 
} 
 
strategyMaxWait[strategy] = 
    maxWait; 
 
    printf("------------------------------\n"); 
 
    printf("\n"); 
 
    printf("==============================\n"); 
 
    if (strategy == 1) { 
        printf("FIFO SIMULATION COMPLETE\n"); 
    } 
    else if (strategy == 2) { 
        printf("SHORTEST CHARGE FIRST COMPLETE\n"); 
    } 
    else if (strategy == 3) { 
        printf("LOWEST BATTERY FIRST COMPLETE\n"); 
    } 
    else { 
        printf("EMERGENCY + AGING COMPLETE\n"); 
    } 
 
    printf("==============================\n"); 
} 
 
/* ============================= */ 
/* OLD FUNCTION NAMES            */ 
/* ============================= */ 
 
void runSimulation() { 
 
    runStrategy(1); 
} 
 
void runShortestFirst() { 
 
    runStrategy(2); 
} 
 
void runLowestBatteryFirst() { 
 
    runStrategy(3); 
} 
 
void runEmergencyAging() { 
 
    runStrategy(4); 
 
}

/* ============================= */ 
/* COMPARE ALL STRATEGIES        */ 
/* ============================= */ 
 
void compareStrategies() { 
 
    printf("\n"); 
    printf("========================================\n"); 
    printf("        STRATEGY COMPARISON\n"); 
    printf("========================================\n"); 
 
    printf( 
        "%-24s %-8s %-7s %-11s %-10s %-13s\n", 
        "Strategy", 
        "Served", 
        "Lost", 
        "Avg Wait", 
        "Max Wait", 
        "Utilization" 
    ); 
 
    printf( 
        "--------------------------------------------------------------------------\n" 
    ); 
 
    printf( 
        "%-24s %-8d %-7d %-11.2f %-10d %.2f%%\n", 
        "FIFO", 
        strategyServed[1], 
        strategyLost[1], 
        strategyAverageWait[1], 
        strategyMaxWait[1], 
        strategyUtilization[1] 
    ); 
 
    printf( 
        "%-24s %-8d %-7d %-11.2f %-10d %.2f%%\n", 
        "Shortest Charge First", 
        strategyServed[2], 
        strategyLost[2], 
        strategyAverageWait[2], 
        strategyMaxWait[2], 
        strategyUtilization[2] 
    ); 
 
    printf( 
        "%-24s %-8d %-7d %-11.2f %-10d %.2f%%\n", 
        "Lowest Battery First", 
        strategyServed[3], 
        strategyLost[3], 
        strategyAverageWait[3], 
        strategyMaxWait[3], 
        strategyUtilization[3] 
    ); 
 
    printf( 
        "%-24s %-8d %-7d %-11.2f %-10d %.2f%%\n", 
        "Emergency + Aging", 
        strategyServed[4], 
        strategyLost[4], 
        strategyAverageWait[4], 
        strategyMaxWait[4], 
        strategyUtilization[4] 
    ); 
 
    printf( 
        "========================================\n" 
    ); 
}