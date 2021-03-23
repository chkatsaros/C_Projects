#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>
#include "check.h"

char bitXor(char x, char y){
    return x ^ y;
}

int main(int argc, char **argv){
	
	int j=0, key_len, end;
	char buff, crypt;
	
	if (argc<2){
		printf("***Not enough arguments!***\n");
	}
	else {
		key_len=strlen(argv[1]);
		while(1){
			end=myread(0,&buff,sizeof(buff));
			if (!end){
				break;
			}
			crypt = bitXor(buff,argv[1][j]);
			mywrite(1,&crypt,sizeof(crypt));
			j=(j+1)%key_len;
		}
		
	}
	
	return 0;
}

