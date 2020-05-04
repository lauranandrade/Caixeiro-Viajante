#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define TAM_POP 300
#define TAM_TOR 210
#define NUM_GEN 100

//Variáveis globais
int n; //Número de cidades
double menor; //Distância do menor caminho
int** coordenadas; //Matriz para as coordenadas
double** DC; //Matriz das distâncias entre todas as cidades
int* ordem; //Array para gurdar a ordem na qual as cidades foram visitadas
int** populacao; //Matriz população


//Funções
void evolui_populacao();
void gerar_populacao();
int selecao_torneio();
int get_fittest(int* torneio);
int contem(int* array, int a);
int* copia(int* lista);
int* embaralhar(int* array);
int* cruzamento(int pai, int mae);
int** criaMatriz(int x, int y);
double menorCaminho(int* rota);
double distancia(int i, int j);
double** criaMatrizDouble(int x, int y);


/* Copia uma lista e retorna um ponteiro para a cópia
   Parâmetros:
   1. lista
 */
int* copia(int* lista) 
{
	int* retorno = malloc(n * sizeof(int));
	for(int i = 0; i < n; ++i)
		retorno[i] = lista[i];
	return retorno;
}

/* Retorna o melhor indivíduo do torneio, aquele com a menor distância total
   Parâmetros:
   1. torneio
 */
int get_fittest(int* torneio)
{
	int melhor = 0;
	double dist = menorCaminho(populacao[torneio[melhor]]);
	
	for (int i = 1; i < TAM_TOR; ++i) {
		double aux = menorCaminho(populacao[torneio[i]]);
		if (aux < dist) {
			melhor = torneio[i];
			dist = aux;
		}
	}

	menor = dist;
	ordem = copia(populacao[melhor]);
	return melhor;
}

/* Cria um torneio, ou seja, uma disputa entre indivíduos aleatórios da população para 
   identificar o melhor (fittest) - com a menor distÂncia - e retorna a posição do 
   elemento que ganhou no array população.
 */
int selecao_torneio()
{
	int fittest;
	int* torneio = malloc(TAM_TOR * sizeof(int));

	for (int i = 0; i < TAM_TOR; ++i) {
		int randomId = rand() % TAM_POP;
		randomId = randomId > 0 ? randomId : randomId*-1;
		torneio[i] = randomId;
	}

	fittest = get_fittest(torneio);

	return fittest;
}

/* Verifica se um elemento está no array
   Parâmetros: 
   1. array
   2. elemento
*/
int contem(int* array, int a)
{
	int retorno = 1;
	int i;
	for(i = 0; i < n && array[i] != a; ++i);
	if (i == n) 
		retorno = 0;
	return retorno;
}

/* Retorna uma sequência que é um cruzamento das populacao[pai] e populacao[mae]
   Parâmetros:
   1. pai
   2. mae
 */
int* cruzamento(int pai, int mae)
{
	int inicio ,fim;
	inicio = fim = 0;
	int* filho = malloc(n * sizeof(int));
	for(int i = 0; i < n; ++i){
		filho[i] = -1;
	}
	while(inicio == fim) {
		int r = rand() % n;
		inicio = r > 0 ? r : r*-1;
		r = rand() %n;
		fim = r > 0 ? r : r*-1;
	}

	//Adiciona partes do pai ao filho
	for (int i = 0; i < n; ++i) {
		if (inicio < fim && i > inicio && i < fim) {
			filho[i] = populacao[pai][i];
		} else if (inicio > fim){
			if ( !(i < inicio && i > fim))
				filho[i] = populacao[pai][i];
		}
	}

	//Adiciona partes da mãe ao filho
	for (int i = 0; i < n; ++i) {
		if ( ! contem(filho,populacao[mae][i])) {
			//Encontra a posição vazia do filho
			int j;
			for (j = 0; j < n && filho[j] != -1; ++j);
			filho[j] = populacao[mae][i];
		}
	}

	return filho;
}

/* Evolui a população criando torneios e fazendo cruzamentos
   com os ganhadores dos torneios.
 */
void evolui_populacao()
{
	int** filho = criaMatriz(TAM_POP,n);
	int pai, mae;

	for (int i = 0; i < TAM_POP; ++i) {
		//Seleciona os pais
		pai = selecao_torneio(); //Seleciona o pai através de um torneio
		mae = selecao_torneio(); //Seleciona a mãe através de um torneio
		filho[i] = cruzamento(pai, mae); //Gera o filho
	}

	populacao = filho;
} 

/* Embaralha elementos do array.
   Parâmetros:
   1. array
 */
