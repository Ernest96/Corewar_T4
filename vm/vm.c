#include "../corewar.h"

int8_t	find_flag(char *s)
{
	if (!ft_strcmp(s, "-dump"))
		return (1);
	if (!ft_strcmp(s, "-n"))
		return (2);
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

void p_zero()
{
	int i;

	i = 0;
	while(++i < 5)
	{
		g_arr[i].nr = 0;
		g_arr[i].name = NULL;
	}
}

void	ft_check_players()
{
	int8_t flag;
	int8_t i;

	flag = 0;
	i = 0;
	while (++i < 5)
		if (g_arr[i].nr)
			++flag;
	if (flag < 2)
		exit(0);
}

void	set_null()
{
	int i;

	i = -1;
	while(++i < 4)
		g_order[i] = NULL;
}

unsigned char *ft_read_name(int i)
{
	int fd;
	unsigned char *s;
	unsigned char temp[2180];
	char c;

	g_j[i - 1] = 0;
	if(g_arr[i].name == NULL)
		return (NULL);
	s = (unsigned char*)malloc(sizeof(char)*4096);
	ft_bzero(s, 4096);
	fd = open(g_arr[i].name, O_RDONLY);
	if (fd < 0)
	{
		printf("%s\n\n",g_arr[i].name);
		exit (0);
	}
	read(fd, temp, 2180);
	while(read(fd, &c, 1))
	{
		s[g_j[i - 1]] = c;
		++g_j[i - 1];
	}
	return (s);
}

void	print_order(void)
{
	int i;
	int j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		printf("nume = %s\n instr:\n", g_arr[i + 1].name);
		while(++j < g_j[i])
			printf("%.2x ",g_order[i][j]);
	}

}

void	insert_gamelife()
{
	int i;

	g_order = (unsigned char**)malloc(sizeof(char*) * 4);
	set_null();
	i = 0;
	while (++i < 5)
		g_order[i - 1] = ft_read_name(i);
	print_order();
}

int	main(int argc, char **argv)
{
	int8_t flag;
	int i;
	int dump;
	int8_t p_n;
	int8_t found;

	found = 0;
	if (argc == 1)
		return (0);
	if (argv[1][0] != '-')
		return (0);
	i = 0;
	p_zero();
	while (++i < argc)
	{
		flag = find_flag(argv[i]);
		if (!flag)
			return (0);
		if (flag == 1)
		{
			++i;
			if (i >= argc || found)
				return (0);
			found = 1;
			if (str_is_num(argv[i]))
				dump = ft_atoi(argv[i]);
			else
				return(0);
		}
		if (flag == 2)
		{
			if (i - argc > -2)
				return(0);
			if(str_is_num(argv[i+1]))
				p_n = ft_atoi(argv[i+1]);
			else
				return(0);
			if (p_n < 1 || p_n > 4)
				return(0);
			if (g_arr[p_n].nr)
				return(0);
			g_arr[p_n].nr = p_n;
			if (ft_strcmp(argv[i + 2] + (ft_strlen(argv[i + 2]) - 4), ".cor"))
				return (0);
			g_arr[p_n].name = ft_strdup(argv[i+2]);
			i += 2;
		}
	}
	ft_check_players();
	insert_gamelife();
	printf("e ok\n");
	(void)dump;
}