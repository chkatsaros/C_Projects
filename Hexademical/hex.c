#include <stdio.h>

//I askisi (auti) egine se sinergasia twn foititwn Christoou Katsarou kai Euastathiou Tsitsopoulou

/* Ipologizei tis psifous ka8e ipopsifiou proedrou se mia taksi 16 atomwn ,dexetai ari8mo se dekaeksadiko sistima.*/


int main(int argc, char* argv[]) {
	
	int i,candidates,j,psifoi,nikitis,nikitis_psifoi ;
	unsigned short votes;
	
	printf("Enter number of candidates: ");
	scanf("%d",&candidates);
	
	
	while (candidates<0 || candidates>16){
		if (candidates>16){
			printf("ERROR: There are not so many students in this class! :P\n");	/* Bebaiwnomaste oti o ari8mos einai mesa se logika plaisia */
			printf("Enter number of candidates: ");
			scanf("%d",&candidates);
		}
		if (candidates<0){
			printf("ERROR: the number of candidates must be positive!\n");
			printf("Enter number of candidates: ");
			scanf("%d",&candidates);
		}
	}
	
	nikitis = 0;
	nikitis_psifoi = 0;
	
	for (i=0; i<candidates; i++){
		psifoi = 0;
		printf("Enter votes for candidate %d: ",i);		/* Ginetai ipologismos twn psifwn ka8e ipopsifiou eksetazontas to teleutaio psifio*/
		scanf("%hx",&votes);
		for (j=0; j<=15; j++){							/* tis diadikis anaparastasis twn psifwn pou do8ikan se 16adiki morfi*/
			if (votes & 0x1){
				psifoi = psifoi + 1;
			}
			votes = votes>>1;
		}
		if (psifoi>nikitis_psifoi){
			nikitis = i;
			nikitis_psifoi = psifoi;
		}
		printf("\t%hu votes\n",psifoi);
	}
	printf("The winner is candidate %d with %d votes\n",nikitis,nikitis_psifoi);
	
	
	return(0);
}