#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define POP_SIZE 100 //tamanho populacao
#define NUM_GENERATIONS 1000000 // geracoes
#define MUTATION_RATE 0.2 // taxa de mutacao // parametro do alg1 a ser modificado 
#define TARGET_FITNESS 0.0001 //criterio para validacao da resposta

typedef struct {
    double x;
    double fitness;
} Individual;

double evaluateFitness(double x) {//valor do individuo no contexto
    return fabs((2*cos(0.039*x) + 5*sin(0.05*x) + 0.5*cos(0.01*x) + 10*sin(0.07*x) + 5*sin(0.1*x) + 5*sin(0.035*x))*10+500);
   // return fabs(3 * x * x - 12 * x + 9);
}

void initializePopulation(Individual populacao[]) {
    srand( (unsigned)time(NULL) );
    for (int i = 0; i < POP_SIZE; i++) {// inicializa com valores aleatorios e calcula seu valor na funcao
        populacao[i].x = ((double)rand() * 2000)/RAND_MAX;
        populacao[i].fitness = evaluateFitness(populacao[i].x);
    }
}

void crossover(Individual *individual, Individual best){// media entre melhor de todos (pai) e individuo(filho)
  
    individual->x = (individual->x + best.x)/2;
    individual->fitness = evaluateFitness(individual->x);

}

void mutate(Individual *individual) {
    if ((double)rand() / RAND_MAX < MUTATION_RATE) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        individual->x += ((double)rand() / RAND_MAX * 2.0 - 1.0) * 0.1;// parametro do alg1 a ser modificado 
        individual->fitness = evaluateFitness(individual->x);
    }
}

Individual findBest(Individual populacao[]) {
    Individual melhor = populacao[0];
    for (int i = 1; i < POP_SIZE; i++) {//percorre os individuos procurando o melhor
        if (populacao[i].fitness < melhor.fitness) {
            melhor = populacao[i];
        }
    }
    return melhor;
}

int main() {
    srand(time(NULL));

    Individual populacao[POP_SIZE];
    
    initializePopulation(populacao);

    Individual melhor = findBest(populacao);//melhor da primeira geracao
    for (int geracao = 0; geracao < NUM_GENERATIONS; geracao++) {//para NUM_GENERATIONS geracoes e POP_SIZE individuos
        for (int i = 0; i < POP_SIZE; i++) {
            crossover(&populacao[i], melhor);
            mutate(&populacao[i]);
            populacao[i].fitness = evaluateFitness(populacao[i].x);
        }
        melhor = findBest(populacao);
        
        if (melhor.fitness < TARGET_FITNESS) {
            printf("Aproximação da raiz encontrada: x = %lf, f'(x) = %lf\n", melhor.x, melhor.fitness);
            break;
        }
    }
    printf("Aproximação da raiz encontrada: x = %lf, f'(x) = %lf\n", melhor.x, melhor.fitness);

    
    return 0;
}
