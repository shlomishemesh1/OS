#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(void)
{
	printf("parent process\n");
	
	system("ls -l");
	
	printf("parent process\n");
	
	return (EXIT_SUCCESS);
}
