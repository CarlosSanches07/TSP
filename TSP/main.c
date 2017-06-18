#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

#include "EUC_2D_Reader.c"
#include "EUC_3D_Reader.c"

main(){
	int control; //To verify what the kind of datSet 0 = Explicit; 1 = 2D; 2 = 3D

	char path[50] = "libs\\";//"libs\\";//Standard path of the libs
	char file_name[50];

	printf("What tsp data file do you want to read ?\n(type the name of file ex:\"kroA100.tsp\")");
	gets(file_name);//Get the  .tsp file name
	strcat(path,file_name); //Concat the standard path to the file name

	FILE* file = fopen(path, "r");//Open the file to readd

	char str[128];
	Queue* Q2D = NULL;
	Q3D* Queue3D = NULL;
	while(fscanf(file, "%s", str)!= EOF){
		/*
			Checks the dataSet kind, and allocs to the queue
		*/
		if(!strcmp(str,"EUC_2D")){
			printf("ue");
			Q2D = alloc2D(file); 
			control = 1;
			if(Q2D != NULL)
				printf("Starting to solve the queue...");
			else
				printf("ERROR");
			break;
		}else
			if(!strcmp(str,"EUC_3D")){
				Queue3D = alloc3D(file);
				control = 2;
				if(Queue3D != NULL)
					printf("Starting to solve the queue...");
				else
					printf("ERROR");
				break;
			}
	}
	printQ(Queue3D);
	fclose(file);
	switch(control){
		case 1:
			cost_matrix_2D(Q2D);
			break;
		case 2 :
			printf("test");
			compleerMatrix(Queue3D);
			break;
	}
	getch();
}
