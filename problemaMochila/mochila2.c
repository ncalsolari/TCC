#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 
#include <limits.h>



#define POP_SIZE_ALGEVO 100
#define NUM_GENERATIONS_ALGEVO 100
#define MUTATION_RATE_ALGEVO 0.1
#define MAX_GENERATIONS 200
#define NUM_ITEMS 50
#define MEDIA_ALG 3



typedef struct {
    int peso;
    int valor;
} Item;

Item items[NUM_ITEMS];

int capacidadeMochila = 100;


typedef struct {
    int cromossomo[NUM_ITEMS];  // 1 e 0 para representar itens incluidos ou não, respectivamente
    int fitness;
} Individual;

typedef struct {
    int popSize;
    int generation;
    double mutationRate;
    int crossoverType;
    int fitness;
Individual melhorResultado;

} IndividualAlgEvo;



void initializePopulationAlgEvo(IndividualAlgEvo populacao[]){
    for (int i = 0; i < POP_SIZE_ALGEVO; i++) {// inicializa com valores aleatorios e calcula seu valor na funcao
        populacao[i].popSize = (int)(((double)rand() / RAND_MAX) * 1000);// 0 a 1000
        populacao[i].generation = (int)(((double)rand() / RAND_MAX) * 1000);// 0 a 1000
        populacao[i].mutationRate = (double)rand() / RAND_MAX;//0 a 1
        populacao[i].crossoverType = (int)rand() % 2;//0 ou 1
        populacao[i].fitness = -1;
        for(int i = 0; i < NUM_ITEMS; i++){
        populacao[i].melhorResultado.cromossomo[i] = 0;
        }
        populacao[i].melhorResultado.fitness = INT_MAX;
    }
}

int calculateFitness(Individual individual) {
    int pesoTotal = 0;
    int valorTotal = 0;

    for (int i = 0; i < NUM_ITEMS; i++) {//calculo do peso e valor da mochila
        if (individual.cromossomo[i] == 1) {
            pesoTotal += items[i].peso;
            valorTotal += items[i].valor;
        }
    }

    
    if (pesoTotal > capacidadeMochila) {//invalida mochilas com peso maior do que o máximo permitido
        return -1;
    }

    return valorTotal;
}

void initializePopulation(Individual populacao[], int popSize) {
    for (int i = 0; i < popSize; i++) {//inicializa a populacao incluindo aleatoriamente os itens
        for (int j = 0; j < NUM_ITEMS; j++) {
            populacao[i].cromossomo[j] = rand() % 2; // Inicialização aleatória
        }

        populacao[i].fitness = calculateFitness(populacao[i]);
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

void crossover(Individual *individual, Individual pai, bool melhor){
    if(!melhor){

        for (int i = 0; i < NUM_ITEMS; i++) {//iguala o filho com o melhor candidato

            individual->cromossomo [i]= pai.cromossomo[i];

        }

        individual->fitness = pai.fitness; //altera aleatoriamente um dos itens do filho
        int i = (int)rand() % 5;

        individual->cromossomo[i] =  1 - individual->cromossomo[i]; 

    }
    

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
            individual->generation += ((double)rand() / RAND_MAX) * (individual->generation/2);// parametro do alg1 a ser modificado   

        }else{
            individual->generation -= ((double)rand() / RAND_MAX) * (individual->generation/2);// parametro do alg1 a ser modificado   
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
    for (int i = 0; i < NUM_ITEMS; i++) {//respeitando a taxa de mutacao altera um ou mais itens aleatoriamente
        if ((double)rand() / RAND_MAX < mutationRate) {
            individual->cromossomo[i] = 1 - individual->cromossomo[i]; 
        }
    }
}

IndividualAlgEvo findBestAlgEvo(IndividualAlgEvo populacao[],int *best){
     IndividualAlgEvo melhor = populacao[0];
    for (int i = 1; i < POP_SIZE_ALGEVO; i++) {//percorre os individuos procurando o melhor
        if ((populacao[i].fitness <= melhor.fitness && populacao[i].melhorResultado.fitness >= melhor.melhorResultado.fitness) || (melhor.melhorResultado.fitness == -1)) {
            melhor = populacao[i];
            *best = i;
        }
    }
    return melhor;
}



Individual findBest(Individual populacao[], int popSize, int* melhor, int* controleFitness) {
   
    Individual melhorSolucao = populacao[0];
    for (int i = 1; i < popSize; i++) {//procura o melhor individuo da populacao
        *controleFitness +=1;
        if (populacao[i].fitness > melhorSolucao.fitness) {
            melhorSolucao = populacao[i];
            *melhor = i;
        }
    }
    return melhorSolucao;
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
            media = melhores[i];
        }
    }

    return media;

}


