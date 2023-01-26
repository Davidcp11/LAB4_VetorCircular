/*  David Costa Pereira                               */
/*  Turma 3                                           */
/*  Exercicio 4 : Agenda Eletrônica + vetor circular  */
/*                                                    */
/*  Programa compilado com CodeBlocks 20.03           */

#include <iostream>
#include <string.h>
typedef struct lista lista;
typedef struct dados vetor;
struct dados{
    char tarefa[45];
    int prioridade;
};
struct lista{
    vetor *vet;
    int posini;
    int posifim;
    int n_elementos;
    int n_max;
};
///****************************************************************************
///                      FUNCOES PUBLICAS
///****************************************************************************
///Recebe um pointer para a lista e o numero max de elementos e inicializa a lista, fazendo malloc para o vetor circular
void Inicializar(lista *L,int mx){
    L->posini=0;
    L->posifim=1;
    L->n_elementos=0;
    L->n_max=mx;
    L->vet = (vetor *)(malloc(mx * sizeof(vetor)));
}
///****************************************************************************
///Insere um elemento na lista
void Insert(lista *L, char tarefa[], int prioridade){
    ///Caso a lista esteja vazia
    if(L->n_elementos==0){
        L->vet[(L->posifim + L->n_max - 1) % L->n_max].prioridade = prioridade;
        strcpy(L->vet[(L->posifim + L->n_max - 1) % L->n_max].tarefa, tarefa);
    }
    ///Caso nao esteja vazia, frazer shift para o lado mais curto
    else{
        int med;
        int aux;
        bool achou= false;
        ///Item do meio
        med = ((L->posifim + L->posini+L->n_max)/2)%L->n_max;
        ///Shift para a esquerda
        if(prioridade<=L->vet[med].prioridade){
            aux=L->posini;
            L->posini=(aux + L->n_max - 1) % L->n_max;
            while (!achou){
                if(prioridade > L->vet[aux].prioridade) {
                    L->vet[(aux + L->n_max - 1) % L->n_max] = L->vet[aux];
                    aux = (aux + L->n_max + 1) % L->n_max;
                } else {
                    L->vet[(aux + L->n_max - 1) % L->n_max].prioridade=prioridade;
                    strcpy(L->vet[(aux + L->n_max - 1) % L->n_max].tarefa, tarefa);
                    achou = true;
                }
            }
        }
        ///Shift para a direita
        else{
            aux=(L->posifim + L->n_max - 1) % L->n_max;
            L->posifim=(L->posifim + L->n_max + 1) % L->n_max;
            while (!achou){
                if(prioridade<L->vet[aux].prioridade) {
                    L->vet[(aux + L->n_max + 1) % L->n_max] = L->vet[aux];
                    aux=(aux + L->n_max - 1) % L->n_max;
                } else {
                    L->vet[(aux + L->n_max + 1) % L->n_max].prioridade=prioridade;
                    strcpy(L->vet[(aux + L->n_max + 1) % L->n_max].tarefa, tarefa);
                    achou = true;
                }
            }
        }
    }
    L->n_elementos++;
}
///****************************************************************************
///Recebe a lista e da free no vetor circular
void Finalizar(lista *L){
    free(L->vet);
}
///****************************************************************************
///Recebe a lista e devolve o item de maior prioridade do vetor circular
dados Maximum(lista *L){
    return L->vet[(L->posifim + L->n_max - 1) % L->n_max];
}
///****************************************************************************
///Remove o item de maior prioridade do vetor circular
void RemoveMax(lista *L){
    if(L->n_elementos>1) {
        L->posifim = (L->posifim + L->n_max - 1) % L->n_max;
    }
    L->n_elementos--;
}
///****************************************************************************
///Verifica se a fila esta cheia
bool FilaCheia(lista *L){
    if(L->n_elementos == L->n_max)
        return true;
    return false;
}
///****************************************************************************
///Verifica se a fila esta vazia
bool FilaVazia(lista *L){
    if(L->n_elementos==0)
        return true;
    return false;
}

int main(){
    ///Arquivos de entrada e saida
    FILE *entrada, *saida;
    entrada = fopen("entrada4.txt","r");
    saida = fopen("Lab4_David_Costa_Pereira.txt","w");
    ///Lista de prioridade
    lista list_p;
    int prioridade;
    char tarefa[45];
    int n;
    char leitura[72];
    ///Linhas iniciais do arquivo de saida
    fprintf(saida,"Bom dia flor do dia\n");
    fprintf(saida,"Hora de organizar seu dia\n");
    fprintf(saida,"Ou fazer as tarefas do dia anterior\n");
    fprintf(saida,"Bom dia flor do dia\n");
    fprintf(saida,"Bom dia flor do dia\n");
    fprintf(saida,"--------------------------------------------------\n");
    fprintf(saida,"RESPOSTAS DAS CONSULTAS\n");
    fprintf(saida,"--------------------------------------------------\n");
    for (int i = 0; i <5; ++i) {
        fgets(leitura, 75, entrada);
    }
    ///Numero max de lementos na agenda
    fscanf(entrada," %d ", &n);
    for (int i = 0; i <3; ++i) {
        fgets(leitura, 72, entrada);
    }
    ///Inicializando a agenda
    Inicializar(&list_p,n);
    ///Lendo operacoes
    do{
        fscanf(entrada," %s ",&leitura);
        if(strcmp(leitura,"NOVA")==0){
            fscanf(entrada," %d ",&prioridade);
            fgets(tarefa,45,entrada);
            tarefa[strlen(tarefa)-1]='\0';
            if(!FilaCheia(&list_p)) {
                Insert(&list_p, tarefa, prioridade);
            } else fprintf(saida,"ERRO     Agenda cheia. Impossivel inserir.\n");
        } else if (strcmp(leitura,"PROXIMA")==0){
            if(!FilaVazia(&list_p)) {
                fprintf(saida, "%2.d       ", Maximum(&list_p).prioridade);
                fprintf(saida, "%s\n", Maximum(&list_p).tarefa);
                RemoveMax(&list_p);
            } else fprintf(saida,"AVISO    Nao ha tarefas na agenda\n");
        }
    } while (strcmp(leitura,"FIM")!=0);
    fprintf(saida,"--------------------------------------------------\n");
    fprintf(saida,"FICA PARA O DIA SEGUINTE\n");
    fprintf(saida,"--------------------------------------------------\n");
    ///Mensagenm final
    if(!FilaVazia(&list_p)) {
        while (!FilaVazia(&list_p)) {
            fprintf(saida, "%2.d       %s\n", Maximum(&list_p).prioridade, Maximum(&list_p).tarefa);
            RemoveMax(&list_p);
        }
    } else fprintf(saida,"Agenda vazia! Nao restam tarefas para o dia seguinte.");
    ///Finalizando a agenda
    Finalizar(&list_p);
    ///Fechand arquivos de entrada e saida de dados
    fclose(entrada);
    fclose(saida);
    return 0;
}
