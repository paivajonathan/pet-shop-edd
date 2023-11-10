#include <stdio.h>

typedef struct Animal {
    char nome[51];
    char tutor[51];
    int servico; // Banho (1), Tosa (2), Ambos(3)
    int status; // Aguardando(1), Andamento (2) ou finalizado (3)
} Animal;

typedef struct FilaEntrada {
    Animal animal;
    struct FilaEntrada *prox;
} FilaEntrada;

typedef struct FilaSaida {
    Animal animal;
    struct FilaSaida *prox;
} FilaSaida;

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
