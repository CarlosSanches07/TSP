#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

#include "EUC_2D_Reader.c"

main(){
	Queue* Q = NULL;// A queue to alloc the data

	char path[50] = "libs\\";//"libs\\";//Standard path of the libs
	char file_name[50];

	printf("What tsp data file do you want to read ?\n(type the name of file ex:\"kroA100.tsp\")");
	gets(file_name);//Get the  .tsp file name
	strcat(path,file_name); //Concat the standard path to the file name

	FILE* file = fopen(path, "r");//Open the file to readd

	char str[128];

	while(fscanf(file, "%s", str)!= EOF){

		if(strcmp(str,"EUC_2D")){//Verify the dataSet kind
			Q = alloc2D(file); // if the dataSet is EUC_2D kind, alloc the data of .tsp file to the queue
			break;
		}
	}

	fclose(file);

	if(Q != NULL){
		printf("Queue allocated!! Starting solve... %d",Q->size);

		cost_matrix_2D(Q);//Build a matrix with the distances between cities
	}else{
		printf("failed to alloc");
	}
}
