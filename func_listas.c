#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

void criar_tarefa(Cartao *n)
{

    int x;
    char c[200];
    (n->id)=contador_id;
    contador_id++;
    do{
        printf("Insira prioridade: ");
        x=scanf("%d",&(n->prioridade));
        gets(c);
    }while(n->prioridade<=0 || n->prioridade>10 || x<1);


    printf("Data de criacao (dd/mm/aaaa): ");
    preencher_data(&n->dt_criacao);


    fflush(stdin);
    printf("Descreva a tarefa: ");
    gets(n->descricao);

}

List_apont_tarefas inserir_to_do(List_apont_tarefas lista,List_apont_tarefas novo)
{
    List_apont_tarefas actual = lista, ant = NULL;

    while(actual != NULL && (actual->ponteiro)->info.prioridade > (novo->ponteiro)->info.prioridade){
        ant = actual;
        actual = actual->next;
    }

    /*VERIFICAR TODOS*/
    if (actual != NULL){
        if (novo->ponteiro->info.prioridade == actual->ponteiro->info.prioridade && comparar_datas(&(novo->ponteiro->info.dt_criacao), &(actual->ponteiro->info.dt_criacao)) == 1){
            ant = actual;
            actual = actual->next;
        }
    }

    novo->next = actual;
    if (ant != NULL){
        ant->next=novo;
    }else{
        lista = novo;
    }
    return lista;
}

List_apont_tarefas inserir_doing(List_apont_tarefas lista,List_apont_tarefas novo, List_pessoas pessoas)
{
    int confirma=0,x;
    char c[200];
    List_pessoas resp=NULL;


    (novo->ponteiro)->info.estagio = 2;
    while(confirma  == 0){

        do{
            printf("Qual e o prazo para terminar a tarefa(dd/mm/aaaa)? ");
            preencher_data(&novo->ponteiro->info.prazo);
        }while(comparar_datas(&novo->ponteiro->info.prazo,&novo->ponteiro->info.dt_criacao)!=1);

        do{
            printf("Qual é o id do responsavel?");
            x=scanf(" %d", &(novo->ponteiro)->info.id_responsavel);
            gets(c);
        }while((novo->ponteiro)->info.id_responsavel<0 && x<1);

        resp = pesquisa_pessoa(pessoas, (novo->ponteiro)->info.id_responsavel);
        if(resp != NULL)
            imprimir_pessoa(resp);

        confirma = verificar_responsavel( novo, resp);

    }

    (novo->ponteiro->resp_tarefa)=resp;
    resp->tarefas_correspondentes = criar_todo(resp->tarefas_correspondentes,novo->ponteiro);


    lista = ordenar_alfabetica(lista, novo);
    return lista;

}
List_apont_tarefas inserir_done(List_apont_tarefas lista,List_apont_tarefas novo)
{
    List_apont_tarefas actual = lista, ant = NULL;
    /*INSERIR DATA DE FIM*/
    novo->ponteiro->info.estagio = 3;
    do{
        printf("Qual a data de fim da tarefa(dd/mm/aaaa)? ");
        preencher_data(&(novo->ponteiro)->info.dt_fim);
    }while(comparar_datas(&(novo->ponteiro)->info.dt_fim,&(novo->ponteiro)->info.dt_criacao)!=1);


    while(actual != NULL && (comparar_datas(&(actual->ponteiro)->info.dt_fim,&(novo->ponteiro)->info.dt_fim)==-1)){
        ant = actual;
        actual = actual->next;
    }
    novo->next = actual;
    if (ant != NULL){
        ant->next=novo;
    }else{
        lista = novo;
    }
    return lista;
}

int verificar_responsavel(List_apont_tarefas novo, List_pessoas resp)
{

    List_apont_tarefas actual;
    int check = 0, confirma=0, count=0;

    if (resp==NULL){
        printf("Nao existe uma pessoa com este id na lista! Insira um id valido para o responsavel:\n");
        return confirma;
    }else{
        actual = (resp->tarefas_correspondentes);
        while(actual!= NULL){

            count ++;
            check += diferenca_dias(&(novo->ponteiro)->info.prazo, &(actual->ponteiro)->info.prazo);
            actual = actual->next;
        }

        if (check == count && count<max_tarefas){
            confirma = 1;
        }else{
            confirma = 0;
            printf("Esta pessoa nao pode receber esta tarefa por motivos de prazo ou pela quantidade de tarefas que ja possui! Por Favor escolha outro responsavel: ");
        }

        return confirma;
    }
}
List_apont_tarefas criar_todo(List_apont_tarefas to_do, List_tarefas novo)
{
    List_apont_tarefas al;

    al=(List_apont_tarefas)malloc(sizeof(List_apontadores));
    (al->ponteiro)=novo;
    to_do = inserir_to_do(to_do,al);
    return to_do;
}

