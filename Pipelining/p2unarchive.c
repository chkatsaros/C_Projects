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

#define MAX_SIZE 256
#define MAX_BYTES 512

int main(int argc, char **argv){
	
	int filename_len, i;
	char filename[MAX_SIZE], file[MAX_BYTES], name[MAX_SIZE];
	struct stat buff;
	struct utimbuf times;
	
	
	if (argc<2){
		printf("***Not enough arguments!***\n");
	}
	else {
		
		if (!mkdir(argv[1],S_IRWXU)){
						
			while(1){
				
				for (i=0; i<MAX_SIZE; i++){
					filename[i]='\0';
					name[i]='\0';
				}
				for (i=0; i<MAX_BYTES; i++){
					file[i]='\0';
				}
				
				if (!myread(0,&filename_len,sizeof(filename_len))) break;
				if (!myread(0,&filename[0],filename_len)) break;
				for (i=filename_len; i>0; i--){
					if (filename[i]=='/'){
						char *ptr = &filename[i];
						ptr++;
						strcpy(name,argv[1]);
						strcat(name,"/");
						strcat(name,ptr);
						break;
					}
				}
				if (!myread(0,&buff.st_atime,sizeof(struct timespec))) break;
				times.actime = buff.st_atime;
				if (!myread(0,&buff.st_mtime,sizeof(struct timespec))) break;
				times.modtime = buff.st_mtime;
				if (!myread(0,&buff.st_mode,sizeof(mode_t))) break;
				if (!myread(0,&buff.st_size,sizeof(off_t))) break;
				int fd = open(name,O_WRONLY | O_CREAT,S_IRWXU);
				while (buff.st_size>MAX_BYTES){
					if (!myread(0,&file[0],MAX_BYTES)) break;
					mywrite(fd,file,MAX_BYTES);
					for (i=0; i<MAX_BYTES; i++){
						file[i]='\0';
					}
					buff.st_size-=MAX_BYTES;
				}
				if (!myread(0,&file[0],buff.st_size)) break;
				mywrite(fd,file,buff.st_size);
				utime(name,&times);
				fchmod(fd,buff.st_mode);
				close(fd);
			}
					
		}
		else
			perror("Couldn 't create file demanded");
	}
	
	
	
	
	
	return 0;
}

