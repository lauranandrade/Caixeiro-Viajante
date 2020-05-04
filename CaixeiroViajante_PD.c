#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define INFINITO 0x7fffffff

//Variáveis globais
int n; //Número de cidades
int npow; //Número de cidades ao quadrado
int** p; //Matriz best
int** coordenadas; //Matriz para as coordenadas
double** g; //Matriz cost
double** DC; //Matriz das distâncias entre todas as cidades

//Funções
void TSP();
void caminho(int start,int set);
int** criaMatriz(int i, int j);
double calcula(int start,int set);
double distancia(int i, int j);
double** criaMatrizDouble(int i, int j);

/* Divide o caminho em caminhos menores (subproblemas) e acha a menor distância desses caminhos.
   Depois usa estes caminhos menores para calcular o menor caminho que passa por todas as cidades.
   Parâmetros:
   1. start: cidade inicial
   2. set: cidade final
*/
double calcula(int start, int set)
{	int masked, mask;
    double temp; 
    double menor = INFINITO;
	if(g[start][set] != -1.0)
		return g[start][set];
	for(int i = 0; i < n; ++i)
	{	//npow-1 porque sempre removemos a cidade incial do conjunto
		mask=(npow-1) - (1<<i);//remove o i-ésimo vértice desse set
		masked = set&mask;
		if(masked != set)//Caso seja obtido o mesmo set (porque o i-ésimo vértice não é contado, uma vez que é o inicial)
		{	
			temp = DC[start][i] + calcula(i, masked);//calcula o set removido (subproblema)
			if(temp < menor)
				menor = temp;
				p[start][set] = i;//remover o i-ésimo vértice resulta no mínimo
		}
	}
		return g[start][set] = menor;//retorna o menor
}

/* Calcula a distância entre duas cidades
   Parâmetros:
   1. start: cidade inicial
   2. set: cidade final
*/
void caminho(int start, int set)
{
	if(p[start][set] == -1) 
		return; //chegou no final
	int x = p[start][set];
	int mask = (npow-1)-(1<<x); //1 é shiftado de c bits pois p e g tem 2 elevado a n colunas, então trabalhamos com potências de 2
	int masked = set&mask; //remove p do conjunto
	printf("%d ", x+1);
	caminho(x, masked);
}

/* Inicializa as matrizes g e p, grava em g a distâncias de todas as cidades até a inicial,
   chama o método que irá calcular o menor caminho e imprime esse valor e a ordem das cidades
   visitadas.
*/
void TSP()
{	for(int i = 0; i < n; ++i){
		for(int j = 0;j < npow; j++){ 
				g[i][j] = p[i][j] = -1; //inicializando as matrizes
		}
	}
	for(int i = 0; i < n; ++i){
		g[i][0] = DC[i][0]; //aresta entre i e 0(cidade 1)
	}
	double menor = calcula(0,npow-2); //npow-2 para excluir o vértice 1
	printf("%.2f\n",menor);
	printf("1 ");
	caminho(0,npow-2);
	printf("\n");
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

/* Função que lê a entrada, inicializa variáveis e estruturas de dados, e chama
   o método para calcular o menor caminho.
*/
int main(int argc, char const *argv[])
{
	scanf(" %d", &n); //Lê o número de cidades

	npow = (int)pow(2,n);
	coordenadas = criaMatriz(2, n); //Cria uma matriz para guardar as coordenadas das cidades
	DC = criaMatrizDouble(n, n); //Cria uma matriz para guardar todas as distâncias entre todas as cidades
	g = criaMatrizDouble(n, npow); //Cria uma matriz para 
	p = criaMatriz(n, npow); //Cria uma matriz para 

	for(int i = 0; i <= n; ++i){
		int x, y; //Coordenadas da cidade
		scanf(" %d %d", &x, &y); //Lê as coordenadas da cidade
		coordenadas[0][i] = x; //[0] = x
		coordenadas[1][i] = y; //[1] = y
	}
	
	double d;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; j++)
		{
			d = distancia(i, j); //Chama o método que irá calcular a distância entre duas cidades
			DC[i][j] = d; //Guarda na matriz das distâncias 
			DC[j][i] = d; //Guarda na matriz das distâncias
		}
	}

	TSP();

	return 0;
}
