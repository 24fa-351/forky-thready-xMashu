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

void pattern1(int things)
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
    }

    while(wait(NULL) > 0);
    fprintf(file, "\n");
    fclose(file);

}

void pattern2(int things)
{
    FILE *file = fopen("results.txt", "a");
    fprintf(file, "Pattern 2:\n");
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
        else
        {
            wait(NULL); 
        }
    }

    fprintf(file, "\n");
    fclose(file);
}


int main(int argc, char *argv[]) {

   int things = 0;
   int patternNumber = 0;

   sscanf(argv[1], "%d", &things); 
   sscanf(argv[2], "%d", &patternNumber);

    srand(time(NULL));

    if (patternNumber == 1)
    {
        pattern1(things);
    }
    else if(patternNumber == 2)
    {
        pattern2(things);
    }

return 0;
}