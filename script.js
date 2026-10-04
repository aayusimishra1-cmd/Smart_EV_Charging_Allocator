// ========================================
// SMART EV CHARGING ALLOCATOR
// ========================================

const PATIENCE = 10;
const POWER = 50;

const vehiclesData = [
    [1, 0, 20, false],
    [2, 0, 40, false],
    [3, 1, 30, false],
    [4, 2, 50, true],
    [5, 2, 60, false]
];


// ========================================
// CREATE VEHICLES
// ========================================

function makeVehicles() {

    return vehiclesData.map(v => {

        const duration = ((80 - v[2]) * 10) / POWER;

        return {
            id: v[0],
            arrival: v[1],
            battery: v[2],
            target: 80,
            emergency: v[3],
            duration: duration,
            status: "Waiting",
            wait: 0,
            charger: 0
        };

    });

}


// ========================================
// PRIORITY
// ========================================

function priority(a, b, strategy) {

    if (strategy === "fifo") {
        return a.arrival - b.arrival || a.id - b.id;
    }

    if (strategy === "shortest") {
        return a.duration - b.duration || a.id - b.id;
    }

    if (strategy === "battery") {
        return a.battery - b.battery ||
               a.arrival - b.arrival ||
               a.id - b.id;
    }

    // Emergency + Aging
    return Number(b.emergency) - Number(a.emergency) ||
           a.arrival - b.arrival ||
           a.id - b.id;
}


// ========================================
// RUN SIMULATION
// ========================================

function simulate(strategy) {

    const v = makeVehicles();

    const chargers = [
        {
            id: 1,
            busy: false,
            finish: 0,
            vehicle: 0,
            lastVehicle: 0,
            start: 0,
            end: 0
        },
        {
            id: 2,
            busy: false,
            finish: 0,
            vehicle: 0,
            lastVehicle: 0,
            start: 0,
            end: 0
        }
    ];

    let waiting = [];
    let time = 0;

    let served = 0;
    let lost = 0;
    let totalWait = 0;
    let maxWait = 0;
    let chargeTime = 0;

    let events = [];

    while (true) {

        // -------------------------------
        // ARRIVALS
        // -------------------------------

        v.forEach(x => {

            if (
                x.status === "Waiting" &&
                x.arrival <= time &&
                !waiting.includes(x)
            ) {

                waiting.push(x);

                events.push([
                    "arrival",
                    `Time ${time}: V${x.id} arrived`
                ]);

            }

        });


        // -------------------------------
        // FINISH CHARGING
        // -------------------------------

        chargers.forEach(c => {

            if (c.busy && c.finish === time) {

                const x = v.find(
                    y => y.id === c.vehicle
                );

                x.status = "Completed";

                events.push([
                    "finish",
                    `Time ${time}: V${x.id} finished on Charger ${c.id}`
                ]);

                c.busy = false;
                c.vehicle = 0;

            }

        });


        // -------------------------------
        // PATIENCE CHECK
        // -------------------------------

        waiting = waiting.filter(x => {

            const wait = time - x.arrival;

            if (wait > PATIENCE) {

                x.status = "Lost";
                x.wait = wait;

                lost++;

                events.push([
                    "lost-log",
                    `Time ${time}: V${x.id} waited too long and left`
                ]);

                return false;
            }

            return true;

        });


        // -------------------------------
        // ALLOCATE CHARGERS
        // -------------------------------

        chargers.forEach(c => {

            if (
                c.busy ||
                waiting.length === 0
            ) {
                return;
            }

            waiting.sort(
                (a, b) =>
                    priority(a, b, strategy)
            );

            const x = waiting.shift();

            x.status = "Charging";
            x.wait = time - x.arrival;
            x.charger = c.id;

            c.busy = true;
            c.vehicle = x.id;
            c.finish = time + x.duration;

            // Save charger history
            c.lastVehicle = x.id;
            c.start = time;
            c.end = c.finish;

            served++;

            totalWait += x.wait;

            maxWait =
                Math.max(maxWait, x.wait);

            chargeTime += x.duration;

            events.push([
                "start",
                `Time ${time}: V${x.id} started on Charger ${c.id}`
            ]);

        });


        // -------------------------------
        // CHECK COMPLETION
        // -------------------------------

        const active =
            chargers.some(c => c.busy);

        const future =
            v.some(
                x =>
                    x.status === "Waiting" &&
                    x.arrival > time
            );

        if (
            !active &&
            !future &&
            waiting.length === 0
        ) {
            break;
        }


        // -------------------------------
        // NEXT EVENT
        // -------------------------------

        let next = Infinity;

        v.forEach(x => {

            if (
                x.status === "Waiting" &&
                x.arrival > time
            ) {
                next =
                    Math.min(next, x.arrival);
            }

        });

        chargers.forEach(c => {

            if (c.busy) {
                next =
                    Math.min(next, c.finish);
            }

        });

        if (next === Infinity) {
            break;
        }

        time = next;
    }


    // -------------------------------
    // UTILIZATION
    // -------------------------------

    const utilization =
        time > 0
            ? Math.min(
                100,
                chargeTime /
                (2 * time) *
                100
            )
            : 0;


    return {
        vehicles: v,
        chargers: chargers,
        served: served,
        lost: lost,
        avg: served
            ? totalWait / served
            : 0,
        max: maxWait,
        utilization: utilization,
        events: events,
        totalTime: time
    };

}


