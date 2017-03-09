#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define memsize 4096
#define CYCLE_TO_DIE			1536
#define CYCLE_DELTA				50

typedef	struct	    s_mat
{
	char	        *laba;
	char            *instr;
	unsigned char   acb;
	char	        *arg1;
	char	        *arg2;
	char	        *arg3;
}				    t_mat;


typedef	struct	    s_instr
{
	unsigned char   code;
	unsigned char   acb;
	unsigned int    arg1;
	unsigned int    arg2;
	unsigned int    arg3;
}				    t_instr;

typedef struct     s_proc
{
    char    reg[16];
    int     pc;
	int		ip;
    int     carry;
    int     live;
}               t_proc;

t_proc  *processes;
int     nr_proc;
char    a[memsize];
int     i = 0;
/*
int				execute_instr(t_instr instr)
{
	unsigned char code;

	code = instr.code;
	if (code == 1)
		return (live(instr));
	else if (code == 2)
		return (ld(instr));
	else if (code == 3)
		return (st(instr));
	else if (code == 4)
		return (add(intstr));
	else if (code == 5)
		return (sub(instr));
	else if (code == 6)
		return (and(instr));
	else if (code == 7)
		return (or(instr));
	else if (code == 8)
		return (xor(instr));
	else if (code == 9)
		return (zjmp(instr));
	else if (code == 10)
		return (ldi(instr));
	else if (code == 11)
		return (sti(instr));
	else if (code == 12)
		return (fork(instr));
	else if (code == 13)
		return (lld(instr));
	else if (code == 14)
		return (lldi(instr));
	else if (code == 15)
		return lfork(instr);
	else if (code == 16)
		return (aff(instr));
	return (1);
}	
*/

int				execute_instr(t_instr instr)
{
	printf("Instructiunea cu codul %d %d\n", instr.code, instr.acb);
	return (0);
}

unsigned int	get_arg(unsigned char acb, int i)
{
    unsigned int result;

    if (acb == 1)
    {
      	processes[i].ip++;
        return (a[(processes[i].ip - 1) % memsize] & 0x000000FF);
    }
    else if (acb == 2)
    {
        result = 0;
        result = ((unsigned int)a[processes[i].ip % memsize]) << 24;
        result = result | ((unsigned int)a[processes[i].ip + 1 % memsize]) << 16;
        result = result | ((unsigned int)a[processes[i].ip + 2 % memsize]) << 8;
        result = result | ((unsigned int)a[processes[i].ip + 3 % memsize]);
        processes[i].ip += 4;
        return (result);
    }
    else if (acb == 3)
    {
        result = 0;
        result = ((unsigned int)a[processes[i].ip % memsize]) << 8;
        result = result | ((unsigned int)a[processes[i].ip + 1 % memsize]);
        processes[i].ip += 2;
        return (result);
    }
    return (0);
}

int		get_args(t_instr *instr, int i)
{
    unsigned int acb;

    acb = instr->acb;
    if (instr->code >= 16 || acb <= 0 || acb > 3)
        return (1);
    instr->arg1 = get_arg(acb >> 6 & 0xff, i);
    instr->arg2 = get_arg(acb >> 4 & 0xff, i);
    instr->arg3 = get_arg(acb >> 2 & 0xff, i);
    return (execute_instr(*instr));
}

void    execute_proc()
{
    int i;
	int flag;

    i = 0;
	flag = 0;
    while (i < nr_proc)
    {
        t_instr   instr;
        instr.code = a[processes[i].pc % memsize];
        processes[i].ip++;
        instr.acb = a[processes[i].pc % memsize];
        processes[i].ip++;
        flag = get_args(&instr, i);
		if (flag == 0)
			processes[i].pc = processes[i].ip;
		else
		{
			printf("\nBIJJJA\n");
			processes[i].pc++;
		}
        i++;
    }
}

int		decrease_cycle(int *nr_cycle)
{
    static int	count;
	int			i;
	int			flag;

	i = 0;
	flag = 0;
	while (i < nr_proc)
	{
		if (processes[i].live >= 21)
		{
			nr_cycle -= CYCLE_DELTA;
			flag = 1;
		}
		processes[i].live = 0;
		i++;
	}
	if (flag == 0)
    	count++;
	if (count == 10)
    {
        nr_cycle -= CYCLE_DELTA;
        count = 0;
    }
	return (flag);
}

void    run()
{
	int	cyc;
    int nr_cycle;
    int alive;

    nr_cycle = CYCLE_TO_DIE;
    alive = 1;
    while (nr_cycle >= 0 && alive)
    {
        cyc = -1;
        while (++cyc < nr_cycle)
        {
            execute_proc();
        }
        alive = decrease_cycle(&nr_cycle);
    }
}

int     main()
{
	processes = (t_proc *)malloc(sizeof(t_proc));
	bzero(processes[0].reg, 16);
	processes[0].pc = 0;
	processes[0].ip = 0;
	processes[0].carry = 1;
	processes[0].live = 0;

	int i;
	bzero(a, 4096);
	nr_proc = 1;
	a[0] = 0x0b;
	a[1] = 0x68;
	a[2] = 0x01;
	a[3] = 0x00;
	a[4] = 0x00;
	a[5] = 0x00;
	a[6] = 0x13;
	a[7] = 0x00;
	a[8] = 0x00;
	a[9] = 0x00;
	a[10] = 0x01;
	a[11] = 0x06;
	a[12] = 0x64;
	a[13] = 0x01;
	a[14] = 0x00;
	a[15] = 0x00;
	a[16] = 0x00;
	a[17] = 0x00;
	a[18] = 0x01;
	a[19] = 0x01;
	a[20] = 0x80;
	a[21] = 0x00;
	a[22] = 0x00;
	a[23] = 0x00;
	a[24] = 0x01;
	a[25] = 0x09;
	a[26] = 0x80;
	a[27] = 0xff;
	a[28] = 0xff;
	a[29] = 0xff;
	a[30] = 0xfa;
    run();
}
