#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#include "EUC_2D_Reader.c"
#include "EUC_3D_Reader.c"
#include "Explicit_Reader.c"

typedef struct List{ //lista usada para armazenar as cidades do caminho percorrido
	int index;
	struct List* next;
}list;

list* citiesCovered(int);
int travelledDistance(list*, float**);
list* insert(list*, int);
int exists(list*, int);
void print(clock_t, list*, int);

main(){
	clock_t time_start; 
	time_start = clock();
	
	int totalDistance=0;
	int dimension, i, j;
	
	list* L = NULL;
	
	char path[50] = "libs\\";//"libs\\";//Standard path of the libs
	char file_name[50];
	
	float** matrix; 
	
	printf("What tsp data file do you want to read ?\n(type the name of file ex:\"kroA100.tsp\")");
	gets(file_name);//Get the  .tsp file name
	strcat(path,file_name); //Concat the standard path to the file name

	FILE* file = fopen(path, "r");//Open the file to readd

	char str[128];
	Queue* Q2D = NULL;
	Q3D* Queue3D = NULL;
	dimension = readDimension(file);
	
	file = fopen(path, "r");
	
	while(fscanf(file, "%s", str)!= EOF){
		/*
			Checks the dataSet kind, and allocs to the queue
		*/
		//printf("%s \n", str);
		if(!strcmp(str,"EUC_2D")){
			Q2D = alloc2D(file); 
			matrix = cost_matrix_2D(Q2D);
			//dimension = Q2D->size;
			if(Q2D != NULL)
				printf("Starting to solve the queue...");
			else
				printf("ERROR");
			break;
		}else
			if(!strcmp(str,"EUC_3D")){
				Queue3D = alloc3D(file);
				matrix = compleerMatrix(Queue3D);
				//dimension = Queue3D->size;
				if(Queue3D != NULL)
					printf("Starting to solve the queue...");
				else
					printf("ERROR");
				break;
			}else
				if(!strcmp(str,"EXPLICIT")){
					matrix = makeExplicitMatrix(file, dimension);
					break;
				}
	}
	fclose(file);

	
	L = citiesCovered(dimension);
	totalDistance = travelledDistance(L, matrix);
	
	print(time_start, L, totalDistance);

	/*printf("\n\n");
	for(i = 0; i < dimension; i++){
		for(j = 0; j < dimension; j++){
			printf("%.2f ", matrix[i][j]);
		}
		printf("\n");
	}*/
	
	getch();
	
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



int travelledDistance(list* L, float** m){  //função que retorna a distância total percorrida pelo caixeiro
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
		printf("\n%da cidade: %d\n", i, aux->index);  //lista das cidades que foram visitadas
		aux = aux->next;
		i++;
	}
	printf("Distancia total percorrida: %d\n", totalDistance);
	printf("Tempo de execucao do algoritmo: %.3f", (double)(clock() - time_start)/(double)CLOCKS_PER_SEC);
}