// ========================================
// DISPLAY VEHICLES
// ========================================

function showVehicles(result) {

    const box =
        document.getElementById("vehicles");

    box.innerHTML = "";

    result.vehicles.forEach(x => {

        let cls = x.status.toLowerCase();

        if (
            x.emergency &&
            x.status === "Waiting"
        ) {
            cls = "emergency";
        }

        box.innerHTML += `

            <div class="vehicle">

                <h3>Vehicle V${x.id}</h3>

                <p>
                    Arrival: ${x.arrival}
                </p>

                <p>
                    Battery:
                    ${x.battery}% → ${x.target}%
                </p>

                <p>
                    Duration:
                    ${x.duration} units
                </p>

                ${
                    x.wait > 0
                    ? `<p>Wait: ${x.wait} units</p>`
                    : ""
                }

                ${
                    x.charger
                    ? `<p>Charger: ${x.charger}</p>`
                    : ""
                }

                <span class="status ${cls}">
                    ${x.status}
                </span>

            </div>

        `;

    });

}


// ========================================
// UPDATE METRICS
// ========================================

function updateMetrics(r) {

    document.getElementById("served")
        .textContent = r.served;

    document.getElementById("lost")
        .textContent = r.lost;

    document.getElementById("avg")
        .textContent = r.avg.toFixed(2);

    document.getElementById("max")
        .textContent = r.max;

    document.getElementById("util")
        .textContent =
        r.utilization.toFixed(2) + "%";
}


// ========================================
// CHARGER STATUS
// ========================================

function updateChargers(result) {

    result.chargers.forEach(c => {

        const element =
            document.getElementById(
                "c" + c.id
            );

        if (!c.lastVehicle) {

            element.innerHTML =
                "Available";

            return;
        }

        element.innerHTML = `
            Available<br>
            <small>
                Last used by V${c.lastVehicle}<br>
                Session: ${c.start} → ${c.end}
            </small>
        `;

    });

}


// ========================================
// EVENT LOG
// ========================================

function showLog(events) {

    const log =
        document.getElementById("log");

    log.innerHTML = "";

    events.forEach(e => {

        log.innerHTML +=
            `<div class="${e[0]}">${e[1]}</div>`;

    });

}


// ========================================
// STRATEGY COMPARISON
// ========================================

function comparison() {

    const strategies = [
        ["fifo", "FIFO"],
        ["shortest", "Shortest Charge First"],
        ["battery", "Lowest Battery First"],
        ["emergency", "Emergency + Aging"]
    ];

    const table =
        document.getElementById(
            "comparison"
        );

    table.innerHTML = "";

    strategies.forEach(s => {

        const r = simulate(s[0]);

        table.innerHTML += `

            <tr>

                <td>${s[1]}</td>
                <td>${r.served}</td>
                <td>${r.lost}</td>
                <td>${r.avg.toFixed(2)}</td>
                <td>${r.max}</td>
                <td>${r.utilization.toFixed(2)}%</td>

            </tr>

        `;

    });

}


// ========================================
// RUN SIMULATION BUTTON
// ========================================

function runSimulation() {

    const strategy =
        document.getElementById(
            "strategy"
        ).value;

    const result =
        simulate(strategy);

    showVehicles(result);

    updateMetrics(result);

    updateChargers(result);

    showLog(result.events);

    comparison();

}


// ========================================
// INITIAL DISPLAY
// ========================================

function initialDisplay() {

    showVehicles({
        vehicles: makeVehicles()
    });

}

initialDisplay();