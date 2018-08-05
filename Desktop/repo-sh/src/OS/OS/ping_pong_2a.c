#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "utils.h"

volatile int g_flag = 0;
siginfo_t mysiginfo = {0};

void Handler(int sig, siginfo_t *siginfo, void *context);

int main(void)
{
	int child_status = 0;
    struct sigaction act = {0};

    act.sa_sigaction = Handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
   
    while (1)
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
        if (0 != kill(mysiginfo.si_pid, SIGUSR2))
        {
            PRINT_ERROR("kill error");
        }
    }

	return (EXIT_SUCCESS);
}

void Handler(int sig, siginfo_t *siginfo, void *context)
{
	g_flag = 1;
    
    mysiginfo = *siginfo;
    
	return;
}

