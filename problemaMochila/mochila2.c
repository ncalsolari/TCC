#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 


#define POP_SIZE_ALGEVO 100
#define NUM_GENERATIONS_ALGEVO 100
#define MUTATION_RATE_ALGEVO 0.1
#define MAX_GENERATIONS 200



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
    int popSize;
    double mutationRate;
    int crossoverType;
    int fitness;

} IndividualAlgEvo;


typedef struct {
    int cromossomo[5];  // 1 e 0 para representar itens incluidos ou não, respectivamente
    int fitness;
} Individual;


void initializePopulationAlgEvo(IndividualAlgEvo populacao[]){
    for (int i = 0; i < POP_SIZE_ALGEVO; i++) {// inicializa com valores aleatorios e calcula seu valor na funcao
        populacao[i].popSize = (int)(((double)rand() / RAND_MAX) * 1000);// 0 a 1000
        populacao[i].mutationRate = (double)rand() / RAND_MAX;//0 a 1
        populacao[i].crossoverType = (int)rand() % 2;//0 ou 1
        populacao[i].fitness = -1;
    }
}

void initializePopulation(Individual populacao[], int popSize) {
    for (int i = 0; i < popSize; i++) {//inicializa a populacao incluindo aleatoriamente os itens
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

void crossover(Individual *individual, Individual pai, bool melhor){
    if(!melhor){

        for (int i = 0; i < sizeof(items) / sizeof(Item); i++) {//iguala o filho com o melhor candidato

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
    for (int i = 0; i < sizeof(items) / sizeof(Item); i++) {//respeitando a taxa de mutacao altera um ou mais itens aleatoriamente
        if ((double)rand() / RAND_MAX < mutationRate) {
            individual->cromossomo[i] = 1 - individual->cromossomo[i]; 
        }
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


Individual selectParent(Individual populacao[], int popSize, int* melhor) {
   
    Individual melhorSolucao = populacao[0];
    for (int i = 1; i < popSize; i++) {//procura o melhor individuo da populacao
        if (populacao[i].fitness > melhorSolucao.fitness) {
            melhorSolucao = populacao[i];
            *melhor = i;
        }
    }
    return melhorSolucao;
}

int main() {
    srand(time(NULL));
    IndividualAlgEvo populacaoAlgEvo[POP_SIZE_ALGEVO];
    int controleBest;

    initializePopulationAlgEvo(populacaoAlgEvo);
    IndividualAlgEvo melhorAlgEvo = populacaoAlgEvo[0];

    for(int geracao = 0; geracao < NUM_GENERATIONS_ALGEVO; geracao++){
        for(int pop = 0; pop < POP_SIZE_ALGEVO; pop++){

            int controleGeracao = 0;
            int fitnessAlgEvo = -1; //quantas geracoes demoraram para se alcancar uma resposta solida

            Individual populacao[populacaoAlgEvo[pop].popSize];
            Individual filhos[populacaoAlgEvo[pop].popSize];
            Individual melhoresIndividuos[10000]; //max de geracoes q ele vai guardar

            initializePopulation(populacao, populacaoAlgEvo[pop].popSize);
            for (int i = 0; i < populacaoAlgEvo[pop].popSize; i++){//inicializacao da primeira geracao
                
                populacao[i].fitness = calculateFitness(populacao[i]);
            }
        

           // for (int generation = 0; generation < populacaoAlgEvo[pop]; generation++) {//para NUM_GENERATIONS geracoes e POP_SIZE individuos
            while(controleGeracao<5 && fitnessAlgEvo < MAX_GENERATIONS){
                fitnessAlgEvo += 1;
                int melhorTodos = 0;
            
                Individual pai = selectParent(populacao, populacaoAlgEvo[pop].popSize, &melhorTodos);//selecao do melhor da geracao

                bool melhorControle = false;//variavel de controle para impedir a alteraraca do melhor da geracao
                for (int i = 0; i < populacaoAlgEvo[pop].popSize; i++) {//crossover e mutacao
                    
                    melhorControle = (melhorTodos == i) ? true : false;
                    crossover(&filhos[i], pai, melhorControle);
                    mutate(&filhos[i], populacaoAlgEvo[pop].mutationRate);
                    filhos[i].fitness = calculateFitness(filhos[i]);
                }

                for (int i = 0; i < populacaoAlgEvo[pop].popSize; i++) {//substituicao da nova geracao
                    populacao[i] = filhos[i];
                }

                Individual melhorFitness = populacao[0];
                melhoresIndividuos[fitnessAlgEvo] = populacao[0];
                for (int i = 1; i < populacaoAlgEvo[pop].popSize; i++) {//melhor solucao de todas as geracoes
                    if (populacao[i].fitness > melhorFitness.fitness) {
                        melhorFitness = populacao[i];
                        melhoresIndividuos[fitnessAlgEvo] = populacao[i];
                    }
                }

                if(fitnessAlgEvo > 0 && (melhoresIndividuos[fitnessAlgEvo].fitness == melhoresIndividuos[fitnessAlgEvo-1].fitness)){
                    controleGeracao += 1;
                }

            }

           
           Individual melhorFitness = populacao[0];
            melhoresIndividuos[fitnessAlgEvo] = populacao[0];
            for (int i = 1; i < populacaoAlgEvo[pop].popSize; i++) {//melhor solucao de todas as geracoes
                if (populacao[i].fitness > melhorFitness.fitness) {
                    melhorFitness = populacao[i];
                    melhoresIndividuos[fitnessAlgEvo] = populacao[i];
                }
            }

            printf("Melhor valor encontrado: %d\n", melhorFitness.fitness);
            printf("Itens selecionados:\n");
            for (int i = 0; i < sizeof(items) / sizeof(Item); i++) {
                if (melhorFitness.cromossomo[i] == 1) {
                    printf("Item %d - Peso: %d, Valor: %d\n", i + 1, items[i].peso, items[i].valor);
                }
            }

            populacaoAlgEvo[pop].fitness = fitnessAlgEvo; //quantas geracoes demoraram para chegar a uma resposta solida (5 respostas iguais seguidas)
            printf("Melhor fitness do indiv da simulacao: %d\n", populacaoAlgEvo[pop].fitness);
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
