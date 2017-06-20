int readDimension(FILE* file){
	char string[30];
	int dimension;
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

void upper_row_matrix(float** matrix, FILE* file, int dimension){
	char str[128];
	int i = 0,j = 0;
	
	fscanf(file, "%s", str);
	fscanf(file, "%s", str);
	
	do{
		if(j == dimension){
			i++;	
			j = i;
		}
		if(i == j){
			matrix[i][j] = 0;
			j++;
			if(i == dimension-1)
				break;
		}
		
		matrix[i][j] = atof(str);
	    matrix[j][i] = atof(str);
	    j++;
		
		
	}while(fscanf(file,"%s",str)!= EOF); 
}

void not_upper_row(float** matrix, FILE* file ,int dimension){
 	char str[128];
	int i = 0,j = 0;
	
	do{
		if(strcmp(str, "0") == 0)
			break;
	}while(fscanf(file, "%s", str) != EOF);
	
	do{
		if(strcmp(str, "DISPLAY_DATA_SECTION") == 0)
			break;	
			
		matrix[i][j] = atof(str);
		if(j < dimension-1)
			j++;
		else{
			i++;
			j=0;
		}
		
		
		//printf("%s\n", str);
		
 	}while(fscanf(file,"%s",str)!= EOF);
}

float** makeExplicitMatrix(FILE* file, int dimension){
	int i = 0;
	char str[128];
	float** matrix =NULL;
	matrix = (float**)malloc(dimension * sizeof(float*));
	for(i = 0; i<dimension;i++)
		matrix[i] =(float*)malloc(dimension * sizeof(float));
	 
	
	
	do{
		printf("%s\n", str);
		if(!strcmp(str,"UPPER_ROW")){
			printf("\nPeparing to alloc UPPER ROW type...");
			upper_row_matrix(matrix,file,dimension);
			break;
		}else if(!strcmp(str, "FULL_MATRIX")){
			printf("\nPreparing to alloc the matrix...");
			not_upper_row(matrix,file,dimension);
			break;	
		}
	}while(fscanf(file,"%s",str) != EOF);
	return matrix;
}
