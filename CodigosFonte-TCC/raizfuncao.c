#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h>
#include <limits.h>
#define POP_SIZE_ALGEVO 175 //tamanho populacao
#define NUM_GENERATIONS_ALGEVO 60 // geracoes
#define MUTATION_RATE_ALGEVO 0.3 // taxa de mutacao // parametro do alg1 a ser modificado 
#define TARGET_FITNESS 0.0001 //criterio para validacao da resposta
#define MEDIA_ALG 3 // Algoritmos filhos serao representados pela media deste numero - 2 (pois o melhor e o pior serao removidos)



typedef struct {
    double x; // Gene dos individuos do Algoritmo Filho
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
            individual->popSize += ((double)rand() / RAND_MAX) * (individual->popSize/5);

        }else{
            individual->popSize -= ((double)rand() / RAND_MAX) * (individual->popSize/5);   
        }
    }

    if ((double)rand() / RAND_MAX < MUTATION_RATE_ALGEVO) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        if((int)rand() % 2){
            individual->generation += ((double)rand() / RAND_MAX) * (individual->generation/5);

        }else{
            individual->generation -= ((double)rand() / RAND_MAX) * (individual->generation/5);  
        }
    }

    if ((double)rand() / RAND_MAX < MUTATION_RATE_ALGEVO) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        if((int)rand() % 2){
            individual->mutationRate += ((double)rand() / RAND_MAX) * (individual->mutationRate/5); 
        }else{
            individual->mutationRate -= ((double)rand() / RAND_MAX) * (individual->mutationRate/5);
        }
    }

    if ((double)rand() / RAND_MAX < MUTATION_RATE_ALGEVO) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        individual->crossoverType = 1 - individual->crossoverType ;
        
    }
}

void mutate(Individual *individual, double mutationRate) {
    if ((double)rand() / RAND_MAX < mutationRate) {//respeitando a taxa de mutacao acrescenta um pequeno valor ao individuo
        individual->x += ((double)rand() / RAND_MAX * 2.0 - 1.0) * 0.1; 
        individual->fitness = evaluateFitness(individual->x);
    }
}

IndividualAlgEvo findBestAlgEvo(IndividualAlgEvo populacao[],int *best){//selecao por equilibrio
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

/*IndividualAlgEvo findBestAlgEvo(IndividualAlgEvo populacao[],int *best){//selecao ponderada
    IndividualAlgEvo melhor = populacao[0];
    *best = 0;
    double notaMelhor;
    double notaAtual;

    for (int i = 1; i < POP_SIZE_ALGEVO; i++) {//percorre os individuos procurando o melhor
        notaMelhor = melhor.fitness*0.5 + melhor.melhorResultado.fitness*0.5; //media pndereada dos fitness, o 1 eh o inverso pois originalmente quanto menor o valor melhor e aqui eu quero o maior valor sendo melhor, logo 1/(menor Valor)
        notaAtual = populacao[i].fitness*0.5 + populacao[i].melhorResultado.fitness*0.5;
        if (notaAtual < notaMelhor) {
            melhor = populacao[i];
            *best = i;
        }
    }
    return melhor;
}*/

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
    Individual melhor = melhores[0]; 
    int indiceMelhor = 0;
    for(int i = 1; i<MEDIA_ALG; i++){// separa o melhor para eliminacao
        if(melhores[i].fitness < melhor.fitness){
            melhor = melhores[i];
            indiceMelhor = i;
        }
    }

    Individual pior = melhores[0];
    int indicePior = 0;
    for(int i = 1; i<MEDIA_ALG; i++){// separa o pior para eliminacao
        if(melhores[i].fitness > pior.fitness){
            pior = melhores[i];
            indicePior = i;
        }
    }

    Individual media;
    for(int i = 0; i<MEDIA_ALG; i++){// elimina pior e melhor
        if(i != indiceMelhor && i != indicePior){
            media.fitness = melhores[i].fitness;
            media.x = melhores[i].x;

        }
    }

    return media;

}



