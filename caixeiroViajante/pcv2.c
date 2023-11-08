#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 

#define POP_SIZE_ALGEVO 150 // Tamanho da população
#define NUM_GENERATIONS_ALGEVO 75  // Número máximo de gerações
#define MUTATION_RATE_ALGEVO 0.5  // Taxa de mutação
#define N 50 // Número de cidades
#define MEDIA_ALG 3


int matrizDistancias[N][N]; //AUMENTAR TAMANHO DA MATRIZ (NMERO DE CIDADES) PARA OS TESTES, ESTA ACHANDO A RESOLUCAO MTO RAPIDO
// Matriz de distâncias entre as cidades


//N cidades, entao o genes[N] eh a ordem em que se visita tais cidades
typedef struct {
    int genes[N];
    int fitness;
} Individual;

typedef struct{
    int generation;
    int popSize;
    double mutationRate;
    int crossoverType;
    int fitness;
    Individual melhorResultado;
} IndividualAlgEvo;

int calculateFitness(Individual individuo) {
    int distTotal = 0;
    for (int i = 0; i < N - 1; i++) { //percorre e soma as distancias entre caminhos (cidade atual + proxima cidade)
        distTotal += matrizDistancias[individuo.genes[i]][individuo.genes[i + 1]];
    }
    distTotal += matrizDistancias[individuo.genes[N - 1]][individuo.genes[0]];
    return distTotal;
}

void initializePopulationAlgEvo(IndividualAlgEvo populacao[]){
    for (int i = 0; i < POP_SIZE_ALGEVO; i++) { //inicializado com os valores de 0 a 3 em ordem (indicando as cidades 1, 2 3 e 4)
        
        populacao[i].generation = (int)(((double)rand() / RAND_MAX) * 1000);// 0 a 1000
        populacao[i].popSize = (int)(((double)rand() / RAND_MAX) * 1000);// 0 a 1000
        populacao[i].mutationRate = (double)rand() / RAND_MAX;//0 a 1
        populacao[i].crossoverType = (int)rand() % 2;//0 ou 1
        populacao[i].fitness = -1;

    }

}

void initializePopulation(Individual populacao[], int popSize) {
    for (int i = 0; i < popSize; i++) { //inicializado com os valores de 0 a 3 em ordem (indicando as cidades 1, 2 3 e 4)
        for (int j = 0; j < N; j++) {
            populacao[i].genes[j] = j; 
        }
        for (int j = 0; j < N; j++) { //valores aleatorios sao distribuidos para os caminhos antes ordenados (genes[N])
            int pos = rand() % N;     
            int temp = populacao[i].genes[j]; 
            populacao[i].genes[j] = populacao[i].genes[pos];
            populacao[i].genes[pos] = temp;
        }
         populacao[i].fitness = calculateFitness(populacao[i]);//calculo do fitness da geracao 0

    }
}


IndividualAlgEvo findBestAlgEvo(IndividualAlgEvo populacao[],int *best){
    IndividualAlgEvo melhor = populacao[0];
    int notaMelhor;
    int notaAtual;

    for (int i = 1; i < POP_SIZE_ALGEVO; i++) {//percorre os individuos procurando o melhor
       // notaMelhor = (1/melhor.fitness)*0.5 + melhor.melhorResultado.fitness*0.5; //media pndereada dos fitness, o 1 eh o inverso pois originalmente quanto menor o valor melhor e aqui eu quero o maior valor sendo melhor, logo 1/(menor Valor)
       // notaAtual = (1/populacao[i].fitness)*0.5 + populacao[i].melhorResultado.fitness*0.5;
       // if ((notaAtual > notaMelhor) || (melhor.melhorResultado.fitness == -1)) {
        if (populacao[i].fitness <= melhor.fitness && populacao[i].melhorResultado.fitness <= melhor.melhorResultado.fitness) {
            melhor = populacao[i];
            *best = i;
        }
    }
    return melhor;
}

