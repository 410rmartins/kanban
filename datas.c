#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

void preencher_data(Data* dt)
{
    int verifica;
    do{
        scanf("%d/%d/%d",&dt->dia,&dt->mes,&dt->ano);
        while(dt->mes==0)
        {
            fflush(stdin);
            verifica=0;
            printf("Insira de novo a data com o formato (dd/mm/AA):");
            scanf("%d/%d/%d",&dt->dia,&dt->mes,&dt->ano);
        }
        if (dt->ano%4==0 && dt->mes==2 && dt->dia>29){
            printf("data nao e valida! Insira novamente:");
            verifica=0;
        }else if(dt->ano%4!=0 && dt->mes==2 && dt->dia>28){
            printf("data nao e valida! Insira novamente:");
            verifica = 0;
        }else if((dt->mes==1 || dt->mes==3 || dt->mes==5 || dt->mes==7 || dt->mes==8 || dt->mes==10 || dt->mes == 12) && dt->dia>31){
            printf("data nao e valida! Insira novamente:");
            verifica = 0;
        }else if((dt->mes==4 || dt->mes==6 || dt->mes==9|| dt->mes==11 ) && dt->dia>30){
            printf("data nao e valida! Insira novamente:");
            verifica = 0;
        }else{
            verifica =1;
        }

    }while(verifica == 0);
}



int comparar_datas (Data *d1,Data *d2)
{
    if((*d1).ano<(*d2).ano)
        return -1;
    else if((*d1).ano>(*d2).ano)
        return 1;
    else if((*d1).mes<(*d2).mes)
        return -1;
    else if((*d1).mes>(*d2).mes)
        return 1;
    else if((*d1).dia<(*d2).dia)
        return -1;
    else if((*d1).dia>(*d2).dia)
        return 1;
    else
        return 0;
}

int diferenca_dias (Data *d1,Data *d2)
{
    int x,y=8,k;
    k=comparar_datas(d1,d2);
    if((*d1).mes==(*d2).mes && (*d1).ano==(*d2).ano){
        if(k==1||k==0)
            y=(*d1).dia-(*d2).dia;
        else
            y=(*d2).dia-(*d1).dia;
    }
    else if((*d1).mes!=(*d2).mes && (*d1).ano==(*d2).ano){
        if(k==1||k==0){
            if((*d2).mes==1||(*d2).mes==3||(*d2).mes==5||(*d2).mes==7||(*d2).mes==8||(*d2).mes==10||(*d2).mes==12)
                y=(31+(*d1).dia)-(*d2).dia;
            else if ((*d2).mes==4||(*d2).mes==6||(*d2).mes==9||(*d2).mes==11)
                y=(30+(*d1).dia)-(*d2).dia;
            else if((*d2).ano % 4 == 0 && ((*d2).ano % 400 == 0 || (*d2).ano % 100 != 0))
                y=(29+(*d1).dia)-(*d2).dia;
            else
                y=(28+(*d1).dia)-(*d2).dia;
        }
        else{
            if((*d1).mes==1||(*d1).mes==3||(*d1).mes==5||(*d1).mes==7||(*d1).mes==8||(*d1).mes==10||(*d1).mes==12)
                y=(31+(*d2).dia)-(*d1).dia;
            else if ((*d1).mes==4||(*d1).mes==6||(*d1).mes==9||(*d1).mes==11)
                y=(30+(*d2).dia)-(*d1).dia;
            else if((*d1).ano % 4 == 0 && ((*d1).ano % 400 == 0 || (*d1).ano % 100 != 0))
                y=(29+(*d2).dia)-(*d1).dia;
            else
                y=(28+(*d2).dia)-(*d1).dia;
        }
    }
    else{
        if(((*d1).mes==1 &&(*d2).mes==12 )|| ((*d2).mes==1 &&(*d1).mes==12)){
            if(k==1||k==0)
                y=(31+(*d1).dia)-(*d2).dia;
            else
                y=(31+(*d2).dia)-(*d1).dia;
    }
    }
    x=7;
    if(y>=x)
        return 1;
    else
        return 0;
}


List_tarefas inserir_ordenado_data(List_tarefas al,List_tarefas novo)
{
    List_tarefas actual=al, ant=NULL;

    while(actual != NULL && (comparar_datas(&actual->info.dt_criacao,&novo->info.dt_criacao)==-1)){
        ant = actual;
        actual = actual->next;
    }
    novo->next = actual;
    if (ant != NULL){
        ant->next=novo;
    }else{
        al = novo;
    }

    return al;
}
