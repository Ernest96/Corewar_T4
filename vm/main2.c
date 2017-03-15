#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../corewar.h"
#define CYCLE_TO_DIE			1536
#define CYCLE_DELTA				50

//typedef	struct	    s_mat
//{
//	char	        *laba;
//	char            *instr;
//	unsigned char   acb;
//	char	        *arg1;
//	char	        *arg2;
//	char	        *arg3;
//}				    t_mat;






int     nr_proc;
char    a[MEMSIZE];


//int             zjmp(t_instr instr, int i)
//{
//    if (instr.acb != 80)
//        return (1);
//    if (processes[i].carry == 0)
//        return (0);
//    processes[0].pc += (signed int)instr.arg1;
//    processes[0].ip = processes[0].pc;
//    return (0);
//}
//

int         live(t_instr instr, int juc ,int procs)
{
    if (instr.acb != 0x80)
        return (1);
    if(instr.arg1 == 0 || instr.arg1 > 4)
        return (1);
    g_arr[juc].proc[procs].live++;

    return (0);
}

int         ld(t_instr instr, int juc, int procs)
{
    if (instr.acb != 0x90 && instr.acb != 0xD0)
        return (1);
    if (instr.arg2 <= 0 || instr.arg2 > 16)
        return (1);
    g_arr[juc].proc[procs].reg[instr.arg2 - 1] = (unsigned)instr.arg1;
    return (0);
}

int         st(t_instr instr, int juc ,int procs)
{
    if (instr.acb != 0x50 && instr.acb != 0x70)
        return (1);
    if (instr.arg1 > 16 || instr.arg1 <= 0)
        return (1);
    if (((instr.acb >> 4 & 0x3) == 1) && (instr.arg2 > 16 || instr.arg2 <= 0))
        return (1);
    if ((instr.acb >> 4 & 0x3) == 1)
        g_arr[juc].proc[procs].reg[instr.arg2 - 1] =
                g_arr[juc].proc[procs].reg[instr.arg1 - 1];
    else
    {
        g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD)] = 1;// g_arr[juc].proc[procs].reg[instr.arg1] >> 24 & 0xff;
        g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD) + 1] =  1;//g_arr[juc].proc[procs].reg[instr.arg1] >> 16 & 0xff;
        g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD) + 2] =  1;//g_arr[juc].proc[procs].reg[instr.arg1] >> 8 & 0xff;
        g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD) + 3] =  1; //g_arr[juc].proc[procs].reg[instr.arg1] & 0xff;
    }
    return 0;
}
int				execute_instr(t_instr instr, int juc, int procs)
{
	unsigned char code;

    code = instr.code;
    printf("Jucatorul %d\t",g_arr[juc].nr);
	printf("Instructiunea cu codul %.2x %.2x |", instr.code, instr.acb);
    printf("Arg1 %x, Arg2 %x, Arg3 %x\t\t", instr.arg1, instr.arg2, instr.arg3);
    for (int i = 0; i < 16; i++)
        printf("%d | ", g_arr[juc].proc->reg[i]);
    printf("\n");

	if (code == 1)
		return (live(instr, juc, procs));
	else if (code == 2)
		return (ld(instr, juc, procs));
    else if (code == 3)
		return (st(instr, juc, procs));
        /*
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
		return (zjmp(instr, i));
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
		return (aff(instr));*/

	return (0);
}

//int				execute_instr(t_instr instr, int juc, int procs)
//{
//
//    if (instr.code == 9)
//    {
//        //printf("\n%d %d\n",processes[0].pc, (signed int)instr.arg1);
//        processes[0].pc += (signed int)instr.arg1;
//        processes[0].ip = processes[0].pc;
//        //printf("\n%d\n", processes[0].pc);
//    }
//
//    printf("Jucatorul %d\t",g_arr[juc].nr);
//	printf("Instructiunea cu codul %.2x %.2x |", instr.code, instr.acb);
//    printf("Arg1 %x, Arg2 %x, Arg3 %x\n", instr.arg1, instr.arg2, instr.arg3);
//	return (0);
//}

int	get_arg(unsigned char acb, int juc, int procs)
{
    int result;

    if (acb == 1)
    {
      	g_arr[juc].proc[procs].ip++;
        return (g_mem[(g_arr[juc].proc[procs].ip - 1) % MEMSIZE] & 0x000000FF);
    }
    else if (acb == 2)
    {
        result = 0;
        result = ((unsigned int)g_mem[g_arr[juc].proc[procs].ip % MEMSIZE]) << 24;
        result = result | ((unsigned int)g_mem[g_arr[juc].proc[procs].ip + 1 % MEMSIZE]) << 16;
        result = result | ((unsigned int)g_mem[g_arr[juc].proc[procs].ip + 2 % MEMSIZE]) << 8;
        result = result | ((unsigned int)g_mem[g_arr[juc].proc[procs].ip + 3 % MEMSIZE]);
        g_arr[juc].proc[procs].ip += 4;
        return (result);
    }
    else if (acb == 3)
    {
        result = 0;
        result = ((unsigned int)g_mem[g_arr[juc].proc[procs].ip % MEMSIZE]) << 8;
        result = result | ((unsigned int)g_mem[g_arr[juc].proc[procs].ip + 1 % MEMSIZE]);
        g_arr[juc].proc[procs].ip += 2;
        return (result);
    }
    return (0);
}

