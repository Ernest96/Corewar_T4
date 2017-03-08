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
	while(q < g_i)
	{
		if (q && is_return(q))
			printf("\n");
		printf("%2.2x ", g_a[q]);
		q++;
	}
}

void	set_label(char *label, int o)
{
    if(!label)
        return;
    g_search[g_si].str = ft_strdup(label);
    g_search[g_si].loc = g_i;
    g_search[g_si].off = o;
    g_search[g_si].start = g_ip;
    g_si++;
}

void	check_label(char *arg)
{
    if(!arg)
        return;
    g_found[g_fi].str = ft_strdup(arg);
    g_found[g_fi].loc = g_ip;
    g_fi++;
}

void    set_instr(char *arg)
{
	unsigned char	q;

	q = 0;
	while (q <= 16)
	{
		if (ft_strcmp(arg, g_instr[q]) == 0)
		{
			//printf("\ninstructiunea %s, are codul %d\n", arg, q);
			g_a[g_i] = q;
			g_i++;
			break;
		}
		q++;
	}
}

unsigned char	set_acb(unsigned char acb)
{
	g_a[g_i] = acb;
	g_i++;
	return (acb);
}



void    set_indir(char *arg)
{
	int indir;

	if (arg[0] == ':')
	{
        set_label(arg+1, 2);
		g_a[g_i] = 0;
		g_a[g_i + 1] = 0;
		g_i += 2;
		return ;
	}
	indir = atoi(arg);
	g_a[g_i] = indir >> 8 & 0xff;
	g_a[g_i + 1] = indir & 0xff;
	g_i += 2;
}

void    set_dir(char *arg)
{
	int dir;

	if (arg[1] == ':')
	{
        set_label(arg+2, 4);
		g_a[g_i] = 0;
		g_a[g_i + 1] = 0;
		g_a[g_i + 2] = 0;
		g_a[g_i + 3] = 0;
		g_i += 4;
		return ;
	}
	dir = atoi(arg + 1);
	g_a[g_i] = dir >> 24 & 0xff;
	g_a[g_i + 1] = dir >> 16 & 0xff;
	g_a[g_i + 2] = dir >> 8 & 0xff;
	g_a[g_i + 3] = dir & 0xff;
	g_i += 4;
}



void    set_reg(char *arg)
{
	int reg;

	reg = atoi(arg + 1);
	g_a[g_i] = reg;
    g_i++;
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

        q = -1;
        while(++q < g_size)
        {
            check_label(g_mat[q].laba);
            if (g_mat[q].instr == NULL)
                continue ;
            set_instr(g_mat[q].instr);
            temp = set_acb(g_mat[q].acb);
            set_arg(g_mat[q].arg1, temp >> 6 & 0x3);
            set_arg(g_mat[q].arg2, temp >> 4 & 0x3);
            set_arg(g_mat[q].arg3, temp >> 2 & 0x3);
            g_ip = g_i;
            test[p_test] = g_i;
            p_test++;
        }
}

void    print_struct2(void)
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
            if(!(ft_strcmp(g_search[temp].str, g_found[j].str)))
            {
                if(g_search[temp].off == 2)
                {
                    g_a[g_search[temp].loc] = (g_found[j].loc - g_search[temp].start) >> 8 & 0xff;
                    g_a[g_search[temp].loc + 1] = (g_found[j].loc - g_search[temp].start) & 0xff;
                }
                else
                {
                    g_a[g_search[temp].loc] = (g_found[j].loc - g_search[temp].start) >> 24 & 0xff;
                    g_a[g_search[temp].loc + 1] = (g_found[j].loc - g_search[temp].start) >> 16 & 0xff;
                   g_a[g_search[temp].loc + 2] = (g_found[j].loc - g_search[temp].start) >> 8 & 0xff;
                    g_a[g_search[temp].loc + 3] = (g_found[j].loc - g_search[temp].start) & 0xff;
                }
                break;
            }
        }
    }
}