int main() {
    srand(time(NULL));
    IndividualAlgEvo populacaoAlgEvo[POP_SIZE_ALGEVO];
    int controleBest = 0;
    int fitnessAlgEvo = 0;

    initializePopulationAlgEvo(populacaoAlgEvo);
    IndividualAlgEvo melhorAlgEvo = populacaoAlgEvo[0];

    Individual melhoresResultados [MEDIA_ALG];

    for (int i = 0; i < NUM_ITEMS; i++) {
        items[i].peso = 1 + rand() % 10;  // Peso aleatório entre 1 e 10 (ajuste conforme necessário)
        items[i].valor = 1 + rand() % 20; // Valor aleatório entre 1 e 20 (ajuste conforme necessário)
    }

    for(int geracao = 0; geracao < NUM_GENERATIONS_ALGEVO; geracao++){
        for(int pop = 0; pop < POP_SIZE_ALGEVO; pop++){
            for(int x = 0; x < MEDIA_ALG; x++){

                fitnessAlgEvo = 0; //quantas avaliacoes o codigo executou 
                int melhorTodos = 0;

                Individual populacao[populacaoAlgEvo[pop].popSize];
                Individual melhoresIndividuos[10000]; //max de geracoes q ele vai guardar

                initializePopulation(populacao, populacaoAlgEvo[pop].popSize); //inicializacao da primeira geracao

                Individual melhor = findBest(populacao, populacaoAlgEvo[pop].popSize, &melhorTodos, &fitnessAlgEvo);//selecao do melhor da geracao

                for(int j = 0; j < populacaoAlgEvo[pop].generation; j++){
                    melhorTodos = 0;
                    bool melhorControle = false;//variavel de controle para impedir a alteraraca do melhor da geracao
                    for (int i = 0; i < populacaoAlgEvo[pop].popSize; i++) {//crossover e mutacao
                        melhorControle = (melhorTodos == i) ? true : false;
                        crossover(&populacao[i], melhor, melhorControle);
                        mutate(&populacao[i], populacaoAlgEvo[pop].mutationRate);
                        populacao[i].fitness = calculateFitness(populacao[i]);
                    }

                
                    melhor = findBest(populacao,populacaoAlgEvo[pop].popSize,&melhorTodos, &fitnessAlgEvo);
                }

                melhoresResultados[x] = melhor;
            }
            Individual melhor = mediaIndividuo(melhoresResultados);
            populacaoAlgEvo[pop].melhorResultado = mediaIndividuo(melhoresResultados);
            populacaoAlgEvo[pop].fitness = fitnessAlgEvo; //quantas geracoes demoraram para chegar a uma resposta solida (5 respostas iguais seguidas)
            if(melhor.fitness == -1){
                int a;
                a=0;
            }

            printf("Melhor individuo (valor mochila): %d\n", melhor.fitness);
            printf("Avaliacoes feitas: %d\n", populacaoAlgEvo[pop].fitness);
            printf("Geracao: %d  Indiv: %d\n\n\n", geracao, pop);

            /*
            printf("Itens selecionados:\n");
            for (int i = 0; i < NUM_ITEMS; i++) {
                if (melhor.cromossomo[i] == 1) {
                    printf("Item %d - Peso: %d, Valor: %d\n", i + 1, items[i].peso, items[i].valor);
                }
            }
            */

            
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
    printf("Melhor de todos\nPopSize: %d\nGeneration: %d\nMutationRate: %lf\nCrossType: %d\nFitness: %d\n", melhorAlgEvo.popSize,melhorAlgEvo.generation,melhorAlgEvo.mutationRate,melhorAlgEvo.crossoverType,melhorAlgEvo.fitness);
    printf("Valor encontrado: %d\n", melhorAlgEvo.melhorResultado.fitness);

    printf("Itens selecionados:\n");
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (melhorAlgEvo.melhorResultado.cromossomo[i] == 1) {
            printf("Item %d - Peso: %d, Valor: %d\n", i + 1, items[i].peso, items[i].valor);
        }
    }

    return 0;
}
