#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h>
#include <limits.h>
//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   token git
#define POP_SIZE_ALGEVO 100 //tamanho populacao
#define NUM_GENERATIONS_ALGEVO 200 // geracoes
#define MUTATION_RATE_ALGEVO 0.2 // taxa de mutacao // parametro do alg1 a ser modificado 
#define TARGET_FITNESS 0.0001 //criterio para validacao da resposta
#define MEDIA_ALG 3 //algoritmos filhos serao representados pela media deste numero - 2 (pois o melhor e o pior serao removidos)



typedef struct {
    double x;
    double fitness;
} Individual;

typedef struct {
    int popSize;
    int generation;
    double mutationRate;
    int crossoverType;
    int fitness;
    Individual melhorResultado;

} IndividualAlgEvo;

double evaluateFitness(double x) {//valor do individuo no contexto
    return fabs((2*cos(0.039*x) + 5*sin(0.05*x) + 0.5*cos(0.01*x) + 10*sin(0.07*x) + 5*sin(0.1*x) + 5*sin(0.035*x))*10+500);
   // return fabs(3 * x * x - 12 * x + 9);
}

void initializePopulationAlgEvo(IndividualAlgEvo populacao[]){
    for (int i = 0; i < POP_SIZE_ALGEVO; i++) {// inicializa com valores aleatorios e calcula seu valor na funcao
        populacao[i].popSize = (int)(((double)rand() / RAND_MAX) * 1000);// 0 a 1000
        populacao[i].generation = (int)(((double)rand() / RAND_MAX) * 1000);// 0 a 1000
        populacao[i].mutationRate = (double)rand() / RAND_MAX;//0 a 1
        populacao[i].crossoverType = (int)rand() % 2;//0 ou 1
        populacao[i].fitness = -1;
        populacao[i].melhorResultado.x = INT_MAX;
        populacao[i].melhorResultado.fitness = INT_MAX;

    }
}

void initializePopulation(Individual populacao[], int popSize) {
    for (int i = 0; i < popSize; i++) {// inicializa com valores aleatorios e calcula seu valor na funcao
        int chance = (int)rand()%2;
        populacao[i].x = chance ? ((double)rand()/RAND_MAX) * 1000 : (-1)*(((double)rand()/RAND_MAX) * 1000);
        populacao[i].fitness = evaluateFitness(populacao[i].x);
    }
}

void crossoverAlgEvo(IndividualAlgEvo *individual, IndividualAlgEvo best){
    if(best.crossoverType){//tipo 1 de crossover, media dos valores pai+filho
        individual->popSize = (individual->popSize + best.popSize)/2;
        individual->generation = (individual->generation + best.generation)/2;
        individual->mutationRate = (individual->mutationRate + best.mutationRate)/2;
    }
    else{//tipo 2 de crossover, iguala um dos valroes do filho igual ao pai
        double chance = (double)rand() / RAND_MAX;
        if (chance < 0.3) {
            individual->popSize =  best.popSize;
        }else if(chance < 0.6){
            individual->mutationRate =  best.mutationRate;
        }else{
            individual->generation = best.generation;
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
            individual->popSize += ((double)rand() / RAND_MAX) * (individual->popSize/5);// parametro do alg1 a ser modificado   

        }else{
            individual->popSize -= ((double)rand() / RAND_MAX) * (individual->popSize/5);// parametro do alg1 a ser modificado   
        }
    }

    if ((double)rand() / RAND_MAX < MUTATION_RATE_ALGEVO) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        if((int)rand() % 2){
            individual->generation += ((double)rand() / RAND_MAX) * (individual->generation/5);// parametro do alg1 a ser modificado   

        }else{
            individual->generation -= ((double)rand() / RAND_MAX) * (individual->generation/5);// parametro do alg1 a ser modificado   
        }
    }

    if ((double)rand() / RAND_MAX < MUTATION_RATE_ALGEVO) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        if((int)rand() % 2){
            individual->mutationRate += ((double)rand() / RAND_MAX) * (individual->mutationRate/5);// parametro do alg1 a ser modificado 
        }else{
            individual->mutationRate -= ((double)rand() / RAND_MAX) * (individual->mutationRate/5);// parametro do alg1 a ser modificado   
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
     *best = 0;
    for (int i = 1; i < POP_SIZE_ALGEVO; i++) {//percorre os individuos procurando o melhor
        if (populacao[i].fitness <= melhor.fitness && populacao[i].melhorResultado.fitness <= melhor.melhorResultado.fitness) {
            melhor = populacao[i];
            *best = i;
        }
    }
    return melhor;
}

Individual findBest(Individual populacao[], int popSize, int* controleFitness) {
    Individual melhor = populacao[0];
    for (int i = 1; i < popSize; i++) {//percorre os individuos procurando o melhor
    *controleFitness += 1;
        if (populacao[i].fitness < melhor.fitness) {
            melhor = populacao[i];
        }
    }
    return melhor;
}

