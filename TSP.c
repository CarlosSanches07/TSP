#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define a 100
#define b 100

int main(){
	int i = 0, j = 0, x, k=2;
	FILE *arq;
	char *result;
	char Str[100];
	int matrix[100][2];
	int nois[a][b];
	char prm[] = "1";
	arq = fopen("kroA100.tsp", "rt");
	
	if (arq == NULL)
	{
	    printf("Problemas na CRIACAO do arquivo\n");
	}else{
		while (!feof(arq))
		{
		    result = fscanf(arq, "%s", &Str);
			if(result){
				if((strcmp(prm, Str) == 0)){
					while(!feof(arq)){
						result = fscanf(arq, "%d", &x);
						if(result){
							if(x == k){
								k++;
								i++;
								j=0;
							}
							else{
								matrix[i][j] = x;
								j++;
							}
						} 
						else break;
					}
				}else{  
			    	printf("%s",Str);
				}
			}
		}
		fclose(arq);	
	} 
 
  	printf("\n");
	for(i = 0; i< a; i++){
		for(j = 0; j<b; j++){
			nois[i][j] = sqrt(pow((matrix[i][0] - matrix[j][0]), 2) + pow((matrix[i][1] - matrix[j][1]), 2));
		}
	}
	
	for(i = 0; i < a; i++){
		for(j = 0; j< b; j++){
			printf("%d\t", nois[i][j]);
		}
		printf("\n");
	}
}
