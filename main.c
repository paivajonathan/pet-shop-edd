#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define AGUARDANDO 1
#define ANDAMENTO 2
#define FINALIZADO 3

#define CACHORRO 1
#define GATO 2
#define OUTRO_ANIMAL 3

#define BANHO 1
#define TOSA 2
#define BANHO_TOSA 3

#define MAXIMO_ANDAMENTO 3
#define MINIMO_ID 1

typedef struct Animal {
    int id;
    char nome[51];
    char tutor[51];
    int especie; // Cachorro (1), Gato (2), Outro (3)
    int servico; // Banho (1), Tosa (2), Ambos(3)
    int status; // Aguardando (1), Andamento (2) ou finalizado (3)
} Animal;

typedef struct Fila {
    Animal animal;
    struct Fila* prox;
} Fila;

Fila* entrada = NULL;
Fila* saida = NULL;
Animal servicos_andamento[MAXIMO_ANDAMENTO];
int quantidade_andamento = 0;
int contador_id = MINIMO_ID;

/* ==================== COMEÇO FUNÇÕES UTILITÁRIAS ==================== */

void aguardar_usuario(void) 
{
    printf("Pressione ENTER para continuar...\n");
    getchar();
}

/* ==================== FIM FUNÇÕES DE ANIMAL ==================== */


/* ==================== COMEÇO FUNÇÕES DE ANIMAL ==================== */

Animal receber_dados(void) 
{
    Animal animal;

    printf("Digite o nome do animal:\n");
    scanf("%[^\n]s", animal.nome);
    getchar();

    printf("Digite o nome do tutor do animal:\n");
    scanf("%[^\n]s", animal.tutor);
    getchar();

    printf("Digite a especie do animal:\n1 - Cachorro\n2 - Gato\n3 - Outro\n");
    scanf("%d", &animal.especie);
    getchar();

    while (animal.especie < CACHORRO || animal.especie > OUTRO_ANIMAL) 
    {
        printf("\nEspecie invalida!\n");
        printf("Digite a especie do animal:\n1 - Cachorro\n2 - Gato\n3 - Outro\n");
        scanf("%d", &animal.especie);
        getchar();
    }

    printf("Digite o serviço desejado:\n1 - Banho\n2 - Tosa\n3 - Ambos\n");
    scanf("%d", &animal.servico);
    getchar();

    while(animal.servico < BANHO || animal.servico > BANHO_TOSA) 
    {
        printf("\nServiço invalido!\n");
        printf("Digite o serviço desejado:\n1 - Banho\n2 - Tosa\n3 - Ambos\n");
        scanf("%d", &animal.servico);
        getchar();
    }

    animal.id = contador_id++; // atribui o id e incrementa o contador
    animal.status = AGUARDANDO;

    return animal;
}

void printar_animal(Animal animal) 
{
    printf("\nId: %d\n", animal.id);
    printf("Nome: %s\n", animal.nome);
    printf("Tutor: %s\n", animal.tutor);
    printf("Especie: %d\n", animal.especie);
    printf("Servico: %d\n", animal.servico);
    printf("Status: %d\n\n", animal.status);
}

void copiar_animal(Animal* origem, Animal* destino) 
{
    destino->id = origem->id;
    strcpy(destino->nome, origem->nome);
    strcpy(destino->tutor, origem->tutor);
    destino->especie = origem->especie;
    destino->servico = origem->servico;
    destino->status = origem->status;
}

/* ==================== FIM DAS FUNÇÕES DE ANIMAL ==================== */


/* ==================== COMEÇO DAS FUNÇÕES DE FILA ==================== */

int enfileirar(Animal animal, Fila** fila)
{
    Fila* novo = (Fila*) malloc(sizeof(Fila));
    if (novo == NULL) return 0;
    novo->animal = animal;
    novo->prox = NULL;

    if (*fila == NULL) *fila = novo;
    else 
    {
        Fila* aux = *fila;
        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = novo;
    }

    return 1;
}

void desenfileirar(Fila** fila) 
{
    if (*fila == NULL) return;

    Fila* aux = *fila;
    *fila = (*fila)->prox;
    free(aux);
    aux = NULL;
}

int remover_de_fila(Fila** fila, int id) 
{
    if (*fila == NULL) return 0;

    Fila* atual = *fila;
    Fila* anterior = NULL;

    while (atual != NULL && atual->animal.id != id) 
    {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) return 0; // caso o elemento não seja encontrado

    if (anterior == NULL) // caso o elemento a ser removido seja o primeiro
        *fila = atual->prox;
    else
        anterior->prox = atual->prox;

    free(atual);
    atual = NULL;

    return 1;
}