Individual mediaIndividuo(Individual melhores[]){
    Individual melhor = melhores[0]; //vasculho pelo melhor
    int indiceMelhor = 0;
    for(int i = 1; i<MEDIA_ALG; i++){
        if(melhores[i].fitness < melhor.fitness){
            melhor = melhores[i];
            indiceMelhor = i;
        }
    }

    Individual pior = melhores[0];
    int indicePior = 0;
    for(int i = 1; i<MEDIA_ALG; i++){
        if(melhores[i].fitness > pior.fitness){
            pior = melhores[i];
            indicePior = i;
        }
    }

    Individual media;
    for(int i = 0; i<MEDIA_ALG; i++){
        if(i != indiceMelhor && i != indicePior){
            media.fitness = melhores[i].fitness;
            media.x = melhores[i].x;

        }
    }

    return media;

}

int mediaFitness(int melhores[]){
    int melhor = melhores[0]; //vasculho pelo melhor
    int indiceMelhor;
    for(int i = 1; i<MEDIA_ALG; i++){
        if(melhores[i] < melhor){
            melhor = melhores[i];
            indiceMelhor = i;
        }
    }

    int pior = melhores[0];
    int indicePior;
    for(int i = 1; i<MEDIA_ALG; i++){
        if(melhores[i] > pior){
            pior = melhores[i];
            indicePior = i;
        }
    }

    int media;
    for(int i = 1; i<MEDIA_ALG; i++){
        if(i != indiceMelhor && i != indicePior){
            media += melhores[i];

        }
    }

    media = media/(MEDIA_ALG-2);

    return media;

}



int main() {

    srand(time(NULL));
    IndividualAlgEvo populacaoAlgEvo[POP_SIZE_ALGEVO];
    int controleBest;
    
    initializePopulationAlgEvo(populacaoAlgEvo);
    IndividualAlgEvo melhorAlgEvo = populacaoAlgEvo[0];

    Individual melhoresResultados [MEDIA_ALG];
    int controleFitness = 0;

    for(int geracao = 0; geracao < NUM_GENERATIONS_ALGEVO; geracao++){
        for(int pop = 0; pop < POP_SIZE_ALGEVO; pop++){
            for(int x = 0; x < MEDIA_ALG; x ++){

                Individual populacao[populacaoAlgEvo[pop].popSize];
                controleFitness = 0;
        
                initializePopulation(populacao,populacaoAlgEvo[pop].popSize);

                Individual melhor = findBest(populacao,populacaoAlgEvo[pop].popSize, &controleFitness);//melhor da primeira geracao
        
                // while(melhor.fitness>TARGET_FITNESS && controleFitness>0){
                for(int j = 0; j < populacaoAlgEvo[pop].generation; j ++){

                    for (int i = 0; i < populacaoAlgEvo[pop].popSize; i++) {
                        crossover(&populacao[i], melhor);
                        mutate(&populacao[i],populacaoAlgEvo[pop].mutationRate);
                        populacao[i].fitness = evaluateFitness(populacao[i].x);
                    }  
                    melhor = findBest(populacao,populacaoAlgEvo[pop].popSize, &controleFitness);
                
                }

                melhoresResultados[x] = melhor;

            }
            Individual melhor = mediaIndividuo(melhoresResultados);
            populacaoAlgEvo[pop].melhorResultado = mediaIndividuo(melhoresResultados); //melhor; //o melhor resultado alcançado
            populacaoAlgEvo[pop].fitness = controleFitness; //mediaFitness(melhoresFitness); //controleFitness; //o nmero de avaliaçoes feitas
            if(melhor.fitness>6){
                int a;
                a= 2;
            }
            printf("Aproximação da raiz encontrada: x = %lf, f'(x) = %lf\n", melhor.x, melhor.fitness);
            printf("Geracao: %d  Pop:%d\nFitness: %d\n\n", geracao,pop,populacaoAlgEvo[pop].fitness);

        }

        melhorAlgEvo = findBestAlgEvo(populacaoAlgEvo,&controleBest);
        //melhorAlgEvo = findBestAlgEvo(populacaoAlgEvo,&controleBest);

        for(int pop = 0; pop < POP_SIZE_ALGEVO; pop++){
            if(controleBest != pop){
                crossoverAlgEvo(&populacaoAlgEvo[pop], melhorAlgEvo);
                mutateAlgEvo(&populacaoAlgEvo[pop]); 
            }
            
        }
        

    }


    melhorAlgEvo = findBestAlgEvo(populacaoAlgEvo,&controleBest);
    printf("Melhor de todos\nPopSize: %d\nGeneration: %d\nMutationRate: %lf\nCrossType: %d\nFitness: %d\n", melhorAlgEvo.popSize,melhorAlgEvo.generation,melhorAlgEvo.mutationRate,melhorAlgEvo.crossoverType,melhorAlgEvo.fitness);
    printf("melhor valor x: %lf\nf(x): %lf\n",melhorAlgEvo.melhorResultado.x, melhorAlgEvo.melhorResultado.fitness);
    
    return 0;
}
