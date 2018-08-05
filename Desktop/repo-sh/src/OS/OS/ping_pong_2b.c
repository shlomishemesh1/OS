#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "utils.h"

volatile int g_flag = 0;

void Handler(int signal_num);

int main(int argc, char *argv[])
{
	pid_t ping = atoi(argv[1]);
	int child_status = 0;
    struct sigaction act = {0};

    act.sa_handler = Handler;
    sigaction(SIGUSR2, &act, NULL);
   
    while (1)
    {  
        sleep(1);
        if (0 != kill(ping, SIGUSR1))
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

	return (EXIT_SUCCESS);
}

void Handler(int signal_num)
{
	g_flag = 2;
	
	return;
}

