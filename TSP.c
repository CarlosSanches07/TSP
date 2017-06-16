#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

typedef struct List{ //lista usada para armazenar as cidades do caminho percorrido
	int index;
	struct List* next;
}list;

int** allocateMatrix(int, int);  //***** prototipos das funções
int readDimension();
void readCoordinates(int, int**);
void make2DMatrix(FILE*, int**, int**, int);
void make3DMatrix(FILE*, int**, int**, int);
void makeExplicitMatrix(FILE*, int**, int);
list* citiesCovered(int);
int travelledDistance(list*, int**);
list* insert(list*, int);
int exists(list*, int);
void print(clock_t, list*, int); //*****

main(){
	clock_t time_start; 
	time_start = clock(); //inicia a contagem do clock
	int i, j, dimension, totalDistance;
	int** m;
	list* L = NULL;
	dimension = readDimension(); //função que lê a dimensão do arquivo
	m = allocateMatrix(dimension, dimension); //função que aloca uma matriz dinamicamente
	readCoordinates(dimension, m); //função para a leitura das cordenadas e transforma tudo na matriz de distancia das cidades
	L = citiesCovered(dimension); //função para gerar aleatoriamente quais cidades serão percorridas
	totalDistance = travelledDistance(L, m); //função que gera o caminho total percorrido 
	print(time_start, L, totalDistance); //função usada para printar os valores finais na tela
}

int** allocateMatrix(int rows, int columns){ //aloca a matriz dinamicamente
	int i, j;
	
	int** m = (int**) malloc(rows * sizeof(int*));
	
	for(i = 0; i < rows; i++){
		m[i] = (int*) malloc(columns * sizeof(int));
		for(j = 0; j < columns; j++){
			m[i][j] = 0;
		}
	}
	
	return m;
} 

int readDimension(){
	FILE *file;
	char string[30];
	int dimension;
	
	file = fopen("kroA100.tsp", "rt"); //função usada para abrir o arquivo
	
	if(file == NULL)
		printf("Problema na leitura do arquivo!\n");
	else{
		while(!feof(file)){ //enquanto for possivel ler o arquivo, ele continua nesse while
			fscanf(file, "%s", &string);
			if(strcmp("DIMENSION:", string) == 0){		//duas comparações aqui, por que alguns data set tem o ':' separado da palavra 'DIMENSION'
				fscanf(file, "%d", &dimension);			//ai, ao invés dele pegar a dimensão, ele retornava ':'
				fclose(file);
				return dimension;
			}else if(strcmp("DIMENSION", string) == 0){
				fscanf(file, "%s", &string);
				fscanf(file, "%d", &dimension);
				fclose(file);
				return dimension;
			}
		}
		fclose(file);
		printf("Dimensao nao encontrada!\n");
	}
}

void readCoordinates(int dimension, int **m){ //função para ler as coordenadas e fazer a conversão para distâncias euclidianas
	FILE *file;
	char string[50];
	int **mValues;
	
	file = fopen("kroA100.tsp", "rt");
	
	if(file == NULL)
		printf("Problema na leitura do arquivo!\n");
	else{
		while(!feof(file)){
			fscanf(file, "%s", &string);
			if(strcmp("EUC_2D", string) == 0){  			
				mValues = allocateMatrix(dimension, 2); //caso o arquivo for 2D,ele aloca uma matriz 'd x 2' para guardar os valores 
				make2DMatrix(file, m, mValues, dimension); 
				free(mValues);								//libera a matriz de valores alocada
			}else if(strcmp("EUC_3D", string) == 0){
				mValues = allocateMatrix(dimension, 3); //mesma coisa se o arquivo for 3D, aloca uma 'd x 3' 
				make3DMatrix(file, m, mValues, dimension);
				free(mValues);
			}else if(strcmp("EXPLICIT", string) == 0){	//em arquivos explicit ele envia a matriz 'd x d' alocada na main
				makeExplicitMatrix(file, m, dimension);
			}	
		}
	}
	fclose(file);
}

void make2DMatrix(FILE* file, int **m, int **mValues, int dimension){  //função que gera a matriz de distancias euclidianas no caso de ser um arquivo 2D
	int value, cont = 1, i=0, j=0;
	char string[20];
	int result;
	
	while(!feof(file)){
		fscanf(file, "%s", &string);
		if(strcmp("1", string) == 0){  //1 é o primeiro valor antes dos valores das coordenadas, por isso usei ele como referência para começar a guardar os valores
			while(!feof(file)){
				result = fscanf(file, "%d", &value);
				if(result){			//enquanto o resultado for true, ele continua executando 
					if(cont == 0){  //caso o contador for 0, ele incremente 'i', seta 'j' com 0 e incrementa 'cont' 
						i++;
						j=0;
						cont++;
					}else{			//quando o valor de cont for diferente de 0, ele aloca o valor recebido do data set na posição [i][j] da matriz
						mValues[i][j] = value;
						j++;      
						if(cont == 2) //caso o contador chegue a 2, ele é setado com 0, assim ele não aloca na matriz os indices, somente as coordenadas
							cont = 0; //aloca 2 valores, pula um (o indice) e aloca mais dois
						else
							cont++;
					}
				}else break; //caso não seja mais possivel trabalhar com o valor resultado do fscanf ele da um break;
			}	
		}
	}
	
	for(i = 0; i< dimension; i++){ 		//função matemática para montar a matriz com os valores euclidianos
		for(j = 0; j<dimension; j++){
			m[i][j] = sqrt(pow((mValues[i][0] - mValues[j][0]), 2) + pow((mValues[i][1] - mValues[j][1]), 2));
		}
	}
}

