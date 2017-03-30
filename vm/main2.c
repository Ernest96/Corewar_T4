#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../corewar.h"
#define CYCLE_TO_DIE			1536
#define CYCLE_DELTA				50



//de testat ldi
// daca primul jucator se deplaseaza negativ

int             aff(t_instr instr, int juc, int procs)
{
    if (instr.acb != 0x40)
        return (1);
    printf("%c", instr.arg1 % 256);
    return(0);
}

int             zjmp(t_instr instr, int juc, int procs)
{
    if (instr.acb != 0x80)
        return (1);
    if (g_arr[juc].proc[procs].carry == 0)
        return (0);
    g_arr[juc].proc[procs].pc = (g_arr[juc].proc[procs].pc + instr.arg1) % MEMSIZE;
    g_arr[juc].proc[procs].ip = g_arr[juc].proc[procs].pc;
    return (0);
}

int         add(t_instr instr, int juc, int procs)
{
    int res;

    if (instr.acb != 0x54)
        return (1);
    if (instr.arg1 <= 0 || instr.arg1 > 16)
        return (1);
    if (instr.arg2 <= 0 || instr.arg2 > 16)
        return (1);
    if (instr.arg3 <= 0 || instr.arg3 > 16)
        return (1);
    res = g_arr[juc].proc[procs].reg[instr.arg1 - 1] + g_arr[juc].proc[procs].reg[instr.arg2 - 1];
    g_arr[juc].proc[procs].reg[instr.arg3 - 1] = res;
    return (0);
}

int         sub(t_instr instr, int juc, int procs)
{
    int res;

    if (instr.acb != 0x54)
        return (1);
    if (instr.arg1 <= 0 || instr.arg1 > 16)
        return (1);
    if (instr.arg2 <= 0 || instr.arg2 > 16)
        return (1);
    if (instr.arg3 <= 0 || instr.arg3 > 16)
        return (1);
    res = g_arr[juc].proc[procs].reg[instr.arg1 - 1] - g_arr[juc].proc[procs].reg[instr.arg2 - 1];
    g_arr[juc].proc[procs].reg[instr.arg3 - 1] = res;
    return (0);
}


int         live(t_instr instr, int juc ,int procs)
{
    if (instr.acb != 0x80)
        return (1);
    if(instr.arg1 <= 0 || instr.arg1 > 4)
        return (1);
    g_arr[juc].proc[procs].live++;
    printf("Live pentru juc %d si proc %d = %d\n" , juc, procs , g_arr[juc].proc[procs].live);

    return (0);
}

int         ld(t_instr instr, int juc, int procs)
{
    if (instr.acb != 0x90 && instr.acb != 0xD0)
        return (1);
    if (instr.arg2 <= 0 || instr.arg2 > 16)
        return (1);
    g_arr[juc].proc[procs].reg[instr.arg2 - 1] = instr.arg1 % IDX_MOD;//poate fi ups
    return (0);
}

int         lld(t_instr instr, int juc, int procs)
{
    if (instr.acb != 0x90 && instr.acb != 0xD0)
        return (1);
    if (instr.arg2 <= 0 || instr.arg2 > 16)
        return (1);
    g_arr[juc].proc[procs].reg[instr.arg2 - 1] = instr.arg1;//poate fi ups
    return (0);
}

int         ldi(t_instr instr, int juc ,int procs)
{
    int result;
    short vasea;
    int val1;
    int val2;
    int val3;

    if (instr.acb >> 6 == 0)
        return (1);
    if ((instr.acb >> 4 & 0x3) == 0 || (instr.acb >> 4 & 0x3) == 3)
        return (1);
    if ((instr.acb >> 2 & 0x3) != 1 )
        return (1);
    if ((instr.acb & 0x3) != 0)
        return (1);
    if (instr.acb >> 6 == 1 && (instr.arg1 <= 0 || instr.arg1 > 16))
        return (1);
    if ((instr.acb >> 4 & 0x3) == 1 && (instr.arg2 <= 0 || instr.arg2 > 16))
        return (1);
    if ((instr.acb >> 2 & 0x3) == 1 && (instr.arg3 <= 0 || instr.arg3 > 16))
        return (1);
    if (instr.acb >> 6 == 1)
        val1 = g_arr[juc].proc[procs].reg[instr.arg1 - 1];
    else if (instr.acb >> 6 == 2)
        val1 = instr.arg1;
    else
    {
        vasea = g_mem[g_arr[juc].proc[procs].pc + (instr.arg1 % IDX_MOD)] << 8;
        vasea = vasea | g_mem[g_arr[juc].proc[procs].pc + (instr.arg1 % IDX_MOD) + 1];
        val1 = vasea;
    }
    if ((instr.acb >> 4 & 0x3) == 1 )
        val2 = g_arr[juc].proc[procs].reg[instr.arg2 - 1];
    else
        val2 = instr.arg2;
    val3 = val1 + val2;
    result = ((unsigned int)g_mem[g_arr[juc].proc[procs].pc + (val3 % IDX_MOD)]) << 24;
    result = result | ((unsigned int)g_mem[g_arr[juc].proc[procs].pc + 1 + (val3 % IDX_MOD)]) << 16;
    result = result | ((unsigned int)g_mem[g_arr[juc].proc[procs].pc + 2  + (val3 % IDX_MOD)]) << 8;
    result = result | ((unsigned int)g_mem[g_arr[juc].proc[procs].pc + 3 + (val3 % IDX_MOD)]);
    g_arr[juc].proc[procs].reg[instr.arg3 - 1] = result;
    return (0);

}

