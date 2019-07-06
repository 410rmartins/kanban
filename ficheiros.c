#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


List_pessoas criar_pessoas(List_pessoas lista)
{
    List_pessoas novo;
    FILE *fp;
    fflush(stdin);

    fp=fopen("pess.txt","r");

    while (!feof(fp)){
        novo=(List_pessoas)malloc(sizeof(Responsavel));
        fscanf(fp," %[^\t]s",novo->info.nome);
        fscanf(fp," %[^\t]s",novo->info.email);
        fscanf(fp," %d",&novo->info.id);
        novo->tarefas_correspondentes = NULL;
        lista=inserir_pessoa(lista,novo);
    }
    fclose(fp);
    return lista;
}

List_pessoas inserir_pessoa(List_pessoas lista,List_pessoas novo)
{
    List_pessoas actual=lista,ant=NULL;
    while(actual != NULL && actual->info.id > novo->info.id){
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

List_apont_tarefas inserir_doing_ficheiro(List_apont_tarefas lista,List_tarefas novo, List_pessoas pessoas)
{
    List_apont_tarefas x=NULL,al,actual = lista, ant = NULL;

    List_pessoas resp=NULL;
    al=(List_apont_tarefas)malloc(sizeof(List_apontadores));
    (al->ponteiro)=novo;
    resp = pesquisa_pessoa(pessoas, (al->ponteiro)->info.id_responsavel);
    (al->ponteiro->resp_tarefa)=resp;
    x=criar_todo(x,novo);
    (resp->tarefas_correspondentes) = x;


    while(actual != NULL && strcmp((((actual->ponteiro)->resp_tarefa)->info.nome), ((al->ponteiro)->resp_tarefa)->info.nome)>0){
        ant = actual;
        actual = actual->next;
    }
    al->next = actual;
    if (ant != NULL){
        ant->next=al;
    }else{
        lista = al;
    }
    contador_tarefas_doing ++;
    return lista;

}

List_apont_tarefas inserir_done_ficheiro(List_apont_tarefas lista,List_tarefas novo, List_pessoas pessoas)
{
     List_apont_tarefas al,actual = lista, ant = NULL;

    List_pessoas resp=NULL;
    al=(List_apont_tarefas)malloc(sizeof(List_apontadores));
    (al->ponteiro)=novo;
    resp = pesquisa_pessoa(pessoas, (al->ponteiro)->info.id_responsavel);
    (al->ponteiro->resp_tarefa)=resp;


    while(actual != NULL && (comparar_datas(&(actual->ponteiro)->info.prazo,&(al->ponteiro)->info.prazo)==-1)){
        ant = actual;
        actual = actual->next;
    }
    al->next = actual;
    if (ant != NULL){
        ant->next=al;
    }else{
        lista = al;
    }
    return lista;

}
List_tarefas carregar_listas(List_tarefas lista,List_pessoas pessoas,List_apont_tarefas* to_do,List_apont_tarefas* doing,List_apont_tarefas* done)
{
    List_tarefas novo;
    FILE *fp;
    fp=fopen("aux_lista.txt","r");

    while (!feof(fp)){
        novo=(List_tarefas)malloc(sizeof(Tarefas));
        fscanf(fp," %d",&novo->info.id);
        fscanf(fp," %d",&novo->info.prioridade);
        fscanf(fp," %d",&novo->info.estagio);
        fscanf(fp," %[^\t]s",novo->info.descricao);
        fscanf(fp," %d/%d/%d",&novo->info.dt_criacao.dia,&novo->info.dt_criacao.mes,&novo->info.dt_criacao.ano);

        if(novo->info.estagio==1){
            lista=inserir_ordenado_data(lista,novo);
            *to_do=criar_todo(*to_do,novo);
            contador_id ++;
        }
        else if(novo->info.estagio==2){
            fscanf(fp," %d",&novo->info.id_responsavel);
            fscanf(fp," %d/%d/%d",&novo->info.prazo.dia,&novo->info.prazo.mes,&novo->info.prazo.ano);
            lista=inserir_ordenado_data(lista,novo);
            *doing=inserir_doing_ficheiro(*doing,novo,pessoas);
            contador_id ++;
        }
        else if(novo->info.estagio==3){
            fscanf(fp," %d",&novo->info.id_responsavel);
            fscanf(fp," %d/%d/%d",&novo->info.prazo.dia,&novo->info.prazo.mes,&novo->info.prazo.ano);
            fscanf(fp," %d/%d/%d",&novo->info.dt_fim.dia,&novo->info.dt_fim.mes,&novo->info.dt_fim.ano);
            lista=inserir_ordenado_data(lista,novo);
            *done=inserir_done_ficheiro(*done,novo,pessoas);
            contador_id ++;
            }
        }
    fclose(fp);
    return lista;
}

void imprimir_apontadores_ficheiros(List_apont_tarefas lista,FILE *fp)
{
    List_apont_tarefas actual = lista;

    while(actual!=NULL){
        fprintf(fp,"DESCRICAO:%s\t ID:%d\t PRIORIDADE:%d\t DATA_CRIACAO:%d/%d/%d\t  PRAZO:%d/%d/%d\t DATA_FIM:%d/%d/%d\n",actual->ponteiro->info.descricao,actual->ponteiro->info.id,actual->ponteiro->info.prioridade,actual->ponteiro->info.dt_criacao,actual->ponteiro->info.prazo,actual->ponteiro->info.dt_fim);
        actual=actual->next;
    }
    free(actual);
}
void colocar_ficheiro(List_apont_tarefas to_do,List_apont_tarefas doing,List_apont_tarefas done)
{
    char ficheiro[100];
    FILE *fp;
    fflush(stdin);
    printf("insira o nome do ficheiro onde quer guardar os dados das listas:\n");
    gets(ficheiro);
    fp=fopen(ficheiro,"w");
    fprintf(fp,"=======================================TO DO=============================================\n");
    imprimir_apontadores_ficheiros(to_do,fp);
    fprintf(fp,"=======================================DOING=============================================\n");
    imprimir_apontadores_ficheiros(doing,fp);
    fprintf(fp,"=======================================DONE==============================================\n");
    imprimir_apontadores_ficheiros(done,fp);
    fclose(fp);
}

void guardar_lista(List_tarefas lista)
{
    List_tarefas actual=lista;
    FILE *fp;
    fflush(stdin);
    fp=fopen("aux_lista.txt","w");
    while(actual!=NULL)
    {
        if(actual->info.estagio==1){
            fprintf(fp,"%d\t%d\t%d\t%s\t%d/%d/%d\n",actual->info.id,actual->info.prioridade,actual->info.estagio,actual->info.descricao,actual->info.dt_criacao);
        }
        else if(actual->info.estagio==2){
            fprintf(fp,"%d\t%d\t%d\t%s\t%d/%d/%d\t%d\t%d/%d/%d\n",actual->info.id,actual->info.prioridade,actual->info.estagio,actual->info.descricao,actual->info.dt_criacao,actual->info.id_responsavel,actual->info.prazo);
        }
        else if(actual->info.estagio==3){
            fprintf(fp,"%d\t%d\t%d\t%s\t%d/%d/%d\t%d\t%d/%d/%d\t%d/%d/%d\n",actual->info.id,actual->info.prioridade,actual->info.estagio,actual->info.descricao,actual->info.dt_criacao,actual->info.id_responsavel,actual->info.prazo,actual->info.dt_fim);
        }
        actual=actual->next;
    }
    fclose(fp);
}


