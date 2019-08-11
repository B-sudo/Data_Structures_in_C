#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define N 40

typedef struct node *node_ptr;
struct node{
    int coeff;
    node_ptr next;
};

void pass(int* a,int div){
    node_ptr bucket[10]={NULL};
    node_ptr p[10]={NULL};
    int i,num;
    for(i=0;i<10;i++){
        bucket[i]=(node_ptr)malloc(sizeof(struct node));
        p[i]=bucket[i];
        p[i]->coeff=0;
        p[i]->next=NULL;
    }
    for(i=0;i<N;i++){
        num=(a[i]/div)%10;
        p[num]=p[num]->next=(node_ptr)malloc(sizeof(struct node));
        p[num]->coeff=a[i];
        p[num]->next=NULL;
    }
    i=0;
    for(num=0;num<10;num++){
        while(bucket[num]->next!=NULL){
            bucket[num]=bucket[num]->next;
            a[i++]=bucket[num]->coeff;
        }
    }
}

int main(){
    int a[N];
    int i;
    for(i=0;i<N;i++){
        a[i]=rand()%1000;
        printf("\t%d",a[i]);
    }
    printf("\n");
    pass(a,1);
    pass(a,10);
    pass(a,100);
    for(i=0;i<N;i++){
        printf("\t%d",a[i]);
    }
}