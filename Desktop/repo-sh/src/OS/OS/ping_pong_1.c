#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "utils.h"
/*TODO: comments */
volatile int g_flag = 0;

void Handler(int signal_num);
void Handler2(int signal_num);

int main(void)
{
	pid_t child = 0;
	int child_status = 0;
    struct sigaction act = {0};
    struct sigaction act2 = {0};

    child = fork();
    if (0 > child)
    {
        PRINT_ERROR("fork error");
        return (EXIT_FAILURE);
    }
    
    if (0 == child)
    {
        act2.sa_handler = Handler2;
        sigaction(SIGUSR2, &act2, NULL);
        printf("in child id = %d\n", getpid());
        while (1)
        {  
            sleep(1);
            if (0 != kill(getppid(), SIGUSR1))/* send a signal to a process*/
            {
                PRINT_ERROR("kill error");
            }
            pause();
            if (2 == g_flag)
            {
                puts("pong");
                g_flag = 0;
            }
            else
            {
                PRINT_ERROR("not the right signal");
                return (1);
            }
        }
        return (0);
    }


    printf("in parent id = %d\n", getpid());
    act.sa_handler = Handler;
    sigaction(SIGUSR1, &act, NULL);

    while (1)
    {
        pause();
        if (1 == g_flag)
        {
            puts("ping");
            g_flag = 0;
            sleep(1);
            if (0 != kill(child, SIGUSR2))
            {
                PRINT_ERROR("kill error");
            }
        }
        else
        {
            PRINT_ERROR("handler error");
            return (1);
        }
    }

    if (0 > wait(&child_status))
    {
        PRINT_ERROR("wait error");
        return (EXIT_FAILURE);
    }
    
	return (EXIT_SUCCESS);
}

void Handler(int signal_num)
{
	g_flag = 1;
	
	return;
}

void Handler2(int signal_num)
{
	g_flag = 2;
	
	return;
}
