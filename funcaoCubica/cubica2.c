#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h>
//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   token git
#define POP_SIZE_ALGEVO 100 //tamanho populacao
#define NUM_GENERATIONS_ALGEVO 100 // geracoes
#define MUTATION_RATE_ALGEVO 0.2 // taxa de mutacao // parametro do alg1 a ser modificado 
#define TARGET_FITNESS 0.0001 //criterio para validacao da resposta

typedef struct {
    int popSize;
    double mutationRate;
    int crossoverType;
    int fitness;

} IndividualAlgEvo;

typedef struct {
    double x;
    double fitness;
} Individual;

double evaluateFitness(double x) {//valor do individuo no contexto
    return fabs((2*cos(0.039*x) + 5*sin(0.05*x) + 0.5*cos(0.01*x) + 10*sin(0.07*x) + 5*sin(0.1*x) + 5*sin(0.035*x))*10+500);
    //return fabs(3 * x * x - 12 * x + 9);
}

void initializePopulationAlgEvo(IndividualAlgEvo populacao[]){
    for (int i = 0; i < POP_SIZE_ALGEVO; i++) {// inicializa com valores aleatorios e calcula seu valor na funcao
        populacao[i].popSize = (int)(((double)rand() / RAND_MAX) * 1000);// 0 a 1000
        populacao[i].mutationRate = (double)rand() / RAND_MAX;//0 a 1
        populacao[i].crossoverType = (int)rand() % 2;//0 ou 1
        populacao[i].fitness = -1;
    }
}

void initializePopulation(Individual populacao[], int popSize) {
    for (int i = 0; i < popSize; i++) {// inicializa com valores aleatorios e calcula seu valor na funcao
        populacao[i].x = (double)rand() / RAND_MAX * 10.0 - 5.0;
        populacao[i].fitness = evaluateFitness(populacao[i].x);
    }
}

void crossoverAlgEvo(IndividualAlgEvo *individual, IndividualAlgEvo best){
    if(best.crossoverType){//tipo 1 de crossover, media dos valores pai+filho
        individual->popSize = (individual->popSize + best.popSize)/2;
        individual->mutationRate = (individual->mutationRate + best.mutationRate)/2;
    }
    else{//tipo 2 de crossover, iguala um dos valroes do filho igual ao pai
        if ((double)rand() / RAND_MAX < 0.5) {
            individual->popSize =  best.popSize;
        }else{
            individual->mutationRate =  best.mutationRate;
        }


    }

}

void crossover(Individual *individual, Individual best){// media entre melhor de todos (pai) e individuo(filho)
  
    individual->x = (individual->x + best.x)/2;
    individual->fitness = evaluateFitness(individual->x);

}

void mutateAlgEvo(IndividualAlgEvo *individual){
     if ((double)rand() / RAND_MAX < MUTATION_RATE_ALGEVO) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        if((int)rand() % 2){
            individual->popSize += ((double)rand() / RAND_MAX) * (individual->popSize/2);// parametro do alg1 a ser modificado   

        }else{
            individual->popSize -= ((double)rand() / RAND_MAX) * (individual->popSize/2);// parametro do alg1 a ser modificado   
        }
    }

     if ((double)rand() / RAND_MAX < MUTATION_RATE_ALGEVO) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        if((int)rand() % 2){
            individual->mutationRate += ((double)rand() / RAND_MAX) * (individual->mutationRate/2);// parametro do alg1 a ser modificado 
        }else{
            individual->mutationRate -= ((double)rand() / RAND_MAX) * (individual->mutationRate/2);// parametro do alg1 a ser modificado   
        }
    }

     if ((double)rand() / RAND_MAX < MUTATION_RATE_ALGEVO) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        individual->crossoverType = 1 - individual->crossoverType ;// parametro do alg1 a ser modificado 
        
    }
}

void mutate(Individual *individual, double mutationRate) {
    if ((double)rand() / RAND_MAX < mutationRate) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        individual->x += ((double)rand() / RAND_MAX * 2.0 - 1.0) * 0.1;// parametro do alg1 a ser modificado 
        individual->fitness = evaluateFitness(individual->x);
    }
}

IndividualAlgEvo findBestAlgEvo(IndividualAlgEvo populacao[],int *best){
     IndividualAlgEvo melhor = populacao[0];
    for (int i = 1; i < POP_SIZE_ALGEVO; i++) {//percorre os individuos procurando o melhor
        if (populacao[i].fitness < melhor.fitness) {
            melhor = populacao[i];
            *best = i;
        }
    }
    return melhor;
}

Individual findBest(Individual populacao[], int popSize) {
    Individual melhor = populacao[0];
    for (int i = 1; i < popSize; i++) {//percorre os individuos procurando o melhor
        if (populacao[i].fitness < melhor.fitness) {
            melhor = populacao[i];
        }
    }
    return melhor;
}

int main() {

    srand(time(NULL));
    IndividualAlgEvo populacaoAlgEvo[POP_SIZE_ALGEVO];
    int controleBest;
    
    initializePopulationAlgEvo(populacaoAlgEvo);
    IndividualAlgEvo melhorAlgEvo = populacaoAlgEvo[0];

    for(int geracao = 0; geracao < NUM_GENERATIONS_ALGEVO; geracao++){
        for(int pop = 0; pop < POP_SIZE_ALGEVO; pop++){


            Individual populacao[populacaoAlgEvo[pop].popSize];
            int controleFitness = 1;
        
            initializePopulation(populacao,populacaoAlgEvo[pop].popSize);

            Individual melhor = findBest(populacao,populacaoAlgEvo[pop].popSize);//melhor da primeira geracao
        
            while(melhor.fitness>TARGET_FITNESS && controleFitness>0){
                for (int i = 0; i < populacaoAlgEvo[pop].popSize; i++) {
                    crossover(&populacao[i], melhor);
                    mutate(&populacao[i],populacaoAlgEvo[pop].mutationRate);
                    populacao[i].fitness = evaluateFitness(populacao[i].x);
                }
                
                melhor = findBest(populacao,populacaoAlgEvo[pop].popSize);
                controleFitness += 1;
                
                
            }

            populacaoAlgEvo[pop].fitness = controleFitness;

            printf("Aproximação da raiz encontrada: x = %lf, f'(x) = %lf\n", melhor.x, melhor.fitness);
            printf("Geracao: %d  Pop:%d\nFitness: %d\n\n", geracao,pop,populacaoAlgEvo[pop].fitness);

        }

        melhorAlgEvo = findBestAlgEvo(populacaoAlgEvo,&controleBest);
        for(int pop = 0; pop < POP_SIZE_ALGEVO; pop++){
            if(controleBest != pop){
                crossoverAlgEvo(&populacaoAlgEvo[pop], melhorAlgEvo);
                mutateAlgEvo(&populacaoAlgEvo[pop]); 
            }
            
        }
        

    }


    melhorAlgEvo = findBestAlgEvo(populacaoAlgEvo,&controleBest);
    printf("Melhor de todos\nPopSize: %d\nMutationRate: %lf\nCrossType: %d\nFitness: %d\n", melhorAlgEvo.popSize,melhorAlgEvo.mutationRate,melhorAlgEvo.crossoverType,melhorAlgEvo.fitness);


    
    return 0;
}