int         lldi(t_instr instr, int juc ,int procs)
{
    int result;
    short vasea;
    int val1;
    int val2;
    int val3;

    if (instr.acb >> 6 == 0)
        return (1);
    if ((instr.acb >> 4 & 0x3) == 0 || (instr.acb >> 4 & 0x3) == 3)
        return (1);
    if ((instr.acb >> 2 & 0x3) != 1 )
        return (1);
    if ((instr.acb & 0x3) != 0)
        return (1);
    if (instr.acb >> 6 == 1 && (instr.arg1 <= 0 || instr.arg1 > 16))
        return (1);
    if ((instr.acb >> 4 & 0x3) == 1 && (instr.arg2 <= 0 || instr.arg2 > 16))
        return (1);
    if ((instr.acb >> 2 & 0x3) == 1 && (instr.arg3 <= 0 || instr.arg3 > 16))
        return (1);
    if (instr.acb >> 6 == 1)
        val1 = g_arr[juc].proc[procs].reg[instr.arg1 - 1];
    else if (instr.acb >> 6 == 2)
        val1 = instr.arg1;
    else
    {
        vasea = g_mem[g_arr[juc].proc[procs].pc + (instr.arg1 % MEMSIZE)] << 8;
        vasea = vasea | g_mem[g_arr[juc].proc[procs].pc + (instr.arg1 % MEMSIZE) + 1];
        val1 = vasea;
    }
    if ((instr.acb >> 4 & 0x3) == 1 )
        val2 = g_arr[juc].proc[procs].reg[instr.arg2 - 1];
    else
        val2 = instr.arg2;
    val3 = val1 + val2;
    result = ((unsigned int)g_mem[g_arr[juc].proc[procs].pc + (val3 % MEMSIZE)]) << 24;
    result = result | ((unsigned int)g_mem[g_arr[juc].proc[procs].pc + 1 + (val3 % MEMSIZE)]) << 16;
    result = result | ((unsigned int)g_mem[g_arr[juc].proc[procs].pc + 2  + (val3 % MEMSIZE)]) << 8;
    result = result | ((unsigned int)g_mem[g_arr[juc].proc[procs].pc + 3 + (val3 % MEMSIZE)]);
    g_arr[juc].proc[procs].reg[instr.arg3 - 1] = result;
    return (0);

}

int         sti(t_instr instr, int juc, int procs)
{
    int res;
    int val1;
    int val2;
    int val3;
    short vasea;

    if(instr.acb >> 6 != 1)
        return(1);
    if(juc == 2)
        vasea = 0;
    if(instr.arg1 <= 0 || instr.arg1 > 16)
        return(1);
    if((instr.acb >> 4 & 0x3) == 0)
        return(1);
    if((instr.acb & 0x3) != 0)
        return(1);
    if((instr.acb >> 2 & 0x3) == 0 || (instr.acb >> 2 & 0x3) == 3)
        return(1);
    if ((instr.acb >> 4 & 0x3) == 1 && (instr.arg2 <= 0 || instr.arg2 > 16))
        return (1);
    if ((instr.acb >> 2 & 0x3) == 1 && (instr.arg3 <= 0 || instr.arg3 > 16))
        return (1);
    if ((instr.acb >> 4 & 0x3) == 1 )
        val1 = g_arr[juc].proc[procs].reg[instr.arg2 - 1];
    else if ((instr.acb >> 4 & 0x03) == 2)
        val1 = instr.arg2;
    else
    {
        vasea = g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD)] << 8;
        vasea = vasea | g_mem[g_arr[juc].proc[procs].pc + ((instr.arg2 + 1) % IDX_MOD)];
        val1 = vasea;
    }
    if ((instr.acb >> 2 & 0x3) == 1 )
        val2 = g_arr[juc].proc[procs].reg[instr.arg3 - 1];
    else
        val2 = instr.arg3;
    val3 = val1 + val2;
    res = g_arr[juc].proc[procs].reg[instr.arg1 - 1];
    g_mem[g_arr[juc].proc[procs].pc + (val3 % IDX_MOD)] = res >> 24 & 0xff;
    g_mem[g_arr[juc].proc[procs].pc + (val3 % IDX_MOD) + 1] =  res >> 16 & 0xff;
    g_mem[g_arr[juc].proc[procs].pc + (val3 % IDX_MOD) + 2] =  res >> 8 & 0xff;
    g_mem[g_arr[juc].proc[procs].pc + (val3 % IDX_MOD) + 3] =  res & 0xff;
    return (0);
}

