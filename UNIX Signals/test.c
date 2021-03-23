#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t gotsig = -1;

static void handler(int signum);

int main(int argc, char **argv){
	
	if (argc<5){
		printf("***Not enough arguments!***\n"); 
	}
	else if (strcmp(argv[1],"-m")!=0 || strcmp(argv[3],"-b")!=0) {
		printf("***Wrong arguments!***\n");
		printf("Exiting...\n");
		sleep(2);
	}
	else {
		int count = 0;
		struct sigaction sa;
		
		if(sigemptyset(&sa.sa_mask)==-1) {
			perror("sigemptyset");
		}
		if(sigaddset(&sa.sa_mask,SIGUSR1)==-1) {
			perror("sigaddset");
		}
		
		if (atoi(argv[4]) == 1){
			if(sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL)==-1) {
				perror("sigprocmask");
			}
			sa.sa_handler = handler;
			sa.sa_flags = SA_RESTART;
			if(sigaction(SIGUSR1, &sa, NULL)==-1) {
				perror("sigaction");
			}
			while (1){
				if (gotsig == SIGUSR1){
					count = 0;
					gotsig = -1;
				}
				count++;
				if (count > atoi(argv[2])) break;
				if (count == atoi(argv[2])/2) {
					if(sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL)==-1) {
						perror("sigprocmask2");
					}
				} 
				printf("pid = %d\ncount = %d\nM = %d\n\n", getpid(), count, atoi(argv[2]));
				sleep(5);
			}
		}
		else if (atoi(argv[4]) == 0){
			sa.sa_handler = handler;
			sa.sa_flags = SA_RESTART;
			if(sigaction(SIGUSR1, &sa, NULL)==-1) {
				perror("sigaction");
			}
			while (1){
				if (gotsig == SIGUSR1){
					count = 0;
					gotsig = -1;
				}
				count++;
				if (count > atoi(argv[2])) break;
				printf("pid = %d\ncount = %d\nM = %d\n\n", getpid(), count, atoi(argv[2]));
				sleep(5);
			}
		}
		else {
			printf("***Wrong argument (4)!***\n");
			printf("Exiting...\n");
		}
	}
	
	return 0;
}

static void handler(int signum) {
    
    if (signum==SIGUSR1){
		gotsig = signum;
	}
}
