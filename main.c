#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define MAXIMO_ANDAMENTO 3

typedef struct Animal {
    int id;
    char nome[51];
    char especie[51];
    char tutor[51];
    int servico; // Banho (1), Tosa (2), Ambos(3)
    int status; // Aguardando (1), Andamento (2) ou finalizado (3)
} Animal;

typedef struct Fila {
    Animal animal;
    struct Fila* prox;
} Fila;

Fila* entrada = NULL;
Fila* saida = NULL;
Animal servicos_andamento[3];
int quantidade_andamento = 0;
int contador_cadastro = 1;

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

    printf("Digite a espécie do animal:\n");
    scanf("%[^\n]s", animal.especie);
    getchar();

    printf("Digite o nome do tutor do animal:\n");
    scanf("%[^\n]s", animal.tutor);
    getchar();

    printf("Digite o serviço desejado:\n1 - Banho\n2 - Tosa\n3 - Ambos\n");
    scanf("%d", &animal.servico);
    getchar();

    while(animal.servico < 1 || animal.servico > 3) 
    {
        printf("Serviço inválido!\n");
        printf("Digite o serviço desejado:\n1 - Banho\n2 - Tosa\n3 - Ambos\n");
        scanf("%d", &animal.servico);
    }

    animal.id = contador_cadastro++;
    animal.status = 1;

    return animal;
}

void printar_animal(Animal animal) 
{
    printf("\nId: %d\n", animal.id);
    printf("Nome: %s\n", animal.nome);
    printf("Espécie: %s\n", animal.especie);
    printf("Tutor: %s\n", animal.tutor);
    printf("Serviço: %d\n", animal.servico);
    printf("Status: %d\n\n", animal.status);
}

void copiar_animal(Animal origem, Animal destino) 
{
    destino.id = origem.id;
    strcpy(destino.nome, origem.nome);
    strcpy(destino.especie, origem.especie);
    strcpy(destino.tutor, origem.tutor);
    destino.servico = origem.servico;
    destino.status = origem.status;
}

/* ==================== FIM DAS FUNÇÕES DE ANIMAL ==================== */


/* ==================== COMEÇO DAS FUNÇÕES DE FILA ==================== */

void enfileirar(Animal animal, Fila** fila) 
{
    Fila* novo = (Fila*) malloc(sizeof(Fila));
    if (novo == NULL) 
    {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
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
}

void desenfileirar(Fila** fila) 
{
    if (*fila == NULL) return;

    Fila* aux = *fila;
    *fila = (*fila)->prox;
    free(aux);
    aux = NULL;
}

int remover_entrada_fila(Fila** fila, int id) 
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

void printar_servicos_andamento(void) 
{
    if (quantidade_andamento == 0) 
    {
        printf("Nenhum serviço em andamento!\n");
        aguardar_usuario();
        return;
    }

    for (int i = 0; i < quantidade_andamento; i++) 
        printar_animal(servicos_andamento[i]);
    
    aguardar_usuario();
}

int buscar_servico_andamento(int id) 
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

int adicionar_servico_andamento(Animal animal) 
{
    if (quantidade_andamento == MAXIMO_ANDAMENTO) 
    {
        printf("Limite de serviços em servicos_andamento atingido!\n");
        return 0;
    }

    servicos_andamento[quantidade_andamento++] = animal;
    return 1;
}

int remover_servico_andamento(int id) 
{
    if (quantidade_andamento == 0) return 0;

    int posicao = buscar_servico_andamento(id);
    if (posicao == -1) return 0;

    quantidade_andamento--;
    for (int i = posicao; i < quantidade_andamento; i++) 
        copiar_animal(servicos_andamento[i + 1], servicos_andamento[i]);

    return 1;
}

/* ==================== FIM DAS FUNÇÕES DE ARRAY ==================== */


/* ==================== COMEÇO DAS FUNÇÕES DO PETSHOP ==================== */

void cadastrar_animal(void) 
{
    Animal animal = receber_dados();
    enfileirar(animal, &entrada);

    printf("\nAnimal cadastrado com sucesso!\n");
    aguardar_usuario();
}

void atender_animal(void) 
{
    if (entrada == NULL) 
    {
        printf("Fila de entrada vazia!\n");
        aguardar_usuario();
        return;
    }
    
    Animal animal = entrada->animal;
    animal.status = 2;  // em andamento

    if (!adicionar_servico_andamento(animal)) return;
    desenfileirar(&entrada);

    printf("Atendendo seguinte animal:\n");
    printar_animal(animal);
}

void finalizar_servico(void) 
{
    int id, posicao;

    printf("Digite o id do animal:\n");
    scanf("%d", &id);

    posicao = buscar_servico_andamento(id);
    
    if (posicao == -1) 
    {
        printf("Serviço em andamento não encontrado!\n");
        return;
    }

    Animal animal = servicos_andamento[posicao];
    animal.status = 3;  // finalizado

    enfileirar(animal, &saida);
    remover_servico_andamento(id);

    printf("Finalizando serviço:\n");
    printar_animal(animal);
}

void cancelar_servico(void) {
    int id;

    printf("Digite o id do animal:\n");
    scanf("%d", &id);

    // Somente cancela se o serviço não estiver finalizado
    if (!remover_entrada_fila(&entrada, id)) {
        if (!remover_servico_andamento(id)) {
            printf("Serviço não encontrado!\n");
            return;
        }
    }

    printf("Serviço cancelado com sucesso!\n");
}

void entregar_animal(void) 
{
    if (saida == NULL) 
    {
        printf("Fila de saída vazia!\n");
        return;
    }

    Animal animal = saida->animal;
    desenfileirar(&saida);

    printf("Entregando seguinte animal:\n");
    printar_animal(animal);
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
    printf("Digite a opção desejada:\n");
    printf("1 - Cadastrar animal\n");
    printf("2 - Atender animal\n");
    printf("3 - Finalizar serviço\n");
    printf("4 - Cancelar serviço\n");
    printf("5 - Entregar animal\n");
    printf("6 - Imprimir fila de entrada\n");
    printf("7 - Imprimir fila de saída\n");
    printf("8 - Imprimir serviços em andamento\n");
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
                printar_servicos_andamento();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
                break;
        }
    } while (opcao != 0);

    limpar_fila(&entrada);
    limpar_fila(&saida);

    return 0;
}
