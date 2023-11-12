#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int id; // identificador único
    char nome[51];
    char tutor[51];
    int especie; // Cachorro (1), Gato (2), Outro (3)
    int servico; // Banho (1), Tosa (2), Ambos (3)
    int status; // Aguardando (1), Andamento (2), Finalizado (3)
} Animal;

typedef struct Fila { // estrutura genérica de fila
    Animal animal;
    struct Fila* prox;
} Fila;

Fila* entrada = NULL;
Fila* saida = NULL;
Animal servicos_andamento[MAXIMO_ANDAMENTO];
int quantidade_andamento = 0;
int contador_id = MINIMO_ID;

/* ==================== COMEÇO FUNÇÕES UTILITÁRIAS ==================== */

/**
 * Aguarda a confirmação do usuário para continuar o programa.
*/
void aguardar_usuario(void) 
{
    printf("Pressione ENTER para continuar...\n");
    getchar();
}

/**
 * Limpa o buffer do teclado.
*/
void limpar_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Exibe no console o menu de opções.
*/
void mostrar_menu(void) 
{
    printf("Digite a opcao desejada:\n");
    printf("1 - Cadastrar animal\n");
    printf("2 - Atender animal\n");
    printf("3 - Alterar status de servico em andamento\n");
    printf("4 - Cancelar servico\n");
    printf("5 - Entregar animal\n");
    printf("6 - Imprimir fila de entrada\n");
    printf("7 - Imprimir servicos em andamento\n");
    printf("8 - Imprimir fila de saida\n");
    printf("0 - Sair\n");
}

/* ==================== FIM FUNÇÕES UTILITÁRIAS ==================== */


/* ==================== COMEÇO FUNÇÕES DE ANIMAL ==================== */

/**
 * Recebe os dados pertinentes, realizando as devidas validações, para
 * criar uma nova estrutura de Animal.
 * @return novo animal com os dados preenchidos.
*/
Animal receber_dados(void) 
{
    Animal animal;

    printf("\nDigite o nome do animal:\n");
    scanf("%50[^\n]", animal.nome);
    limpar_buffer();

    printf("\nDigite o nome do tutor do animal:\n");
    scanf("%50[^\n]", animal.tutor);
    limpar_buffer();

    printf("\nDigite a especie do animal:\n1 - Cachorro\n2 - Gato\n3 - Outro\n");
    scanf("%d", &animal.especie);
    limpar_buffer();

    while (animal.especie < CACHORRO || animal.especie > OUTRO_ANIMAL) 
    {
        printf("\nEspecie invalida!\n");
        printf("Digite a especie do animal:\n1 - Cachorro\n2 - Gato\n3 - Outro\n");
        scanf("%d", &animal.especie);
        limpar_buffer();
    }

    printf("\nDigite o serviço desejado:\n1 - Banho\n2 - Tosa\n3 - Ambos\n");
    scanf("%d", &animal.servico);
    limpar_buffer();

    while(animal.servico < BANHO || animal.servico > BANHO_TOSA) 
    {
        printf("\nServiço invalido!\n");
        printf("Digite o serviço desejado:\n1 - Banho\n2 - Tosa\n3 - Ambos\n");
        scanf("%d", &animal.servico);
        limpar_buffer();
    }

    animal.id = contador_id++; // atribui o id e incrementa o contador
    animal.status = AGUARDANDO;

    return animal;
}

/**
 * Exibe no console os dados de um animal.
 * @param animal animal que terá os dados exibidos.
*/
void printar_animal(Animal animal) 
{
    printf("Id: %d\n", animal.id);
    printf("Nome: %s\n", animal.nome);
    printf("Tutor: %s\n", animal.tutor);
    printf("Especie: %d\n", animal.especie);
    printf("Servico: %d\n", animal.servico);
    printf("Status: %d\n\n", animal.status);
}

/**
 * Copia os dados de um animal para outro.
 * @param destino animal que receberá os dados.
 * @param origem animal que será copiado.
*/
void copiar_animal(Animal* destino, const Animal* origem) 
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

/**
 * Insere um novo elemento no final da fila.
 * @param animal dados do animal a serem inseridos
 * @param fila qual das filas será inserido o novo elemento
 * @return 1 se o animal foi inserido com sucesso, 0 caso contrário.
*/
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

