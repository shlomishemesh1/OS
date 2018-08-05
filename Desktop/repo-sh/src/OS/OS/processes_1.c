#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(void)
{
	int counter = 0;
	
	printf("%d\n", getpid());
	
	while (counter < 7)
	{
		printf("%d\n", counter++);
		sleep(2);
	}
	
	printf("\nexit normally\n");
	
	return (EXIT_SUCCESS);
}
