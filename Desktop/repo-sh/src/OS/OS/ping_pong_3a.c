#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "utils.h"

volatile int g_flag = 0;
volatile int exit_flag = 0;

void Handler(int signal_num);
void Handler2(int signal_num);

int main(void)
{
	pid_t child = 0;
	int child_status = 0;
    struct sigaction act = {0};
    struct sigaction act2 = {0};
    char *args[] = {"pingpong3b.out", NULL};
    int i = 0;
    child = fork();
    if (0 > child)
    {
        PRINT_ERROR("fork error");
        return (EXIT_FAILURE);
    }
    
    if (0 == child)
    {
        execvp(args[0], args);
        /* If got here an error happened */
        PRINT_ERROR("execvp error");
        return (EXIT_FAILURE);
    }
    
    act.sa_handler = Handler;
    sigaction(SIGUSR1, &act, NULL);
   
    act2.sa_handler = Handler2;
    sigaction(SIGINT, &act2, NULL);//interrupt from keyboard

    while (!exit_flag)
    {  
        pause();
        if (1 == g_flag)
        {
            puts("ping");
            g_flag = 0;
        }
        else
        {
            PRINT_ERROR("not the right signal");
            return (1);
        }

        sleep(1);
        if (0 != kill(child, SIGUSR2))
        {
            PRINT_ERROR("kill error");
        }
    }

    if (0 > wait(&child_status))
    {
        PRINT_ERROR("wait error");
        return (EXIT_FAILURE);
    }

    puts("pingpong3a.out exit normally");
	return (EXIT_SUCCESS);
}

void Handler(int signal_num)
{
	g_flag = 1;

	return;
}

void Handler2(int signal_num)
{
	exit_flag = 1;

	return;
}