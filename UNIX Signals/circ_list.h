#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define N 25
//Κόμβος λίστας
typedef struct task{
	
	pid_t pid;
	int argc;
	char *argv[N];
	int state;
	struct task *next;
}taskT;

taskT *head = NULL;

bool isEmpty(){
	return (head == NULL);
}
//Προσθήκη καινούργιου στοιχείου στη λίστα
void newNode (pid_t pid, int argc, char *argv[]){
	
	taskT *curr = head;
	taskT *temp = (taskT *)malloc(sizeof(taskT));
	int i;
	char *my_argv[N];
	
	if (isEmpty()) {
		head = temp;
		head->next = head;
		temp->next = head;
		temp->pid = pid;
		temp->argc = argc;
		for (i=0; i<argc; i++){
			my_argv[i] = strdup(argv[i]);
			temp->argv[i]=my_argv[i];
		}
		printf("\n***Node successfully added!***\n\n");
	}
	else {
		curr = head->next;
		while (curr->next != head){
			curr = curr->next;
		}
		curr->next = temp;
		temp->next = head;
		temp->pid = pid;
		temp->argc = argc;
		for (i=0; i<argc; i++){
			my_argv[i] = strdup(argv[i]);
			temp->argv[i]=my_argv[i];
		}
		printf("\n***Node successfully added!***\n\n");
	}
}
//Αναζήτηση στοιχείου με βάση το pid
taskT *search_task (pid_t pid){
	
	taskT *curr = head;
	int found = 0;
	
	if (curr->pid == pid){
		found++;
		return (curr);
	}
	curr = curr->next;
	while (curr!=head){
		if (curr->pid == pid){
			found++;
			return (curr);
		}
		curr = curr->next;
	}
	
	return (NULL);
}
//Διαγραφή στοιχείου απο τη λίστα
void delete (pid_t pid){
	
	taskT *curr = head;
	
	while (curr->next != head) {
		curr = curr->next;
	}
	taskT *prev = curr;
	curr = head;
	
	if(!isEmpty()){
		if (head->next == head) {
			for (int i=0; i<head->argc; i++)
				free(head->argv[i]);
			free(head);
			head = NULL;
		} 
		else{
			while (curr->next!=head){
				if (curr->pid == pid){
					if (curr == head) head = curr->next;
					prev->next = curr->next;
					for (int i=0; i<curr->argc; i++)
						free(curr->argv[i]);
					free(curr);
					break;
				}
				prev = curr;
				curr = curr->next;
			}
			if (curr->next == head) {
				if (curr->pid == pid){
					prev->next = curr->next;
					for (int i=0; i<curr->argc; i++)
						free(curr->argv[i]);
					free(curr);
				}
			}
		}
	}
}
//Έλεγχος αν τρέχει η διεργασία
bool isRunning (pid_t pid){
	
	taskT *curr = head;
	
	if (!isEmpty()){
		if (curr->pid==pid){ 
			return (curr->state);
		}
		curr = curr->next;
		while(curr!=head){
			if (curr->pid==pid){ 
				return (curr->state);
			}
			curr = curr->next;
		}
	}
	return 0;
}
//Εκτύπωση λίστας
void print_list (){
	
	taskT *curr = head;
	
	if(!isEmpty()){
		printf("pid: %d, name: (%s, %s, %s, %s, %s)", curr->pid, curr->argv[0], curr->argv[1], curr->argv[2], curr->argv[3], curr->argv[4]);
		if (isRunning(curr->pid)) printf(" (R)");
		printf("\n");
		curr = curr->next;
		while (curr!=head){
			printf("pid: %d, name: (%s, %s, %s, %s, %s)", curr->pid, curr->argv[0], curr->argv[1], curr->argv[2], curr->argv[3], curr->argv[4]);
			if (isRunning(curr->pid)) printf(" (R)");
			printf("\n");
			curr = curr->next;
		}
	}
	else {
		printf("***List is empty!***\n");	
	}
}
//Άπελευθέρωση της δεσμευμένης μνήμης
void free_me () {
	
	taskT *curr = head;
	
	if(!isEmpty()){
		if (head->next == head) {
			for (int i=0; i<head->argc; i++)
				free(head->argv[i]);
			free(head);
			head = NULL;
		} 
		else{
			while (curr->next != head){
				head->next = curr->next;
				for(int i=0; i<curr->argc; i++)
					free(curr->argv[i]);
				free(curr);
				curr = curr->next;		
			}
		}
	}
}