int main() {

    char *filename = "FUNCAONOVAAVALIACAO2.txt";

    // abre arquivo para escrita
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error opening the file %s", filename);
        return -1;
    }

    //inicializa variaveis do AP
    srand(time(NULL));
    IndividualAlgEvo populacaoAlgEvo[POP_SIZE_ALGEVO];
    int controleBest;
    
    initializePopulationAlgEvo(populacaoAlgEvo);
    IndividualAlgEvo melhorAlgEvo = populacaoAlgEvo[0];

    Individual melhoresResultados [MEDIA_ALG];
    int controleFitness = 0;

    for(int geracao = 0; geracao < NUM_GENERATIONS_ALGEVO; geracao++){ //para cada NUM_GENERATIONS geracoes de POP_SIZE populacoes 
        for(int pop = 0; pop < POP_SIZE_ALGEVO; pop++){
            for(int x = 0; x < MEDIA_ALG; x ++){
                //simulacao de cada individuo do AP
                //inicializa variaveis do AF
                Individual populacao[populacaoAlgEvo[pop].popSize];
                controleFitness = 0;
                initializePopulation(populacao,populacaoAlgEvo[pop].popSize);

                Individual melhor = findBest(populacao,populacaoAlgEvo[pop].popSize, &controleFitness);//melhor da primeira geracao
        
                for(int j = 0; j < populacaoAlgEvo[pop].generation; j ++){//para cada geracao e populacao definidas nos alelos dos individuos do AP, que representam uma simulacao do AF

                    for (int i = 0; i < populacaoAlgEvo[pop].popSize; i++) {//crossover e mutacao
                        crossover(&populacao[i], melhor);
                        mutate(&populacao[i],populacaoAlgEvo[pop].mutationRate);
                        populacao[i].fitness = evaluateFitness(populacao[i].x);
                    }  
                    melhor = findBest(populacao,populacaoAlgEvo[pop].popSize, &controleFitness);
                
                }

                melhoresResultados[x] = melhor;

            }
            Individual melhor = mediaIndividuo(melhoresResultados);
            populacaoAlgEvo[pop].melhorResultado = mediaIndividuo(melhoresResultados); //o melhor resultado alcançado
            populacaoAlgEvo[pop].fitness = controleFitness; //o nmero de avaliaçoes feitas
            if(melhor.fitness>6){
                int a;
                a= 2;
            }
            printf("Aproximação da raiz encontrada: x = %lf, f'(x) = %lf\n", melhor.x, melhor.fitness);
            printf("Geracao: %d  Pop:%d\nFitness: %d\n\n", geracao,pop,populacaoAlgEvo[pop].fitness);

        }

        melhorAlgEvo = findBestAlgEvo(populacaoAlgEvo,&controleBest);
        // escreve no arquivo
        fprintf(fp, "%d, %d, %lf;\n",geracao, melhorAlgEvo.fitness, melhorAlgEvo.melhorResultado.fitness);

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
    
    // fecha o arquivo
    fclose(fp);
    return 0;
}









/*
//Algoritmo Filho o qual resolve o problema da raiz de funao sem o Algoritmo Pai implementado
int main() {

    srand(time(NULL));
    Individual melhoresResultados [MEDIA_ALG];
    int controleFitness = 0;

    Individual populacao[populacaoAlgEvo[pop].popSize];
    controleFitness = 0;

    initializePopulation(populacao,populacaoAlgEvo[pop].popSize);

    Individual melhor = findBest(populacao,populacaoAlgEvo[pop].popSize, &controleFitness);//melhor da primeira geracao

    for(int j = 0; j < populacaoAlgEvo[pop].generation; j ++){

        for (int i = 0; i < populacaoAlgEvo[pop].popSize; i++) {
            crossover(&populacao[i], melhor);
            mutate(&populacao[i],populacaoAlgEvo[pop].mutationRate);
            populacao[i].fitness = evaluateFitness(populacao[i].x);
        }  
        melhor = findBest(populacao,populacaoAlgEvo[pop].popSize, &controleFitness);
    
    }

    return 0;
}

*/