#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utime.h>


ssize_t mywrite(int fd, void *buf, size_t count){
	int wrote;
	wrote=write(fd,buf,count);
	if(wrote==0)
		return wrote;
	if (wrote==-1){
		fprintf(stderr,"Error occured: %s.\n",strerror(errno));
		exit (0);
	}
	while(wrote<count){
		wrote=wrote+write(fd,wrote+buf,(count-wrote));
		count=count-wrote;
	}
	return wrote;
}

ssize_t myread(int fd, void *buf, size_t count){
	int r;
	r=read(fd,buf,count);
	if(r==0)
		return r;
	if (r==-1){
		fprintf(stderr,"Error occured: %s.\n",strerror(errno));
		exit (0);
	}
	while(r<count){
		r=r+read(fd,r+buf,(count-r));
		count-=r;
	}
	return r;
}

off_t mylseek(int fd, off_t offset, int whence){
	
	off_t bytes;
	
	if ((bytes = lseek(fd,offset,whence))==-1){
		perror("Error occured in function lseek");
		exit(0);
	}
	
	return(lseek(fd,offset,whence));
}

pid_t myfork(void){
	
	pid_t pid;
	
	if ((pid = fork())==-1){
		perror("Error occured in function fork");
		exit(0);
	}
	
	return (pid);
}

int mystat(const char *pathname, struct stat *buf){
	
	int status;
	
	if ((status = stat(pathname,buf))==-1){
		perror("Error occured in function stat");
		exit (0);
	}
	
	return(status);
}
 
int myfstat(int fd, struct stat *buf){
	
	int fstatus;
	
	if ((fstatus = fstat(fd,buf))==-1){
		perror("Error occured in function fstat");
		exit(0);
	}
	
	return (fstatus);
}

int myutime(const char *filename, const struct utimbuf *times){
	
	int chtime;
	
	if ((chtime = utime(filename,times))==-1){
		perror("Error occured in function utime");
		exit(0);
	}
	
	return (chtime);
}

int myfchmod(int fd, mode_t mode){
	
	int next_mode;
	
	if ((next_mode = fchmod(fd,mode))==-1){
		perror("Error occured in function fchmod");
		exit(0);
	}
	
	return (next_mode);
}
