/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcrivenc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 15:52:20 by vcrivenc          #+#    #+#             */
/*   Updated: 2017/03/03 16:54:19 by dprovorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../corewar.h"
#define memsize 4096


unsigned char       a[memsize];
int                 i = 0;
int                 g_ip = 0;
int                 g_fi = 0;
int                 g_si = 0;
t_pos               g_search[100];
t_pos               g_found[100];
int                 test[100];
int                 p_test=0;

int    is_return(int i)
{
    for(int j=0; j<p_test; j++)
        if(test[j] == i)
            return 1;
    return 0;
}

void    print(void)
{
	int q;

	q = 0;
	while(q < i)
	{
		if (q && is_return(q))
			printf("\n");
		printf("%2.2x ", a[q]);
		q++;
	}
}

void	set_label(char *label, int o)
{
    if(!label)
        return;
    g_search[g_si].str = strdup(label);
    g_search[g_si].loc = i;
    g_search[g_si].off = o;
    g_search[g_si].start = g_ip;
    g_si++;
}

void	check_label(char *arg)
{
    if(!arg)
        return;
    g_found[g_fi].str = strdup(arg);
    g_found[g_fi].loc = g_ip;
    g_fi++;
}

void    set_instr(char *arg)
{
	unsigned char	q;

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

unsigned char	set_acb(unsigned char acb)
{
	a[i] = acb;
	i++;
	return (acb);
}



void    set_indir(char *arg)
{
	int indir;

	if (arg[0] == ':')
	{
        set_label(arg+1, 2);
		a[i] = 0;
		a[i + 1] = 0;
		i+= 2;
		return ;
	}
	indir = atoi(arg);
	a[i] = indir >> 8 & 0xff;
	a[i + 1] = indir & 0xff;
	i += 2;
}

void    set_dir(char *arg)
{
	int dir;

	if (arg[1] == ':')
	{
        set_label(arg+2, 4);
		a[i] = 0;
		a[i + 1] = 0;
		a[i + 2] = 0;
		a[i + 3] = 0;
		i += 4;
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

void    codify(void)
{
		char temp;
        int q;

        q = 0;
        while(q < g_size) {
            check_label(g_mat[q].laba);
            if (g_mat[q].instr == NULL)
                return;
            set_instr(g_mat[q].instr);
            temp = set_acb(g_mat[q].acb);
            set_arg(g_mat[q].arg1, temp >> 6 & 0x3);
            set_arg(g_mat[q].arg2, temp >> 4 & 0x3);
            set_arg(g_mat[q].arg3, temp >> 2 & 0x3);
            g_ip = i;
            test[p_test] = i;
            p_test++;
            q++;
        }
}

void    print_struct(void)
{
    int i = -1;
    while(++i < g_fi)
    {
        printf("\n f_str = %s",g_found[i].str);
        printf("\n f_loc = %d",g_found[i].loc);
        printf("\n f_off = %d",g_found[i].off);
    }
    i = -1;
    while(++i < g_si)
    {
        printf("\n s_str = %s",g_search[i].str);
        printf("\n s_loc = %d",g_search[i].loc);
        printf("\n s_off = %d",g_search[i].off);
    }
}

void    insert_labels(void)
{
    static int temp = -1;
    static int j = -1;
    while (++temp < g_si)
    {
        j = -1;
        while (++j < g_fi)
        {
            if(!(strcmp(g_search[temp].str, g_found[j].str)))
            {
                if(g_search[temp].off == 2)
                {
                    a[g_search[temp].loc] = (g_found[j].loc - g_search[temp].start) >> 8 & 0xff;
                    a[g_search[temp].loc + 1] = (g_found[j].loc - g_search[temp].start) & 0xff;
                }
                else
                {
                    a[g_search[temp].loc] = (g_found[j].loc - g_search[temp].start) >> 24 & 0xff;
                    a[g_search[temp].loc + 1] = (g_found[j].loc - g_search[temp].start) >> 16 & 0xff;
                    a[g_search[temp].loc + 2] = (g_found[j].loc - g_search[temp].start) >> 8 & 0xff;
                    a[g_search[temp].loc + 3] = (g_found[j].loc - g_search[temp].start) & 0xff;
                }
                break;
            }
        }
    }
}
