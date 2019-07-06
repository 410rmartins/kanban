#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"


int contador_id = 1;
int max_tarefas = 2;
int contador_tarefas_doing = 0;

int alterar_max_tarefa()
{
    int novo_max,k;
    char c[200];
    do{
        printf("Qual e o novo maximo de tarefas permitidas por pessoa? ");
        fflush(stdin);
        k=scanf("%d", &novo_max);
        gets(c);
    }while((novo_max<0) && k<1);
    return novo_max;
}


int menu_inicial(List_tarefas* tarefas, List_pessoas* pessoas, List_apont_tarefas* to_do, List_apont_tarefas* doing, List_apont_tarefas* done)
{
    List_tarefas x = NULL;
    List_pessoas y = NULL;
    List_apont_tarefas aux = NULL;
    FILE *fp;

    int op, check = 1, id_aux,k;
    char v, c[200];
    printf("--------------------------------------MENU----------------------------------------\n");
    do{
        printf("Que operaçao pretende realizar? (1-13)?\n");
        printf("1) Listar pessoas \n2) Definir um novo maximo de tarefas \n3) Inserir uma nova em 'To Do' \n4) Alterar a prioridade de uma tarefa \n5) Comecar uma tarefa \n6) Tarefa de Doing para To Do \n7) Alterar o responsavel por uma tarefa \n8) Fechar um cartao \n9) Reabrir um cartao \n10) Vizualizar o quadro completo \n11) Vizualizar as tarefas de uma pessoa \n12) Vizualizar todas as tarefas por ordem de criacao \n13) Sair\n");
        fflush(stdin);
        k=scanf("%d",&op);
        gets(c);
    }while((op<0 || op>14) && k<1);


    switch(op){
    case 1:
        imprimir_pessoa(*pessoas);
        break;

    case 2:
        max_tarefas = alterar_max_tarefa();
        printf("Novo maximo tarefas por pessoa: %d\n", max_tarefas);
        break;

    case 3:

        x = criar_lista(tarefas);
        x->info.estagio = 1;
        *to_do = criar_todo(*to_do, x);
        break;

    case 4:
    /*Alterar apenas a prioridade de uma tarefa e reordenar*/
        printf("Qual a tarefa que pretende alterar a prioridade? ");
        scanf("%d", &id_aux);
        x = pesquisa_lista(*tarefas, id_aux);
        if (x != NULL){
            x = alterar_prioridade(x);
            if (x->info.estagio == 1){
                aux = remover_tarefa(to_do, x->info.id);
                *to_do = inserir_to_do(*to_do, aux);
            }
        }else{
            printf("A tarefa escolhida nao existe!\n ");
        }
        break;

    case 5:
    /*Mudar de To Do para Doing*/
        printf("Qual o id da tarefa que pretende comecar? ");
        scanf("%d", &id_aux);
        aux = remover_tarefa(to_do, id_aux);

        if (aux == NULL)
            printf("A tarefa que procura n se encontra na lista 'to do'! \n");
        else if (contador_tarefas_doing >= MAX_DOING)
            printf("Nao pode inserir mais tarefas no doing! Chegou ao maximo de tarefas em doing ao mesmo tempo (5)\n");
        else
            *doing = inserir_doing(*doing, aux, *pessoas);
            contador_tarefas_doing ++;

        break;

    case 6:
    /*MUDAR DE DOING PARA TO DO*/
        printf("Qual o id da tarefa que pretende mudar? ");
        scanf("%d", &id_aux);
        aux = remover_tarefa(doing, id_aux);
        if (aux == NULL)
            printf("A tarefa que procura n se encontra na lista 'doing'! \n");
        else{
            aux->ponteiro->info.estagio = 1;
            aux->ponteiro->resp_tarefa = NULL;
            aux->ponteiro->info.prazo.dia=0;
            aux->ponteiro->info.prazo.mes=0;
            aux->ponteiro->info.prazo.ano=0;
            *to_do = inserir_to_do(*to_do, aux);
            contador_tarefas_doing --;
        }

        break;

    case 7:
    /*Alterar o responsavel da tarefa*/
        printf("Qual o id da tarefa que pretende reatribuir? ");
        scanf("%d", &id_aux);
        aux = remover_tarefa(doing, id_aux);
        if (aux != NULL){
            alterar_responsavel(aux, *pessoas);
            *doing  = ordenar_alfabetica(*doing, aux);
        }else{
            printf("Nao pode alterar o responsavel desta tarefa uma vez que esta nao se encontra em execucao.\n");
        }

        break;

    case 8:

        printf("Qual o id da tarefa que terminou? ");
        scanf("%d", &id_aux);
        aux = remover_tarefa(doing, id_aux);

        if (aux == NULL)
            printf("A tarefa que procura n se encontra na lista 'doing'! \n");
        else{
            y = aux->ponteiro->resp_tarefa;
            *done = inserir_done(*done, aux);
            aux = remover_tarefa(&y->tarefas_correspondentes, id_aux);
            free(aux);
        }
        contador_tarefas_doing --;
        break;

    case 9:
    /*Reabrir a tarefa (Passar de Done para To Do)*/
        printf("Qual o id da tarefa que pretende reabrir? ");
        scanf("%d", &id_aux);
        aux = remover_tarefa(done, id_aux);
        if (aux != NULL){
            aux->ponteiro->resp_tarefa = NULL;
            aux->ponteiro->info.estagio = 1;
            aux->ponteiro->info.prazo.dia=0;
            aux->ponteiro->info.prazo.mes=0;
            aux->ponteiro->info.prazo.ano=0;
            aux->ponteiro->info.dt_fim.dia=0;
            aux->ponteiro->info.dt_fim.mes=0;
            aux->ponteiro->info.dt_fim.ano=0;
            *to_do = inserir_to_do(*to_do, aux);
        }else{
            printf("A tarefa que escolheu nao esta acabada ou nao existe!\n");
        }

        break;

    case 10:
    /*Vizualizar o quadro com todas as fazes do pipeline*/
        printf("=======================================TO DO=============================================\n");
        imprimir_apontadores(*to_do);
        printf("=======================================DOING=============================================\n");
        imprimir_apontadores(*doing);
        printf("=======================================DONE==============================================\n");
        imprimir_apontadores(*done);

        break;

    case 11:
    /*Vizualizar todas as tarefas de uma pessoa e verificar em que estado do pipeline se encontram (ainda falta a ultima parte)*/
        printf("Qual o id da pessoa que procura? ");
        scanf("%d", &id_aux);
        y = pesquisa_pessoa(*pessoas, id_aux);
        if (y == NULL){
            printf("Nao existe uma pessoa com este id na lista!");
        }else{
            if (y->tarefas_correspondentes == NULL)
                printf("Esta pessoa nao e responsavel por nenhuma tarefa!");
            else{
                printf("=======================================DOING=============================================\n");
                imprimir_apontadores(y->tarefas_correspondentes);
            }

        }
        break;

    case 12:

        imprimir_tarefas(*tarefas);
        break;

    case 13:

        do{
            fflush(stdin);
            printf("Deseja guardar as alteracoes feitas (S/N)? \n");
            v = getchar();
            v = toupper(v);
            if(v != 'S' && v != 'N')
                printf("Opcao nao valida use 'S' para guardar as alteracoes e 'N' para fechar o programa sem guardar alteracoes\n");
        }while( v != 'S' && v != 'N');

        if ( v == 'S'){
            guardar_lista(*tarefas);
            colocar_ficheiro(*to_do, *doing, *done);
            fp = fopen("auxiliar_var.txt","w");
            fprintf(fp,"%d\t", max_tarefas);
        }
        check = 0;

        break;

    default:
        printf("Numero invalido, Tente de novo\n");

    }
    system("pause");
    return check;

}

int main()
{

    List_tarefas tarefas = NULL;
    List_apont_tarefas to_do = NULL, doing = NULL, done = NULL;
    List_pessoas pessoas = NULL;
    FILE *fp;
    int check=1;

    fp  = fopen("auxiliar_var.txt", "r");
    fscanf(fp,"%d", &max_tarefas);
    pessoas = criar_pessoas(pessoas);
    tarefas = carregar_listas(tarefas, pessoas, &to_do, &doing, &done);

    while (check){
        check = menu_inicial(&tarefas, &pessoas, &to_do, &doing, &done);
        system("cls");
    }

    destroi_apontadores(to_do);
    destroi_apontadores(doing);
    destroi_apontadores(done);
    destroi_lista(tarefas);

    return 0;
}