int* embaralhar(int* array)
{
	int* novo = copia(array);

	//i começa de 1 para computar apenas sequências começando da cidade 1
	for (int i=1; i<n-1; ++i) {
		int j = i + rand() / (RAND_MAX / (n-i) +1);
		int t = novo[j];
		novo[j] = novo[i];
		novo[i] = t;
	}
	return novo;
}
	
/* Gera uma população que é uma lista de sequências aleatórias.
   Cada sequência tem tamanho n e a população tem tamanho TAM_POP. 
 */
void gerar_populacao()
{
	populacao = criaMatriz(TAM_POP, n);
	for (int i=0; i<TAM_POP; ++i) {
		populacao[i] = embaralhar(ordem);
	}
}

/*Calcula a distância entre 2 cidades
  Parâmetros:
  1. i: número da coluna em que se encontra a cidade 1
  2. j: número da coluna em que se encontra a cidade 2
*/
double distancia(int i, int j)
{
	double x = coordenadas[0][i] - coordenadas[0][j];
	double y = coordenadas[1][i] - coordenadas[1][j];
	double p1 = pow(x, 2);
	double p2 = pow(y, 2);
	return (sqrt(p1 + p2));
}

/*Cria matrizes de números inteiros
  Parâmetros:
  1. i: número de linhas da matriz
  2. j: número de colunas da matriz
*/
int** criaMatriz(int i, int j) 
{
	int** retorno = (int**)malloc(sizeof(int*)*i);
	for(int x = 0; x < i; x++) {
		retorno[x] = (int*)malloc(sizeof(int*)*j);
	}
	return retorno;
}

/*Cria matrizes de números reais
  Parâmetros:
  1. i: número de linhas da matriz
  2. j: número de colunas da matriz
*/
double** criaMatrizDouble(int i, int j) 
{
	double** retorno = (double**)malloc(sizeof(double*)*i);
	for(int x = 0; x < i; x++) {
		retorno[x] = (double*)malloc(sizeof(double*)*j);
	}
	return retorno;
}

/*Função que calcula a distância percorrida de um caminho.
  Parâmetros:
  1. rota: ordem na qual as cidades foram visitadas
*/
double menorCaminho(int* rota)
{
	double soma = 0;

	//Soma a distância entre as cidades
	for (int i = 1; i < n; ++i){
		soma = soma + DC[rota[i-1]][rota[i]];
	}
	
	soma = soma + DC[rota[n-1]][rota[0]]; //Soma a distância da última cidade até a primeira

	return soma;
}

/* Inicializa variáveis e estruturas de dados, chama o método para gerar a população, evoluir a população, achar o menor caminho
   e imprime a menor rota e a ordem na qual as cidade foram visitadas.
*/
int main(int argc, char const *argv[])
{
	scanf(" %d", &n); //Lê o número de cidades

	ordem = malloc(n * sizeof(int)); //Array para gurdar a ordem na qual as cidades foram visitadas
	coordenadas = criaMatriz(2, n); //Cria a matriz para guardar as coordenadas das cidades
	DC = criaMatrizDouble(n, n); //Matriz para guardar todas as distâncias entre todas as cidades

	for(int i = 0; i < n; ++i){
		ordem[i] = i;
		int x, y; //Coordenadas da cidade
		scanf(" %d %d", &x, &y); //Lê as coordenadas da cidade
		coordenadas[0][i] = x; //[0] = x
		coordenadas[1][i] = y; //[1] = y
	}
	
	double d;
	for (int i = 0; i < n; ++i)
	{
		for (int j = i+1; j < n; ++j)
		{
			d = distancia(i, j); //Chama o método que irá calcular a distância entre duas cidades
			DC[i][j] = d; //Guarda na matriz das distâncias 
			DC[j][i] = d; //Guarda na matriz das distâncias
		}
	}

	gerar_populacao(); //Chama o método para gerar a população

	//Chama o método de evoluir a população NUM_GEN vezes
	for (int i=0; i< NUM_GEN; ++i) {
		evolui_populacao();
	}

	int* torneio_geral = malloc(sizeof(int) * TAM_POP); //Array torneio_geral

	//Inicializa torneio_geral
	for (int i = 0; i < TAM_POP; ++i){
		torneio_geral[i] = i;
	}

	get_fittest(torneio_geral); //Chama o método para achar o menor caminho

	printf("%.2f\n", menor); //Printa a menor distância

	for (int i = 0; i < n; ++i) //Printa a ordem na qual as cidades foram visitadas
	{
		printf("%d ", ordem[i]+1); 
	}

	printf("\n");
	return 0;
}
