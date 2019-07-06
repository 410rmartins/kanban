#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#define MAX_TAREFAS_DOING 5
#define MAX_DESC 200
#define MAX_NOME 30
#define TAM_EMAIL 30
#define MAX_DOING 5

int contador_id;
int max_tarefas;
int contador_tarefas_doing;

typedef struct {
    int dia, mes, ano;
}Data;

typedef struct {
    int id, prioridade, estagio, id_responsavel;
    char descricao[MAX_DESC];
    Data dt_criacao, prazo, dt_fim;
}Cartao;

typedef struct {
   char nome[MAX_NOME], email[TAM_EMAIL];
   int id;
}Pessoa;

typedef struct tarefas *List_tarefas;
typedef struct responsavel *List_pessoas;
typedef struct tarefas {
     Cartao info;
     List_pessoas resp_tarefa;
     List_tarefas next;
}Tarefas;

typedef struct lista_ponteiros* List_apont_tarefas;
typedef struct lista_ponteiros{
    List_tarefas ponteiro;
    List_apont_tarefas next;
}List_apontadores;

typedef struct responsavel{
    Pessoa info;
    List_apont_tarefas tarefas_correspondentes;
    List_pessoas next;
}Responsavel;

/*DATAS*/
void preencher_data(Data* dt);
void criar_tarefa(Cartao *n);
int comparar_datas (Data *d1,Data *d2);
int diferenca_dias (Data *d1,Data *d2);

/*LISTAS*/
List_tarefas inserir_ordenado_data(List_tarefas al,List_tarefas novo);
List_apont_tarefas inserir_to_do(List_apont_tarefas lista,List_apont_tarefas novo);
List_apont_tarefas inserir_doing(List_apont_tarefas lista,List_apont_tarefas novo, List_pessoas pessoas);
List_apont_tarefas inserir_done(List_apont_tarefas lista,List_apont_tarefas novo);
int verificar_responsavel(List_apont_tarefas novo, List_pessoas resp);
List_apont_tarefas criar_todo(List_apont_tarefas to_do, List_tarefas novo);
List_apont_tarefas criar_doing(List_apont_tarefas doing, List_tarefas novo, List_pessoas pessoas);
List_apont_tarefas criar_done(List_apont_tarefas done, List_tarefas novo);
List_tarefas criar_lista(List_tarefas* lista);
List_tarefas pesquisa_lista(List_tarefas lista,int id);
List_tarefas alterar_prioridade(List_tarefas novo);
void imprimir_tarefas(List_tarefas lista);
void imprimir_pessoa(List_pessoas lista);
void imprimir_apontadores(List_apont_tarefas lista);
List_apont_tarefas criar_done(List_apont_tarefas done, List_tarefas novo);
List_apont_tarefas inserir_done(List_apont_tarefas lista,List_apont_tarefas novo);
List_apont_tarefas remover_tarefa(List_apont_tarefas* lista_actual, int id_tarefa);
List_apont_tarefas pesquisa_lista_apont(List_apont_tarefas lista, int id);
int menu_inicial(List_tarefas* tarefas, List_pessoas* pessoas, List_apont_tarefas* to_do, List_apont_tarefas* doing, List_apont_tarefas* done);
void alterar_responsavel(List_apont_tarefas tarefa, List_pessoas pessoas);
List_apont_tarefas destroi_apontadores(List_apont_tarefas lista);
List_tarefas destroi_lista(List_tarefas lista);
List_apont_tarefas ordenar_alfabetica (List_apont_tarefas lista,List_apont_tarefas novo);
int comparar_strings(char a[],char b[]);

/*FUNÇÕES FICHEIROS*/
List_tarefas carregar_listas(List_tarefas lista,List_pessoas pessoas,List_apont_tarefas* to_do,List_apont_tarefas* doing,List_apont_tarefas* done);
List_apont_tarefas inserir_done_ficheiro(List_apont_tarefas lista,List_tarefas novo, List_pessoas pessoas);
List_apont_tarefas inserir_doing_ficheiro(List_apont_tarefas lista,List_tarefas novo, List_pessoas pessoas);
void colocar_ficheiro(List_apont_tarefas to_do,List_apont_tarefas doing,List_apont_tarefas done);
void imprimir_apontadores_ficheiros(List_apont_tarefas lista,FILE *fp);
List_pessoas inserir_pessoa(List_pessoas lista,List_pessoas novo);
List_pessoas pesquisa_pessoa(List_pessoas lista,int id);
List_pessoas criar_pessoas(List_pessoas lista);
void guardar_lista(List_tarefas lista);

#endif
