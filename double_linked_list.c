#include<stdio.h>
#include<stdlib.h>

typedef struct node *node_ptr;
struct node{
    node_ptr pre;
    int element;
    node_ptr next;
};

typedef node_ptr LIST;
typedef node_ptr position;

void create_list(LIST L){
    int num;
    position p=L;
    printf("\ninput a number with 0 to quit");
    scanf("%d",&num);
    while(num!=0){
        p->next=(node_ptr)malloc(sizeof(struct node));
        p->next->next=NULL;
        p->next->element=num;
        p->next->pre=p;
        p=p->next;
        printf("\ninput a number with 0 to quit");
        scanf("%d",&num);
    }
    printf("\nlist has been created\n");
}

void print_list(LIST L){
    position p=L;
    if(L->next==NULL) {
        printf("empty list");
        return;
    }
    while(p->next!=NULL){
        p=p->next;
        printf("\t%d",p->element);
    }
    printf("\nlist has been printed\n");
}

void swap_list(LIST L,int a){
    int i;
    position p=L;
    if(a<=0||L->next==NULL||L->next->next==NULL){
        printf("\nwrong");
        return;
    }
    for(i=0;i<a;i++){
        p=p->next;
        if(p->next==NULL){
            printf("\noverflow");
            return;
        }
    }
    if(p->next->next==NULL){
        p->pre->next=p->next;
        p->next->pre=p->pre;
        p->pre=p->next;
        p->next->next=p;
        p->next=NULL;

    }
    else{
        p->pre->next=p->next;
        p->next->pre=p->pre;
        p->next=p->next->next;
        p->pre=p->next->pre;
        p->pre->next=p;
        p->next->pre=p;
    }
    printf("\nlist has been swaped\n");
}

int main(){
    int a;
    LIST L;
    L=(node_ptr)malloc(sizeof(struct node));
    L->next=L->pre=NULL;
    L->element=0;
    create_list(L);
    print_list(L);
    printf("\ninput a number to swap with -1 to exit");
    scanf("%d",&a);
    while(a!=-1){
        swap_list(L,a);
        print_list(L);
        printf("\ninput a number to swap with -1 to exit");
        scanf("%d",&a);
    }
}
