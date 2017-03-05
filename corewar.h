/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcrivenc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 16:38:56 by vcrivenc          #+#    #+#             */
/*   Updated: 2017/03/05 18:21:48 by dprovorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
#define COREWAR_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include "get_next_line.h"
# define AC_R     		"\x1b[31m"
# define AC_G	  		"\x1b[92m"
# define AC_Y	  		"\x1b[33m"
# define AC_B    		"\x1b[96m"
# define AC_M 			"\x1b[35m"
# define AC_C    		"\x1b[36m"
# define AC_RES  		"\x1b[0m"

typedef	struct	s_mat
{
	char	*laba;
	char	*instr;
	int8_t	acb;
	char	*arg1;
	char	*arg2;
	char	*arg3;
}				t_mat;

typedef struct 	s_pos
{
	int 	loc;
    int     start;
	char 	*str;
	int 	off;
}				t_pos;

char *g_name, *g_comment;
int	g_offset;
int	g_line;
t_mat *g_mat;
char *g_stack[100];
int g_size;
int g_top;
int8_t g_can;
int		get_next_line(int fd, char **line);
void	ft_usage(void);
void	ft_invalid_input(void);
void	ft_comp_error(void);
void	ft_check_words_1(char **words, int8_t flag);
void	free_words(char ***words);
void	ft_check_name_comment(char **s, int fd);
int8_t ft_only_space(char *line);
void	ft_skip_spaces(int *fd);
int 	ft_check_dir(char **words, int8_t flag);
void	ft_check_ext(char *s);
#endif