void make3DMatrix(FILE* file, int **m, int **mValues, int dimension){ //função para a matriz 3D, praticamente igual a função para a matriz 2D
	int value, cont = 1, i=0, j=0;
	char string[20];
	int result;
	
	while(!feof(file)){
		fscanf(file, "%s", &string);
		if(strcmp("1", string) == 0){
			while(!feof(file)){
				result = fscanf(file, "%d", &value);
				if(result){
					if(cont == 0){
						i++;
						j=0;
						cont++;
					}else{
						mValues[i][j] = value;
						j++;
						if(cont == 3) //diferença esta que, aqui, o cont anda até 3, deixando ele alocar mais um valor na matriz antes de pegar o indice
							cont = 0;
						else
							cont++;
					}
				}else break;
			}	
		}
	}
	
	for(i = 0; i< dimension; i++){	 	//função de valores euclidianos tbm é diferente, adicionando mais dois valores
		for(j = 0; j<dimension; j++){
			m[i][j] = sqrt(pow((mValues[i][0] - mValues[j][0]), 2) + pow((mValues[i][1] - mValues[j][1]), 2) + pow((mValues[i][3] - mValues[j][3]), 2));
		}
	}
}

void makeExplicitMatrix(FILE* file, int **m, int dimension){ //função que recebe a matriz ja explicita
	int value, i=0, j=0;
	char string[20];
	int result;
	
	while(!feof(file)){
		fscanf(file, "%s", &string);
		if(strcmp("UPPER_ROW", string) == 0){ 	//comparação pra ver se a matriz é "upper row", nesse caso, so recebe uma parte da matriz simetrica sem os zeros
			fscanf(file, "%s", &string);		//por exemplo uma matriz 5 x 5     1 2 3 4 
			if(strcmp("EDGE_WEIGHT_SECTION", string) == 0){	 				 //      1 2 3 
				while(!feof(file)){										    //         1 2
					if(i == j){								                //           1
						m[i][j] = 0; // quando 'i' e 'j' forem iguais, ele aloca um 0 e incremente 'j'
						j++;
					}else{  		
						result = fscanf(file, "%d", &value);
						if(result){					//se 'i' e 'j' forem diferentes
							m[i][j] = value; 		//ele aloca o valor naquela coordenada, e nela invertida
							m[j][i] = value;		//para preencher toda a matriz de maneira simetrica
							if(j < dimension - 1)	//enquanto 'j' for menor que a dimensão - 1, ele vai somando 'j'	
								j++;
							else{
								i++;				//quando 'j' for igual a dimensão - 1, ele incrementa 'i' e 'j' recebe o valor de 'i'
								j = i;
							}
						}else break;
					}	
				}
			}	
		}else{		//se a matriz não for upper row, ele é recebida normalmente
			if(strcmp("EDGE_WEIGHT_SECTION", string) == 0){		//comparação por ser o ultimo valor antes da matriz no data set
				while(!feof(file)){
					result = fscanf(file, "%d", &value);
					if(result){
						m[i][j] = value;
						if(j < dimension-1)
							j++;
						else{
							i++;
							j=0;
						}
					}else break;	
				}
			}
		}	
	}
}

list* insert(list* L, int id){  //função para inserir um valor na lista
	if(L == NULL){
		L = (list*) malloc(sizeof(list));
		L->index = id;
		L->next = NULL;
		return L;
	}
	
	list* aux = L;
	while(aux->next != NULL){
		aux = aux->next;
	}
	
	aux->next = (list*) malloc(sizeof(list));
	aux->next->index = id;
	aux->next->next = NULL;
	
	return L;
}

list* citiesCovered(int dimension){ //função que recebe numeros aleatorios não repetidos
	list* L = NULL;
	srand((unsigned)time(NULL));
	int control, random, i, cont = 0;
	
	control = 1 + (rand() % dimension);
	
	for(i = 0; i < control; i++){
		random = 1 + (rand() % dimension);
		if(!exists(L, random))  //caso o valor não existir na lista, ele é inserido, se ele ja existir, nada o laço continua
			L = insert(L, random);
	}
	
	return L;
}

int exists(list* L, int x){  //função para verificar se valor x existe na lista
	list* aux = L;
	
	while(aux!= NULL && aux->index != x){
		aux = aux->next;
	}
	
	if(aux == NULL)
		return 0;
	else
		return 1;
}

int travelledDistance(list* L, int** m){  //função que retorna a distância total percorrida pelo caixeiro
	int i, j, totalDistance=0;
	
	list* aux = L;
	
	while(aux->next != NULL){
		totalDistance += m[aux->index - 1][aux->next->index - 1]; //'totalDistance' soma e recebe a matriz na posição 
		aux = aux->next;										 //do index atual da lista com o index do proximo valor
	}															//cidade 12 e 26, por exemplo, fica m[12][26], pegando assim a distancia entra as duas cidades
	
	return totalDistance;
}

void print(clock_t time_start, list* L, int totalDistance){ //função usada para printar os ultimos valores, junto do clock iniciado no começo da main
	list* aux = L;
	int i = 1;
	
	while(aux != NULL){
		printf("%da cidade: %d\n", i, aux->index);  //lista das cidades que foram visitadas
		aux = aux->next;
		i++;
	}
	printf("Distancia total percorrida: %d\n", totalDistance);
	printf("Tempo de execucao do algoritmo: %.3f", (double)(clock() - time_start)/(double)CLOCKS_PER_SEC);
}
