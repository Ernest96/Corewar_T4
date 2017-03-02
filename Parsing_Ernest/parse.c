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

void	ft_check_ext(char *s)
{
	int last;

	last = ft_strlen(s);
	if (last - 1 != 's' && last - 2 != '.')
	{
		ft_putstr("Invalid extension");
		exit(0);
	}
}

int	main(int argc, char **argv)
{ 
	int i;
	int	fd;
	char *line;

	i = 0;
	while (++i < argc)
	{
		ft_check_ext(argv[i]);
		fd = open(argv[i], O_RDONLY);
		while (get_next_line(fd, &line))
		{
			ft_putstr(line);
			ft_putchar('\n');
			free(line);
		}
	}
}
