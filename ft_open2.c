#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<aio.h>

char *get_next_line(int fd)
{
    static char buffer[1024];
    static int pos = 0, bytes = 0;
    static char line[1024];

    int i = 0;
    while (1) {
        if (pos >= bytes) {
            bytes = read(fd, buffer, sizeof(buffer));
            pos = 0;
            if (bytes <= 0) return NULL;
        }
        line[i++] = buffer[pos++];
        if (line[i-1] == '\n') break;
    }
    line[i] = '\0';
    return line;
}

int ft_strlen(char *s)
{
	int i = 0;
	while(s[i])
		i++;
	return(i);
}

void ft_putstr(char *s)
{
    if (!s) return;
    write(1, s, ft_strlen(s));
}

int ft_open(const char *file, char* const argv[], char type)
{
	int pip[2];
	pid_t pid;

	if(!file || !argv || (type != 'r' && type != 'w'))
		return(-1);

	if(pipe(pip) == -1)
		return(-1);

	pid = fork();

	if(pid < 0)
	{
		close(pip[0]);
		close(pip[1]);
		return(-1);
	}

	if(pid == 0)//child
	{
		if(type == 'r')
		{
			close(pip[0]);
			if(dup2(pip[1], STDOUT_FILENO) == -1)
				exit(1);
			close(pip[1]);
		}
		else
		{
			close(pip[1]);
			if(dup2(pip[0], STDIN_FILENO) == -1)
				exit(1);
			close(pip[0]);
		}

		execvp(file, argv);
		perror("execvp");
		exit(1);

	}

	//parent
	if(type == 'r')
	{
		close(pip[1]);
		return(pip[0]);
	}
	else
	{
		close(pip[0]);
		return(pip[1]);
	}

}

int main (void)
{

	int fd = ft_open("ls", (char* const[]) {"ls", NULL},'r');
	char*line;
	while((line = get_next_line(fd)))
		ft_putstr(line);

	close(fd);
}
