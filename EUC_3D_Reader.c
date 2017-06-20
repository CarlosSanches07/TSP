#include<math.h>
#include<string.h>

typedef struct list3D{
	int index;
	float x, y, z;
	struct list3D* next;
}List3D;

typedef struct Queue3D{
	int size;
	List3D* start;
	List3D* end;
}Q3D;

Q3D* create3D(){
	Q3D* Q = (Q3D*) malloc(sizeof(Q3D));
	Q->size = 0;
	Q->start = NULL;
	Q->end = NULL;
	return Q;
}

void insert3D(Q3D* Q, int id, float x, float y, float z){
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
		printf("\nCity: %d X: %f Y: %f Z: %f",aux->index, aux->x, aux->y, aux->z);
	}
	printf("\n");
}

Q3D* alloc3D(FILE* file){
	char string[100], city[4], x[50], y[50], z[50];
	
	do{
		if(!strcmp(string, "NODE_COORD_SECTION")){
			printf("Start to Alloc the queue...\n\n");
			break;
		}
	}while(fscanf(file, "%s", string) != EOF);
	
	Q3D* Queue = create3D();
	
	while(fscanf(file, "%s" "%s" "%s" "%s", city, x, y, z) != EOF){
		insert3D(Queue, atoi(city), atof(x), atof(y), atof(z));
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

float distance3D(Q3D* Queue, int c1, int c2){
	List3D *coordinate1 = searchCities(Queue, c1+1);
	List3D *coordinate2 = searchCities(Queue, c2+1);
	
	return sqrt(pow((coordinate1->x - coordinate2->x), 2) + pow((coordinate1->y - coordinate2->y), 2) +  pow((coordinate1->z - coordinate2->z), 2));
}

void printMatrix(float** matrix, int size){
	int i, j;
	
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			printf("%f\t", matrix[i][j]);
		}
		printf("\n");
	}
}

float** compleerMatrix(Q3D* Queue){
	int i,j;
	float** matrix = (float**) malloc(Queue->size * sizeof(float*));
	for(i = 0; i < Queue->size; i++){
		matrix[i] = (float*) malloc(Queue->size * sizeof(float));
	}
	for(i = 0; i < Queue->size; i++){
		for(j = 0; j < Queue->size; j++){
			if(i == j)
				matrix[i][j] = 0;
			else
				matrix[i][j] = distance3D(Queue, i, j);
		}
	}
	
	return matrix;
}
