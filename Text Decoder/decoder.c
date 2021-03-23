#include<stdio.h>
#define SIZE 1023
#define PRINTED 95

int main (int argc,char*argv[]){
	
	int i,j,count[PRINTED],xar_size,max,max_8esi,key,stop;
	char  xar[SIZE],myASCII[PRINTED];
	
	i=0;
	j=0;
	
	for (i=0; i<PRINTED; i++) {					//mhdenizoume ton pinaka metrisis suxnothtas kai kataskeyh pinaka me tous ektupwsimous
		count[i]=0;
		myASCII[i]=' '+i;
	}

	i=0;										//diabasma to keimenou
	xar[i] = getchar();
	while (xar[i]!=EOF && i<SIZE){
		i++;
		xar[i]=getchar();
		xar_size=i;
	}
	
	/*for (i=0; i<xar_size; i++){					//ektypwsh keimenou
		printf("%c",xar[i]);
	}
	printf("\n\n\n\n\n\n\n\n\n\n");*/
	
	for (i=0; i<xar_size; i++){					//metrisi suxnothtas
		for (j=0; j<PRINTED; j++){
			if (myASCII[j]==xar[i]){
				count[j]=count[j] +1;
			}
		}
	}
	
	max=count[0];								//euresi tou suxnoterou stoixeioy
	max_8esi=0;
	for (j=0; j<PRINTED; j++){
		if (max<count[j]){
			max_8esi=j;
			max=count[j];
		}
	}
	
	key=myASCII[max_8esi]-32;					//ektypwsh tou key
	printf("KEY: %d\n\n",key);
	
	
												//euresi tou pragmatikou keimenou	
	for (i=0; i<xar_size; i++){
		stop=0;
		for (j=0; ((j<PRINTED) && (stop!=1)); j++){
			if (xar[i]==myASCII[j] && xar[i]!='\n'){
				if (j-key<0){
					stop=1;
					xar[i]=myASCII[PRINTED-(key-j)];
				}
				else{
					xar[i]=myASCII[j-key];
				}
			}
		}
	}
	
	printf("TEXT: ");
	for (i=0; i<xar_size; i++){					//ektypwsh keimenou
		printf("%c",xar[i]);
	}
	printf("\n");
	

	
	return(0);
}