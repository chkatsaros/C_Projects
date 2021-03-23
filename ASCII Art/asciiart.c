# include <stdio.h>

int main(int argc, char *argv[]){

	int kiones,i,j,z ;
	
	/* eisagwgh ari9mou kionwn */
	
	printf("Parakalw dwste ton ari8mo kionwn pou epi8umeite: \n") ;
	printf("!!PROSOXH!! o ari8mos prepei na einai artios, apo 2 ews 10\n") ;
	printf("O ari8mos sas: ");
	scanf("%d",&kiones) ;
	printf("\n\n");
	
	while (kiones%2!=0 || kiones<=0 || kiones>10) {
		printf("O ari8mos pou plhktrologhsate den einai apodektos! \n") ;
		printf("Parakalw dwste ton ari8mo kionwn pou epi8umeite \n") ;
		printf("!!PROSOXH!! o ari8mos prepei na einai artios, apo 2 ews 10\n") ;
		printf("O ari8mos sas: ");
		scanf("%d",&kiones) ;
		printf("\n\n");
	}
	
	printf("\n\n");
	
	/* SKEPH */

	for (z=kiones/2; z>=1; z--){
		for (j=1; j<=kiones; j++){
			if (j==z || j == kiones - z +1){
				if (j<=kiones/2){
					printf("__..--''");
				}
				else{
					printf("''--..__");
				}
			}
			else{ 
				for (i=1; i<=8; i++){
					printf(" ") ;
				}
			}
		}
		printf("\n");
	}
	
	/* KIONOKRANO */
	
	for(i=1; i<=kiones*8; i++){
		printf("=");
	}
	printf("\n");
	for(i=1; i<=kiones*4; i++){
		printf("[]");
	}
	printf("\n");
	for(i=1; i<=kiones; i++){
		printf(" @.==.@ ");
	}
	printf("\n");
	
	/* KOLONES */
	
	for (z=1; z<=8; z++){
		for (j=1; j<=kiones; j++){
			if (z==8){
				if (j==1){
					printf("  |''|__");
				}
				else if (j==kiones){
					printf("__|''|  ");
				}
				else{
					printf("__|''|__");
				}
			}
			else{
				printf("  |''|  ");
			}
		}
		printf("\n");
	}
	
	/* SKALOPATIA */
	
	for (z=2; z>=0; z--){
		for (j=1; j<=kiones; j++){
			for (i=1; i<=8; i++){
				if ((j==1 && i <=z) || (j==kiones && i> 8 -z)){
					printf(" ");
				}
				else{
					printf("=");
				}
			}
		}
		printf("\n");
	}
	
	printf("\n\n");
	
	return(0) ;
}