List_apont_tarefas criar_doing(List_apont_tarefas doing, List_tarefas novo, List_pessoas pessoas)
{
    List_apont_tarefas al;

    al=(List_apont_tarefas)malloc(sizeof(List_apontadores));
    (al->ponteiro)=novo;
    doing = inserir_doing(doing,al,pessoas);
    return doing;
}
List_apont_tarefas criar_done(List_apont_tarefas done, List_tarefas novo)
{
    List_apont_tarefas al;

    al=(List_apont_tarefas)malloc(sizeof(List_apontadores));
    (al->ponteiro)=novo;
    done = inserir_done(done,al);
    return done;
}


List_tarefas criar_lista(List_tarefas* lista)
{
    List_tarefas novo;
    novo=(List_tarefas)malloc(sizeof(Tarefas));
    criar_tarefa(&novo->info);
    *lista=inserir_ordenado_data(*lista,novo);


    return novo;
}

List_tarefas pesquisa_lista(List_tarefas lista,int id)
{
    List_tarefas actual=lista;

    while((actual)!=NULL && (actual)->info.id!=id){
        actual=(actual)->next;
    }
    if((actual)!=NULL &&(actual)->info.id!=id)
        actual=NULL;

    return(actual);
}

List_tarefas alterar_prioridade(List_tarefas novo)
{
    char c[200];
    int x;
    do{
        printf("Insira a nova prioridade:");
        x = scanf("%d",&(novo->info).prioridade);
        gets(c);
        if((novo->info).prioridade<=0 || (novo->info).prioridade>10){
            printf("A prioridade tem valores entre 1 e 10 escolha um valor valido!\n");
        }
    }while((novo->info).prioridade <=0 || (novo->info).prioridade>10 || x<1);
    return novo;

}

void imprimir_tarefas(List_tarefas lista)
{
    List_tarefas actual;
    actual=lista;
    while(actual!=NULL){
        printf("ID: %d\n",(actual->info).id);
        printf("PRIORIDADE:%d\n",(actual->info).prioridade);
        printf("DESCRICAO: %s\n", (actual->info).descricao);
        printf("DATA CRAICAO:%d/%d/%d\n",(actual->info).dt_criacao.dia,(actual->info).dt_criacao.mes,(actual->info).dt_criacao.ano);

        if (actual->info.estagio == 2){
            printf("RESPONSAVEL: %s\n",(actual->resp_tarefa)->info.nome);
            printf("PRAZO TAREFA: %d/%d/%d\n",(actual->info).prazo.dia,(actual->info).prazo.mes,(actual->info).prazo.ano);
        }else if(actual->info.estagio == 3){
            printf("RESPONSAVEL: %s\n",(actual->resp_tarefa)->info.nome);
            printf("PRAZO TAREFA: %d/%d/%d\n",(actual->info).prazo.dia,(actual->info).prazo.mes,(actual->info).prazo.ano);
            printf("DATA FIM: %d/%d/%d\n",(actual->info).dt_fim.dia,(actual->info).dt_fim.mes,(actual->info).dt_fim.ano);
        }

        actual=actual->next;
    }
}


List_pessoas pesquisa_pessoa(List_pessoas lista,int id)
{
    List_pessoas actual=lista;
    while((actual)!=NULL && actual->info.id != id){
        actual=(actual)->next;
    }
    if((actual)!=NULL &&(actual)->info.id!=id)
        actual=NULL;
    return(actual);
}



void imprimir_pessoa(List_pessoas lista)
{
    List_pessoas actual;
    actual=lista;
    while(actual!=NULL){
        printf("NOME: %s\n EMAIL: %s\n ID:%d\n",actual->info.nome,actual->info.email,actual->info.id);
        actual=actual->next;
    }

}

