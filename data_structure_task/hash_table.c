#include<stdio.h>
#define N (2 << 11)

unsigned int gain_num=0, fail_num=0;
unsigned int gain_time=0, fail_time=0;

struct elem
{
    unsigned int value;
    char act;       //1 have been inserted
    char tomb;      //1 have been inserted and deleted
} H[N]={{0,0,0}};
unsigned int char2int()
{
    unsigned int num = 0;
    char c;
    c = getchar();
    while (c < 58 && c > 47)
    {
        num = num * 10 + c - 48;
        c = getchar();
    }
    return num;
}

unsigned short Hash(unsigned int value)
{
    unsigned long long storage=value*value;
    return (unsigned short)((storage >> 26) % N);
}

unsigned short Prob(unsigned int x)
{
    if (x % 2)
        return (unsigned short) (x % N);
    else
        return (unsigned short) ((x + 1) % N);
}

void Insert(unsigned int value)
{
    unsigned short temp, record, hash=Hash(value), prob=Prob(value);
    for (temp = 0; temp < N; temp++)
    {
        record = (hash + temp * prob) % N;
        if (H[record].act == 0 || H[record].tomb == 1)
        {
            H[record].value = value;
            H[record].act = 1;
            H[record].tomb = 0;
            printf("%u %u\n", temp + 1, record);
            return ;
        }
    }
}

void Search(unsigned int value)
{
    unsigned short temp, record, hash=Hash(value), prob=Prob(value);
    for (temp = 0; temp < N; temp++)
    {
        record = (hash + temp * prob) % N;
        if (H[record].act == 0)         //fail
        {
            printf("%u %d\n", temp + 1, -1);
            fail_num++;
            fail_time += temp +1;
            return ;
        }
        else if (H[record].act == 1 && H[record].tomb == 0 && H[record].value == value)        //gain
        {
            H[record].tomb = 1;
            printf("%u %u\n", temp + 1, record);
            gain_num++;
            gain_time += temp + 1;
            return ;
        }
    }
}

int main()
{
    unsigned int num;
    int count;
    for (count = 0; count <10000; count ++)
    {
        num = char2int();
        if (char2int())
            Insert(num);
        else
            Search(num);
    }
    printf("%u/%u\n", gain_time, gain_num);
    printf("%u/%u", fail_time, fail_num);
}


