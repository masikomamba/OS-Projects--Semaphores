// Masiko Mamba

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHM_KEY 0x1234 // Shared memory key

int main() {
    int shmid;
    int* total = 0;

    // Create shared memory segment
    shmid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0777);
    if (shmid == -1) { // Check for error in shared memory creation
        perror("shared memory");
        return 1;
    }

    // Attach shared memory segment to parent process
    total = (int*)shmat(shmid, NULL, 0);
    if (total == (int*)-1) { // Check for error in memory attachment
        perror("Shared memory attach");
        return 1;
    }

    // Create and initialize a named semaphore for process synchronization
    sem_t *mutex = sem_open("/sem_mutex", O_CREAT, 0644, 1);
    if (mutex == SEM_FAILED) { // Check for error in semaphore creation
        perror("sem_open");
        return 1;
    }

    // Fork to create 4 child processes
    for (int i = 0; i < 4; i++) {
        pid_t pid = fork(); // Create a child process
        if (pid == 0) { // Child process code
            int increments[] = { 100000, 200000, 300000, 500000 };
            int increment_value = increments[i]; // Set increment value for this process
            int process_number = i; // Process number for display

            // Loop to increment the shared total by the defined amount
            for (int j = 0; j < increment_value; ++j) {
                sem_wait(mutex); // Lock the semaphore to ensure exclusive access to shared memory
                (*total)++; // Increment the shared variable
                sem_post(mutex); // Release the semaphore
            }

            // Display the final counter value for this process
            sem_wait(mutex); // Lock before printing to prevent concurrent access issues
            printf("From Process %d: counter = %d.\n", process_number + 1, *total);
            sem_post(mutex); // Release the semaphore

            return 1; // Child process exits after completing its task
        }
    }

    // Detach the shared memory from the parent process
    if (shmdt(total) == -1) { // Check for error in detaching memory
        perror("shmdt");
        return 1;
    }

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) { // Check for error in removing memory
        perror("shmctl");
        return 1;
    }

    // Parent process waits for all child processes to complete
    pid_t childProcess_pid = 0;
    for (int i = 0; i < 4; i++) {
        childProcess_pid = wait(NULL); // Wait for each child process to exit
    }

    printf("\n");
    // Print message each time a child process exits
    for (int i = 0; i < 4; i++) {
        printf("Child with ID: %d has just exited.\n", childProcess_pid + i);
    }

    printf("\nEnd of Program\n");

    // Close and unlink the semaphore to release system resources
    sem_close(mutex);
    sem_unlink("/sem_mutex");

    return 0;
}
