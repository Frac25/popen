#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<aio.h>

char	*get_next_line(int fd);
void	ft_putstr(char *s);



int ft_open(const char *file, char* const argv[], char type)
{
	int pip[2];
	pid_t pid;

	if((type != 'r') && (type != 'w'))
	{
		return(-1);
	}

	if (pipe(pip) < 0)
	{
		return(-1);
	}

	pid = fork();
	if(pid == -1)
	{
		return(-1);
	}

	//enfant
	if(pid == 0)
	{
		if(type == 'r')
		{
			close(pip[0]);
			if (dup2(pip[1], 1) <0)
			{
				close(pip[1]);
				exit(1);
			}
			close(pip[1]);
		}
		if(type == 'w')
		{
			close(pip[1]);
			if(dup2(pip[0], 0) < 0)
			{
				close(pip[0]);
				exit(1);
			}
			close(pip[0]);
		}

		execvp(file, argv);
		perror("execvp");
		exit(1);
	}


	//pere
	if(type == 'r')
	{
		close(pip[1]);
		return(pip[0]);
	}
	if(type == 'w')
	{
		close(pip[0]);
		return(pip[1]);
	}
	return(-1);

}

int main (void)
{

	int fd = ft_open("ls", (char* const[]) {"ls", NULL},'r');
	char*line;
	while((line = get_next_line(fd)))
		ft_putstr(line);

	return(0);

}
