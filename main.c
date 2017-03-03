/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcrivenc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 15:52:20 by vcrivenc          #+#    #+#             */
/*   Updated: 2017/03/03 16:40:42 by vcrivenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include <stdio.h>
#include <stdlib.h>
#define memsize 4096


char    a[memsize];
int     i = 0;

void    print()
{
	int q;

	q = 0;
	while(q < i)
	{
		if (q && q % 10 == 0)
			printf("\n");
		printf("%2.2x ", a[q]);
		q++;
	}
}

void	set_label(char *label)
{

}
void	check_label(char *arg)
{
	//printf("Label {%s}", arg);
}

void    set_instr(char *arg)
{
	char	q;

	q = 0;
	while (q <= 16)
	{
		if (strcmp(arg, g_instr[q]) == 0)
		{
			//printf("\ninstructiunea %s, are codul %d\n", arg, q);
			a[i] = q;
			i++;
			break;
		}
		q++;
	}
}

char	set_acb(char acb)
{
	a[i] = acb;
	i++;
	return (acb);
}



void    set_indir(char *arg)
{
	int indir;

	/*if (arg[0] == ':')
	{
		a[i] = 0;
		a[i + 1] = 0;
		i+= 2;
		return ;
	}
	indir = atoi(arg);
	a[i] = a[indir] >> 0xff;
	a[i + 1] = a[indir] & 0xff;
	i += 2;*/
}

void    set_dir(char *arg)
{
	int dir;

	if (arg[1] == ':')
	{
		a[i] = 0;
		a[i + 1] = 0;
		a[i + 2] = 0;
		a[i + 3] = 0;
		i += 4;
		set_label(arg);
		return ;
	}
	dir = atoi(arg + 1);
	a[i] = dir >> 24 & 0xff;
	a[i + 1] = dir >> 16 & 0xff;
	a[i + 2] = dir >> 8 & 0xff;
	a[i + 3] = dir & 0xff;
	i += 4;
}



void    set_reg(char *arg)
{
	int reg;

	reg = atoi(arg + 1);
	a[i] = reg;
	i++;
}

void    set_arg(char *arg, int acb)
{
	if (acb == 0 || arg == NULL)
		return ;
	if (acb == 1)
		set_reg(arg);
	else if (acb == 3)
		set_indir(arg);
	else
		set_dir(arg);
}

void    codify(t_mat line)
{
		char temp;

		check_label(line.laba);
		if (line.instr == NULL)
			return;
		set_instr(line.instr);
		temp = set_acb(line.acb);
		set_arg(line.arg1, temp >> 6 & 0x3);
		set_arg(line.arg2, temp >> 4 & 0x3);
		set_arg(line.arg3, temp >> 2 & 0x3);
}


int     main()
{
	t_mat   line;
	line.laba = "live:";
	line.instr = "ldi";
	line.arg1 = "%10";
	line.acb = 2 << 6;
	codify(line);
	print();
}


