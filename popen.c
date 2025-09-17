#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>


FILE* ft_popen(char* command, char* mode)
{
	FILE *fp;
	int pip[2];
	pid_t pid;

	if(mode[0] != 'r' && mode[0] != 'w')
		return (NULL);

	if(pipe(pip) == -1)
	{
		perror("pipe");
		return NULL;
	}

	pid = fork();
	if(pid < 0) // fork failed
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		return NULL;
	}

	if(pid == 0) // child
	{
		if(mode[0] == 'r')
		{
			close(pip[0]);
			if(dup2(pip[1], STDOUT_FILENO) == -1)
				exit(1);
			close(pip[1]);
		}
		if(mode[0] == 'w')
		{
			close(pip[1]);
			if(dup2(pip[0], STDIN_FILENO) == -1)
				exit(1);
			close(pip[0]);
		}
		execl("/bin/sh", "sh", "-c", command, (char *)NULL);
		exit(1);
	}

	if(mode[0] == 'r')
	{
		close(pip[1]);
		fp = fdopen(pip[0],"r" );
	}
	if(mode[0] == 'w')
	{
		close(pip[0]);
		fp = fdopen(pip[1],"w" );
	}

	return (fp);

}


int main(void)
{
	FILE *fp;
	char buf[256];

	fp = ft_popen("ls", "r");
	if (fp == NULL) {
		perror("popen");
		return 1;
	}

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		printf("%s", buf);
	}

	pclose(fp);
	return 0;
}