int         st(t_instr instr, int juc ,int procs)
{
    int val1;

    if (instr.acb != 0x50 && instr.acb != 0x70)
        return (1);
    if (instr.arg1 > 16 || instr.arg1 <= 0)
        return (1);
    if (((instr.acb >> 4 & 0x3) == 1) && (instr.arg2 > 16 || instr.arg2 <= 0))
        return (1);
    if ((instr.acb >> 4 & 0x3) == 1)
    {
        val1 = g_arr[juc].proc[procs].reg[instr.arg2 - 1];
        g_mem[g_arr[juc].proc[procs].pc + (val1 % IDX_MOD)] = g_arr[juc].proc[procs].reg[instr.arg1 -1] >> 24 & 0xff;
        g_mem[g_arr[juc].proc[procs].pc + (val1 % IDX_MOD) + 1] =  g_arr[juc].proc[procs].reg[instr.arg1-1] >> 16 & 0xff;
        g_mem[g_arr[juc].proc[procs].pc + (val1 % IDX_MOD) + 2] =  g_arr[juc].proc[procs].reg[instr.arg1-1] >> 8 & 0xff;
        g_mem[g_arr[juc].proc[procs].pc + (val1 % IDX_MOD) + 3] =  g_arr[juc].proc[procs].reg[instr.arg1-1] & 0xff;
    }
    else
    {
        g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD)] = g_arr[juc].proc[procs].reg[instr.arg1 -1] >> 24 & 0xff;
        g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD) + 1] =  g_arr[juc].proc[procs].reg[instr.arg1-1] >> 16 & 0xff;
        g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD) + 2] =  g_arr[juc].proc[procs].reg[instr.arg1-1] >> 8 & 0xff;
        g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD) + 3] =  g_arr[juc].proc[procs].reg[instr.arg1-1] & 0xff;
    }
    return 0;
}

int             and(t_instr instr, int juc, int procs)
{
    int res;
    int val1;
    int val2;

    if ((instr.acb >> 6) == 0)
        return (1);
    if  ((instr.acb >> 4 & 0x3) == 0)
        return (1);
    if ((instr.acb >> 2 & 0x3) != 1)
        return (1);
    if ((instr.acb & 0x3) != 0)
        return (1);
    if (instr.acb >> 6 == 1 && (instr.arg1 <= 0 || instr.arg1 > 16))
        return (1);
    if ((instr.acb >> 4 & 0x3) == 1 && (instr.arg2 <= 0 || instr.arg2 > 16))
        return (1);
    if ((instr.acb >> 2 & 0x3) == 1 && (instr.arg3 <= 0 || instr.arg3 > 16))
        return (1);
    if (instr.acb >> 6 == 1 )
        val1 = g_arr[juc].proc[procs].reg[instr.arg1 - 1];
    else if (instr.acb >> 6 == 2)
        val1 = instr.arg1;
    else
    {
        val1 = g_mem[g_arr[juc].proc[procs].pc + (instr.arg1 % IDX_MOD)] >> 8;
        val1 = val1 | g_mem[g_arr[juc].proc[procs].pc + ((instr.arg1 + 1) % IDX_MOD)];
    }
    if ((instr.acb >> 4 & 0x3) == 1 )
        val2 = g_arr[juc].proc[procs].reg[instr.arg2 - 1];
    else if (instr.acb >> 4 == 2)
        val2 = instr.arg2;
    else
    {
        val2 = g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD)] >> 8;
        val2 = val2 | g_mem[g_arr[juc].proc[procs].pc + ((instr.arg2 + 1) % IDX_MOD)];
    }
    res = val1 & val2;
    g_arr[juc].proc[procs].reg[instr.arg3 - 1] = res;
    return (1);
}

