#include <stdio.h>
#include <stdlib.h>

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


/* ==================== COMEÇO FUNÇÕES DE ANIMAL ==================== */

Animal receber_dados(void) {
    Animal animal;

    printf("Digite o nome do animal:\n");
    scanf("%[^\n]s", animal.nome);

    printf("Digite a espécie do animal:\n");
    scanf("%[^\n]s", animal.especie);

    printf("Digite o nome do tutor do animal:\n");
    scanf("%[^\n]s", animal.tutor);

    printf("Digite o serviço desejado:\n1 - Banho\n2 - Tosa\n3 - Ambos\n");
    scanf("%d", &animal.servico);

    while(animal.servico < 1 || animal.servico > 3) {
        printf("Serviço inválido!\n");
        printf("Digite o serviço desejado:\n1 - Banho\n2 - Tosa\n3 - Ambos\n");
        scanf("%d", &animal.servico);
    }

    animal.id = contador_cadastro++;
    animal.status = 1;

    return animal;
}

void printar_animal(Animal animal) {
    printf("Nome: %s\n", animal.nome);
    printf("Espécie: %s\n", animal.especie);
    printf("Tutor: %s\n", animal.tutor);
    printf("Serviço: %d\n", animal.servico);
    printf("Status: %d\n", animal.status);
}
/* ==================== FIM DAS FUNÇÕES DE ANIMAL ==================== */


/* ==================== COMEÇO DAS FUNÇÕES DE FILA ==================== */

void enfileirar(Animal animal, Fila* fila) {
    Fila* novo = (Fila*) malloc(sizeof(Fila));
    if (novo == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    novo->animal = animal;
    novo->prox = NULL;

    if (fila == NULL) {
        fila = novo;
    } else {
        Fila* aux = fila;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

void desenfileirar(Fila* fila) {
    if (fila == NULL) {
        return;
    }
    Fila* aux = fila;
    fila = fila->prox;
    free(aux);
    aux = NULL;
}

void limpar_fila(Fila* fila) {
    if (fila == NULL) {
        return;
    }

    Fila* aux;
    while (fila != NULL) {
        aux = fila;
        fila = fila->prox;
        free(aux);
        aux = NULL;
    }
}

void printar_fila(Fila* fila) {
    Fila* aux = fila;
    while (aux != NULL) {
        printar_animal(aux->animal);
        aux = aux->prox;
    }
}

/* ==================== FIM DAS FUNÇÕES DE FILA ==================== */


/* ==================== COMEÇO DAS FUNÇÕES DE ARRAY ==================== */

void printar_servicos_andamento() {
    for (int i = 0; i < quantidade_andamento; i++) {
        printar_animal(servicos_andamento[i]);
    }
}

int exibir_servico_andamento(int id) {
    int posicao = -1;
    for (int i = 0; i < quantidade_andamento; i++) {
        if (servicos_andamento[i].id == id) {
            posicao = i;
            break;
        }
    }
    printar_animal(servicos_andamento[posicao]);
    return posicao;
}

int adicionar_servico_andamento(Animal animal) {
    if (quantidade_andamento == MAXIMO_ANDAMENTO) {
        printf("Limite de serviços em servicos_andamento atingido!\n");
        return 0;
    }

    servicos_andamento[quantidade_andamento++] = animal;
    return 1;
}

void remover_servico_andamento(int id) {
    if (quantidade_andamento == 0) {
        printf("Não há serviços em servicos_andamento!\n");
        return;
    }
}

/* ==================== FIM DAS FUNÇÕES DE ARRAY ==================== */


/* ==================== COMEÇO DAS FUNÇÕES DO PETSHOP ==================== */

void cadastrar_animal(void) {
    Animal animal = receber_dados();
    enfileirar(animal, entrada);
}

void atender_animal(void) {
    if (entrada == NULL) {
        printf("Fila de entrada vazia!\n");
        return;
    }
    
    Animal animal = entrada->animal;
    if (!adicionar_servico_andamento(animal)) return;
    desenfileirar(entrada);
}

void finalizar_servico(int id) {
    int posicao = exibir_servico_andamento(id);
    
    if (posicao == -1) {
        printf("Serviço em andamento não encontrado!\n");
        return;
    }

    Animal animal = servicos_andamento[posicao];
    animal.status = 3;  // finalizado
    enfileirar(animal, saida);
    remover_servico_andamento(id);
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

int main() {
    printf("Hello, World!\n");
    return 0;
}