Individual findBest(Individual populacao[], int* controleFitness, int* melhor, int popSize) {
    
    Individual melhorSolucao = populacao[0];
    for(int i=0; i<popSize; i++){//percorre a populacao procurando o melhor individuo, o que possui o melhor (menor valor) do fitness
        *controleFitness +=1;
        if(populacao[i].fitness < melhorSolucao.fitness){
            melhorSolucao = populacao[i];
            *melhor = i;
        }
    }
    return melhorSolucao;
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

void crossover(Individual *populacao, Individual pai, bool melhor) {
    int genesCruzados = N/2 ; // parametro do alg1 a ser modificado 
    int modificados[genesCruzados];
    int cruzados[N][2];
    int controle[N];

  
    if(!melhor){ //o melhor individuo nao sofre crossover

         for(int a = 0; a < N; a++){// inicializacao de variaveis de controle
            controle[a] = 0;  //controle das cidades ja visitadas
            cruzados[a][0] = 0; //se posicao (ordem das cidades) foi modificada entao 1 
            cruzados[a][1] = -1; //valor da posicao modificada (qual cidade)
        }

        /*a ideia aqui eh manter metade dos genes iguais aos do pai e reembaralhar os restantes
        e para isso as variaveis de controle ajudam a impedir duplicidade de valores que 
        viriam a formar caminhos inválidos*/


        for(int j = 0; j<genesCruzados; j++){

            int geneRandon = rand() % N; //posiçao dos genes q vou copiar do pai
            populacao->genes[geneRandon] = pai.genes[geneRandon]; //valor do gene
            cruzados[geneRandon][0] = 1; //salvo posicao modificada
            cruzados[geneRandon][1] = pai.genes[geneRandon];
            controle[pai.genes[geneRandon]] = 1;
        }

        for(int k = 0; k<N; k++){

            if(cruzados[k][0] != 1){ //quer dizer que nao modiqfiquei ainda esse gene
                for(int l = 0; l < N; l++){
                    if (controle[l] == 0){ //varro pra achar uma cidade ainda nao visitada
                        populacao->genes[k] = l; //atribuo essa cidade ao caminho
                        cruzados[k][0] = 1; //atualizo variaveis de controle
                        cruzados[k][1] = l; //salvo qual gene modifiquei e qual seu valor
                        controle[l] = 1; //salvo que a cidade l ja foi visitada
                        l = N; //pulo para analisar o proximo gene
                        
                    }

                }
            }


        }

    }else{
        for(int i = 0; i < N; i++){

            populacao->genes[i] = pai.genes[i];

        }
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

void mutate(Individual *individuo) {
    int taxaMutacao = 3; /// parametro do alg1 a ser modificado 
    int mutacao = rand() % 10;

    if(mutacao <= taxaMutacao){// sao invertidos dois valores de caminho (duas cidades sao trocadas de lugar)
        int gene1 = rand() % N;
        int gene2 = rand() % N;


        int temp = individuo->genes[gene1];
        individuo->genes[gene1] = individuo->genes[gene2];
        individuo->genes[gene2] = temp;

    }
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

    char *filename = "CAIXEIRO5.txt";

    // open the file for writing
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error opening the file %s", filename);
        return -1;
    }

    srand(time(NULL));
    IndividualAlgEvo populacaoAlgEvo[POP_SIZE_ALGEVO];
    int controleBest = 0;
    int fitnessAlgEvo = 0;

    initializePopulationAlgEvo(populacaoAlgEvo);
    IndividualAlgEvo melhorAlgEvo = populacaoAlgEvo[0];

    Individual melhoresResultados [MEDIA_ALG];


    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
             matrizDistancias[i][j] = rand() % 100; // Gere números aleatórios entre 0 e 99 
            // Reflete os valores para a metade inferior da matriz
            matrizDistancias[j][i] = matrizDistancias[i][j];
        }
    }

        // Imprime a matriz
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%4d ", matrizDistancias[i][j]);
        }
        printf("\n");
    }




    for (int geracao = 0; geracao < NUM_GENERATIONS_ALGEVO; geracao++){  //para cada NUM_GENERATIONS geracoes de POP_SIZE populacoes 
        for (int pop = 0; pop < POP_SIZE_ALGEVO; pop++){   
            for(int x = 0; x < MEDIA_ALG; x++){
            //inicializando variaveis de populacao
                int melhorTodos = 0;
                fitnessAlgEvo = 0; //total de avaliacoes feitas


                Individual populacao[populacaoAlgEvo[pop].popSize];

                initializePopulation(populacao,populacaoAlgEvo[pop].popSize);

                Individual melhor = findBest(populacao, &fitnessAlgEvo, &melhorTodos, populacaoAlgEvo[pop].popSize);//selecao do melhor da geracao

            
                for(int j = 0; j < populacaoAlgEvo[pop].generation; j ++){ 
                    
                    melhorTodos = 0;
                    bool melhorControle = false;//variavel de controle para impedir a alteraraca do melhor da geracao
                    for (int i = 0; i < populacaoAlgEvo[pop].popSize; i ++) {//crossover com o melhor de todos e mutacao aleatorioa

                        melhorControle = (melhorTodos == i) ? true : false;
                        crossover(&populacao[i], melhor, melhorControle);
                        mutate(&populacao[i]);
                        populacao[i].fitness = calculateFitness(populacao[i]);
                    
                    }    

                    melhor = findBest(populacao, &fitnessAlgEvo, &melhorTodos, populacaoAlgEvo[pop].popSize);
                
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

            
        }


        melhorAlgEvo = findBestAlgEvo(populacaoAlgEvo,&controleBest);
        // write to the text file
        fprintf(fp, "%d, %d, %d;\n",geracao, melhorAlgEvo.fitness, melhorAlgEvo.melhorResultado.fitness);
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
    //printf("Melhor de todos\nPopSize: %d\nMutationRate: %lf\nCrossType: %d\nFitness: %d\n", melhorAlgEvo.popSize,melhorAlgEvo.mutationRate,melhorAlgEvo.crossoverType,melhorAlgEvo.fitness);


   
    // close the file
    fclose(fp);
    return 0;
}
