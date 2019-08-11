#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SPACE 30

//cursor 
struct node{
    int elem;
    int next;
};

struct node SMT_ARRAY[SPACE];
int header=0,position;

void CreateArray(){
    int i;
    for(i=0;i<SPACE;i++){
        SMT_ARRAY[i].next=(i+1)%30;
        SMT_ARRAY[i].elem=rand()*i;
    }
    printf("\nCreated Successfully!");
}

void InsertArray(int x){

    if(SMT_ARRAY[0].next==0){
        printf("\nOut of space!");
    }
    if(header==0){
        position=SMT_ARRAY[header].next;
        SMT_ARRAY[header].next=SMT_ARRAY[position].next;
        SMT_ARRAY[position].next=header;
        header=position;
        SMT_ARRAY[position].elem=x;
    }
    else{
        position=SMT_ARRAY[0].next;
        SMT_ARRAY[0].next=SMT_ARRAY[position].next;
        SMT_ARRAY[position].next=header;
        header=position;
        SMT_ARRAY[position].elem=x;
    }
}

int PopArray(int position){
    int hook;
    if(position==0){
        printf("\nWrong");
        return 0;
    }
    else if(position==header){
        header=SMT_ARRAY[position].next;
        return position;
    }
    else{
        hook=header;
        while(SMT_ARRAY[hook].next!=position&&hook!=0){
            hook=SMT_ARRAY[hook].next;
        }
        if(hook==0){
            printf("\nNot Found");
            return 0;
        }
        else{
            SMT_ARRAY[hook].next=SMT_ARRAY[position].next;
            printf("\nPop successfully");
            return position;
        }
    
    }
}

int FindArray(int x){
    position=header;
    while(position!=0&&x!=SMT_ARRAY[position].elem){
        position=SMT_ARRAY[position].next;
    }
    if(position==0){
        printf("\nDoes not exist");
        return 0;
    }
    else{
        position=PopArray(position);
        SMT_ARRAY[position].next=header;
        header=position;
        printf("\nFound successfully");
        return position;
    }
}

void PrintArray(){
    position=header;
    while(position!=0){
        printf("\t%d",SMT_ARRAY[position].elem);
        position=SMT_ARRAY[position].next;
    }
    printf("\nPrinted successfully");
}

void main(){
    CreateArray();
    PrintArray();
    InsertArray(34);
    InsertArray(55);
    PrintArray();
    InsertArray(77);
    InsertArray(99);
    InsertArray(10);
    PrintArray();
    FindArray(77);
    PrintArray();
    FindArray(98);
    PrintArray();
}