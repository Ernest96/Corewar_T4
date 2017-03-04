#include "../corewar.h"

void	ft_check_dir(char **words, int8_t flag)
{
	if (words[0] != NULL && !flag)
	{
		if (words[0][0] == '.')
		{
			if (!(!ft_strcmp(words[0], ".name") ||
					!ft_strcmp(words[0], ".comment")))
				ft_comp_error();
		}
	}
	if (!flag && words[0] != NULL)
		++words;
	while (*words != NULL)
	{
		if (**words == '.')
		   ft_comp_error();
		++words;
	}
}

void	ft_check_ext(char *s)
{
	int last;

	last = ft_strlen(s);
	if (s[last] - 1 != 's' && s[last - 2] != '.')
	{
		write(1, AC_R, 5);
		ft_putstr("Fatal: ");
		write(1, AC_RES, 4);
		ft_putstr("Invalid extension\n");
		exit(0);
	}
	g_name = ft_strdup("unnamed");
	g_comment = ft_strdup("");
}