int             or(t_instr instr, int juc, int procs)
{
    int res;
    int val1;
    int val2;

    if ((instr.acb >> 6) == 0)
        return (1);
    if  ((instr.acb >> 4 & 0x3) == 0)
        return (1);
    if ((instr.acb >> 2 & 0x3) != 1)
        return (1);
    if ((instr.acb & 0x3) != 0)
        return (1);
    if (instr.acb >> 6 == 1 && (instr.arg1 <= 0 || instr.arg1 > 16))
        return (1);
    if ((instr.acb >> 4 & 0x3) == 1 && (instr.arg2 <= 0 || instr.arg2 > 16))
        return (1);
    if ((instr.acb >> 2 & 0x3) == 1 && (instr.arg3 <= 0 || instr.arg3 > 16))
        return (1);
    if (instr.acb >> 6 == 1 )
        val1 = g_arr[juc].proc[procs].reg[instr.arg1 - 1];
    else if (instr.acb >> 6 == 2)
        val1 = instr.arg1;
    else
    {
        val1 = g_mem[g_arr[juc].proc[procs].pc + (instr.arg1 % IDX_MOD)] >> 8;
        val1 = val1 | g_mem[g_arr[juc].proc[procs].pc + ((instr.arg1 + 1) % IDX_MOD)];
    }
    if ((instr.acb >> 4 & 0x3) == 1 )
        val2 = g_arr[juc].proc[procs].reg[instr.arg2 - 1];
    else if (instr.acb >> 4 == 2)
        val2 = instr.arg2;
    else
    {
        val2 = g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD)] >> 8;
        val2 = val2 | g_mem[g_arr[juc].proc[procs].pc + ((instr.arg2 + 1) % IDX_MOD)];
    }
    res = val1 | val2;
    g_arr[juc].proc[procs].reg[instr.arg3 - 1] = res;
    return (1);
}

int             xor(t_instr instr, int juc, int procs)
{
    int res;
    int val1;
    int val2;

    if ((instr.acb >> 6) == 0)
        return (1);
    if  ((instr.acb >> 4 & 0x3) == 0)
        return (1);
    if ((instr.acb >> 2 & 0x3) != 1)
        return (1);
    if ((instr.acb & 0x3) != 0)
        return (1);
    if (instr.acb >> 6 == 1 && (instr.arg1 <= 0 || instr.arg1 > 16))
        return (1);
    if ((instr.acb >> 4 & 0x3) == 1 && (instr.arg2 <= 0 || instr.arg2 > 16))
        return (1);
    if ((instr.acb >> 2 & 0x3) == 1 && (instr.arg3 <= 0 || instr.arg3 > 16))
        return (1);
    if (instr.acb >> 6 == 1)
        val1 = g_arr[juc].proc[procs].reg[instr.arg1 - 1];
    else if (instr.acb >> 6 == 2)
        val1 = instr.arg1;
    else
    {
        val1 = g_mem[g_arr[juc].proc[procs].pc + (instr.arg1 % IDX_MOD)] >> 8;
        val1 = val1 | g_mem[g_arr[juc].proc[procs].pc + ((instr.arg1 + 1) % IDX_MOD)];
    }
    if ((instr.acb >> 4 & 0x3) == 1 )
        val2 = g_arr[juc].proc[procs].reg[instr.arg2 - 1];
    else if (instr.acb >> 4 == 2)
        val2 = instr.arg2;
    else
    {
        val2 = g_mem[g_arr[juc].proc[procs].pc + (instr.arg2 % IDX_MOD)] >> 8;
        val2 = val2 | g_mem[g_arr[juc].proc[procs].pc + ((instr.arg2 + 1) % IDX_MOD)];
    }
    res = val1 ^ val2;
    g_arr[juc].proc[procs].reg[instr.arg3 - 1] = res;
    return (1);
}

int forks(t_instr instr, int juc, int procs)
{
    int j;
    int beg;
    int pr;
    int temp;

    temp = 0;
    if (instr.acb != 0x80)
        return (1);
    j = g_arr[juc].proc[procs].begin;
    beg = g_arr[juc].proc[procs].pc + (instr.arg1 % IDX_MOD);
    pr = ++(g_arr[juc].pr_n) - 1;
    while (j <= g_arr[juc].proc[procs].end)
    {
        g_mem[(beg + temp) % MEMSIZE] = g_mem[j % MEMSIZE];
        temp++;
        j++;
    }
    g_arr[juc].proc[pr].begin = beg;
    g_arr[juc].proc[pr].end = j - 1;
    g_arr[juc].proc[pr].carry =  g_arr[juc].proc[procs].carry;
    j = -1;
    while (++j < 16)
        g_arr[juc].proc[pr].reg[j] = g_arr[juc].proc[procs].reg[j];
    return (0);
}

