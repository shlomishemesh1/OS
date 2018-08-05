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
	int child_status = 0;
    struct sigaction act = {0};
    struct sigaction act2 = {0};
    int i = 0;

    act.sa_handler = Handler;
    sigaction(SIGUSR2, &act, NULL);
    act2.sa_handler = Handler2;
    sigaction(SIGINT, &act2, NULL);//KEYBOARD SIGNAL

    while (!exit_flag)
    {  
        sleep(1);
        if (0 != kill(getppid(), SIGUSR1))
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

    puts("pingpong3b.out exit normally");
	return (EXIT_SUCCESS);
}

void Handler(int signal_num)
{
	g_flag = 2;
	
	return;
}

void Handler2(int signal_num)
{
	exit_flag = 1;

	return;
}