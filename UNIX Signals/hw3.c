#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>
#include "circ_list.h"

#define MAX_STRING 256
#define SIZE 25

static volatile sig_atomic_t gotsig = -1;

//static void handler(int signum);
static void handler_alarm(int signum);
void handler_child(int signum);


int main (int argc, char **argv){
	
	int my_argc=0;
	char action[MAX_STRING];
	char *ptr,*my_argv[SIZE];
	struct sigaction sa, sa_alarm,sa_child = { {0} };
	
	sa_child.sa_handler = handler_child;
	sa_alarm.sa_handler = handler_alarm;
	sa_alarm.sa_flags = SA_RESTART;
	sa_child.sa_flags = SA_RESTART;
	
	
	//Εξαίρεση σημάτω απο το σετ
	if(sigemptyset(&sa.sa_mask)==-1) {
		perror("sigemptyset");
	}
	//Προσθήκη σήματος SIGUSR1 στο σετ
	if(sigaddset(&sa.sa_mask,SIGUSR1)==-1) {
		perror("sigaddset");
	}
	//Μπλοκάρισμα σήματος sigusr1
	if(sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL)==-1) {
		perror("sigprocmask");
	}
	if(sigaction(SIGUSR1, &sa, NULL)==-1) {
		perror("sigaction");
	}
	if(sigaction(SIGALRM, &sa_alarm, NULL)==-1) {
		perror("sigaction");
	}
	if(sigaction(SIGCHLD, &sa_child, NULL)==-1) {
		perror("sigaction");
	}
	
	alarm(20);
	
	while (1){
		for (int i=0; i<MAX_STRING; i++) action[i]='\0';
		ptr = NULL;
		my_argc=0;
		for (int i=0; i<SIZE; i++) my_argv[i]=NULL;
		printf("-> exec\t <progname> <args>\n-> term\t <pid>\n-> sig\t <pid>\n-> info\n-> quit\n");
		fgets(action,MAX_STRING,stdin);
		action[strlen(action)-1] = '\0';
		ptr=&action[0];
		if (strncmp("exec",action,strlen("exec"))==0){
			//Εύρεση ορισμάτων
			ptr=ptr+strlen("exec")+1;				
			ptr = strtok(ptr," ");				
			my_argv[my_argc]=ptr;
			while (ptr!=NULL){
				my_argc++;
				ptr = strtok(NULL," ");
				my_argv[my_argc]=ptr;
			}
			//Δημιουργία παιδιού
			pid_t pid = fork();				
			//Περίπτωση παιδιού
			if (!pid){														
				if(sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL)==-1) {
					perror("sigprocmask");
				}
				//Τρεξιμο διεργασίας
				execvp(my_argv[0],my_argv);
			}
			//Περίπτωση άδειας λίστας 
			if (isEmpty()){
				newNode(pid, my_argc, my_argv);
				search_task(pid)->state = 1;
			}
			//Περίπτωση λίστας με στοιχεία
			else {											
				newNode(pid, my_argc, my_argv);
				search_task(pid)->state = 0;
				kill(search_task(pid)->pid, SIGSTOP);
			}
		}
		else if (strncmp("term",action,strlen("term"))==0){
			ptr=ptr+strlen("term")+1;
			//Αποστολή σήματος sigterm
			kill(atoi(ptr),SIGTERM);
			delete(atoi(ptr));			
		}
		else if (strncmp("sig",action,strlen("sig"))==0){
			if(sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL)==-1) {
				perror("sigprocmask");
			}
			ptr=ptr+strlen("sig")+1;
			kill(atoi(ptr),SIGUSR1);
		}
		else if (strncmp("info",action,strlen("info"))==0){
			print_list();
		}
		else if (strncmp("quit",action,strlen("quit"))==0){
			taskT *curr = head;
			//Τερματισμός των διεργασιών και άδεισμα της λίστας
			if (!isEmpty()){
				if (curr->next == head) {
					kill(curr->pid, SIGTERM);
				}
				else{
					while (curr->next != head){
						kill(curr->pid, SIGTERM);
						curr = curr->next;
					}
				}
				free_me();
			}
			return 0;
		}
		else {
			printf("***No such action!***\n"); 
		}
		
	}
	
	return 0;
}

//Ξυπνητήρι
static void handler_alarm(int signum){
	
	taskT *curr = head;
	
	printf("Drin Drin!\n");
	//Χρονοδρομολόγηση
	if(!isEmpty()){
		while (curr->next !=head) {
			if (isRunning(curr->pid)){
				kill(curr->pid, SIGSTOP);
				curr->state = 0;
				kill(curr->next->pid,SIGCONT);
				curr->next->state=1;
				break;
			}
			curr = curr->next;
		}
		if (curr->next == head){
			kill(curr->pid, SIGSTOP);
			curr->state = 0;
			kill(curr->next->pid,SIGCONT);
			curr->next->state=1;
		}
	}
	alarm(20); 
}
//Σε περίπτωση τερματισμού μιας διεργασίας αρχίζει η επόμενη
void handler_child(int signum){
	
	
	if (!isEmpty()){
		taskT *curr = head;
				
		while (curr->next != head){
			if(isRunning(curr->pid)) break;
				curr = curr->next;
		}
		int status;
		pid_t return_pid = waitpid(curr->pid,&status,WNOHANG);
		if (return_pid == -1) {
			perror("waitpid");
		}
		else if (return_pid!=0){
			curr->state = 0;
			kill(curr->next->pid,SIGCONT);
			curr->next->state=1;
			//Διαγραφή διεργασίας που τελείωσε
			delete(curr->pid);
		}
	}
}
	


