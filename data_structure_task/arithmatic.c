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

POLYN polyn[10] = {NULL};
int command;
FILE *fin = NULL, *fout = NULL;

polyn_ptr MakeNode()                //分配节点空间 完成初始化
{
    polyn_ptr hook;
    hook = (polyn_ptr)malloc(sizeof(struct polyn_node));
    hook->coeff = 0.0;
    hook->exp = 0;
    hook->next = NULL;
    return hook;
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

void ReversePolyn(POLYN P)       //反序 用于除法 非辗转相除法
{
    polyn_ptr hook, transfer;
    int max_exp;
    if (P == NULL || P->next == NULL)
        return ;
    hook = P->next;
    max_exp = hook->exp;
    hook->exp = 0;
    while (hook->next != NULL)
    {
        transfer = hook->next;
        hook->next = transfer->next;
        transfer->next = P->next;
        P->next = transfer;
        transfer->exp = max_exp - transfer->exp;
    } 
    return ;
}

STATUS ShrinkPolyn(POLYN P)         //将多项式中系数为0的节点删除
{
    polyn_ptr hook, destroyer;
    if (P == NULL)
        return ERROR;
    hook = P;
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
    polyn[seq] = MakeNode();        //创建头结点
    fscanf(fin, "%lf %d", &coeff, &exp); //输入第一对数据
    while (coeff != 0.0 || exp != 0)
    {
        if (exp < 0)
            return ERROR;
        if (InsertPolyn(polyn[seq], coeff, exp) == OK)
            fscanf(fin, "%lf %d", &coeff, &exp);
        else 
            return ERROR;
    }
    if (ShrinkPolyn(polyn[seq]) == OK)
        return OK;
    else
        return ERROR;
}

STATUS PrintPolyn(int seq)          //对未创建多项式以及空多项式 输出NULL 其余降幂排列， 小数保留四位 
{
    polyn_ptr hook;
    if (is_proper_seq(seq) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(seq) == TRUE)
    {
        fprintf(fout, "NULL\n");
        return OK;
    }
    if (is_empty(seq) == TRUE)
    {
        fprintf(fout, "0\n");
        return OK;
    }
    else
    {
        hook = polyn[seq]->next;
        while (hook != NULL)
        {
            fprintf(fout, "%.4lf", hook->coeff);
            if (hook->exp == 0) ;
            else if (hook->exp == 1)
                fprintf(fout, "x");
            else
                fprintf(fout, "x^%d", hook->exp);
            if (hook->next != NULL && hook->next->coeff > 0)
                fprintf(fout, "+");
            hook = hook->next;
        }
        fprintf(fout, "\n");
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

STATUS _AddPolyn(POLYN P_1, POLYN P_2, POLYN SUM)           //重构 P_1 P_2 SUM 需非空
{
    polyn_ptr hook_1, hook_2, hook_sum;
    if (P_1 == NULL || P_2 == NULL || SUM == NULL)
        return ERROR;
    hook_1 = P_1->next;
    hook_2 = P_2->next;
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
    return OK;
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
    if (_AddPolyn(polyn[add_1], polyn[add_2], SUM) != OK)
        return ERROR;
    if (DestroyPolyn(sum) == OK)
    {
        polyn[sum] = SUM;
        return OK;
    }
    else
        return ERROR;
}

POLYN _SubtractPolyn(POLYN MND, POLYN SBR)      //减法核心 多项式存在
{
    if (MND == NULL || SBR == NULL)
        return NULL;
    POLYN ERRAND;
    polyn_ptr hook_1, hook_2, hook_errand;
    ERRAND = MakeNode();
    hook_1 = MND->next;
    hook_2 = SBR->next;
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
            if (hook_1->coeff != hook_2->coeff)
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
    return ERRAND;
}

STATUS SubtractPolyn(int minuend, int subtractor, int errand)   //将存在的两个多项式相减，和存储至errand序号    
{
    polyn_ptr hook_1, hook_2, hook_errand;
    POLYN ERRAND;
    if (is_proper_seq(minuend) == SEQ_OVERFLOW || is_proper_seq(subtractor) == SEQ_OVERFLOW || is_proper_seq(errand) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(minuend) == TRUE || is_null(subtractor) == TRUE)
        return ERROR;
    ERRAND = _SubtractPolyn(polyn[minuend], polyn[subtractor]);
    if (ERRAND == NULL)
        return ERROR;
    if (DestroyPolyn(errand) == OK)
    {
        polyn[errand] = ERRAND;
        return OK;
    }
    else
        return ERROR;
}

POLYN _MutiplePolyn(POLYN M_1, POLYN M_2)         //重构 多项式指针要求非空
{
    POLYN AMASS;
    polyn_ptr hook_1, hook_2, hook_amass, catch;
    double _coeff;
    int _exp;
    if (M_1 == NULL || M_2 == NULL)
        return NULL;
    AMASS = MakeNode();
    hook_1 = M_1->next;
    hook_2 = M_2->next;
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
            else if (hook_amass->next->exp == _exp)
            {
                hook_amass = hook_amass->next;
                hook_amass->coeff += _coeff;
                hook_2 = hook_2->next;
            }
            else
                hook_amass = hook_amass->next;            
        }
        hook_amass = AMASS;
        hook_2 = M_2->next;
        hook_1 = hook_1->next;
    }
    if (ShrinkPolyn(AMASS) != OK)
        return NULL;
    return AMASS;
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
    AMASS = _MutiplePolyn(polyn[mtpler_1], polyn[mtpler_2]);
    if (AMASS == NULL)
        return ERROR;
    if (DestroyPolyn(amass) == OK)
    {
        polyn[amass] = AMASS;
        return OK;
    }
    else
        return ERROR;
    return OK;
}

STATUS CalculatePolyn(int seq, double value)            //求值运算， 保留四位小数   TODO 浮点数表示误差 
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
    fprintf(fout, "%.4lf\n", outcome);
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

STATUS Def_IntegPolyn(int seq, double x1, double x2)            //定积分 计算从x1到x2的积分值   
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
    fprintf(fout, "%.4lf\n", outcome);
    return OK;    
}