/**
 * Remove o primeiro elemento da fila.
 * @param fila qual das filas será removido o elemento.
*/
void desenfileirar(Fila** fila) 
{
    if (*fila == NULL) return;

    Fila* aux = *fila;
    *fila = (*fila)->prox;
    free(aux);
    aux = NULL;
}

/**
 * Remove um elemento da fila com base no id do animal.
 * @param fila qual das filas será removido o elemento.
 * @param id identificador do animal.
 * @return 1 se o animal foi removido com sucesso, 0 caso contrário.
*/
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

/**
 * Remove todos os elementos da fila, liberando a memória alocada.
 * @param fila qual das filas será realizado esse processo.
*/
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

/**
 * Exibe no console os dados de todos os animais da fila.
 * @param fila qual das filas serão exibidos os dados.
*/
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

/**
 * Exibe no console os dados de todos os animais da array de serviços em andamento.
*/
void printar_array(void) 
{
    printf("\n>>> Servicos em andamento <<<\n\n");
    
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

/**
 * Busca o índice de um animal na array de serviços em andamento, com base no seu id.
 * @param id identificador do animal.
 * @return índice do animal na array, -1 caso não seja encontrado.
*/
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

/**
 * Adiciona um animal na array de serviços em andamento.
 * @param animal animal que será adicionado.
 * @return 1 se o animal foi adicionado com sucesso, 0 caso contrário.
*/
int adicionar_em_array(Animal animal) 
{
    if (quantidade_andamento == MAXIMO_ANDAMENTO) return 0;

    servicos_andamento[quantidade_andamento++] = animal;
    return 1;
}

/**
 * Remove um animal da array de serviços em andamento.
 * @param id identificador do animal.
 * @return 1 se o animal foi removido com sucesso, 0 caso contrário.
*/
int remover_de_array(int id) 
{
    if (quantidade_andamento == 0) return 0;

    int posicao = buscar_em_array(id);
    if (posicao == -1) return 0;

    quantidade_andamento--;
    for (int i = posicao; i < quantidade_andamento; i++) 
        copiar_animal(&servicos_andamento[i], &servicos_andamento[i + 1]);

    return 1;
}

/* ==================== FIM DAS FUNÇÕES DE ARRAY ==================== */


/* ==================== COMEÇO DAS FUNÇÕES DO PETSHOP ==================== */

/**
 * Cadastra um novo animal na fila de entrada, com base nos dados recebidos, caso possível.
*/
void cadastrar_animal(void) 
{
    Animal animal = receber_dados();

    if (!enfileirar(animal, &entrada))
    {
        printf("\nNao foi possivel cadastrar o animal!\n");
        aguardar_usuario();
        return;
    }

    printf("\nAnimal %d cadastrado com sucesso!\n", animal.id);
    aguardar_usuario();
}

/**
 * @brief Atende um animal da fila de entrada, alterando seu status para "em andamento".
 * 
 * Copia os dados do primeiro animal da fila de entrada, alterando seu status para
 * em andamento, para adicioná-lo na array de serviços em andamento,
 * e então remove o primeiro da fila de entrada. O processo ocorre nessa ordem,
 * para evitar que os dados do animal sejam perdidos, caso ocorra algum erro.
*/
void atender_animal(void) 
{
    if (entrada == NULL)
    {
        printf("\nNao ha animal para ser atendido!\n");
        aguardar_usuario();
        return;
    }
    
    Animal animal = entrada->animal;
    animal.status = ANDAMENTO;

    if (!adicionar_em_array(animal))
    {
        printf("\nNao eh possivel atender mais animais ao mesmo tempo!\n");
        aguardar_usuario();
        return;
    }
    desenfileirar(&entrada);

    printf("\nAtendendo seguinte animal:\n");
    printar_animal(animal);
    aguardar_usuario();
}

/**
 * @brief Altera o status de um animal da array de serviços em andamento para "finalizado".
 * 
 * Busca o animal na array de serviços em andamento, com base no id recebido do usuário,
 * e então copia seus dados para uma nova estrutura, alterando seu status para "finalizado".
 * Em seguida, adiciona o animal na fila de saída, e remove o animal da array de serviços em andamento.
 * O processo ocorre nessa ordem, para evitar que os dados do animal sejam perdidos,
 * caso ocorra algum erro.
*/
void alterar_status_servico(void) 
{
    int id, posicao;

    printf("\nDigite o id do animal para alterar o status do servico:\n");
    scanf("%d", &id);
    limpar_buffer();

    while (id < MINIMO_ID || id > contador_id) // validar id
    {
        printf("\nId invalido!\n");
        printf("nDigite o id do animal para alterar o status do servico:\n");
        scanf("%d", &id);
        limpar_buffer();
    }

    posicao = buscar_em_array(id);
    if (posicao == -1) // verificar se id está na array
    {
        printf("\nServico em andamento nao encontrado!\n");
        aguardar_usuario();
        return;
    }

    Animal animal = servicos_andamento[posicao];
    animal.status = FINALIZADO;

    if (!enfileirar(animal, &saida))
    {
        printf("\nNao foi possivel adicionar o animal na fila de saida!\n");
        aguardar_usuario();
        return;
    } 
    remover_de_array(id);

    printf("\nFinalizando servico:\n");
    printar_animal(animal);
    aguardar_usuario();
}

/**
 * @brief Cancela um serviço, removendo o animal da fila de entrada ou da array de serviços em andamento.
 * 
 * Recebe o id do animal e o status atual do serviço, para então remover o animal da fila de entrada ou da
 * array de serviços em andamento, dependendo do status recebido.
*/
void cancelar_servico(void) {
    int id, status_atual;

    printf("\nDigite o id do animal:\n");
    scanf("%d", &id);
    limpar_buffer();

    while (id < MINIMO_ID || id > contador_id) // caso o id seja inválido
    {
        printf("\nId invalido!\n");
        printf("Digite o id do animal:\n");
        scanf("%d", &id);
        limpar_buffer();
    }

    printf("\nDigite o status atual do servico:\n1 - Aguardando\n2 - Em andamento\n");
    scanf("%d", &status_atual);
    limpar_buffer();

    while (status_atual < AGUARDANDO || status_atual > ANDAMENTO)  // caso o status seja inválido
    {
        printf("\nStatus invalido!\n");
        printf("Digite o status atual do servico:\n1 - Aguardando\n2 - Em andamento\n");
        scanf("%d", &status_atual);
        limpar_buffer();
    }

    if (status_atual == AGUARDANDO) // caso o serviço esteja na fila de entrada
    {
        if (!remover_de_fila(&entrada, id)) 
        {
            printf("\nAnimal nao encontrado aguardando!\n");
            aguardar_usuario();
            return;
        }
    }
    else if (status_atual == ANDAMENTO) // caso o serviço esteja em andamento
    {
        if (!remover_de_array(id)) 
        {
            printf("\nServico em andamento nao encontrado!\n");
            aguardar_usuario();
            return;
        }
    }

    printf("\nServico cancelado com sucesso!\n");
    aguardar_usuario();
}

/**
 * @brief Entrega o animal para o tutor, removendo-o da fila de saída.
 * 
 * Remove o primeiro animal da fila de saída, exibindo seus dados no console.
*/
void entregar_animal(void) 
{
    if (saida == NULL) 
    {
        printf("\nNao ha animais para serem entregues!\n");
        aguardar_usuario();
        return;
    }

    Animal animal = saida->animal;
    desenfileirar(&saida);

    printf("\nEntregando seguinte animal:\n");
    printar_animal(animal);
    aguardar_usuario();
}

/* ==================== FIM DAS FUNÇÕES DO PETSHOP ==================== */

int main(void) 
{
    int opcao = -1;

    do 
    {
        mostrar_menu();
        scanf("%d", &opcao);
        limpar_buffer();

        if (opcao == 0)
        {
            printf("\nSaindo...\n");
            aguardar_usuario();
            break;
        }

        switch (opcao)
        {
            case 1:
                cadastrar_animal();
                break;
            case 2:
                atender_animal();
                break;
            case 3:
                alterar_status_servico();
                break;
            case 4:
                cancelar_servico();
                break;
            case 5:
                entregar_animal();
                break;
            case 6:
                printf("\n>>> Fila de entrada <<<\n\n");
                printar_fila(&entrada);
                break;
            case 7:
                printar_array();
                break;
            case 8:
                printf("\n>>> Fila de saida <<<\n\n");
                printar_fila(&saida);
                break;
            default:
                printf("\nOpcao invalida!\n\n");
                aguardar_usuario();
                break;
        }
        opcao = -1; // reseta opção, para evitar loop infinito com entrada inválida.
    } while (opcao != 0);

    limpar_fila(&entrada);
    limpar_fila(&saida);

    return 0;
}
