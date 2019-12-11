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
    for (count = 1; count < N+1; count++ )
        table[count] = char2int();
}

void change(int * table, int a, int b)
{
    table[0] = table[a];
    table[a] = table[b];
    table[b] = table[0];
}

void CombSort(int * table)
{  
    int top = N, temp;
    while (top != 0)
    {
        if (2 * top > N)
            top--;
        else if (2 * top == N)
        {
            if (table[top] < table[2 * top])
                change(table, top, 2 * top);
            top--;
        }
        else
        {
            temp = top;
            while (temp * 2 + 1<= N)
            {
                temp = table[2 * temp] > table[2 * temp + 1] ? 2 * temp : 2 * temp + 1;
                if (table[temp] > table[temp / 2])
                    change(table, temp, temp / 2);
                else
                    break;
            }
            if (temp * 2 == N)
            {
                if (table[temp] < table[2 * temp])
                    change(table, temp, 2 * temp);
            }
            top--;
        }     
    }    
}

void Comb(int * table, int bottom)
{
    int temp=1;
    while (temp * 2 + 1 <= bottom)
    {
        temp = table[2 * temp] > table[2 * temp + 1] ? 2 * temp : 2 * temp + 1;
        if (table[temp] > table[temp / 2])
            change(table, temp, temp / 2);
        else
            break;
    }
    if (temp * 2 == bottom)
    {
        if (table[temp] < table[2 * temp])
            change(table, temp, 2 * temp);
    }
}

void HeapSort(int * table)
{
    int bottom=N;
    CombSort(table);
    while (bottom != 1)
    {
        change(table, 1, bottom);
        Comb(table, --bottom);
    }
}

int main()
{
    int count;
    Init(table);
    HeapSort(table);
    for (count = 1; count < N; count++)
        printf("%d,", table[count]);
    printf("%d", table[N]);
}