int		get_args(t_instr *instr, int juc , int proc)
{
    unsigned int acb;

    acb = instr->acb;
    if (instr->code >= 16)
        return (1);
    instr->arg1 = 0;
    instr->arg2 = 0;
    instr->arg3 = 0;
    if (acb > 0xFC)
        return 0;
    if ((acb >> 6 & 0x3) == 0)
        return 0;
    else
        instr->arg1 = get_arg(acb >> 6 & 0x3, juc , proc);
    //printf("ACB = %x", (acb >> 4) & 0x3);
    if (((acb >> 4) & 0x3) == 0)
        return (execute_instr(*instr, juc, proc));
    else
        instr->arg2 = get_arg(acb >> 4 & 0x3, juc, proc);

    if (((acb >> 2) & 0x3) == 0)
        return (execute_instr(*instr, juc, proc));
    else
        instr->arg3 = get_arg(((acb >> 2) & 0x3), juc, proc);
    return (execute_instr(*instr, juc, proc));
}

void    execute_proc(int j)
{
    int i;
	int flag;

	flag = 0;
    i = 0;
    while (i < g_arr[j].pr_n)
    {
        t_instr instr;
        instr.code = g_mem[g_arr[j].proc[i].ip % MEMSIZE];
        g_arr[j].proc[i].ip++;
        instr.acb = g_mem[g_arr[j].proc[i].ip % MEMSIZE];
        g_arr[j].proc[i].ip++;
        flag = get_args(&instr, j, i);
        if (flag == 0)
            g_arr[j].proc[i].pc = g_arr[j].proc[i].ip;
        else
        {
            printf("\nBIJJJA\n");
            g_arr[j].proc[i].pc++;
            g_arr[j].proc[i].ip = g_arr[j].proc[i].pc;
        }
        if (g_arr[j].proc[i].pc >= g_arr[j].proc[i].end) {
            g_arr[j].proc[i].pc = g_arr[j].proc[i].begin;
            g_arr[j].proc[i].ip = g_arr[j].proc[i].begin;
            printf("resetare la jucatorul %d", j);
        }
        i++;
    }
}

int		decrease_cycle(int *nr_cycle)
{
    static int	count;
	int			i;
    int         j;
	int			flag;

	i = 0;
	flag = 0;
	while (++i <= 4)
	{
        if (g_arr[i].filename == NULL)
            continue ;
        j = -1;
        while (++j < g_arr[i].pr_n)
        {
		    if (g_arr[i].proc[j].live >= 21)
		    {
			    nr_cycle -= CYCLE_DELTA;
			    flag = 1;
		    }
        }
        g_arr[i].proc[0].live = 0;
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

void    print_mem() {
    int i = 0;
    while (i < MEMSIZE)
    {
        if ((i % 32 )== 0 && i != 0)
            printf("\n");
        printf("%.2x ", g_mem[i]);
        //printf("gagag");
        i++;
    }
}
void    run()
{
	int	cyc;
    int nr_cycle;
    int alive;
    int juc;

    nr_cycle = 10;
    alive = 1;
    while (nr_cycle >= 0 )
    {
        juc = 0;
        while (++juc <= 5)
        {
            if (g_arr[juc].filename == NULL)
                continue ;
            //printf("\nJUCATOR cu nr %d\n", juc);
            execute_proc(juc);
            if (juc == 2)
                print_mem();
        }
        //alive = decrease_cycle(&nr_cycle);
        nr_cycle--;
    }
}



int     ft_start()
{
//	processes = (t_proc *)malloc(sizeof(t_proc));
//	bzero(processes[0].reg, 16);
//	processes[0].pc = 0;
//	processes[0].ip = 0;
//	processes[0].carry = 1;
//	processes[0].live = 0;
//
//
//	nr_proc = 1;
//	a[0] = 0x0b;
//	a[1] = 0x68;
//	a[2] = 0x01;
//	a[3] = 0x00;
//	a[4] = 0x00;
//	a[5] = 0x00;
//	a[6] = 0x13;
//	a[7] = 0x00;
//	a[8] = 0x00;
//	a[9] = 0x00;
//	a[10] = 0x01;
//	a[11] = 0x06;
//	a[12] = 0x64;
//	a[13] = 0x01;
//	a[14] = 0x00;
//	a[15] = 0x00;
//	a[16] = 0x00;
//	a[17] = 0x00;
//	a[18] = 0x01;
//	a[19] = 0x01;
//	a[20] = 0x80;
//	a[21] = 0x00;
//	a[22] = 0x00;
//	a[23] = 0x00;
//	a[24] = 0x01;
//	a[25] = 0x09;
//	a[26] = 0x80;
//	a[27] = 0xff;
//	a[28] = 0xff;
//	a[29] = 0xff;
//	a[30] = 0xfa;
    //bzero(g_mem, 4096);
    print_mem();
    run();
    return (0);
}