void limpar_fila(Fila** fila) 
{
    if (*fila == NULL) return;

    Fila* aux;
    while (*fila != NULL) 
    {
        aux = *fila;
        *fila = (*fila)->prox;
        free(aux);
        aux = NULL;
    }
}

void printar_fila(Fila** fila) 
{
    system(CLEAR);
    if (*fila == NULL) 
    {
        printf("Fila vazia!\n");
        aguardar_usuario();
        return;
    }

    Fila* aux = *fila;
    
    while (aux != NULL) 
    {
        printar_animal(aux->animal);
        aux = aux->prox;
    }

    aguardar_usuario();
}

/* ==================== FIM DAS FUNÇÕES DE FILA ==================== */


/* ==================== COMEÇO DAS FUNÇÕES DE ARRAY ==================== */

void printar_array(void) 
{
    system(CLEAR);
    if (quantidade_andamento == 0) 
    {
        printf("Nenhum servico em andamento!\n");
        aguardar_usuario();
        return;
    }

    for (int i = 0; i < quantidade_andamento; i++) 
        printar_animal(servicos_andamento[i]);
    
    aguardar_usuario();
}

int buscar_em_array(int id) 
{
    int posicao = -1;
    for (int i = 0; i < quantidade_andamento; i++) 
    {
        if (servicos_andamento[i].id == id) 
        {
            posicao = i;
            break;
        }
    }
    return posicao;
}

int adicionar_em_array(Animal animal) 
{
    if (quantidade_andamento == MAXIMO_ANDAMENTO) return 0;

    servicos_andamento[quantidade_andamento++] = animal;
    return 1;
}

int remover_de_array(int id) 
{
    if (quantidade_andamento == 0) return 0;

    int posicao = buscar_em_array(id);
    if (posicao == -1) return 0;

    quantidade_andamento--;
    for (int i = posicao; i < quantidade_andamento; i++) 
        copiar_animal(&servicos_andamento[i + 1], &servicos_andamento[i]);

    return 1;
}

/* ==================== FIM DAS FUNÇÕES DE ARRAY ==================== */


/* ==================== COMEÇO DAS FUNÇÕES DO PETSHOP ==================== */

void cadastrar_animal(void) 
{
    system(CLEAR);
    Animal animal = receber_dados();

    if (!enfileirar(animal, &entrada))
    {
        printf("Nao foi possivel cadastrar o animal!\n");
        aguardar_usuario();
        return;
    }

    printf("\nAnimal %d cadastrado com sucesso!\n", animal.id);
    aguardar_usuario();
}

void atender_animal(void) 
{
    system(CLEAR);

    if (entrada == NULL)
    {
        printf("Nao ha animal para ser atendido!\n");
        aguardar_usuario();
        return;
    }
    
    Animal animal = entrada->animal;
    animal.status = ANDAMENTO;

    if (!adicionar_em_array(animal))
    {
        printf("Nao eh possivel atender mais animais ao mesmo tempo!\n");
        aguardar_usuario();
        return;
    }
    desenfileirar(&entrada);

    printf("Atendendo seguinte animal:\n");
    printar_animal(animal);
    aguardar_usuario();
}

void finalizar_servico(void) 
{
    system(CLEAR);
    int id, posicao, novo_status;

    printf("Digite o id do animal:\n");
    scanf("%d", &id);
    getchar();

    while (id < MINIMO_ID || id > contador_id)
    {
        printf("\nId invalido!\n");
        printf("Digite o id do animal:\n");
        scanf("%d", &id);
        getchar();
    }

    posicao = buscar_em_array(id);
    if (posicao == -1) 
    {
        printf("Servico em andamento nao encontrado!\n");
        aguardar_usuario();
        return;
    }

    printf("Para qual status deseja alterar?\n2 - Em andamento\n3 - Finalizado\n");
    scanf("%d", &novo_status);
    getchar();

    while (novo_status < ANDAMENTO || novo_status > FINALIZADO) 
    {
        printf("\nStatus invalido!\n");
        printf("Para qual status deseja alterar?\n2 - Em andamento\n3 - Finalizado\n");
        scanf("%d", &novo_status);
        getchar();
    }

    if (novo_status == ANDAMENTO) 
    {
        printf("Servico permanece em andamento.\n");
        aguardar_usuario();
        return;
    }

    Animal animal = servicos_andamento[posicao];
    animal.status = FINALIZADO;  // finalizado

    if (!enfileirar(animal, &saida))
    {
        printf("Nao foi possivel adicionar o animal na fila de saida!\n");
        aguardar_usuario();
        return;
    } 
    remover_de_array(id);

    printf("Finalizando servico:\n");
    printar_animal(animal);
    aguardar_usuario();
}

