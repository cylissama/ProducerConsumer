# Producer Consumer CS425
 A solution to the producer-consumer problem using pthread library

#Problem Description
- multiple consumer/producer threads
- bounded buffer shared with consumer and producer
- integer shared only among the producers
- each producer thread repeatedly inserts the current value of the integer var to the bounded buffer if it is not full, then increments the integer var by one
- each consumer thread repeatedly picks up an integer value from the bounded buffer and prints the value such that integer values are printed consecutively, i.e., 0, 1, 2, ...up to a certain integer (specified by a user). To have a better idea of the behavior of consumers, the consumer ID should be displayed along with the buffered integers, showing which consumer thread prints which buffered integer. So the output looks like the following:
-     0, 1
      1, 1
      2, 1
      3, 2
      4, 2
      5, 3
  where the first column is a buffered integer, and the second column is a user-friendly consumer ID. You need to apply the following principles and concepts to analyze the producers-consumers prob-
lem and design.

- minimize number of shared variables
- critical section problem: shared variables should be placed into a critical section, and mutual exclusion is applied to threads that are sharing the variable
- busy waiting and blocking wait: acquiring a spinlock can lead to busy waiting, while calling a blocking wait operation of a semaphore won’t
- overflow issue and underflow issue: a producer should avoid overflow, while a consumer should avoid underflow
- only non-shared variables of the main thread are needed to be passed in to a producer or a consumer thread
- This project also includes an experimental assessment of the performance of busy waiting and block- ing wait in varied settings, including buffer size, the number of producer threads, the number of consumer threads, the largest number to print. All these settings are input in the command line and stored in some variables. So the execution of your code should follow the user input format below:
    > ./a.out <buffer_size> <num_producers> <num_consumers> <upper_limit>
