#include "../corewar.h"

int	ft_check_dir(char **words, int8_t flag)
{
	int flag2;

	flag2 = 0;
	if (words[0] != NULL && !flag)
	{
		if (words[0][0] == '.')
		{
			if (!(!ft_strcmp(words[0], ".name") ||
					!ft_strcmp(words[0], ".comment")))
				ft_comp_error();
			else 
				flag2 = 1;
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
	return (flag2);
}

void	ft_check_ext(char *s)
{
	int last;

	last = ft_strlen(s);
	if (!(s[last - 1] == 's' && s[last - 2] == '.'))
	{
		printf("am intrat\n");
		write(1, AC_R, 5);
		ft_putstr("Fatal: ");
		write(1, AC_RES, 4);
		ft_putstr("Invalid extension\n");
		exit(0);
	}
	ft_strcpy(g_name, "X");
	ft_strcpy(g_name, "");
}