POLYN Inverse(POLYN P, int mod)         //  求逆 所有多项式指针非空
{
    POLYN OUTCOME;
    polyn_ptr hook, catch;
    POLYN HALF, HOOK;
    if (P == NULL || P->next == NULL || mod <= 0)
        return NULL;
    catch = P;
    if (mod == 1)           //基本情况  x+5 0.2(mod 1) -0.04x+0.2(mod 2) (-0.04x+0.2)(1+0.04x^2) = -0.0016x^3+0.008x^2--0.04x+0.2
    {
        OUTCOME = MakeNode();
        hook = OUTCOME;
        while (catch->next != NULL)
            catch = catch->next;
        if (catch->exp == 0)
        {
            hook = hook->next = MakeNode();
            hook->exp = 0;
            hook->coeff = 1 / catch->coeff;
            hook->next = NULL;
        }
        return OUTCOME;
    }
    else            
    {
        HALF = Inverse(P, (mod + 1) / 2);            //减半处理 模(mod+1)/2 求逆为HALF
        HOOK = _MutiplePolyn(P, HALF);        
        if (HOOK == NULL)
            return ERROR;
        hook = HOOK;
        if (hook->next == NULL)
        {
            hook = hook->next = MakeNode();
            hook->coeff = 2.0;
            hook->exp = 0;
            hook->next;
        }
        else
        {
            while (hook->next != NULL)
            {
                hook = hook->next;
                hook->coeff = -hook->coeff;
            }
            if (hook->exp == 0)
                hook->coeff += 2;
            else
            {
                hook = hook->next = MakeNode();
                hook->exp = 0;
                hook->coeff = 2.0;
                hook->next = NULL;
            }
        }
        OUTCOME = _MutiplePolyn(HALF, HOOK);
        if (OUTCOME == NULL)
            return NULL;
        hook = OUTCOME;
        while (hook->next != NULL && hook->next->exp > mod - 1)
        {
            catch = hook->next;
            hook->next = catch->next;
            free(catch);
        }             
        return OUTCOME;        
    }
}

