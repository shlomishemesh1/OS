#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void Handler(int signal_num);

/*	An integer type which can be accessed as an atomic entity even in the presence
	of asynchronous interrupts made by signals.
 */
volatile sig_atomic_t flag = 0;

int main(void)
{
	struct sigaction act;
	int counter = 0;
	
	act.sa_handler = Handler;
	
	sigaction(SIGINT, &act, NULL);
	
	while (!flag)
	{
		printf("%d\n", counter++);
		sleep(2);
	}
	
	printf("\nexit normally\n");
	return (EXIT_SUCCESS);
}

void Handler(int signal_num)
{
	flag = !flag;
	
	return;
}
