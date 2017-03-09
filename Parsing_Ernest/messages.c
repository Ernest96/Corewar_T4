/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/09 18:32:32 by ebitca            #+#    #+#             */
/*   Updated: 2017/03/09 18:32:34 by ebitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../corewar.h"

void	ft_usage(void)
{
	write(1, AC_B, 5);
	ft_putstr("usage: ");
	write(1, AC_RES, 4);
	ft_putstr("asm input_file\n");
	exit(0);
}

void	ft_invalid_input(void)
{
	write(1, AC_R, 5);
	ft_putstr("Fatal: ");
	write(1, AC_RES, 4);
	ft_putstr("Invalid input\n");
	exit(0);
}

void	ft_comp_error(void)
{
	write(1, AC_R, 5);
	ft_putstr("Fatal: ");
	write(1, AC_RES, 4);
	ft_putstr("Compilation error\n");
	ft_putstr("Stray nr: ");
	write(1, AC_R, 5);
	ft_putnbr(g_line);
	write(1, AC_RES, 4);
	ft_putchar('\n');
	exit(0);
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
