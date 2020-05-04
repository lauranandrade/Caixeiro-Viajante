#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define INFINITO 0x7fffffff

//Variáveis globais
int n; //Número de cidades
int* ordem; //Array para gurdar a ordem na qual as cidades foram visitadas
int* ordemAux; //Array para guardar a ordem que vai comparar com a atual menor ordem
int** coordenadas; //Matriz para as coordenadas
double menor = INFINITO; //Distância do menor caminho
double** DC; //Matriz das distâncias entre todas as cidades

//Funções
void menorCaminho();
void troca(int i, int j);
void permutacao(int inf, int sup);
int** criaMatriz(int i, int j);
double distancia(int i, int j);
double** criaMatrizDouble(int i, int j);


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
  1. inf: indíce inicial do vetor
  2. sup: índice superior do array
*/
void menorCaminho()
{
	double soma = 0;
	double maior = 0; //0 = false
	//Soma a distância entre as cidades
	for (int i = 0; i < n-1; ++i){
		soma = soma + DC[ordemAux[i]][ordemAux[i+1]];
		if (soma > menor){ //Se a soma já for maior que o menor caminho
			maior = 1;
			break;
		}
	}
	if (maior == 0) {
		soma = soma + DC[ordemAux[0]][ordemAux[n-1]]; //Soma a distância da última cidade até a primeira
		if (soma < menor){ //Verifica se a distância para o caminho que acabou de ser calculado é menor que o atual menor caminho
			menor = soma;
			for (int i = 0; i < n; ++i){
				ordem[i] = ordemAux[i];
			}
		}
	}
}

/*Troca de posição dois elementos do vetor.
  Parâmetros:
  1. inf: índice inferior do vetor
  2. sup: índice superior do array
*/
void troca(int i, int j)
{
	int aux = ordemAux[i];
	ordemAux[i] = ordemAux[j];
	ordemAux[j] = aux;
}

/*Função que calcula todos os caminhos possíveis.
  Assim que acha um caminho, chama o método para calcular a distância total do mesmo.
  Parâmetros:
  1. inf: indíce inicial do vetor
  2. sup: índice superior do array
*/
void permutacao(int inf, int sup)
{
	if(inf == sup){
		menorCaminho(); //Calcula a distância total do caminho
	}
	else {
		for(int i = inf; i <= sup; ++i){
			troca(inf, i);
			permutacao(inf + 1, sup);
			troca(inf, i); // backtracking
		}
	}
}

/* Função que lê a entrada, inicializa variáveis e estruturas de dados, chama
   o método para calcular todas as permutações e imprime os resultados.
*/
int main(int argc, char const *argv[])
{
	scanf(" %d", &n); //Lê o número de cidades

	ordem = malloc(n * sizeof(int)); //Cria um array para gurdar a ordem na qual as cidades foram visitadas
	ordemAux = malloc(n * sizeof(int)); //Cria um array para guardar a ordem de cidades visitadas que vai comparar com a atual menor ordem
	coordenadas = criaMatriz(2, n+1); //Cria a matriz para guardar as coordenadas das cidades
	DC = criaMatrizDouble(n+1, n+1); //Cria a matriz para guardar todas as distâncias entre todas as cidades

	for(int i = 1; i <= n; ++i){
		int x, y; //Coordenadas da cidade
		scanf(" %d %d", &x, &y); //Lê as coordenadas da cidade
		coordenadas[0][i] = x; //[0] = x
		coordenadas[1][i] = y; //[1] = y
		ordemAux[i-1] = i; //Inicializa as cidades
	}
	
	double d;
	for (int i = 1; i <= n; ++i)
	{
		for (int j = n; j > i; j--)
		{
			d = distancia(i, j); //Chama o método que irá calcular a distância entre duas cidades
			DC[i][j] = d; //Guarda na matriz das distâncias 
			DC[j][i] = d; //Guarda na matriz das distâncias
		}
	}

	permutacao(1, n - 1); //Chama o método que irá calcular todos os caminhos possíveis

	printf("%.2f\n", menor); //Printa a menor distância

	for (int i = 0; i < n; ++i) //Printa a ordem na qual as cidades foram visitadas
	{
		printf("%d ", ordem[i]); 
	}

	printf("\n");
	return 0;
}
