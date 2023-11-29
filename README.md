# Producer Consumer CS425
 A solution to the producer-consumer problem using pthread library

## Description
- multiple consumer/producer threads
- bounded buffer shared with consumer and producer
- integer shared only among the producers
- each producer thread repeatedly inserts the current value of the integer var to the bounded buffer if it is not full, then increments the integer var by one
- each consumer thread repeatedly picks up an integer value from the bounded buffer and prints the value such that integer values are printed consecutively, i.e., 0, 1, 2, ...up to a certain integer (specified by a user). To have a better idea of the behavior of consumers, the consumer ID should be displayed along with the buffered integers, showing which consumer thread prints which buffered integer. So the output looks like the following, where the first column is a buffered integer, and the second column is a user-friendly consumer ID. You need to apply the following   principles and concepts to analyze the producers-consumers problem and design.
```
      0, 1
      1, 1
      2, 1
      3, 2
      4, 2
      5, 3
```

- minimize the number of shared variables
- critical section problem: shared variables should be placed into a critical section, and mutual exclusion is applied to threads that are sharing the variable
- busy waiting and blocking wait: acquiring a spinlock can lead to busy waiting while calling a blocking wait operation of a semaphore won’t
- overflow issue and underflow issue: a producer should avoid overflow, while a consumer should avoid underflow
- only non-shared variables of the main thread need to be passed into a producer or a consumer thread
- This project also includes an experimental assessment of the performance of busy waiting and blocking wait in varied settings, including buffer size, the number of producer threads, the number of consumer threads, and the largest number to print. All these settings are input in the command line and stored in some variables. So the execution of your code should follow the user input format: `./a.out <buffer_size> <num_producers> <num_consumers> <upper_limit>`

## Analysis
- Analyze the relationship between producer-consumer problem and critical section problem. Relationship includes similarity and difference, more importantly, the hierachy.
- Analyze the difference between the producers-consumers problem and the producer-consumer problem.

## Design
- Shared and Non-Shared variables
- Critical Section of producer and how many critical sections should it have
- Critical Section of consumer and how many critical sections should it have
- Semaphores
- Data structure that needs to be passed into the producer and consumer

## Implementation

### Major Implementation
- Implementation matches the design
- Correct initialization of variables
- Correct passing data to producers and consumer threads
- Correct use pthread library functions to create and join threads
- Correct implementation of synchronization contraints on producers and consumers, respec-tively
- Correct user input format, i.e., following the above-specified format for user input
- Correct output format and output values, i.e., no gap, no duplication in the output sequence of integers from 0 to a user-specific number (In the context of process synchronization, the correct output does not always imply the correctness of other categories.)

### Critical section using spinlock
- Replace the declaration of mutex by the declaration of a spinlock
- Replace the initization of mutex by the initialization of a spinlock
- Replace all wait() and signal() (i.e., post()) operations on mutex by lock() and unlock() operations on spinlock

## Experimental assessment of binary semaphore and spinlock
