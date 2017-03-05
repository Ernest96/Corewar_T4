/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 17:06:44 by ebitca            #+#    #+#             */
/*   Updated: 2017/03/05 15:51:37 by ebitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../corewar.h"


#define CH_1 char ** words; int8_t seek = 1;
#define CH CH_1 static int8_t flags[3] = {0, 0, 0};

char *g_instr[] = {"", "live", "ld", "st", "add", "sub", "and", "or",
							"xor", "zjmp", "ldi", "sti", "fork", "lld",
							"lldi", "lfork","aff"};

int8_t g_par[] = {-1, 1, 2, 2, 3, 3, 3, 3, 3, 1, 3, 3, 1, 2, 3, 1, 1};

char *g_sabl[] = {"", "010000000", "011100000", "100101000", "100100100",
						  "100100100", "111111100", "111111100", "111111100",
						  "010000000", "111110100", "100111110", "010000000",
						  "011100000", "111110100", "010000000", "100000000"};


int8_t	ft_check_instr(char *instr, int *i)
{
	*i = 0;
	while (*i < 17)
	{
		*i = *i + 1;
		if (!ft_strcmp(instr, g_instr[*i]))
			return (g_par[*i]);
	}
	return (0);
}

void	print_struct()
{
	for (int i = 0; i < g_size; ++i)
	{
		printf("%s\t", g_mat[i].laba);
		printf("%s\t", g_mat[i].instr);
		printf("%s\t", g_mat[i].arg1);
		printf("%s\t", g_mat[i].arg2);
		printf("%s\t", g_mat[i].arg3);
	}
}

int ft_num_words(char **words)
{
	int i;

	i = -1;
	while (words[++i] != NULL)
		;
	return (i);
}

int8_t ft_check_labe(char *labe)
{
	int i;

	i = ft_strlen(labe) - 1;
	if (labe[i] != ':')
		return (0);
	labe[i] = 0;
	g_mat[g_size].laba = ft_strdup(labe);
	return (1);
}

void	just_label()
{
	g_mat[g_size].instr = NULL;
	g_mat[g_size].acb = 0;
	g_mat[g_size].arg1 = NULL;
	g_mat[g_size].arg2 = NULL;
	g_mat[g_size].arg2 = NULL;
}

int8_t	can_accept(int8_t order, int par, int inst)
{
	printf("car = %c", g_sabl[inst][3*order+par]);
	if(g_sabl[inst][3*order+par] == '1')
		return (1);
	return (0);
}

void	set_args(int8_t ord, char *str)
{
	if(ord == 0)
		g_mat[g_size].arg1 = ft_strdup(str);
	if(ord == 1)
		g_mat[g_size].arg2 = ft_strdup(str);
	if(ord == 2)
		g_mat[g_size].arg3 = ft_strdup(str);
}

int8_t check_is_reg(char *s)
{
	if (ft_strlen(s) == 2)
	{
		if (s[0] != 'r')
			return (0);
		if (s[1] < '1' || s[1] > '9')
			return (0);
		return (1);
	}
	else if (ft_strlen(s) == 3)
	{
		if (s[0] != 'r')
			return (0);
		if (s[1] != '1')
			return (0);
		if (s[2] < '1' || s[2] > '6')
			return (0);
		return (1);
	}
	else
		return (0);
}

int8_t str_is_num(char *s)
{
	int i;

	i = -1;
	while (s[++i])
		if (!ft_isdigit(s[i]))
			return (0);
	return (1);
}

int8_t check_direct(char *s)
{
	if (s[0] != '%')
		return (0);
	if (s[1] == ':')
	{
		if (!s[2] || (s[2] >= '0' && s[2] <= '9'))
			return (0);
		g_stack[g_top] = ft_strdup(s + 2);
		++g_top;
	}
	else if (str_is_num(s + 1))
		return (1);
	return (0);
}

int8_t check_indirect(char *s)
{
	if (str_is_num(s))
		return (1);
	if (s[0] == ':')
	{
		if (!s[1] || (s[1] >= '0' && s[1] <= '9'))
			return (0);
		g_stack[g_top] = ft_strdup(s + 1);
		++g_top;
	}
	return (0);
}

void	obnulim()
{
	g_mat[g_size].arg1 = NULL;
	g_mat[g_size].arg2 = NULL;
	g_mat[g_size].arg3 = NULL;
}

int		check_args(char **words, int8_t rez, int j)
{
	int8_t i;

	i = -1;
	obnulim();
	while(++i < rez)
	{
		if(check_is_reg(words[i]))
		{
			if(can_accept(i, 0, j))
			{
				set_args(i, words[i]);
			}
			else
				return (0);
		}
		else if(check_direct(words[i]))
		{
			if(can_accept(i, 1, j))
				set_args(i, words[i]);
			else
				return (0);
		}
		else if(check_indirect(words[i]))
		{
			if(can_accept(i, 2, j))
				set_args(i, words[i]);
			else
				return (0);
		}
		else
			return (0);
	}
	return (1);
}

void	ft_check_words_1(char **words, int8_t flag)
{
	int8_t rez;
	int8_t labe;
	int i;

	if(ft_check_dir(words, flag))
		return ;
	labe = ft_check_labe(words[0]);
	if (labe && !words[1])
	{
		just_label();
		++g_size;
		return ;
	}
	if(!labe)
		g_mat[g_size].laba = NULL;
	rez = ft_check_instr(words[0 + labe], &i);
	if (rez != ft_num_words(words + 1 + labe) || rez == 0)
	{
		printf("\n rez = %s\n",words[2]);
		ft_comp_error();
	}
	g_mat[g_size].instr = ft_strdup(g_instr[i]);
	if(!check_args(words + 1 + labe, rez, i))
	{
		printf("\nAm iesit\n");
		ft_comp_error();
	}
	++g_size;
}

void	ft_diez(char *str)
{
	int size;

	size = -1;
	while(str[++size])
	{
		if(str[size] == '#')
		{
			g_offset+=ft_strlen(str) - size;
			str[size] = 0;
			break;
		}
	}
}

int		good_line(int *fd, char **line)
{
	int rez;

	rez = get_next_line(*fd, line);
	ft_diez(*line);
	return (rez);
}


void	free_words(char ***words)
{
	int i;

	i = -1;
	while (*(*words + ++i)!= NULL)
		free(*(*words + i));
	free(*words);
}

void	ft_init_name_comment(int8_t *f, char *to_in, int8_t *seek, char *s)
{
	free(to_in);
	*f = 1;
	g_offset += ft_strlen(s) + 1;
	*seek = 0;
}

void	ft_check_name_comment(char **s, int fd)
{
	CH;
	if (flags[2] && (flags[0] || flags[1]))
		ft_comp_error();
	words = ft_split_whitespaces(*s);
	ft_check_words_1(words, 0);
	if (ft_strcmp(words[0], ".name") == 0)
	{
		if (flags[0])
			ft_comp_error();
		ft_init_name_comment(flags, g_name, &seek, *s);
		g_name = ft_strdup(words[1]);
	}
	else if (ft_strcmp(words[0], ".comment") == 0)
	{
		if (flags[1])
			ft_comp_error();
		ft_init_name_comment(flags + 1, g_comment, &seek, *s);
		g_comment = ft_strdup(words[1]);
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

void	ft_skip_spaces(int *fd)
{
	int8_t flag;
	char *line;

	flag = 1;
	while (flag)
	{
		if (good_line(fd, &line) == 0)
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
	lseek(*fd, g_offset, SEEK_SET);
}



void	ft_first_check(int *fd)
{
	char *line;

	ft_skip_spaces(fd);
	good_line(fd, &line);
	ft_check_name_comment(&line, *fd);
	free(line);
	ft_skip_spaces(fd);
	good_line(fd, &line);
	ft_check_name_comment(&line, *fd);
	free(line);
	ft_skip_spaces(fd);
}


int	main(int argc, char **argv)
{ 
	int	fd;
	char *line;
	char **words;
	//header_t header;

	if (argc != 2)
		ft_usage();
	g_mat =(t_mat*)malloc(sizeof(t_mat) * 1000);
	g_size = 0;
	g_top = 0;
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		ft_invalid_input();
	ft_check_ext(argv[1]);
	ft_first_check(&fd);
	while (good_line(&fd, &line))
	{
		words = ft_split_whitespaces(line);
		g_offset += ft_strlen(line) + 1;
		ft_check_words_1(words, 1);
		ft_putchar('\n');
		free(line);
		free_words(&words);
		ft_skip_spaces(&fd);
	}
	ft_putstr("name: ");
	ft_putstr(g_name);
	ft_putchar('\n');
	ft_putstr("comment: ");
	ft_putstr(g_comment);
	ft_putchar('\n');
	printf("____________\n");
	print_struct();
}
