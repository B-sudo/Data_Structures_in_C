#include<stdio.h>
#include<time.h>
#define N 500

int table[N+1];

int char2int()
{
    int num = 0;
    char c;
    c = getchar();
    while (c < 58 && c > 47)
    {
        num = num * 10 + c - 48;
        c = getchar();
    }
    return num;
}

void Init(int * table)
{
    int count;
    for (count = 1; count < N + 1; count++ )
        table[count] = char2int();
}

void AccreSort(int * table, int accre, int base)
{
    int num=0, temp=base;
    while (!(temp > N))
    {
        if (temp == base || table[temp] >= table[temp - accre]) temp += accre;
        else
        {
            table[0] = table[temp];
            num = temp - accre;
            while (table[num] > table[0] && num >= base)
            {
                table[num + accre] = table[num];
                num -= accre;
            }
            table[num + accre] = table[0];
            temp += accre;
        }
    }
}

void ShellSort(int * table, int accre)
{
    int base;
    for (base = 0; base < accre; base++)
        AccreSort(table, accre, base);
}

int main()
{
    int count;
    Init(table);
    ShellSort(table, 16);
    ShellSort(table, 8);
    ShellSort(table, 4);
    ShellSort(table, 2);
    ShellSort(table, 1);
    for (count = 1; count < N; count++)
        printf("%d,", table[count]);
    printf("%d", table[N]);
}