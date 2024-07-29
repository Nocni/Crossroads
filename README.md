# Crossroads Simulation in C

## Overview
This project simulates a crossroads using threads to represent cars and semaphores to control their passage through the intersection. The simulation aims to optimize the traffic light timings to minimize the average waiting time for cars.

## Features
**Multi-threaded Simulation**: Each car direction is managed by a separate thread.
**Semaphore-Based Synchronization**: Ensures that cars from different directions do not enter the intersection simultaneously.
**Dynamic Traffic Light Control**: Adjusts the green light duration based on the average waiting times to find the optimal light cycle.

## Code Description
**Main Components**
**Semaphore Initialization**:

Semaphores for each direction (semafor_a, semafor_b, semafor_c, semafor_d) and for the intersection points (raskrsnica_a, raskrsnica_b, raskrsnica_c, raskrsnica_d).

**Car Thread Functions**:

vozi_a, vozi_b, vozi_c, vozi_d: These functions simulate the cars' behavior in each direction, including waiting for the green light, passing through the intersection, and tracking the waiting time.

**Traffic Light Control**:

zeleno: Manages the traffic lights, switching between green lights for AC and BD directions. It adjusts the green light duration dynamically based on the calculated waiting times.

**Simulation Start**:

start: Initializes the car threads for each direction.

**Main Function**:

Initializes semaphores and starts the threads for the traffic light control and car simulation.