POLYN _DivisPolyn(POLYN DND, POLYN DOR)      //除法核心代码
{
    polyn_ptr hook;
    POLYN FACTOR, AL_DOR;
    int mod;
    if (DND == NULL || DOR == NULL)
        return NULL;
    if (DOR->next == NULL)
        return NULL;
    if (DND->next == NULL || DND->next->exp < DOR->next->exp)
        return FACTOR;
    mod = DND->next->exp - DOR->next->exp + 1;
    ReversePolyn(DOR);
    AL_DOR = Inverse(DOR, mod);
    if (AL_DOR == NULL)
        return NULL;
    ReversePolyn(DND);
    FACTOR = _MutiplePolyn(DND, AL_DOR);
    if (FACTOR == NULL)
        return NULL;
    hook = FACTOR->next;
    while (hook != NULL && hook->exp >= mod)
    {
        FACTOR->next = hook->next;
        free(hook);
        hook = FACTOR->next;
    }
    ReversePolyn(FACTOR);
    ReversePolyn(DND);
    ReversePolyn(DOR);
    return FACTOR;
}

STATUS DivisPolyn(int dividend, int divisor, int factor)            //除法 去除余数
{
    POLYN FACTOR;
    if (is_proper_seq(dividend) == SEQ_OVERFLOW || is_proper_seq(divisor) == SEQ_OVERFLOW || is_proper_seq(factor) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(dividend) == TRUE || is_null(divisor) == TRUE)
        return ERROR;
    if (is_empty(divisor) == TRUE)
        return ERROR;
    FACTOR = _DivisPolyn(polyn[dividend], polyn[divisor]);
    if (FACTOR == NULL)
        return ERROR;
    if (DestroyPolyn(factor) == OK)
    {
        polyn[factor] = FACTOR;
        return OK;
    }
    else
        return ERROR;
    return OK;
}

POLYN _ModPolyn(POLYN DND, POLYN DOR)           //基于除法 乘法 减法 实现的模
{
    POLYN RMD, FACTOR;
    polyn_ptr hook;
    int mod;
    if (DND == NULL || DOR == NULL || DOR->next == NULL)
        return NULL;
    mod = DOR->next->exp;
    FACTOR = _DivisPolyn(DND,DOR);
    RMD = _SubtractPolyn(DND, _MutiplePolyn(DOR, FACTOR));
    hook = RMD->next;
    while (hook != NULL && hook->exp >= mod)
    {
        RMD->next = hook->next;
        free(hook);
        hook = RMD->next;
    }
    return RMD;
}

STATUS ModPolyn(int dividend, int divisor, int remainder)       //取模
{
    POLYN RMD;
    if (is_proper_seq(dividend) == SEQ_OVERFLOW || is_proper_seq(divisor) == SEQ_OVERFLOW || is_proper_seq(remainder) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(dividend) == TRUE || is_null(divisor) == TRUE)
        return ERROR;
    if (is_empty(divisor) == TRUE)
        return ERROR;
    RMD = _ModPolyn(polyn[dividend], polyn[divisor]);
    if (RMD == NULL)
        return ERROR;
    if (DestroyPolyn(remainder) == OK)
    {
        polyn[remainder] = RMD;
        return OK;
    }
    else
        return ERROR;
    return OK;
}

POLYN _Com_DivisPolyn(POLYN DND, POLYN DOR)     //利用辗转相除法求最大公因式
{
    POLYN HOOK;
    double norm;
    polyn_ptr hook;
    if (DND == NULL || DOR == NULL)
        return NULL;
    if (DND->next == NULL ||DOR->next == NULL)
        return NULL;
    while (DOR->next != NULL)
    {
        HOOK = _ModPolyn(DND, DOR);
        DND = DOR;
        DOR = HOOK;
    }
    hook = DND;
    if (hook == NULL || hook->next == NULL)
        return ERROR;
    norm = hook->next->coeff;
    while (hook->next != NULL)
    {
        hook = hook->next;
        hook->coeff = hook->coeff / norm;
    }
    return DND;
}

