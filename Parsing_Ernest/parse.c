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

void	ft_init_1(void)
{
	g_instr = (char**)malloc(sizeof(char*) * 17);
	g_instr[0] = ft_strdup("");
	g_instr[1] = ft_strdup("live");
	g_instr[2] = ft_strdup("ld");
	g_instr[3] = ft_strdup("st");
	g_instr[4] = ft_strdup("add");
	g_instr[5] = ft_strdup("sub");
	g_instr[6] = ft_strdup("and");
	g_instr[7] = ft_strdup("or");
	g_instr[8] = ft_strdup("xor");
	g_instr[9] = ft_strdup("zjmp");
	g_instr[10] = ft_strdup("ldi");
	g_instr[11] = ft_strdup("sti");
	g_instr[12] = ft_strdup("fork");
	g_instr[13] = ft_strdup("lld");
	g_instr[14] = ft_strdup("lldi");
	g_instr[15] = ft_strdup("lfork");
	g_instr[16] = ft_strdup("aff");
}

void	ft_init_2(void)
{
	g_sabl = (char**)malloc(sizeof(char*) * 17);
	g_sabl[0] = ft_strdup("");
	g_sabl[1] = ft_strdup("010000000");
	g_sabl[2] = ft_strdup("011100000");
	g_sabl[3] = ft_strdup("100101000");
	g_sabl[4] = ft_strdup("100100100");
	g_sabl[5] = ft_strdup("100100100");
	g_sabl[6] = ft_strdup("111111100");
	g_sabl[7] = ft_strdup("111111100");
	g_sabl[8] = ft_strdup("111111100");
	g_sabl[9] = ft_strdup("010000000");
	g_sabl[10] = ft_strdup("111110100");
	g_sabl[11] = ft_strdup("100111110");
	g_sabl[12] = ft_strdup("010000000");
	g_sabl[13] = ft_strdup("011100000");
	g_sabl[14] = ft_strdup("111110100");
	g_sabl[15] = ft_strdup("010000000");
	g_sabl[16] = ft_strdup("100000000");
}

void ft_init_3(void)
{
	g_name = (char*)malloc(sizeof(char) * 129);
	g_comment = (char*)malloc(sizeof(char) * 2049);
	ft_bzero(g_name, 129);
	ft_bzero(g_comment, 2049);
	g_par[0] = -1;
	g_par[1] = 1;
	g_par[2] = 2;
	g_par[3] = 2;
	g_par[4] = 3;
	g_par[5] = 3;
	g_par[6] = 3;
	g_par[7] = 3;
	g_par[8] = 3;
	g_par[9] = 1;
	g_par[10] = 3;
	g_par[11] = 3;
	g_par[12] = 1;
	g_par[13] = 2;
	g_par[14] = 3;
	g_par[15] = 1;
	g_par[16] = 1;
}

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

void	print_struct(void)
{
	for (int i = 0; i < g_size; ++i)
	{
		printf("%s\t", g_mat[i].laba);
		printf("%s\t", g_mat[i].instr);
		printf("%x\t", g_mat[i].acb);
		printf("%s\t", g_mat[i].arg1);
		printf("%s\t", g_mat[i].arg2);
		printf("%s\t", g_mat[i].arg3);
		printf("\n");
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
	if(g_sabl[inst][3*order+par] == '1')
		return (1);
	return (0);
}


unsigned char	get_acb(int8_t offset, int8_t type)
{
	unsigned char temp;
	temp = type << 2*offset;
	return(temp);
}


void	set_args(int8_t ord, char *str, int8_t type)
{
	if(ord == 0)
	{
		g_mat[g_size].arg1 = ft_strdup(str);
		g_mat[g_size].acb+= get_acb(3, type);
	}
	if(ord == 1)
	{
		g_mat[g_size].arg2 = ft_strdup(str);
		g_mat[g_size].acb+= get_acb(2, type);
	}
	if(ord == 2)
	{
		g_mat[g_size].arg3 = ft_strdup(str);
		g_mat[g_size].acb+= get_acb(1, type);
	}
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
		return (1);
	}
	else if (str_is_num(s + 1))
		return (1);
	return (0);
}

