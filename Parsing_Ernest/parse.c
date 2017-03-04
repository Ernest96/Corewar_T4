/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 17:06:44 by ebitca            #+#    #+#             */
/*   Updated: 2017/03/02 18:15:42 by ebitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../corewar.h"


#define CH_1 char ** words; int8_t seek = 1;
#define CH CH_1 static int8_t flags[3] = {0, 0, 0};

char *g_instr[] = {"", "live", "ld", "st", "add", "sub", "and", "or",
							"xor", "zjmp", "ldi", "sti", "fork", "lld",
							"lldi", "lfork","aff"};

char *g_sabl[] = {"", "010000000", "011100000", "100101000", "100100100",
						  "100100100", "111111100", "111111100", "111111100",
						  "010000000", "111110100", "100111110", "010000000",
						  "011100000", "111110100", "010000000", "100000000"};

void	ft_check_words_1(char **words, int8_t flag)
{
	ft_check_dir(words, flag);
}

void	free_words(char ***words)
{
	int i;

	i = -1;
	while (*(*words + ++i)!= NULL)
		free(*(*words + ++i));
	free(*words);
}

void	ft_check_name_comment(char **s, int fd)
{
	CH;
	if (flags[2])
		ft_comp_error();
	words = ft_split_whitespaces(*s);
	ft_check_words_1(words, 0);
	if (ft_strcmp(words[0], ".name") == 0)
	{
		if (flags[0])
			ft_comp_error();
		free(g_name);
		g_name = ft_strdup(words[1]);
		flags[0] = 1;
		g_offset += ft_strlen(*s) + 1;
		seek = 0;
	}
	else if (ft_strcmp(words[0], ".comment") == 0)
	{
		if (flags[1])
			ft_comp_error();
		free(g_comment);
		g_comment = ft_strdup(words[1]);
		flags[1] = 1;
		g_offset += ft_strlen(*s) + 1;
		seek  = 0;
	}
	free_words(&words);
	if (seek)
	{
		flags[2] = 1;
		lseek(fd, g_offset, SEEK_SET);
	}	
}

int8_t ft_only_space(char *line)
{
	if (*line == 0)
		return (1);
	while (*line)
	{
		if (*line != ' ')
			return (0);
		++line;
	}
	return (1);
}

void	ft_skip_spaces(int fd)
{
	int8_t flag;
	char *line;

	flag = 1;
	while (flag)
	{
		if (get_next_line(fd, &line) == 0)
			return ;
		if (ft_only_space(line))
			g_offset += ft_strlen(line) + 1;
		else
		{
			flag = 0;
			--g_line;
		}
		free(line);
	}
	lseek(fd, g_offset, SEEK_SET);
}

int	main(int argc, char **argv)
{ 
	int	fd;
	char *line;
	char **words;

	if (argc != 2)
		ft_usage();
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		ft_invalid_input();
	ft_check_ext(argv[1]);
	ft_skip_spaces(fd);
	get_next_line(fd, &line);
	ft_check_name_comment(&line, fd);
	ft_skip_spaces(fd);
	get_next_line(fd, &line);
	ft_check_name_comment(&line, fd);
	ft_skip_spaces(fd);
	while (get_next_line(fd, &line))
	{
		words = ft_split_whitespaces(line);
		g_offset += ft_strlen(line) + 1;
		ft_check_words_1(words, 1);
		ft_putstr(line);
		ft_putchar('\n');
		free(line);
		ft_skip_spaces(fd);
	}
	ft_putstr("name: ");
	ft_putstr(g_name);
	ft_putchar('\n');
	ft_putstr("comment: ");
	ft_putstr(g_comment);
	ft_putchar('\n');
}