int lforks(t_instr instr, int juc, int procs)
{
    int j;
    int beg;
    int pr;
    int temp;

    temp = 0;
    if (instr.acb != 0x80)
        return (1);
    j = g_arr[juc].proc[procs].begin;
    beg = g_arr[juc].proc[procs].pc + (instr.arg1 % MEMSIZE);
    pr = ++(g_arr[juc].pr_n) - 1;
    while (j <= g_arr[juc].proc[procs].end)
    {
        g_mem[(beg + temp) % MEMSIZE] = g_mem[j % MEMSIZE];
        temp++;
        j++;
    }
    g_arr[juc].proc[pr].begin = beg;
    g_arr[juc].proc[pr].end = j - 1;
    g_arr[juc].proc[pr].carry =  g_arr[juc].proc[procs].carry;
    j = -1;
    while (++j < 16)
        g_arr[juc].proc[pr].reg[j] = g_arr[juc].proc[procs].reg[j];
    return (0);
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
    else if (code == 4)
        return (add(instr, juc, procs));
    else if (code == 5)
        return (sub(instr, juc, procs));
    else if (code == 6)
        return (and(instr, juc, procs));
    else if (code == 7)
        return (or(instr, juc, procs));
    else if (code == 8)
        return (xor(instr, juc, procs));
    else if (code == 9)
        return (zjmp(instr, juc, procs));
    else if (code == 10)
        return (ldi(instr, juc, procs));
    else if (code == 11)
        return (sti(instr, juc, procs));
    else if (code == 12)
        return (forks(instr, juc, procs));
    else if (code == 13)
        return (lld(instr, juc, procs));
    else if (code == 14)
        return (lldi(instr, juc, procs));
    else if (code == 15)
        return lforks(instr, juc, procs);
    else if (code == 16)
		return (aff(instr, juc, procs));
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
    short vasea;

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
        vasea = ((unsigned short)g_mem[g_arr[juc].proc[procs].ip % MEMSIZE]) << 8;
        vasea= vasea | ((unsigned short)g_mem[g_arr[juc].proc[procs].ip + 1 % MEMSIZE]);
        result = vasea;
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
        if (g_arr[j].proc[i].cycles_to_wait == -1)
        {
            instr.code = g_mem[g_arr[j].proc[i].ip % MEMSIZE];
            g_arr[j].proc[i].cycles_to_wait = g_steps[instr.code];

            continue;
        }
        else if (g_arr[j].proc[i].cycles_to_wait > 0)
        {
            g_arr[j].proc[i].cycles_to_wait--;
            continue;
        }
        instr.code = g_mem[g_arr[j].proc[i].ip % MEMSIZE];
        g_arr[j].proc[i].ip++;
        instr.acb = g_mem[g_arr[j].proc[i].ip % MEMSIZE];
        g_arr[j].proc[i].ip++;
        flag = get_args(&instr, j, i);
        g_arr[j].proc[i].cycles_to_wait = -1;
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
    int         sum;

    i = 0;
    flag = 0;
    while (++i <= 4)
    {
        if (g_arr[i].filename == NULL)
            continue ;
        j = -1;
        sum = 0;
        while (++j < g_arr[i].pr_n)
            sum += g_arr[i].proc[0].live;
        if (sum >= 21)
        {
            (*nr_cycle) -= CYCLE_DELTA;
            printf("Am decrementat\n");
            flag = 1;
        }
        g_arr[i].proc[0].live = 0;
    }
    if (flag == 0)
        count++;
    if (count == 10)
    {
        (*nr_cycle) -= CYCLE_DELTA;
        printf("Am decrementat fortat\n");
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

    nr_cycle = CYCLE_TO_DIE;
    nr_cycle = 100;
    alive = 1;
    while (nr_cycle >= 0)
    {
        juc = 0;
        while (++juc <= 5)
        {
            if (g_arr[juc].filename == NULL)
                continue ;
            execute_proc(juc);
            /*if (juc == 2)
                print_mem();
            printf("\n");*/
        }
        alive = decrease_cycle(&nr_cycle);
        //nr_cycle--;
        printf("{%d}\n", nr_cycle);
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
    printf("\n%zd\n",sizeof(short));
    run();
    return (0);
}
