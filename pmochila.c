#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ITENS 5
#define CAPACIDADE_MOCHILA 16
#define TAMANHO_POPULACAO 50
#define TAXA_MUTACAO 0.1
#define NUM_GERACOES 100

int pesos[NUM_ITENS] = {2, 3, 4, 5, 9};
int valores[NUM_ITENS] = {3, 4, 5, 8, 10};

typedef struct {
    int genes[NUM_ITENS];
    int fitness;
} Individuo;

int calcularFitness(Individuo individuo) {
    int peso_total = 0;
    int valor_total = 0;
    
    for (int i = 0; i < NUM_ITENS; i++) {
        peso_total += individuo.genes[i] * pesos[i];
        valor_total += individuo.genes[i] * valores[i];
    }
    
    if (peso_total > CAPACIDADE_MOCHILA) {
        return 0;
    } else {
        return valor_total;
    }
}

void cruzamento(Individuo pai1, Individuo pai2, Individuo *filho1, Individuo *filho2) {
    int ponto_corte = rand() % NUM_ITENS;
    
    for (int i = 0; i < NUM_ITENS; i++) {
        if (i < ponto_corte) {
            filho1->genes[i] = pai1.genes[i];
            filho2->genes[i] = pai2.genes[i];
        } else {
            filho1->genes[i] = pai2.genes[i];
            filho2->genes[i] = pai1.genes[i];
        }
    }
}

void mutacao(Individuo *individuo) {
    for (int i = 0; i < NUM_ITENS; i++) {
        if ((double)rand() / RAND_MAX < TAXA_MUTACAO) {
            individuo->genes[i] = 1 - individuo->genes[i];
        }
    }
}

int main() {
    srand(time(NULL));
    
    Individuo populacao[TAMANHO_POPULACAO];
    Individuo nova_populacao[TAMANHO_POPULACAO];
    Individuo melhores_individuos[NUM_GERACOES];
    
    for (int i = 0; i < TAMANHO_POPULACAO; i++) {
        for (int j = 0; j < NUM_ITENS; j++) {
            populacao[i].genes[j] = rand() % 2;
        }
        populacao[i].fitness = 0;
    }
    
    for (int geracao = 0; geracao < NUM_GERACOES; geracao++) {
        for (int i = 0; i < TAMANHO_POPULACAO; i++) {
            populacao[i].fitness = calcularFitness(populacao[i]);
        }
        
        for (int i = 0; i < TAMANHO_POPULACAO; i++) {
            int indice_pai1 = rand() % TAMANHO_POPULACAO;
            int indice_pai2 = rand() % TAMANHO_POPULACAO;
            Individuo pai1 = populacao[indice_pai1];
            Individuo pai2 = populacao[indice_pai2];
            
            Individuo filho1, filho2;
            cruzamento(pai1, pai2, &filho1, &filho2);
            
            mutacao(&filho1);
            mutacao(&filho2);
            
            nova_populacao[i] = filho1;
            nova_populacao[i + 1] = filho2;
            i++;
        }
        
        for (int i = 0; i < TAMANHO_POPULACAO; i++) {
            populacao[i] = nova_populacao[i];
        }
        
        int melhor_fitness = populacao[0].fitness;
        Individuo melhor_individuo = populacao[0];
        for (int i = 1; i < TAMANHO_POPULACAO; i++) {
            if (populacao[i].fitness > melhor_fitness) {
                melhor_fitness = populacao[i].fitness;
                melhor_individuo = populacao[i];
            }
        }
        melhores_individuos[geracao] = melhor_individuo;
    }
    
    int melhor_fitness = melhores_individuos[0].fitness;
    Individuo melhor_individuo = melhores_individuos[0];
    for (int i = 1; i < NUM_GERACOES; i++) {
        if (melhores_individuos[i].fitness > melhor_fitness) {
            melhor_fitness = melhores_individuos[i].fitness;
            melhor_individuo = melhores_individuos[i];
        }
    }
    
    printf("Melhor indivíduo: ");
    for (int i = 0; i < NUM_ITENS; i++) {
        printf("%d ", melhor_individuo.genes[i]);
    }
    printf("\n");
    printf("Valor total: %d\n", melhor_fitness);
    
    return 0;
}
