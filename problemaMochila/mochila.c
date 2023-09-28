#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define POP_SIZE 100
#define NUM_GENERATIONS 100
#define MUTATION_RATE 0.1

typedef struct {
    int peso;
    int valor;
} Item;

Item items[] = { //peso e valor dos itens decidido arbitrariamente
    {2, 12},
    {1, 10},
    {3, 20},
    {2, 15},
    {4, 4}
};

int capacidadeMochila = 5;

typedef struct {
    int cromossomo[5];  // 1 e 0 para representar itens incluidos ou não, respectivamente
    int fitness;
} Individual;

void initializePopulation(Individual populacao[]) {
    for (int i = 0; i < POP_SIZE; i++) {//inicializa a populacao incluindo aleatoriamente os itens
        for (int j = 0; j < sizeof(items) / sizeof(Item); j++) {
            populacao[i].cromossomo[j] = rand() % 2; // Inicialização aleatória
        }
    }
}

int calculateFitness(Individual individual) {
    int pesoTotal = 0;
    int valorTotal = 0;

    for (int i = 0; i < sizeof(items) / sizeof(Item); i++) {//calculo do peso e valor da mochila
        if (individual.cromossomo[i] == 1) {
            pesoTotal += items[i].peso;
            valorTotal += items[i].valor;
        }
    }

    
    if (pesoTotal > capacidadeMochila) {//invalida mochilas com peso maior do que o máximo permitido
        return 0;
    }

    return valorTotal;
}

void crossover(Individual *individual, Individual pai){
    for (int i = 0; i < sizeof(items) / sizeof(Item); i++) {//iguala o filho com o melhor candidato

        individual->cromossomo [i]= pai.cromossomo[i];

    }
    individual->fitness = pai.fitness; //altera aleatoriamente um dos itens do filho
    int i = (int)rand() % 5;

    individual->cromossomo[i] =  1 - individual->cromossomo[i]; 

}

void mutate(Individual *individual) {
    for (int i = 0; i < sizeof(items) / sizeof(Item); i++) {//respeitando a taxa de mutacao altera um ou mais itens aleatoriamente
        if ((double)rand() / RAND_MAX < MUTATION_RATE) {
            individual->cromossomo[i] = 1 - individual->cromossomo[i]; 
        }
    }
}

Individual selectParent(Individual populacao[]) {
   
    Individual melhorSolucao = populacao[0];
    for (int i = 1; i < POP_SIZE; i++) {//procura o melhor individuo da populacao
        if (populacao[i].fitness > melhorSolucao.fitness) {
            melhorSolucao = populacao[i];
        }
    }
    return melhorSolucao;
}

int main() {
    srand(time(NULL));

    Individual populacao[POP_SIZE];
    Individual filhos[POP_SIZE];

    initializePopulation(populacao);
    for (int i = 0; i < POP_SIZE; i++){//inicializacao da primeira geracao
        
        populacao[i].fitness = calculateFitness(populacao[i]);
    }
 

    for (int generation = 0; generation < NUM_GENERATIONS; generation++) {//para NUM_GENERATIONS geracoes e POP_SIZE individuos

      
        Individual pai = selectParent(populacao);//selecao do melhor da geracao

        for (int i = 0; i < POP_SIZE; i++) {//crossover e mutacao
            crossover(&filhos[i], pai);
            mutate(&filhos[i]);
            filhos[i].fitness = calculateFitness(filhos[i]);
        }

        for (int i = 0; i < POP_SIZE; i++) {//substituicao da nova geracao
            populacao[i] = filhos[i];
        }
    }

    Individual melhorSolucao = populacao[0];
    for (int i = 1; i < POP_SIZE; i++) {//melhor solucao de todas as geracoes
        if (populacao[i].fitness > melhorSolucao.fitness) {
            melhorSolucao = populacao[i];
        }
    }

    printf("Melhor valor encontrado: %d\n", melhorSolucao.fitness);
    printf("Itens selecionados:\n");
    for (int i = 0; i < sizeof(items) / sizeof(Item); i++) {
        if (melhorSolucao.cromossomo[i] == 1) {
            printf("Item %d - Peso: %d, Valor: %d\n", i + 1, items[i].peso, items[i].valor);
        }
    }

    return 0;
}
