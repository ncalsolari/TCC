#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 


#define N 4  // Número de cidades

int matrizDistancias[N][N] = {
    {0, 10, 15, 20},
    {10, 0, 35, 25},
    {15, 35, 0, 30},
    {20, 25, 30, 0}
};  // Matriz de distâncias entre as cidades

#define POP_SIZE 100  // Tamanho da população
#define NUM_GENERATIONS 1000  // Número máximo de gerações
#define MUTATION_RATE 0.1  // Taxa de mutação

//N cidades, entao o genes[N] eh a ordem em que se visita tais cidades
typedef struct {
    int genes[N];
    int fitness;
} Individuo;


void initializePopulation(Individuo populacao[]) {
    for (int i = 0; i < POP_SIZE; i++) { //inicializado com os valores de 0 a 3 em ordem (indicando as cidades 1, 2 3 e 4)
        for (int j = 0; j < N; j++) {
            populacao[i].genes[j] = j; 
        }
        for (int j = 0; j < N; j++) { //valores aleatorios sao distribuidos para os caminhos antes ordenados (genes[N])
            int pos = rand() % N;     
            int temp = populacao[i].genes[j]; 
            populacao[i].genes[j] = populacao[i].genes[pos];
            populacao[i].genes[pos] = temp;
        }
        populacao[i].fitness = 0;
    }
}

int calculateFitness(Individuo individuo) {
    int distTotal = 0;
    for (int i = 0; i < N - 1; i++) { //percorre e soma as distancias entre caminhos (cidade atual + proxima cidade)
        distTotal += matrizDistancias[individuo.genes[i]][individuo.genes[i + 1]];
    }
    distTotal += matrizDistancias[individuo.genes[N - 1]][individuo.genes[0]];
    return distTotal;
}

Individuo selectParent(Individuo populacao[], Individuo pais, int* melhor) {
    pais = populacao[0];

    for(int i=0; i<POP_SIZE; i++){//percorre a populacao procurando o melhor individuo, o que possui o melhor (menor valor) do fitness
        if(populacao[i].fitness < pais.fitness){
            pais = populacao[i];
            *melhor = i;
        }
    }
    return pais;
}

void crossover(Individuo populacao[], Individuo pai, Individuo *filhos, bool melhor) {
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

        /*a ideia aqui eh manter um dos genes iguais os do pai e reembaralhar os restantes
        e para isso as variaveis de controle ajudam a impedir duplicidade de valores que 
        viriam a formar caminhos inválidos*/


            for(int j = 0; j<genesCruzados; j++){

                int geneRandon = rand() % N; //posiçao dos genes q vou copiar do pai
                filhos->genes[geneRandon] = pai.genes[geneRandon]; //valor do gene
                cruzados[geneRandon][0] = 1; //salvo posicao modificada
                cruzados[geneRandon][1] = pai.genes[geneRandon];
                controle[pai.genes[geneRandon]] = 1;
            }

            for(int k = 0; k<N; k++){

                if(cruzados[k][0] != 1){ //quer dizer que nao modiqfiquei ainda esse gene
                    for(int l = 0; l < N; l++){
                        if (controle[l] == 0){ //varro pra achar uma cidade ainda nao visitada
                            filhos->genes[k] = l; //atribuo essa cidade ao caminho
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

            filhos->genes[i] = pai.genes[i];

        }
    }


}

void mutate(Individuo *individuo) {
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

int main() {
    srand(time(NULL));

    //inicializando variaveis de populacao
    Individuo populacao[POP_SIZE];
    Individuo novaPopulacao[POP_SIZE];
    Individuo *novaPopulacao2[POP_SIZE];

    Individuo melhoresIndividuos[NUM_GENERATIONS];

    initializePopulation(populacao);

    for (int i = 0; i < POP_SIZE; i++) {

        populacao[i].fitness = calculateFitness(populacao[i]);//calculo do fitness da geracao 0

    }

    for (int geracao = 0; geracao < NUM_GENERATIONS; geracao++) {//para cada NUM_GENERATIONS geracoes de POP_SIZE populacoes 
        
        Individuo pais;
        int melhorTodos = 0;
        pais = selectParent(populacao, pais, &melhorTodos);//selecao do melhor da geracao
        
        
        bool melhorControle = false;//variavel de controle para impedir a alteraraca do melhor da geracao
        for (int i = 0; i < POP_SIZE; i ++) {//crossover com o melhor de todos e mutacao aleatorioa

            melhorControle = (melhorTodos == i) ? true : false;
            crossover(populacao, pais, &novaPopulacao[i], melhorControle);
            mutate(&novaPopulacao[i]);
        
        }    
            
        for (int i = 0; i < POP_SIZE; i++) {//nova populacao eh substituida
            populacao[i] = novaPopulacao[i];
        }

        for (int i = 0; i < POP_SIZE; i++) {// calculo do fitness da nova geracao
            populacao[i].fitness = calculateFitness(populacao[i]);
        }


        int melhorFitness = populacao[0].fitness;
        melhoresIndividuos[geracao] = populacao[0];
        for (int i = 1; i < POP_SIZE; i++) { //guarda-se o melhor individuo dessa geracao
            if (populacao[i].fitness < melhorFitness) {
                melhorFitness = populacao[i].fitness;
                melhoresIndividuos[geracao] = populacao[i];
            }
        }
        
    }

    int melhorFitness = melhoresIndividuos[0].fitness;
    Individuo melhorIndividuo = melhoresIndividuos[0];
    for (int i = 1; i < NUM_GENERATIONS; i++) {//depois de calculadas todas as geracacoes encontra-se o melhor de todas elas
        if (melhoresIndividuos[i].fitness < melhorFitness) {
            melhorFitness = melhoresIndividuos[i].fitness;
            melhorIndividuo = melhoresIndividuos[i];
        }
    }

    printf("Melhor distância percorrida: %d\n", melhorFitness);
    printf("Melhor rota: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", melhorIndividuo.genes[i]);
    }
    printf("\n");

    return 0;
}
