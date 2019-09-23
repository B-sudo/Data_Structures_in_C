#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define TRUE              1   //真
#define FALSE           0   //假
#define OK            1   //算法正常完成
#define ERROR           0   //算法执行出错
#define INFEASIBLE      -1  //算法不可实现
#define SEQ_OVERFLOW    -2  //多项式序号溢出

typedef int STATUS;

typedef struct polyn_node *polyn_ptr;

struct polyn_node
{
    double coeff;
    int exp;
    polyn_ptr next;
};

typedef polyn_ptr POLYN;

POLYN polyn[10];
int command;
FILE *fin = NULL, *fout = NULL;

polyn_ptr MakeNode()                //分配节点空间
{
    return (polyn_ptr)malloc(sizeof(struct polyn_node));
}

STATUS is_proper_seq(int seq)       //验证序号是否在0到9之间
{
    if (seq < 0 || seq > 9)
        return SEQ_OVERFLOW;
    else
        return TRUE;
}

STATUS is_null(int seq)             //验证多项式指针是否为空
{
    if (is_proper_seq(seq) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    else if (polyn[seq] == NULL)
        return TRUE;
    else
        return FALSE;
}

STATUS is_empty(int seq)            //验证多项式是否为空
{
    if (is_proper_seq(seq) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    else if (is_null(seq) == TRUE)  //不推荐验证空指针
        return ERROR;
    else if (polyn[seq]->next == NULL)
        return TRUE;
    else
        return FALSE;
}

STATUS ShrinkPolyn(int seq)         //将多项式中系数为0的节点删除
{
    polyn_ptr hook, destroyer;
    if (is_proper_seq(seq) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(seq) == TRUE)
        return ERROR;
    hook = polyn[seq];
    while (hook->next != NULL)
    {
        if (hook->next->coeff == 0.0)
        {
            destroyer = hook->next;
            hook->next = destroyer->next;
            free(destroyer);
        }
        else
            hook = hook->next;
    }
    return OK;
}

STATUS InsertPolyn(POLYN P, double coeff, int exp)      //将系数为coeff 幂为exp的项 插入多项式P中 用于创建多项式
{
    polyn_ptr hook, catch;
    if (P == NULL)
        return ERROR;
    hook = P;
    while (hook != NULL)
    {
        if (hook->next == NULL)
        {
            hook = hook->next = MakeNode();
            hook->coeff = coeff;
            hook->exp = exp;
            hook->next = NULL;
            return OK;
        }
        else if (hook->next->exp == exp)
        {
            hook = hook->next;
            hook->coeff += coeff;
            return OK;
        }
        else if (hook->next->exp < exp)
        {
            catch = MakeNode();
            catch->coeff = coeff;
            catch->exp = exp;
            catch->next = hook->next;
            hook->next = catch;
            return OK;
        }
        else
            hook = hook->next;
    }
    return ERROR;
}

STATUS CreatePolyn(int seq)         //seq处创立多项式，包含头结点
{
    double coeff;
    int exp;
    polyn_ptr hook;
    if (is_proper_seq(seq) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (fin == NULL || fout == NULL)
        return ERROR;
    if (DestroyPolyn(seq) != OK)
        return ERROR;
    polyn[seq] = MakeNode();
    polyn[seq]->coeff = 0.0;
    polyn[seq]->exp = 0;
    polyn[seq]->next = NULL;
    fscanf(fin, "%lf %d", &coeff, &exp);
    while (coeff != 0.0 || exp != 0)
    {
        if (exp < 0)
            return ERROR;
        if (InsertPolyn(polyn[seq], coeff, exp) == OK)
            fscanf(fin, "%lf %d", &coeff, &exp);
        else 
            return ERROR;
    }
    if (ShrinkPolyn(seq) == OK)
        return OK;
    else
        return ERROR;
}

STATUS PrintPolyn(int seq)          //对未创建多项式以及空多项式 输出NULL 其余降幂排列， 小数保留四位 TODO 修正为文件输出
{
    polyn_ptr hook;
    if (is_proper_seq(seq) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(seq) == TRUE)
    {
        printf("NULL\n");
        return OK;
    }
    if (is_empty(seq) == TRUE)
    {
        printf("0\n");
        return OK;
    }
    else
    {
        hook = polyn[seq]->next;
        while (hook != NULL)
        {
            printf("%.4lf", hook->coeff);
            if (hook->exp == 0) ;
            else if (hook->exp == 1)
                printf("x");
            else
                printf("x^%d", hook->exp);
            if (hook->next != NULL)
                printf("+");
            hook = hook->next;
        }
        printf("\n");
        return OK;
    }
    
}

STATUS CopyPolyn(int seq_m, int seq_s)          //将存在的多项式seq_m复制到seq_s处，不论seq_s多项式存在与否
{
    polyn_ptr hook, coper;
    if (is_proper_seq(seq_m) == SEQ_OVERFLOW || is_proper_seq(seq_s) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(seq_m) == TRUE)
        return ERROR;
    if (DestroyPolyn(seq_s) == OK)
    {
        hook = polyn[seq_m];
        coper = polyn[seq_s] = MakeNode();
        coper->coeff = 0.0; 
        coper->next = 0;
        coper->next = NULL;
        while (hook->next != NULL)
        {
            coper->next = MakeNode();
            hook = hook->next;
            coper = coper->next;
            coper->coeff = hook->coeff;
            coper->exp = hook->exp;
            coper->next = NULL;
        }
        return OK;
    }
    else
        return ERROR;
}

STATUS AddPolyn(int add_1, int add_2, int sum)          //将存在的两个多项式相加，和存储至sum序号   
{
    polyn_ptr hook_1, hook_2, hook_sum;
    POLYN SUM;
    if (is_proper_seq(add_1) == SEQ_OVERFLOW || is_proper_seq(add_2) == SEQ_OVERFLOW || is_proper_seq(sum) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(add_1) == TRUE || is_null(add_2) == TRUE)
        return ERROR;
    SUM = MakeNode();
    SUM->coeff = 0.0;
    SUM->exp = 0;
    SUM->next = NULL;
    hook_1 = polyn[add_1]->next;
    hook_2 = polyn[add_2]->next;
    hook_sum = SUM;
    while (hook_1 != NULL && hook_2 != NULL)
    {
        if (hook_1->exp > hook_2->exp)
        {
            hook_sum = hook_sum->next = MakeNode();
            hook_sum->coeff = hook_1->coeff;
            hook_sum->exp = hook_1->exp;
            hook_sum->next = NULL;
            hook_1 = hook_1->next;
        }
        else if (hook_2->exp > hook_1->exp)
        {
            hook_sum = hook_sum->next = MakeNode();
            hook_sum->coeff = hook_2->coeff;
            hook_sum->exp = hook_2->exp;
            hook_sum->next = NULL;
            hook_2 = hook_2->next;
        }
        else
        {
            if (hook_1->coeff + hook_2->coeff != 0)
            {
                hook_sum = hook_sum->next = MakeNode();
                hook_sum->coeff = hook_1->coeff + hook_2->coeff;
                hook_sum->exp = hook_1->exp;
                hook_sum->next = NULL;
            }
            hook_1 = hook_1->next;
            hook_2 = hook_2->next;
        }
    }
    if (hook_1 == NULL)
    {
        while (hook_2 != NULL)
        {
            hook_sum = hook_sum->next = MakeNode();
            hook_sum->coeff = hook_2->coeff;
            hook_sum->exp = hook_2->exp;
            hook_sum->next = NULL;
            hook_2 = hook_2->next;
        }
    }
    else
    {
        while (hook_1 != NULL)
        {
            hook_sum = hook_sum->next = MakeNode();
            hook_sum->coeff = hook_1->coeff;
            hook_sum->exp = hook_1->exp;
            hook_sum->next = NULL;
            hook_1 = hook_1->next;
        }
    }
    if (DestroyPolyn(sum) == OK)
    {
        polyn[sum] = SUM;
        return OK;
    }
    else
        return ERROR;
}

STATUS SubtractPolyn(int minuend, int subtractor, int errand)   //将存在的两个多项式相减，和存储至errand序号    
{
    polyn_ptr hook_1, hook_2, hook_errand;
    POLYN ERRAND;
    if (is_proper_seq(minuend) == SEQ_OVERFLOW || is_proper_seq(subtractor) == SEQ_OVERFLOW || is_proper_seq(errand) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(minuend) == TRUE || is_null(subtractor) == TRUE)
        return ERROR;
    ERRAND = MakeNode();
    ERRAND->coeff = 0.0;
    ERRAND->exp = 0;
    ERRAND->next = NULL;
    hook_1 = polyn[minuend]->next;
    hook_2 = polyn[subtractor]->next;
    hook_errand = ERRAND;
    while (hook_1 != NULL && hook_2 != NULL)
    {
        if (hook_1->exp > hook_2->exp)
        {
            hook_errand = hook_errand->next = MakeNode();
            hook_errand->coeff = hook_1->coeff;
            hook_errand->exp = hook_1->exp;
            hook_errand->next = NULL;
            hook_1 = hook_1->next;
        }
        else if (hook_2->exp > hook_1->exp)
        {
            hook_errand = hook_errand->next = MakeNode();
            hook_errand->coeff = -hook_2->coeff;
            hook_errand->exp = hook_2->exp;
            hook_errand->next = NULL;
            hook_2 = hook_2->next;
        }
        else
        {
            if (hook_1->coeff - hook_2->coeff != 0)
            {
                hook_errand = hook_errand->next = MakeNode();
                hook_errand->coeff = hook_1->coeff - hook_2->coeff;
                hook_errand->exp = hook_1->exp;
                hook_errand->next = NULL;
            }
            hook_1 = hook_1->next;
            hook_2 = hook_2->next;
        }
    }
    if (hook_1 == NULL)
    {
        while (hook_2 != NULL)
        {
            hook_errand = hook_errand->next = MakeNode();
            hook_errand->coeff = -hook_2->coeff;
            hook_errand->exp = hook_2->exp;
            hook_errand->next = NULL;
            hook_2 = hook_2->next;
        }
    }
    else
    {
        while (hook_1 != NULL)
        {
            hook_errand = hook_errand->next = MakeNode();
            hook_errand->coeff = hook_1->coeff;
            hook_errand->exp = hook_1->exp;
            hook_errand->next = NULL;
            hook_1 = hook_1->next;
        }
    }
    if (DestroyPolyn(errand) == OK)
    {
        polyn[errand] = ERRAND;
        return OK;
    }
    else
        return ERROR;
}

STATUS MutiplePolyn(int mtpler_1, int mtpler_2, int amass)      //相乘， 存储至amass 利用ShrinkPolyn将0系数节点删除
{
    polyn_ptr hook_1, hook_2, hook_amass, catch;
    POLYN AMASS;
    double _coeff;
    int _exp;
    if (is_proper_seq(mtpler_1) == SEQ_OVERFLOW || is_proper_seq(mtpler_2) == SEQ_OVERFLOW || is_proper_seq(amass) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(mtpler_1) == TRUE || is_null(mtpler_2) == TRUE)
        return ERROR;
    AMASS = MakeNode();
    AMASS->coeff = 0.0;
    AMASS->exp = 0;
    AMASS->next = NULL;
    hook_1 = polyn[mtpler_1]->next;
    hook_2 = polyn[mtpler_2]->next;
    hook_amass = AMASS;
    while (hook_1 != NULL)
    {
        while (hook_2 != NULL)
        {
            _coeff = hook_1->coeff * hook_2->coeff;
            _exp = hook_1->exp + hook_2->exp;
            if (hook_amass->next == NULL)
            {
                hook_amass = hook_amass->next = MakeNode();
                hook_amass->coeff = _coeff;
                hook_amass->exp = _exp;
                hook_amass->next = NULL;
                hook_2 = hook_2->next;
            }
            else if (hook_amass->next->exp < _exp)
            {
                catch = MakeNode();
                catch->coeff = _coeff;
                catch->exp = _exp;
                catch->next = hook_amass->next;
                hook_amass->next = catch;
                hook_2 = hook_2->next;
            }
            else if (hook_amass->next->exp = _exp)
            {
                hook_amass = hook_amass->next;
                hook_amass->coeff += _coeff;
                hook_2 = hook_2->next;
            }
            else
                hook_amass = hook_amass->next;            
        }
        hook_amass = AMASS;
        hook_2 = polyn[mtpler_2]->next;
        hook_1 = hook_1->next;
    }
    if (DestroyPolyn(amass) == OK)
    {
        polyn[amass] = AMASS;
        return OK;
    }
    else
        return ERROR;
    if (ShrinkPolyn(amass) == OK)
        return OK;
    else
        return ERROR;
}

STATUS CalculatePolyn(int seq, double value)            //求值运算， 保留四位小数   TODO 浮点数表示误差 修正为文件输出
{
    double outcome = 0.0;
    polyn_ptr hook;
    if (is_proper_seq(seq) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(seq) == TRUE)
        return ERROR;
    hook = polyn[seq]->next;
    while (hook != NULL)
    {
        outcome += hook->coeff * pow(value, hook->exp);
        hook = hook->next;
    }
    printf("%.4lf\n", outcome);
    return OK;
}

STATUS DestroyPolyn(int seq)        //允许对空指针操作，将整个多项式摧毁
{
    if (is_proper_seq(seq) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(seq) == TRUE)
        return OK;
    if (EmptyPolyn(seq) == OK)
    {
        free(polyn[seq]);
        polyn[seq] = NULL;
        return OK;
    }
    else
        return ERROR;
}

STATUS EmptyPolyn(int seq)          //对非空指针处理，清空多项式
{
    polyn_ptr hook, destroyer;
    if (is_proper_seq(seq) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(seq) == TRUE)
        return ERROR;
    hook = polyn[seq]->next;
    polyn[seq]->next = NULL;
    while (hook != NULL)
    {
        destroyer = hook;
        hook = hook->next;
        free(destroyer);
    }
    return OK;
}

STATUS DiffPolyn(int seq, int outcome)          //微分， 结果存储至outcome
{
    polyn_ptr hook_seq, hook_outcome;
    POLYN OUTCOME;
    if (is_proper_seq(seq) == SEQ_OVERFLOW || is_proper_seq(outcome) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(seq) == TRUE)
        return ERROR;
    OUTCOME = MakeNode();
    OUTCOME->coeff = 0.0;
    OUTCOME->exp = 0;
    OUTCOME->next = NULL;
    hook_outcome = OUTCOME;
    hook_seq = polyn[seq]->next;
    while (hook_seq != NULL)
    {
        if (hook_seq->exp == 0) ;
        else 
        {
            hook_outcome = hook_outcome->next = MakeNode();
            hook_outcome->coeff = hook_seq->coeff * (double)hook_seq->exp;
            hook_outcome->exp = hook_seq->exp - 1;
            hook_outcome->next = NULL;
        }
        hook_seq = hook_seq->next;
    }
    if (DestroyPolyn(outcome) == OK)
    {
        polyn[outcome] = OUTCOME;
        return OK;
    }
    else
        return ERROR;
}

STATUS Inf_IntegPolyn(int seq, int outcome)     //不定积分，结果存储至outcome，无常数C
{
    polyn_ptr hook_seq, hook_outcome;
    POLYN OUTCOME;
    if (is_proper_seq(seq) == SEQ_OVERFLOW || is_proper_seq(outcome) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(seq) == TRUE)
        return ERROR;
    OUTCOME = MakeNode();
    OUTCOME->coeff = 0.0;
    OUTCOME->exp = 0;
    OUTCOME->next = NULL;
    hook_outcome = OUTCOME;
    hook_seq = polyn[seq]->next;
    while (hook_seq != NULL)
    {
        hook_outcome = hook_outcome->next = MakeNode();
        hook_outcome->coeff = hook_seq->coeff / (double)(hook_seq->exp + 1);
        hook_outcome->exp = hook_seq->exp + 1;
        hook_outcome->next = NULL;        
        hook_seq = hook_seq->next;
    }
    if (DestroyPolyn(outcome) == OK)
    {
        polyn[outcome] = OUTCOME;
        return OK;
    }
    else
        return ERROR;
}

STATUS Def_IntegPolyn(int seq, double x1, double x2)            //定积分 计算从x1到x2的积分值   TODO 修正为文件输出
{
    double outcome = 0.0;
    polyn_ptr hook;
    if (is_proper_seq(seq) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(seq) == TRUE)
        return ERROR;
    hook = polyn[seq]->next;
    while (hook != NULL)
    {
        outcome += hook->coeff / (hook->exp + 1) * (pow(x2, hook->exp + 1) - pow(x1, hook->exp + 1));
        hook = hook->next;
    }
    printf("%.4lf\n", outcome);
    return OK;    
}

STATUS Com_DivisPolyn(int mem_1, int mem_2, int sub)
{}

STATUS Com_MutiplePolyn(int mtpler_1, int mtpler_2, int amass)
{}

STATUS DivisPolyn(int Dividend, int divisor, int factor)
{}

STATUS ModPolyn(int Dividend, int divisor, int remainder)
{}

STATUS InvolPolyn(int seq, int power, int outcome)
{}

void main()
{
    int seq_1, seq_2, seq_3, value;
    fin = fopen("polyn.in", "r");
    fout = fopen("polyn.out", "w");
    if (fin == NULL)
        return;
    fscanf(fin, "%d", &command);
    while (command != 0)
    {
        switch (command)
        {
        case 1:
            fscanf(fin, "%d", &seq_1);
            CreatePolyn(seq_1);
            break;
        case 2:
            fscanf(fin, "%d", &seq_1);
            PrintPolyn(seq_1);
            break;
        case 3:
            fscanf(fin, "%d %d", &seq_1, &seq_2);
            CopyPolyn(seq_1, seq_2);
            break;
        case 4:
            fscanf(fin, "%d %d %d", &seq_1, &seq_2, &seq_3);
            AddPolyn(seq_1, seq_2, seq_3);
            break;
        case 5:
            fscanf(fin, "%d %d %d", &seq_1, &seq_2, &seq_3);
            SubtractPolyn(seq_1, seq_2, seq_3);
            break;
        case 6:
            fscanf(fin, "%d %d %d", &seq_1, &seq_2, &seq_3);
            MutiplePolyn(seq_1, seq_2, seq_3);
            break;
        case 7:
            fscanf(fin, "%d %d", &seq_1, &value);
            CalculatePolyn(seq_1, value);
            break;
        case 8:
            fscanf(fin, "%d", &seq_1);
            DestroyPolyn(seq_1);
            break;
        case 9:
            fscanf(fin, "%d", &seq_1);
            EmptyPolyn(seq_1);
            break;
        case 10:
            fscanf(fin, "%d %d", &seq_1, &seq_2);
            DiffPolyn(seq_1, seq_2);
            break;
        case 11:
            fscanf(fin, "%d %d", &seq_1, &seq_2);
            Inf_IntegPolyn(seq_1, seq_2);
            break;
        case 12:
            fscanf(fin, "%d %d %d", &seq_1, &seq_2, &seq_3);
            Def_IntegPolyn(seq_1, seq_2, seq_3);
            break;
        default:
            break;
        }
        fscanf(fin, "%d", &command);
    }
    fclose(fin);
    fclose(fout);
}