void imprimir_apontadores(List_apont_tarefas lista)
{
    List_apont_tarefas actual=lista;

    while(actual!=NULL){
        printf("ID: %d\n",((actual->ponteiro)->info).id);
        printf("PRIORIDADE: %d\n",((actual->ponteiro)->info).prioridade);
        printf("DESCRICAO: %s\n",((actual->ponteiro)->info).descricao);
        printf("DATA CRIACAO:%d/%d/%d\n",((actual->ponteiro)->info).dt_criacao.dia,((actual->ponteiro)->info).dt_criacao.mes,((actual->ponteiro)->info).dt_criacao.ano);

        if ((actual->ponteiro)->info.estagio == 2){
            printf("RESPONSAVEL: %s\n",(actual->ponteiro->resp_tarefa)->info.nome);
            printf("PRAZO TAREFA: %d/%d/%d\n",(actual->ponteiro->info).prazo.dia,(actual->ponteiro->info).prazo.mes,(actual->ponteiro->info).prazo.ano);
        }else if((actual->ponteiro)->info.estagio == 3){
            printf("RESPONSAVEL: %s\n",(actual->ponteiro->resp_tarefa)->info.nome);
            printf("PRAZO TAREFA: %d/%d/%d\n",(actual->ponteiro)->info.prazo.dia,(actual->ponteiro)->info.prazo.mes,(actual->ponteiro)->info.prazo.ano);
            printf("DATA FIM: %d/%d/%d\n",(actual->ponteiro->info).dt_fim.dia,(actual->ponteiro->info).dt_fim.mes,(actual->ponteiro->info).dt_fim.ano);
        }
        actual = actual->next;
    }

}

List_apont_tarefas pesquisa_lista_apont(List_apont_tarefas lista, int id)
{
    List_apont_tarefas actual=lista;

    while((actual)!=NULL && (actual->ponteiro)->info.id != id){
        actual=actual->next;
    }

    return(actual);
}

List_apont_tarefas remover_tarefa(List_apont_tarefas* lista_actual, int id_tarefa)
{
        List_apont_tarefas ant = NULL, actual = *lista_actual;
        while((actual)!=NULL && (actual->ponteiro)->info.id != id_tarefa){
            ant = actual;
            actual=actual->next;
        }
        if (actual != NULL){
            if (ant == NULL)
                *lista_actual = actual->next;
            else
                ant->next = actual->next;
            actual->next = NULL;
        }
        return actual;
}



void alterar_responsavel(List_apont_tarefas tarefa, List_pessoas pessoas)
{
        int id_pessoa, verifica=0;
        List_apont_tarefas aux = NULL;
        List_pessoas novo_resp = NULL;

        printf("Qual e o novo responsavel para a tarefa? ");
        scanf("%d",&id_pessoa);
        novo_resp = pesquisa_pessoa(pessoas, id_pessoa);
        if (novo_resp != NULL)
            novo_resp ->next = NULL;

        while(verifica == 0) {
            if(verificar_responsavel(tarefa, novo_resp)==0){
                scanf("%d", &id_pessoa);
                verifica = 0;
            }else{
                aux = remover_tarefa(&tarefa->ponteiro->resp_tarefa->tarefas_correspondentes, tarefa->ponteiro->info.id);
                (tarefa->ponteiro->resp_tarefa)=novo_resp;
                novo_resp->tarefas_correspondentes= inserir_to_do(novo_resp->tarefas_correspondentes,aux);
                verifica = 1;
            }

            }
}

List_tarefas destroi_lista(List_tarefas lista)
{
    List_tarefas actual = lista;
    while(actual!=NULL){
        lista=actual->next;
        free(actual);
        actual=lista;
    }
    return lista;
}
List_apont_tarefas destroi_apontadores(List_apont_tarefas lista)
{
     List_apont_tarefas actual = lista;
    while(actual!=NULL){
        lista=actual->next;
        free(actual);
        actual=lista;
    }
    return lista;
}

List_apont_tarefas ordenar_alfabetica (List_apont_tarefas lista,List_apont_tarefas novo)
{
    List_apont_tarefas actual = lista, ant = NULL;


    while(actual != NULL && comparar_strings(((actual->ponteiro)->resp_tarefa)->info.nome,((novo->ponteiro)->resp_tarefa)->info.nome)<0){
        ant = actual;
        actual = actual->next;
    }
    novo->next = actual;
    if (ant != NULL){
        ant->next=novo;
    }else{
        lista = novo;
    }
    return lista;
}

int comparar_strings(char a[],char b[])
{
    int x,i=0;
    char t[MAX_NOME],h[MAX_NOME];
    strcpy(t,a);
    strcpy(h,b);
    while(t[i] != '\0')
    {
        t[i]=toupper(t[i]);
        i++;
    }
    i=0;
    while(h[i]!='\0')
    {
        h[i]=toupper(h[i]);
        i++;
    }
    x=strcmp(t,h);
    return x;
}
