#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
# include <ctype.h>
#include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

char	*ft_strjoin2(char *s1, char *s2)
{
	char		*c;
	size_t		i;
	size_t		j;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	i = 0;
	if (s1 != NULL)
		i = ft_strlen(s1);
	j = 0;
	if (s2 != NULL)
		j = ft_strlen(s2);
	c = (char *) malloc(sizeof(char) * (i + j + 1));
	if (c == NULL)
		return (NULL);
	c[i + j] = '\0';
	while (j-- > 0)
		c[i + j] = s2[j];
	while (i-- > 0)
		c[i] = s1[i];
	free(s1);
	free(s2);
	return (c);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	size_t			j;
	char			*c;

	if (s == NULL)
		return (NULL);
	if (s[0] == '\0')
		return (NULL);
	if (ft_strlen(s) < start)
		len = 0;
	if (ft_strlen(s) < len)
		len = ft_strlen(s);
	if (ft_strlen(s) - start < len)
		len = ft_strlen(s) - start;
	c = malloc(sizeof(char) * (len + 1));
	if (c == NULL)
		return (NULL);
	i = start;
	j = 0;
	while (j < len && s[i])
		c[j++] = s[i++];
	c[j] = '\0';
	return (c);
}


int	chr_start(char *s, char c)
{
	int	i;

	if (s == NULL)
		return (-2);
	i = 0;
	while (s[i] && s[i] != c)
		i++;
	if (s[i] == c)
		return (i + 1);
	return (-1);
}

int	chr_rest(char *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] && s[i] != c)
		i++;
	if (s[i] == c)
	{
		while (s[i] != '\0')
		{
			i++;
			j++;
		}
		return (j);
	}
	else
		return (-1);
}

static char	*ft_buffer(int fd, int *error)
{
	char	*buf_raw;
	int		read_out;
	size_t	i;

	buf_raw = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buf_raw == NULL)
		return (NULL);
	i = 0;
	while (i < (sizeof(char) * (BUFFER_SIZE + 1)))
		buf_raw[i++] = 0;
	read_out = read(fd, buf_raw, BUFFER_SIZE);
	if (read_out <= 0)
	{
		*error = read_out;
		free(buf_raw);
		buf_raw = NULL;
	}
	return (buf_raw);
}

static char	*sequence(int fd, int *error)
{
	char	*buf_short;
	char	*buf_long;
	char	*seq;
	int		i;

	seq = NULL;
	i = -1;
	while (i == -1)
	{
		buf_short = ft_buffer(fd, error);
		if (*error != 0)
		{
			free(seq);
			return (NULL);
		}
		if (buf_short == NULL)
			return (seq);
		buf_long = ft_strjoin2(seq, buf_short);
		seq = buf_long;
		i = chr_start(seq, '\n');
	}
	return (seq);
}

char	*get_next_line(int fd)
{
	char		*seq_s;
	char		*seq_l;
	char		*line;
	static char	*rest;
	int			error;

	error = 0;
	if (BUFFER_SIZE <= 0 || fd < 0 || fd > 10240)
		return (NULL);
	seq_s = NULL;
	if (rest == NULL || chr_start(rest, '\n') == -1)
		seq_s = sequence(fd, &error);
	if (error != 0)
	{
		free(rest);
		rest = NULL;
	}
	if (rest == NULL && seq_s == NULL)
		return (NULL);
	seq_l = ft_strjoin2(rest, seq_s);
	line = ft_substr(seq_l, 0, chr_start(seq_l, '\n'));
	rest = ft_substr(seq_l, chr_start(seq_l, '\n'), chr_rest(seq_l, '\n'));
	free(seq_l);
	return (line);
}