int8_t check_indirect(char *s)
{
	if (str_is_num(s))
		return (1);
	if (s[0] == '%')
	{
		if (!s[1] || (s[1] >= '0' && s[1] <= '9'))
			return (0);
		g_stack[g_top] = ft_strdup(s + 1);
		++g_top;
		return (1);
	}
	return (0);
}

void	obnulim()
{
	g_mat[g_size].acb = 0;
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
				set_args(i, words[i], 1);
			else
				return (0);
		}
		else if(check_direct(words[i]))
		{
			if(can_accept(i, 1, j))
				set_args(i, words[i], 2);
			else
				return (0);
		}
		else if(check_indirect(words[i]))
		{
			if(can_accept(i, 2, j))
				set_args(i, words[i], 3);
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
		ft_comp_error();
	g_mat[g_size].instr = ft_strdup(g_instr[i]);
	if(!check_args(words + 1 + labe, rez, i))
		ft_comp_error();
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
	(void)to_in;
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
		ft_strcpy(g_name, words[1]);
	}
	else if (ft_strcmp(words[0], ".comment") == 0)
	{
		if (flags[1])
			ft_comp_error();
		ft_init_name_comment(flags + 1, g_comment, &seek, *s);
		printf("e bun\n");

		ft_strcpy(g_comment, words[1]);
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
		if (*line != ' ' && *line != '\t')
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

void	ft_labe_error(const char *s)
{
	write(1, AC_R, 5);
	ft_putstr("Fatal: ");
	write(1, AC_RES, 4);
	ft_putstr("Compilation error\n");
	write(1, AC_RES, 4);
	write(1, AC_R, 5);
	ft_putstr(s);
	write(1, AC_RES, 4);
	ft_putchar('\n');
	exit(0);
}

void ft_check_letters(void)
{
	int top;
	int j;
	const char *ch = "abcdefghijklmnopqrstuvwxyz_0123456789";

	top = g_top;
	while (--top != -1)
	{
		j = -1;
		while (g_stack[top][++j])
		if (!ft_strchr(ch, g_stack[top][j]))
				ft_labe_error("Invalid symbol for label");
	}
}

void	ft_check_stack(void)
{
	int top;
	int i;
	int8_t flag;

	top = g_top;

	while (--top != -1)
	{
		i = -1;
		flag = 0;
		while (++i < g_size)
		{
			if (g_mat[i].laba == NULL)
				continue ;
			if (!ft_strcmp(g_stack[top], g_mat[i].laba))
				++flag;
		}
		if (flag != 1)
			ft_labe_error("Invalid label adresation");
	}
}

void	ft_find_label(void)
{
	ft_check_letters();
	ft_check_stack();
}

void	dump(char *file)
{
	int fd1;
	unsigned int magic;

	magic = 0xf383ea00;
	fd1 = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd1 == -1) {
        perror("File cannot be opened");
      	return ;
    }
    write(fd1, &magic, 4);
    write(fd1, g_name, 128);
    write(fd1, g_comment, 2048);
    write(fd1, g_a, g_i);
 }

int	main(int argc, char **argv)
{ 
	int	fd;
	char *line;
	char **words;

	(void)argc;
	ft_init_1();
	ft_init_2();
	ft_init_3();
	g_top = 0;
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
	printf("ee\n");
	ft_putstr("name: ");
	ft_putstr(g_name);
	ft_putchar('\n');
	ft_putstr("comment: ");
	ft_putstr(g_comment);
	printf("aaa\n");
	ft_putchar('\n');
	ft_find_label();
	print_struct();
	ft_putchar('\n');
	
	codify();
    insert_labels();
    printf("g_i = %d\n",g_i);
    printf("g_ip = %d\n",g_ip);
    print();
    line = ft_strdup(argv[1]);
    line[ft_strlen(line) - 1] = 0;
    dump(ft_strjoin(line, "cor"));
}