void cancelar_servico(void) {
    system(CLEAR);
    int id, status_atual;

    printf("Digite o id do animal:\n");
    scanf("%d", &id);
    getchar();

    while (id < MINIMO_ID || id > contador_id) 
    {
        printf("\nId invalido!\n");
        printf("Digite o id do animal:\n");
        scanf("%d", &id);
        getchar();
    }

    printf("Digite o status atual do servico:\n1 - Aguardando\n2 - Em andamento\n");
    scanf("%d", &status_atual);
    getchar();

    while (status_atual < AGUARDANDO || status_atual > ANDAMENTO) {
        printf("\nStatus invalido!\n");
        printf("Digite o status atual do servico:\n1 - Aguardando\n2 - Em andamento\n");
        scanf("%d", &status_atual);
        getchar();
    }

    if (status_atual == AGUARDANDO) // caso o serviço esteja na fila de entrada
    {
        if (!remover_de_fila(&entrada, id)) 
        {
            printf("Animal nao encontrado aguardando!\n");
            aguardar_usuario();
            return;
        }
    }
    else if (status_atual == ANDAMENTO) // caso o serviço esteja em andamento
    {
        if (!remover_de_array(id)) 
        {
            printf("Servico em andamento nao encontrado!\n");
            aguardar_usuario();
            return;
        }
    }

    printf("Servico cancelado com sucesso!\n");
    aguardar_usuario();
}

void entregar_animal(void) 
{
    system(CLEAR);
    if (saida == NULL) 
    {
        printf("Nao ha animais para serem entregues!\n");
        aguardar_usuario();
        return;
    }

    Animal animal = saida->animal;
    desenfileirar(&saida);

    printf("Entregando seguinte animal:\n");
    printar_animal(animal);
    aguardar_usuario();
}

/* ==================== FIM DAS FUNÇÕES DO PETSHOP ==================== */


// entrada dos animais é uma fila por ordem de chegada
// no cadastro entra na fila
// banho tosa ou ambos, serviço definido na hora do cadastro

// petshop não pode ter mais de três animais com status de em andamento

// assim que o status mudar para finalizado, o animal entra na *fila* de saída
// duas filas - entrada, pro cadastro, outra fila pra saída
// mudou o status, sai da fila de entrada e vai pra fila de saída

// fila de saída pode ter uma ordenação diferente da fila de entrada
// na fila de saída tem que ser na ordem que finalizou o serviço
// inclusão automática

// talvez (coisa da cabeça do marcel):
// fila de entrada, fila pra acompanhar quem está em andamento, fila de saída
// aí poderia ter um terceiro status: aguardando (2)

// código deve implementar uma função para cancelamento do serviço, somente se não tiver
// status finalizado
// caso for cancelado na entrada, deve ser retirado da entrada e reorganizada a estrutura

// operaçao de entregar o animal, tira da fila de saída e reorganiza a estrutura
// fila de entrada não tem tamanho, fila de saída não tem tamanho

// o alocamento deve ser feito por estrutura ou de duas em duas estruturas

void mostrar_menu(void) 
{
    system(CLEAR);
    printf("Digite a opcao desejada:\n");
    printf("1 - Cadastrar animal\n");
    printf("2 - Atender animal\n");
    printf("3 - Finalizar servico\n");
    printf("4 - Cancelar servico\n");
    printf("5 - Entregar animal\n");
    printf("6 - Imprimir fila de entrada\n");
    printf("7 - Imprimir fila de saida\n");
    printf("8 - Imprimir servicos em andamento\n");
    printf("0 - Sair\n");
}

int main(void) 
{
    int opcao = -1;

    do 
    {
        mostrar_menu();

        scanf("%d", &opcao);
        getchar();

        switch (opcao) 
        {
            case 1:
                cadastrar_animal();
                break;
            case 2:
                atender_animal();
                break;
            case 3:
                finalizar_servico();
                break;
            case 4:
                cancelar_servico();
                break;
            case 5:
                entregar_animal();
                break;
            case 6:
                printar_fila(&entrada);
                break;
            case 7:
                printar_fila(&saida);
                break;
            case 8:
                printar_array();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção invalida!\n");
                break;
        }
    } while (opcao != 0);

    limpar_fila(&entrada);
    limpar_fila(&saida);

    return 0;
}
