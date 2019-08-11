#include<stdio.h>
#include<stdlib.h>

typedef struct node *node_ptr;

struct node{
    int elem;
    node_ptr next;
};

typedef node_ptr LIST;
typedef node_ptr position;

LIST SMT_LIST;
LIST CreateList(){
    SMT_LIST = (LIST)malloc(sizeof(struct node));
    SMT_LIST->next = NULL;
    SMT_LIST->elem = 0;
    printf("\nCreated Successfully");
    return SMT_LIST;
}

void InsertList(int x){
    position plug=(position)malloc(sizeof(struct node));
    plug->elem = x;
    plug->next = SMT_LIST->next;
    SMT_LIST->next = plug;
    printf("\nInserted Successfully");
}

position PopList(int x){
    position pre_pop=SMT_LIST, pop;
    while(pre_pop->next!=NULL && pre_pop->next->elem!=x){
        pre_pop = pre_pop->next;
    }

    if(pre_pop->next==NULL){
        printf("\nNot Found");
        return NULL;
    }
    else{
        pop = pre_pop->next;
        pre_pop->next = pop->next;
        printf("\nPopped Successfully");
        return pop;
    }
}

void FindList(int x){
    position pop=PopList(x);
    if(pop==NULL){
        printf("\nNot Found");
    }
    else{
        InsertList(x);
        printf("\nFound Successfully");
    }    
}

void PrintList(){
    position anchor=SMT_LIST;
    while(anchor->next!=NULL){
        printf("\t%d",anchor->next->elem);
        anchor = anchor->next;
    }
    printf("\nPrinted Successfully");
}

void main(){
    CreateList();
    PrintList();
    InsertList(34);
    InsertList(55);
    PrintList();
    InsertList(77);
    InsertList(99);
    InsertList(10);
    PrintList();
    FindList(77);
    PrintList();
    FindList(98);
    PrintList();
}