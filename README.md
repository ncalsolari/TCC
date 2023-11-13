# TCC

<h3>SSC0719 - Projeto de Formatura I (2023)</h3>
<h3>Nelson Calsolari Neto</h3>
<p>Professor Orientador: Eduardo do Valle Simões</p>
Aprimoramento de parâmetros de algoritmos evolutivos para resolução de problemas complexos.

<hr>
<h3>Códigos Fontes</h3>
<p>Os códigos-fontes finais estão na pasta "CodigosFonte-TCC", existem 3 códigos, um para cada problema trivial apresentado. Em código está implementado a versão com o Algoritmo Pai melhorando a eficiência dos Algoritmos Filhos que por sua vez resolvem os problemas em questão. As funções com sufixo "AlgEvo" representam as funções do Algoritmo Pai e estão padronizadas nos 3 códigos, as funções sem tal sufixo compreendem as funções dos Algoritmos Filhos, as quais seguem o mesmo padrão de lógica mas possuem variâncias para se adaptar a resolução de cada problema aqui tratado.</p>

<h3>Descrição:</h3>
<p>A idéia consiste em aplicar técnicas de computação evolutiva em cima dos parâmetros de um algoritmo evolutivo funcional. Para isso foram implementados 3 algoritmos evolutivos filhos para 3 problemas distintos (problema do caixeiro viajante, problema da mochila e raiz de equação cúbica)</p>

<p>Os 3 problemas estão separados nas pastas acima e cada um deles possui 2 versões, uma delas apenas com o algoritmo evolutivo filho solucionando o problema e a outra (a versão terminada em "(...)2.c") com o algoritmo evolutivo pai buscando melhorar os parâmetros do algortimo evolutivo filho. </p>

Os parâmetros do algoritmo evolutivo filho escolhidos para alteração foram:<br>
Tamanho da população<br>
Taxa de mutação<br>
Tipo de crossover<br>

<p>O tipo de crossover deciede como os parâmetros do algoritmo evolutivo filho serão passados de geração para geração. Um dos tipos consiste em fazer a média dos valores do melhor com os demais, o outro tipo escolhe um dos parametros do melhor e o copia para o indivíduo da próxima geração.</p>

<hr>


atualizado em 13/11/2023
