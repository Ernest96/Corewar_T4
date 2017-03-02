/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcrivenc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 16:38:56 by vcrivenc          #+#    #+#             */
/*   Updated: 2017/03/02 18:16:39 by ebitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H

#include <stdio.h>
#include <stdint.h>
#include "get_next_line.h"

typedef	struct	s_mat
{
	char	*laba;
	char	*instr;
	int8_t	acb;
	char	*arg1;
	char	*arg2;
	char	*arg3;
}				t_mat;

char *g_instr[] = {"", "live", "ld", "st", "add", "sub", "and", "or",
							"xor", "zjmp", "ldi", "sti", "fork", "lld",
							"lldi", "lfork","aff"};

char *g_sabl[] = {"", "010000000", "011100000", "100101000", "100100100",
						  "100100100", "111111100", "111111100", "111111100",
						  "010000000", "111110100", "100111110", "010000000",
						  "011100000", "111110100", "010000000", "100000000"};
char *g_name, *g_comment;

#endif
