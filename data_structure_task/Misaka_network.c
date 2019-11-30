#include<stdio.h>

typedef struct node_info
{
    char firstaff;
    char aff;           //1 affected 0 not
}node_info;

typedef struct Network 
{
    int node_num;
    int aff_num;
    node_info node[1501];
    char arcs[1501][1501];
}Network;

Network G;

int affected_num, outcome=-1, kill=-1;
//FILE *fin = NULL, *fout = NULL;
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

void Init()
{
    int row, rank, count;
    G.node_num = char2int();
    for (row = 0; row < G.node_num; row++)
    {
        for (rank = 0; rank < G.node_num; rank++)
        {
            G.arcs[row][rank] = getchar() - 48;
            getchar();
        }
        G.node[row].aff = G.node[row].firstaff = 0;
    }
    G.aff_num = char2int();
    for (count = 0; count < G.aff_num; count++)
    {
        G.node[char2int()].firstaff = 1;
    }
}

int Traverse(int order)
{
    int count, affected = 0;
    for (count = 0; count < G.node_num; count++)
    {
        if (G.arcs[order][count] == 1 && G.node[count].aff == 0 && count != kill)
        {
            G.node[count].aff = 1;
            affected++;
            affected += Traverse(count);
        }
    }
    return affected;
}

int Affect(){
    int num, affected=G.aff_num;
    for (num = 0; num < G.node_num; num++)
    {
        if (G.node[num].firstaff == 1 && num != kill)
        {    
            G.node[num].aff = 1;
        }
        else 
            G.node[num].aff = 0;
    }
    for (num = 0; num < G.node_num; num++)
    {
        if (G.node[num].firstaff == 1 && num != kill)       //affected
        {
            affected += Traverse(num);
        }
    }
    return affected;
}

int main()
{
    int count, num;
    //fin = fopen("polyn.in", "r");
    //fout = fopen("polyn.out", "w"); //TOBE changed

    Init();
    affected_num = Affect();
    for (count = 0; count < G.node_num; count++)
    {
        if (G.node[count].firstaff == 1)
        {
            kill = count;
            num = Affect();
            if (num < affected_num)
            {
                affected_num = num;
                outcome = kill;
            }
        }
    }
    printf("%d", outcome);
    //fclose(fin);
    //fclose(fout);
}
