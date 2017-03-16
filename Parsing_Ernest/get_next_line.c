/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcrivenc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/04 14:30:26 by vcrivenc          #+#    #+#             */
/*   Updated: 2016/11/13 13:47:36 by vcrivenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../corewar.h"

void	increaze_tab(char **str, int i, int *size_str)
{
	char	*temp;
	int		j;

	j = 0;
	temp = (char *)malloc(sizeof(char) * (*size_str) * 2);
	while (j < i)
	{
		temp[j] = (*str)[j];
		j++;
	}
	if (*str)
		free(*str);
	*size_str *= 2;
	*str = temp;
}

void	add_char(char c, int i, char **line, int *size_str)
{
	if (i + 1 > (*size_str))
		increaze_tab(line, i, size_str);
	(*line)[i] = c;
}

char	get_next_char(int fd)
{
	static char buf[BUFF_SIZE];
	int			nr;
	char		c;

	ST;
	PD;
	c = 0;
	if (old_fd != fd)
	{
		start = 0;
		old_fd = fd;
		while (start < BUFF_SIZE)
			buf[start++] = 0;
		start = 0;
	}
	if (start == 0)
		if ((nr = read(fd, buf, BUFF_SIZE)) == -1 || nr == 0)
			return (nr);
	if (start < BUFF_SIZE && buf[start])
		c = buf[start++];
	if (start == BUFF_SIZE || buf[start] == '\0')
		old_fd = -1;
	return (c);
}

int		get_next_line(const int fd, char **line)
{
	int		i;
	int		size_str;
	char	c;

	i = 0;
	++g_line;
	size_str = BUFF_SIZE + 1;
	if (fd < 0 || !line || BUFF_SIZE < 0)
		return (-1);
	*line = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
	while ((c = get_next_char(fd)) != '\n' && c != -1 && c != 0)
		add_char(c, i++, line, &size_str);
	(*line)[i] = '\0';
	if (c == '\n' && i == 0)
		return (1);
	if ((*line)[0] != '\0')
		return (1);
	return (c);
}
