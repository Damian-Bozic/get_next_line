/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbozic <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:23:28 by dbozic            #+#    #+#             */
/*   Updated: 2024/04/04 14:24:14 by dbozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// ft_strjoin_mod joins two strings, and returns an allocated string.
// str1 must be allocated or NULL, and is freed and set to NULL.
// str2 can be any string or NULL.
// due to size restrictions variables i and j are initialized as so.
static char	*ft_strjoin_mod(char *str1, char *str2, size_t i, size_t j)
{
	char		*rtn;

	if (!str1)
		str1 = ft_strdup("");
	if (!str1)
		return (0);
	rtn = malloc(ft_strlen(str1) + ft_strlen(str2) + 1);
	if (!rtn)
		free (str1);
	if (!rtn)
		return (0);
	while (str1 && str1[i])
	{
		rtn[i] = str1[i];
		i++;
	}
	while (str2 && str2[j])
	{
		rtn[i + j] = str2[j];
		j++;
	}
	rtn[i + j] = '\0';
	free(str1);
	str1 = NULL;
	return (rtn);
}

// read_line_from_fd takes the main_line string and adds to it iteratively
// until main_line has a '\n' in it. if the main_line already had a '\n'
// then nothing is read and main_line is returned.
// if i didnt run out of space it would be faster to check if leftover has
// '\n' in it only once, then read if the buffer has a '\n' in the while loop.
// that would make reading very long lines alot faster.
static char	*read_line_from_fd(int fd, char *leftover)
{
	char	*buffer;
	int		i;

	i = 1;
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer && leftover)
		free(leftover);
	if (!buffer)
		return (NULL);
	while (i > 0 && !(ft_strchr(leftover, '\n')))
	{
		i = read(fd, buffer, BUFFER_SIZE);
		if (i == -1)
			break ;
		buffer[i] = '\0';
		leftover = ft_strjoin_mod(leftover, buffer, 0, 0);
		if (!leftover)
			i = -1;
	}
	free(buffer);
	if (leftover && i == -1)
		free(leftover);
	if (i == -1)
		return (NULL);
	return (leftover);
}

// set_return takes in main_line and searches it for a '\n'.
// if a '\n' is found then it copies to the '\n' in main_line
// to the return line, then null terminates the result.
// if no '\n' is found then it returns a duplicate of main_line.
static char	*set_return(char *main_line)
{
	char	*return_line;
	size_t	i;

	i = 0;
	if (main_line[0] == '\0')
		return (NULL);
	while (main_line[i] != '\n' && main_line[i])
		i++;
	if (main_line[i] == '\n')
	{
		return_line = malloc(i + 2);
		if (!return_line)
			return (NULL);
		ft_strlcpy(return_line, main_line, i + 2);
	}
	else
		return_line = ft_strdup(main_line);
	if (!return_line)
		return (NULL);
	return (return_line);
}

// set_leftover takes in main_line and searches it for a '\n'.
// if a '\n' is found then it copies the rest of the string
// to leftover.
// if '\n' is the final char, leftover remains NULL.
// if no '\n' is found then leftover remains NULL.
static int	set_leftover(char *main_line, char **leftover)
{
	size_t	i;

	i = 0;
	while (main_line[i] != '\n' && main_line[i])
		i++;
	if (main_line[i] == '\n' && main_line[i + 1] != '\0')
	{
		*leftover = ft_strdup(&main_line[i + 1]);
		if (!*leftover)
			return (0);
	}
	return (1);
}

// get_next_line takes in a fd and returns a line from the given fd.
// due to the way the read() function works it is necessary to have
// a static char* that holds any leftover information that is read
// after a '\n' is found to be used in the start of the next line.
char	*get_next_line(int fd)
{
	static char	*leftover = NULL;
	char		*main_line;
	char		*return_line;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	main_line = read_line_from_fd(fd, leftover);
	leftover = NULL;
	if (!main_line)
		return (NULL);
	return_line = set_return(main_line);
	if (!return_line)
	{
		free(main_line);
		return (NULL);
	}
	if (!set_leftover(main_line, &leftover))
	{
		free(return_line);
		free(main_line);
		return (NULL);
	}
	free(main_line);
	return (return_line);
}

/* int     main(void)
{
	// valid fd test
# include "stdio.h"
# include "fcntl.h"

    char	*line;
    int		fd;
	int		lines_to_read = 0;
	// leave lines_to_read on 0 for automatic
		
    fd = open("test.txt", O_RDONLY);
    if (fd == -1)
	{
    	printf("fd open error\n");
		return (0);
	}
	printf ("File Descriptor = %d\n", fd);
	if (lines_to_read <= 0)
	{
		printf("Reading lines until end of file is found\n");
       	while ((line = get_next_line(fd)) != NULL)
        {
        	printf("-->%s", line);
            free(line);
		}
	}
	else
	{
		printf("Reading %d Lines from given file\n", lines_to_read);
		while(lines_to_read > 0)
		{
			line = get_next_line(fd);
			printf("-->%s", line);
			free(line);
			lines_to_read--;
		}
	}
    close(fd);
} */

/* int     main(void)
{
	// invalid fd test
# include "stdio.h"
    char	*line;
    int		fd = 0;
	int		lines_to_read = 2;
	// try fd = 0, 1, 2 for manual inputs from console
	// leave lines_to_read on 0 for automatic

	printf ("File Descriptor = %d\n", fd);
	if (lines_to_read <= 0)
	{
		printf("Reading lines until end of file is found\n");
       	while ((line = get_next_line(fd)) != NULL)
        {
        	printf("-->%s", line);
            free(line);
		}
	}
	else
	{
		printf("Reading %d Lines from given file\n", lines_to_read);
		while(lines_to_read > 0)
		{
			line = get_next_line(fd);
			printf("-->%s", line);
			free(line);
			lines_to_read--;
		}
	}
} */
