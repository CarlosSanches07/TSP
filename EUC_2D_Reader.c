#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct euc_2d
{
	int city;
	float x,y;
	struct euc_2d* next;
}EUC_2D;

typedef struct queue
{
	EUC_2D* start;
	EUC_2D* end;
	int size;
}Queue;

Queue* create(){
	Queue* Q = (Queue*)malloc(sizeof(Queue));
	Q->start = NULL;
	Q->end = NULL;
	Q->size = 0;
	return Q;
}

void insertQueue(Queue* Q,int city, float x, float y){
	EUC_2D* euc;
	euc =(EUC_2D*)malloc(sizeof(EUC_2D));
	euc->city = city;
	euc->x = x;
	euc->y = y;
	if(Q->start == NULL){
		Q->start = euc;
		Q->end = euc;
		euc->next = NULL;
		Q->size = 0;
	}else{
		Q->end->next = euc;
		euc->next = NULL;
		Q->end = euc;
		Q->size ++;
	}
}

void show(Queue* Q){
	EUC_2D* euc;
	for(euc = Q->start; euc != NULL; euc = euc->next)
		printf("\ncity: %d x: %f y: %f",euc->city, euc->x, euc->y);
	printf("\n");
}

Queue* alloc2D(FILE* tsp){
	char str[128], city[2], x[55], y[55];//Strings that will receive the data from fscanf

	/*
		While the entire file is read, it checks when str receives the coordinate section of the node
	*/
	do{
		if(!strcmp(str,"NODE_COORD_SECTION")){
			printf("Start to Alloc the Queue... \n\n");
			break;
		}
	}while(fscanf(tsp, "%s",str)!= EOF);

	Queue* Q = create();

	/*
		Inserts the coordinates of each city in the queue
	*/
	while(fscanf(tsp, "%s %s %s", city, x, y)!= EOF)
		insertQueue(Q, atoi(city), atof(x), atof(y));

	return Q;
}

EUC_2D* search_city(Queue* Q, int city){//Search city by index
	EUC_2D* euc = Q->start;

	while(euc != NULL && euc->city != city)
		euc = euc->next;
	
	return euc;
}

float calc_distance_EUC_2D(Queue* Q, int c1, int c2){//Calculate the distance between to cities
	EUC_2D* city1 = search_city(Q, c1);

	EUC_2D* city2 = search_city(Q, c2);

	return sqrt(pow((city1->x - city2->x), 2)+ pow((city1->y - city2->y), 2));
}

void show_cost_matrix_2D(float** matrix, int size){
	int i, j;
	printf("\n");
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++)
			printf("%4.0f ",matrix[i][j]);
		printf("\n");
	}
}

float** cost_matrix_2D(Queue* Q){
	int i,j;
	float** matrix =(float**)malloc(Q->size * sizeof(float*));
	for(i = 0; i < Q->size; i++)
		matrix[i] =(float*)malloc(Q->size * sizeof(float));
		
	for(i = 0; i < Q->size; i++){
		for(j = 0; j < Q->size; j++){
			if(i == j){
				matrix[i][j] = 0;
			}else{
				matrix[i][j] = calc_distance_EUC_2D(Q, i, j);
			}
		}
	}
	return matrix;
}