STATUS Com_DivisPolyn(int mem_1, int mem_2, int sub)        //求最大公因式 多项式存在且不可为0
{
    POLYN SUB;
    double norm;
    polyn_ptr hook;
    if (is_proper_seq(mem_1) == SEQ_OVERFLOW || is_proper_seq(mem_2) == SEQ_OVERFLOW || is_proper_seq(sub) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(mem_1) == TRUE || is_null(mem_2) == TRUE)
        return ERROR;
    if (is_empty(mem_1) == TRUE || is_empty(mem_2) == TRUE)
        return ERROR;
    SUB = _Com_DivisPolyn(polyn[mem_1], polyn[mem_2]);
    if (SUB == NULL)
        return ERROR;
    if (DestroyPolyn(sub) == OK)
    {
        polyn[sub] = SUB;
        return OK;
    }
    else
        return ERROR;
    return OK;
}

POLYN _Com_MutiplePolyn(POLYN MTP_1, POLYN MTP_2)
{
    POLYN AMASS, SUB;
    double norm;
    polyn_ptr hook;
    if (MTP_1 == NULL || MTP_2 == NULL)
        return NULL;
    if (MTP_1->next == NULL || MTP_2->next == NULL)
        return NULL;
    SUB = _Com_DivisPolyn(MTP_1, MTP_2);
    AMASS = _DivisPolyn(_MutiplePolyn(MTP_1, MTP_2), SUB);
    hook = AMASS;
    if (hook == NULL || hook->next == NULL)
        return ERROR;
    norm = hook->next->coeff;
    while (hook->next != NULL)
    {
        hook = hook->next;
        hook->coeff = hook->coeff / norm;
    }
    return AMASS;
}
STATUS Com_MutiplePolyn(int mtpler_1, int mtpler_2, int amass)      //求最小公倍式，多项式存在且不可为0
{
    POLYN AMASS;
    if (is_proper_seq(mtpler_1) == SEQ_OVERFLOW || is_proper_seq(mtpler_2) == SEQ_OVERFLOW || is_proper_seq(amass) == SEQ_OVERFLOW)
        return SEQ_OVERFLOW;
    if (is_null(mtpler_1) == TRUE || is_null(mtpler_2) == TRUE)
        return ERROR;
    if (is_empty(mtpler_1) == TRUE || is_empty(mtpler_2) == TRUE)
        return ERROR;
    AMASS = _Com_MutiplePolyn(polyn[mtpler_1], polyn[mtpler_2]);
    if (AMASS == NULL)
        return ERROR;
    if (DestroyPolyn(amass) == OK)
    {
        polyn[amass] = AMASS;
        return OK;
    }
    else
        return ERROR;
    return OK;
}

STATUS InvolPolyn(int seq, int power, int outcome)
{}

void main()
{
    int seq_1, seq_2, seq_3, value;
    fin = fopen("test.in", "r");
    fout = fopen("test.out", "w"); //TOBE changed
    if (fin == NULL || fout == NULL)
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
            fscanf(fin, "%d", &seq_1);
            fscanf(fin, "%d", &value);
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
            fscanf(fin, "%d", &seq_1);
            fscanf(fin, "%d", &seq_2);
            fscanf(fin, "%d", &seq_3);
            Def_IntegPolyn(seq_1, seq_2, seq_3);
            break;
        case 13:
            fscanf(fin, "%d", &seq_1);
            fscanf(fin, "%d", &seq_2);
            fscanf(fin, "%d", &seq_3);
            Com_DivisPolyn(seq_1, seq_2, seq_3);
            break;
        case 14:
            fscanf(fin, "%d", &seq_1);
            fscanf(fin, "%d", &seq_2);
            fscanf(fin, "%d", &seq_3);
            Com_MutiplePolyn(seq_1, seq_2, seq_3);
            break;
        case 15:
            fscanf(fin, "%d %d %d", &seq_1, &seq_2, &seq_3);
            DivisPolyn(seq_1, seq_2, seq_3);
            break;
        case 16:
            fscanf(fin, "%d %d %d", &seq_1, &seq_2, &seq_3);
            ModPolyn(seq_1, seq_2, seq_3);
            break;
        default:
            break;
        }
        fscanf(fin, "%d", &command);
    }
    fclose(fin);
    fclose(fout);
}