#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "check.h"

#define MAX_SIZE 256

int main (int argc, char* argv[]){
	
	DIR *dir_ptr;
	struct dirent *dent;
	struct stat stat_of_path;
	char path[MAX_SIZE];
	
	if (argc<2){
		printf("***Not enough arguments!***\n"); 
	}
	else{
		dir_ptr=opendir(argv[1]);
		if (dir_ptr!=NULL){
			dent=readdir(dir_ptr);
			strcpy(path,argv[1]);
			strcat(path,"/");
			strcat(path,dent->d_name);
			stat(path, &stat_of_path);
			while (dent!=NULL){
				if (S_ISREG(stat_of_path.st_mode)){
					printf("%s\n",path);
				}
				dent=readdir(dir_ptr);
				for (int i=0; i<MAX_SIZE; i++)
					path[i]='\0';
				if (dent!=NULL){
					strcpy(path,argv[1]);
					strcat(path,"/");
					strcat(path,dent->d_name);
					stat(path, &stat_of_path);
				}
			}
		}
		else{
			perror("Couldn 't open the directory");
		}
		
	}
	
	closedir(dir_ptr);
	return (0);
}
