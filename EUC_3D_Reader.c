#include<math.h>
#include<string.h>

typedef struct list3D{
	int index, x, y, z;
	struct list3D* next;
}List3D;

typedef struct Queue3D{
	int size;
	List3D* start;
	List3D* end;
}Q3D;

Q3D* create(){
	return (Q3D*) malloc(sizeof(Q3D));
}

void insert3D(Q3D* Q, int id, int x, int y, int z){
	List3D* newList = (List3D*) malloc(sizeof(List3D));
	newList->index = id;
	newList->x = x;
	newList->y = y;
	newList->z = z;
	newList->next = NULL;
	
	if(Q->start != NULL){
		Q->end->next = newList;
		Q->end = newList;
		Q->size++;
	}else{
		Q->size = 0;
		Q->start = newList;
		Q->end = newList;
	}
}

void printQ(Q3D* Q){
	List3D * aux;
	for(aux = Q->start; aux != NULL; aux= aux->next){
		printf("\nCidade: %d X: %d Y: %d Z: %d",aux->index, aux->x, aux->y, aux->z);
	}
	printf("\n");
}

Q3D* aalloc3D(FILE* file){
	char string[100], city[4], x[50], y[50], z[50];
	
	do{
		if(!strcmp(string, "NODE_COORD_SECTION")){
			printf("Alocando fila...\n\n");
			break;
		}
	}while(fscanf(file, "%s", string) != EOF);
	
	Q3D* Queue = create();
	
	while(fscanf(file, "%s" "%s" "%s" "%s", city, x, y, z) != EOF){
		insert3D(Queue, atoi(city), atoi(x), atoi(y), atoi(z));
	}
	
	return Queue;
}

List3D* searchCities(Q3D* Q, int city){
	List3D* searcher = Q->start;
	
	while(searcher != NULL && searcher->index != city){
		searcher = searcher->next;
	}
	
	return searcher;
}

int distance3D(Q3D* Queue, int c1, int c2){
	List3D *coordinate1 = searchCities(Queue, c1);
	List3D *coordinate2 = searchCities(Queue, c2);
	
	return sqrt(pow((coordinate1->x - coordinate2->x), 2) + pow((coordinate1->y - coordinate2->y), 2) +  pow((coordinate1->z - coordinate2->z), 2));
}

void printMatrix(int** matrix, int size){
	int i, j;
	
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}
}

void compleerMatrix(Q3D* Queue){
	int i, j;
	int** matrix = (int**) malloc(Queue->size * sizeof(int*));
	for(i = 0; i < Queue->size; i++){
		matrix[i] = (int*) malloc(Queue->size * sizeof(int));
	}
	
	for(i = 0; i < Queue->size; i++){
		for(j = 0; j < Queue->size; j++){
			if(i == j)
				matrix[i][j] = 0;
			else
				matrix[i][j] = distance3D(Queue, i, j);
		}
	}
	
	printMatrix(matrix, Queue->size);
}
