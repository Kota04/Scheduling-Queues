# Queue-Based Packet Switch Simulator

This C++ program simulates a queue-based packet switch using various queuing algorithms such as INQ (Input Queued), KOUQ (Knockout Unicast Queue), and iSLIP (Iterative Selfish Least-Loaded Matching). It generates traffic, schedules packet transmission, and computes performance metrics such as average packet delay, standard deviation of packet delay, and average link utilization.

## Table of Contents
1. [Introduction](#introduction)
2. [Requirements](#requirements)
3. [Usage](#usage)
4. [Performance Metrics](#performance-metrics)
5. [Contributing](#contributing)
6. [License](#license)

## Introduction 
The program simulates the operation of a packet switch with N input ports, each having an input buffer, and N output ports, each having an output buffer. It implements three queuing algorithms: INQ, KOUQ, and iSLIP. Traffic is generated based on a packet generation probability (p), and packets are scheduled for transmission based on the selected queuing algorithm.

## Requirements 
- C++ compiler supporting C++11 or later
- Standard Template Library (STL)
- Random Number Generator

## Usage 
Compile the program using a C++ compiler, providing the necessary command-line arguments:
```
./program_name N B p queue_type K output_file T
```
- `N`: Number of input and output ports
- `B`: Buffer size for input and output buffers
- `p`: Packet generation probability
- `queue_type`: Type of queuing algorithm (INQ, KOUQ, iSLIP)
- `K`: Knockout value (applicable for KOUQ)
- `output_file`: Name of the output file to store simulation results
- `T`: Maximum time slot for simulation

Example:
```
./program_name 8 10 0.5 KOUQ 3 output.txt 1000
```

## Performance Metrics 
The program computes the following performance metrics:
- Average Packet Delay
- Standard Deviation of Packet Delay
- Average Link Utilization

## Assumptions Taken
- The output buffer (in case of KOUQ) can only accommodate up to B packets.
- The total size of input buffer, including all virtual output queues is B (in case of ISLIP).

## Note
- The Report contains the comparison of three algorithms