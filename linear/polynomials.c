#include<stdio.h>
#include<stdlib.h>

typedef struct node *node_ptr;

struct node{
    int coefficient;
    int exponent;
    node_ptr next;
};

typedef node_ptr POLYNOMIAL;

POLYNOMIAL create_polynomial(){
    POLYNOMIAL POLY=NULL;
    node_ptr p=POLY;
    int degree;
    int num;
    POLY=(node_ptr)malloc(sizeof(struct node));
    POLY->coefficient=POLY->exponent=0;
    POLY->next=NULL;
    p=POLY;
    printf("\nplease input a exponent of the polynomial orderly with -1 to exit");
    scanf("%d",&degree);
    while(degree!=-1){
        p=p->next=(node_ptr)malloc(sizeof(struct node));
        p->exponent=degree;
        p->next=NULL;
        printf("\nplease input the coefficient of the correspondent exponent");
        scanf("%d",&num);
        p->coefficient=num;
        printf("\nplease input a exponent of the polynomial orderly with -1 to exit");
        scanf("%d",&degree);
    }
    return POLY;
}

void print_polynomial(POLYNOMIAL POLY){
    node_ptr p=POLY;
    if(p->next==NULL) {
        printf("\nno polynomial");
        return;
    }
    p=p->next;
    printf("\n%d*x^%d",p->coefficient,p->exponent);
    while(p->next!=NULL){
        p=p->next;
        printf("+%d*x^%d",p->coefficient,p->exponent);
    }
    printf("\nthe polynomial has been printed");
}

POLYNOMIAL add_polynomial(POLYNOMIAL POLY1, POLYNOMIAL POLY2){
    POLYNOMIAL POLY_prod=NULL;
    node_ptr p1=POLY1->next,p2=POLY2->next,p_prod=POLY_prod;
    POLY_prod=(node_ptr)malloc(sizeof(struct node));
    POLY_prod->coefficient=POLY_prod->exponent=0;
    POLY_prod->next=NULL;
    p_prod=POLY_prod;
    while(p1!=NULL&&p2!=NULL){
        if(p1->exponent>p2->exponent){
            p_prod=p_prod->next=(node_ptr)malloc(sizeof(struct node));
            p_prod->exponent=p1->exponent;
            p_prod->coefficient=p1->coefficient;
            p_prod->next=NULL;
            p1=p1->next;
        }
        else if(p1->exponent<p2->exponent){
            p_prod=p_prod->next=(node_ptr)malloc(sizeof(struct node));
            p_prod->exponent=p2->exponent;
            p_prod->coefficient=p2->coefficient;
            p_prod->next=NULL;
            p2=p2->next;
        }
        else{
            p_prod=p_prod->next=(node_ptr)malloc(sizeof(struct node));
            p_prod->exponent=p1->exponent;
            p_prod->coefficient=p1->coefficient+p2->coefficient;
            p_prod->next=NULL;
            p1=p1->next;
            p2=p2->next;
        }
    }
    if(p1==NULL) p_prod->next=p2;
    else p_prod->next=p1;
    return POLY_prod;
}

POLYNOMIAL multiply_polynomial(POLYNOMIAL POLY1, POLYNOMIAL POLY2){
    POLYNOMIAL POLY_prod;
    node_ptr p1=POLY1->next,p2=POLY2->next,p_prod=NULL,hook;
    int m=p1->exponent,n=p2->exponent,max=m+n,count,coeff,exp;
    POLY_prod=(node_ptr)malloc(sizeof(struct node));
    POLY_prod->coefficient=POLY_prod->exponent=0;
    POLY_prod->next=NULL;
    p_prod=POLY_prod;
    while(p1!=NULL){
        while(p2!=NULL){
            coeff=p1->coefficient*p2->coefficient;
            exp=p1->exponent+p2->exponent;
            if(p_prod->next==NULL){
                p_prod=p_prod->next=(node_ptr)malloc(sizeof(struct node));
                p_prod->coefficient=coeff;
                p_prod->exponent=exp;
                p_prod->next=NULL;
                p2=p2->next;
            }
            else if(p_prod->next->exponent<exp){
                hook=p_prod->next;
                p_prod=p_prod->next=(node_ptr)malloc(sizeof(struct node));
                p_prod->next=hook;
                p_prod->exponent=exp;
                p_prod->coefficient=coeff;
                p2=p2->next;
            }
            else if(p_prod->next->exponent==exp){
                p_prod=p_prod->next;
                p_prod->coefficient+=coeff;
                p2=p2->next;
            }
            else{
                p_prod=p_prod->next;
            }
        }
        p_prod=POLY_prod;
        p2=POLY2->next;
        p1=p1->next;
    }
    return POLY_prod;
}
int main(){
    POLYNOMIAL POLY1,POLY2,POLY_prod;
    POLY1=create_polynomial();
    print_polynomial(POLY1);
    POLY2=create_polynomial();
    print_polynomial(POLY2);
    print_polynomial(POLY1);
    POLY_prod=multiply_polynomial(POLY1,POLY2);
    print_polynomial(POLY_prod);
}