# OS-Projects--Semaphores

## Shared Memory

### Overview

This program shows how shared memory usage allows multiple chld processes to increment a shared variable.
The shared memory is created by the shared memory, child processes are created using the fork() function and uses semaphores to synchronize access to the shared variable by a specified amount and then prints the updated value.
The parent process waits for all children to finish executing and then detaches and cleans up the shared memory and prints the results. 

### Requirements

* This program must be run on a Linux system
* Libraries:
  * POSIX shared memory
  * Semaphores

## Compilation instructions

* Install C compiler if it is not already installed on your device
      - use the following command:
        sudo apt install gcc
* On the terminal, navaigate to the folder containing your code by using the **cd** command to move between directories
* Compile the program using gcc
      - gcc -o ExecutableFileName CodeFileName.c

## Execution Instructions

* Run the executable using the terminal using the following command:
    - ./ExecutableFileName
* The program should print process IDs and results.
  - example output:
![image](https://github.com/user-attachments/assets/b59aa534-b255-46ee-9670-dc615613ca49)
    
## Program explanation

### Shared memory creation:
* We begin by creating the shared memory using **shmget()** function and attach it using **shmat()**. This memory is created by all child processes.
*  **total** is initialized in this shared memory segment

### Semaphore Initialization:
* A semaphore (sem_mutex) is created using **sem_open()** to ensure that the access to the shared variable is controlled (Synchronization) across processes.
### Child process creation
* The parent creates the required number of child processes. The child processes are created using the **fork()** function. Each child process increments the shared variable by a specific value in the **increments** array.
### Child Process execution
* Child process executes the remaining functions. The child processes increment using the array of incrementing values. Each child increments once with a specific value in the increment values array. After the process, each child will print its output as the total value after incrementing.
* Each child process accesses **total** by waiting for and posting to sem_mutex around the critical code. 

### Parent process
* The parent waits for the children to finish executing before continuing using the **wait()** function. The parent process records the process IDs of the children while waiting and prints them once done.

### Shared memory cleanup
* Once all the processes have finished executing, the shared memory is detached and removed using the **shmdt()** function and **shmctl** function to clean up the resources used.
* The semaphore is closed and unlinked usng sem_close() and sem_unlink().
