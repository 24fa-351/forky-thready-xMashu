#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sleepRand()
{
    int randTime = rand() % 8 + 1;
    sleep(randTime);
}

void childProcess(int currentProcess, int things, FILE *file)
{
    fprintf(file, "Process %d (PID: %d) beginning\n", currentProcess, getpid());
    fflush(file);

    sleepRand();

    if (currentProcess < things)
    {
        fprintf(file, "Process %d (PID: %d) creating Process %d\n", currentProcess, getpid(), currentProcess + 1);
        fflush(file);

        pid_t p = fork();
        if (p == 0)
        {
            childProcess(currentProcess + 1, things, file);
        }
        else if (p > 0)
        {
            wait(NULL);
        }
        else
        {
            perror("Fork failed");
            fclose(file);
            exit(1);
        }
    }

    fprintf(file, "Process %d (PID: %d) exiting\n", currentProcess, getpid());
    fflush(file);
    fclose(file);
    exit(0);
}

void patternOne(int things)
{
    FILE *file = fopen("results.txt", "a");
    fprintf(file, "Pattern 1:\n");
    fflush(file);

    for (int i = 0; i < things; i++)
    {
        pid_t p = fork();
        if (p == 0)
        {
            fprintf(file, "Process %d (PID: %d) beginning\n", i + 1, getpid());
            fflush(file);

            sleepRand();

            fprintf(file, "Process %d (PID: %d) exiting\n", i + 1, getpid());
            fflush(file);
            fclose(file);
            exit(0);
        }
        else if (p > 0)
        {
            fprintf(file, "Process %d (PID: %d) creating Process %d\n", i + 1, getpid(), i + 2);
            fflush(file);
        }
    }

    while (wait(NULL) > 0)
        ;
    fprintf(file, "\n");
    fclose(file);
}

void patternTwo(int things)
{
    FILE *file = fopen("results.txt", "a");
    if (!file)
    {
        perror("Failed to open results.txt");
        exit(1);
    }

    fprintf(file, "Pattern 2:\n");
    fflush(file);

    pid_t p = fork();
    if (p == 0)
    {
        childProcess(1, things, file);
    }
    else if (p > 0)
    {
        wait(NULL);
    }
    else
    {
        perror("Fork failed");
        fclose(file);
        exit(1);
    }

    fprintf(file, "\n");
    fclose(file);
}

void patternThree(int things)
{
    FILE *file = fopen("results.txt", "a");
    if (!file)
    {
        perror("Failed to open results.txt");
        exit(1);
    }

    fprintf(file, "Pattern 3:\n");
    fflush(file);

    fprintf(file, "Process 1 (PID: %d) beginning\n", getpid());
    fflush(file);

    pid_t rightChild = fork();
    if (rightChild == 0) // right is parent so create left
    {
        fprintf(file, "Process %d (PID: %d) creating Process %d\n", 1, getpid(), 2);
        fflush(file);
        sleepRand();

        pid_t leftChild = fork(); // the created left
        if (leftChild == 0)       // is left child so make left subtree
        {
            fprintf(file, "Process %d (PID: %d) beginning\n", things, getpid());
            fflush(file);
            sleepRand();
            patternThree(things - 1);
        }
        else
        {
            fprintf(file, "Process %d (PID: %d) exiting\n", 1, getpid());
            fflush(file);
            wait(NULL);
            exit(0);
        }
    }
    else if (rightChild > 0) // is right child so make right subtree
    {
        fprintf(file, "Process %d (PID: %d) creating Process %d\n", 1, getpid(), things - 1);
        fflush(file);
        sleepRand();
        patternThree(things - 1); // make right subtree
        wait(NULL);
    }
}

int main(int argc, char *argv[])
{
    int things = 0;
    int patternNumber = 0;

    sscanf(argv[1], "%d", &things);
    sscanf(argv[2], "%d", &patternNumber);

    srand(time(NULL));

    if (patternNumber == 1)
    {
        patternOne(things);
    }
    else if (patternNumber == 2)
    {
        patternTwo(things);
    }
    else if (patternNumber == 3)
    {
        patternThree(things);
    }

    return 0;
}
