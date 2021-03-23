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
#define MAX_BYTES 512

int main(int argc, char **argv){
	
	int fd,i,filename_len;
	char filename[MAX_SIZE],prev_filename[MAX_SIZE],content[MAX_BYTES];
	struct stat buff;
	
	
	scanf("%s",filename);
	while (strcmp(filename,prev_filename)!=0){

		fd = open(filename,O_RDONLY,S_IRWXU);
		fstat(fd,&buff);
		filename_len=strlen(filename);
		mywrite(1,&filename_len,sizeof(filename_len));
		mywrite(1,filename,strlen(filename));
		mywrite(1,&buff.st_atime,sizeof(struct timespec));
		mywrite(1,&buff.st_mtime,sizeof(struct timespec));
		mywrite(1,&buff.st_mode,sizeof(mode_t));
		mywrite(1,&buff.st_size,sizeof(off_t));
		lseek(fd,0,SEEK_SET);
		while (buff.st_size>MAX_BYTES){
			myread(fd,content,MAX_BYTES);
			mywrite(1,content,MAX_BYTES);
			for (i=0; i<MAX_BYTES; i++){
				content[i]='\0';
			}
			buff.st_size-=MAX_BYTES;
		}
		myread(fd,content,buff.st_size);
		mywrite(1,content,buff.st_size);
		
		for (i=0; i<MAX_SIZE; i++){
			prev_filename[i]='\0';
			filename[i]='\0';
		}
		
		strcpy(prev_filename,filename);
		scanf("%s",filename);
	}
	
	
	
	return (0);
}

