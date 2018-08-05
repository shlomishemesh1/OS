#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int g_size = 100;

int main(void)
{
	int child_status = 0;
	pid_t child_pid = 0;
	int l_size = 100;
	g_size = 20;
	l_size = 20;
	
	printf("before fork: g_size = %d & l_size = %d\n", g_size, l_size);
	
	child_pid = fork();
	
	if (0 != child_pid)
	{
		printf("parent: ppid = %d, pid = %d, g_size = %d, l_size = %d\n", getppid(), getpid(), g_size, l_size);
	}
	else
	{
		printf("child: ppid = %d, pid = %d, g_size = %d, l_size = %d\n", getppid(), getpid(), g_size, l_size);
		child_status = 150;
	}
	
	waitpid(child_pid, &child_status, 0);
	
	if (WIFEXITED(child_status))//returns true if the child terminated normally, that is, by calling exit(3) 
								//or _exit(2), or by returning from main().

	{
		printf ("the child process exited normally, with exit code %d\n", 
												WEXITSTATUS(child_status));
	}
											
	return (EXIT_SUCCESS);
}
