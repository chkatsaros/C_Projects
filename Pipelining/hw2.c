#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>
#include <sys/wait.h>
#include "check.h"

int main(int argc, char **argv){
	
	int fd;
	int pipefd1[2], pipefd2[2];
	pid_t child1, child2, child3;
	
	pipe(pipefd1);
	pipe(pipefd2);
	
	if (argc<5){
		printf("***Not enough arguments!***\n");
	}
	else {
		if (strcmp(argv[1],"-E")==0){
			fd = open(argv[4],O_WRONLY | O_EXCL | O_CREAT,S_IRWXU);
			mywrite(fd,"P2CRYPTAR",strlen("P2CRYPTAR"));
			if (fd != -1){
				child1 = myfork();
				if (!child1) {
					dup2(pipefd1[1],1);
					close(pipefd1[0]);
					execl("dirlist","dirlist",argv[2],NULL);
				}
				close(pipefd1[1]);
				child2 = myfork();
				if (!child2) {
					dup2(pipefd1[0],0);
					dup2(pipefd2[1],1);
					close(pipefd1[1]);
					close(pipefd2[0]);
					execl("p2archive","p2archive",NULL);
				}
				close(pipefd1[0]);
				close(pipefd2[1]);
				child3 = myfork();
				if (!child3) {
					dup2(pipefd2[0],0);
					dup2(fd,1);
					close(pipefd2[1]);
					execl("p2crypt","p2crypt",argv[3],NULL);
				}
				close(pipefd2[0]);
			}
			else {
				perror("Open");
				exit(-1);
			}
		}
		else if(strcmp(argv[1],"-D")==0){
			fd=open(argv[4],O_RDONLY,0700);
			char magic_no[strlen("P2CRYPTAR")+1];
			
			for (int i=0; i<strlen("P2CRYPTAR")+1; i++)
				magic_no[i]='\0';
			
			lseek(fd,0,SEEK_SET);
			myread(fd,&magic_no[0],strlen("P2CRYPTAR"));
			
			if (!strcmp(magic_no,"P2CRYPTAR")){
				
				child1 = myfork();
				if (!child1) {
					dup2(fd,0);
					dup2(pipefd1[1],1);
					close(pipefd1[0]);												
					execl("p2crypt","p2crypt",argv[3],NULL);
				}
				close(pipefd1[1]);
				
				child2 = myfork();
				if (!child2) {
					dup2(pipefd1[0],0);
					close(pipefd1[1]);	
					execl("p2unarchive","p2unarchive",argv[2],NULL);
				}
				close(pipefd1[0]);
			}
			else {
				printf("***Couldn 't open selected file!***\n");
			}
			
		}
		else {
			printf("***Wrong argument (1)!***\n");
		}
	}
	waitpid(-1,NULL,0);
	
	return 0;
}

