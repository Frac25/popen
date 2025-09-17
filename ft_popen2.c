#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<aio.h>

char	*get_next_line(int fd);
void	ft_putstr(char *s);



int ft_popen(const char *file, char* const argv[], char type)
{
	int pip[2];
	int pipt;
	pid_t pid;

	pipt = pipe(pip);
	if(pipt < 0)
		return(-1);
	
	pid = fork();
	if(pid < 0)
		return(-1);

	if(pid == 0)
	{
		if(type == 'r')
		{
			close(pip[0]);
			dup2(pip[1], 0);
			execvp(file, argv);
		}
		else if(type == 'w')
		{
			close(pip[1]);
			dup2(pip[0], 1);
			execvp(file, argv);
		}
		else
			return(-1);
		exit(0);		
	}

	if(type == 'r')
	{
		close(pip[1]);
		return(pip[0]);
	}
	else if(type == 'w')
	{
		close(pip[0]);
		return(pip[1]);
	}
	else
		return(-1);
}

int main (void)
{

	int fd = ft_popen("ls", (char* const[]) {"ls", NULL},'r');
	char*line;
	while((line = get_next_line(fd)))
		ft_putstr(line);

	return(0);

}