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
	int j;

	i = 0;
	while(++i < 5)
	{
		j = -1;
		g_arr[i].nr = 0;
		g_arr[i].name = NULL;
		g_arr[i].filename = NULL;
		while(++j < 100)
		{
			ft_bzero(g_arr[i].proc[j].reg, 16);
			g_arr[i].proc[j].reg[0] = i;
			g_arr[i].proc[j].pc = 0;
			g_arr[i].proc[j].ip = 0;
			g_arr[i].proc[j].carry = 1;
			g_arr[i].proc[j].live = 0;
            g_arr[i].proc[j].begin = 0;
            g_arr[i].proc[j].end = 0;
		}
		g_arr[i].pr_n = 1;
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
    g_num = flag;
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
	if(g_arr[i].filename == NULL)
		return (NULL);
	s = (unsigned char*)malloc(sizeof(char)*4096);
	ft_bzero(s, 4096);
	fd = open(g_arr[i].filename, O_RDONLY);
	if (fd < 0)
	{
		printf("PUSSSSSYY%s\n\n",g_arr[i].filename);
		exit (21);
	}
	read(fd, temp, 4);
	ft_bzero(temp, 4);
	read(fd, temp, 128);
	g_arr[i].name = ft_strdup(temp); 
	read(fd, temp, 2048);
	printf("AM citit name ca {%s} si commment {%s}\n", g_arr[i].name, temp);
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
	while (++i < 5)
	{
		j = -1;
		printf("\nnume = %s\ninstr: ", g_arr[i + 1].name);
		while(++j < g_j[i])
			printf("%.2x ",g_order[i][j]);
	}
	printf("\n\n");
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

void	load()
{
	int i;
	int j;
	int delta;
	int offset;

	i = 0;
	delta = MEMSIZE / g_num;
	offset = 0;
	while (++i < 5)
	{
		j = 0;
        if (g_arr[i].filename == NULL) {
            printf("continue la %d\n", i);
            continue;
        }
		while (j < g_j[i - 1])
		{
			g_mem[offset + j] = g_order[i - 1][j];
			j++;
		}

		g_arr[i].proc[0].pc = offset;
		g_arr[i].proc[0].ip = offset;
        g_arr[i].proc[0].begin = offset;
        g_arr[i].proc[0].end = offset + j - 1;
		offset += delta;
	}
}

void    init_steps(void)
{
    g_steps[0] = 0;
    g_steps[1] = 10;
    g_steps[2] = 5;
    g_steps[3] = 5;
    g_steps[4] = 10;
    g_steps[5] = 10;
    g_steps[6] = 6;
    g_steps[7] = 6;
    g_steps[8] = 6;
    g_steps[9] = 20;
    g_steps[10] = 25;
    g_steps[11] = 25;
    g_steps[12] = 800;
    g_steps[13] = 10;
    g_steps[14] = 50;
    g_steps[15] = 1000;
    g_steps[16] = 2;
}

int 	main(int argc, char **argv)
{
	int8_t flag;
	int i;
	int dump;
	int8_t p_n;
	int8_t found;

	found = 0;
    init_steps();
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
			g_arr[p_n].filename = ft_strdup(argv[i+2]);
			i += 2;
		}
	}
	ft_check_players();
	insert_gamelife();
	(void)dump;
	load();
    //print_order();
    ft_start();
}