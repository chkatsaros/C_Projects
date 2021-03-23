#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>	
#include "fileutil.c"
#define INIT_SIZE 100
#define MAX_LOAD 3
#define MAX_LINE_LEN 256

typedef struct entry {
	char *word;
	int amount;
	double score;
	struct entry *next;
}entryT;

typedef struct hash {
	int size;
	int num_entries;
	entryT *table;
}hashT;

hashT *root;

void insert (int index, char* word, double score);
unsigned long hash (char *str);
void pos_insert (char* word, double score);
int pos (char* word);
void rehash();
entryT *find_the_best(entryT* head);
entryT find_the_bestest(entryT *best_word[], int i);
void rehash_insert (int index, entryT* node);
void printer();
double meso_score(char *word);
int exist(char *word);
void free_lists();

int main (int argc, char *argv[]){
	
	char *line, *word;
	int i;
	double score;
	char delim[] = { ' ', '\t' };
		
	if (argc==2 || argc==3){
		
		root=(hashT*)calloc(INIT_SIZE,sizeof(hashT));
		if (root==NULL) {
			printf("***Something went wrong with the allocation of memory!***\n");
			exit(1);
		}
		for (i=0; i<INIT_SIZE; i++){
			root[i].size=INIT_SIZE;
			root[i].num_entries=0;
			root[i].table=NULL;
		}
		while ( (line=read_next_line(argv[1]))!=NULL ){
			line[1]='\0';
			score=atof(line);
			line=line+2;
			word=strtok(line,delim);
			while (word!=NULL){
				for (i=0; i<strlen(word); i++){
					word[i]=tolower(word[i]);
				}
				pos_insert(word,score);
				double load_factor = root->num_entries/root->size;
				if (load_factor>MAX_LOAD){
					rehash();
				}
				word=strtok(NULL,delim);
			}
		}
			
		entryT *best_word[root->size];
		
		for (i=0; i<root->size; i++){
			best_word[i] = find_the_best(root[i].table);
		}
		entryT the_best = find_the_bestest(best_word, 0);
		printf("The most positive word is %c%s%c with a score of %.3lf\n", 34, the_best.word, 34, (double)the_best.score/the_best.amount);
		
		if (argc==3){
			if (strcmp(argv[2],"-p")==0) {
				printer();
			}
			else {
				printf("\n***Sorry I can 't print if you don 't gime me %c-p%c as a parameter.***\n", 34, 34);
			}
		}
		
		char input[MAX_LINE_LEN];
		
		do {
			int count=0;
			printf("\nEnter review or DONE to finish:\n");
			fgets(input, MAX_LINE_LEN-1, stdin);
			input[strlen(input)-1]='\0';
			if (strcmp(input, "DONE")==0){
				break;
			}
			if (input[0]=='\0'){ printf("Sorry, there is no score for this review!\n"); }
			else{
				double sum=0;
				word=strtok(input,delim);
				while (word!=NULL) {
					for (i=0; i<strlen(word); i++){
						word[i]=tolower(word[i]);
					}
					sum += meso_score(word);
					if (exist(word)){
						count++;
					}
					word=strtok(NULL, delim);					
				}
				double review_score=sum/count;
				if (count==0){
					review_score=0;
				}
				printf("Review score: %1.4lf\n",review_score);
				printf("This review is ");
				if (review_score < 2){ printf("negative.\n"); }
				else if (review_score > 2) { printf("positive.\n"); }
				else { printf("neutral.\n"); }
			}
			
		} while (1);
		
		free_lists();
		free(root);
		root=NULL;
	}
	else{
		printf("***Incorrect number of parameters!***\n");
	}
	
	return (0);
}

void insert (int index, char* word, double score){
	
	entryT* curr=root[index].table;
	int flag=0;
		
	while (curr!=NULL){
		if (strcmp(curr->word,word)==0){
			curr->score += score;
			curr->amount++;
			flag++;
			break;
		}
		else{
			curr=curr->next;
		}
	}
	
	curr=root[index].table;
	
	if (!flag){
		if (root[index].table==NULL){
			curr=(entryT*)malloc(sizeof(entryT));
			if (curr==NULL){
				printf("***Something went wrong with the allocation of memory!***\n");
				exit(1);
			}
			curr->next=NULL;
			curr->word=word;
			curr->score=score;
			curr->amount=1;
			root[index].table=curr;
		}
		else{
			while (curr->next!=NULL){
				curr=curr->next;
			}
			curr->next=(entryT*)malloc(sizeof(entryT));
			if (curr==NULL){
				printf("***Something went wrong with the allocation of memory!***\n");
				exit(0);
			}
			curr->next->next=NULL;
			curr->next->word=word;
			curr->next->score=score;
			curr->next->amount=1;
		}
		root->num_entries++;
		
	}
	
}

void pos_insert (char* word, double score){
	
	unsigned long value=hash(word);
	int index=value%root->size;
	
	insert(index, word, score);
}

int pos (char* word){
	
	unsigned long value=hash(word);
	int index=value%(root->size);
	
	return(index);
}

void rehash () {
	
	int i, index;
	
	hashT *tmp = root;
	root = (hashT*)calloc(tmp->size*2,sizeof(hashT));
	if (root==NULL){
		printf("***Something went wrong with memory allocation!***\n");
		exit(1);
	}
	root->size = 2*tmp->size;
	root->num_entries = tmp->num_entries;
	for (i=0; i<root->size; i++){
		root[i].table=NULL;
	}
	
	for (i=0; i<tmp->size; i++){
		
		entryT *curr=tmp[i].table;
				
		while (curr!=NULL){
			if (tmp[i].table->next) { 
				tmp[i].table=tmp[i].table->next;
			}
			tmp[i].table=curr->next;
			curr->next=NULL;
			index=pos(curr->word);
			rehash_insert(index, curr);
			curr=tmp[i].table;
		}
		
	}
	
	free(tmp);
		
}

void rehash_insert (int index, entryT* node){
	
	entryT *curr=root[index].table;
		
	if (root[index].table==NULL){
		root[index].table=node;
	}
	else{
		while (curr->next!=NULL){
			curr=curr->next;
		}
		curr->next=node;
	}
	
}

void printer (){
	
	for (int i=0; i<root->size; i++){
		if (root[i].table!=NULL){
			printf("%4d: ",i);
			entryT *curr=root[i].table;
			while (curr!=NULL){
				printf("[ %c%s%c %d %.2lf ]", 34, curr->word, 34, curr->amount, curr->score);
				if (curr->next!=NULL){
					printf(", ");
				}
				else{
					printf("\n");
				}
				curr=curr->next;
			}
		}
	}
	printf("\n");
	
}

unsigned long hash (char *str){
	
	unsigned long hash = 5381;
	int c;
	
	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}
	
	return (hash);
}

entryT *find_the_best (entryT* head){
	
	double meso_score;
	
	if (head==NULL){
		return (NULL);
	}
	else if (head->next==NULL){
		return (head);
	}	
	else{
		meso_score = head->score/head->amount;
		entryT *tmp=(find_the_best(head->next));
		return(meso_score > tmp->score/tmp->amount ? head : tmp);		
	}
}

entryT find_the_bestest(entryT *best_word[], int i){
	
	double meso_score;
	
	if (best_word[i]!=NULL){
		if (i<root->size) {
			meso_score = best_word[i]->score/best_word[i]->amount;
			entryT tmp=(find_the_bestest(best_word, i+1));
			return (meso_score >= tmp.score/tmp.amount ? *best_word[i] : tmp);
		}
		else {
			return(*best_word[i]);
		}
	}
	else {
		return (find_the_bestest(best_word, i+1));
	}

}

double meso_score(char *word){
	
	double meso_score;
	
	if (word!=NULL){
		for (int i=0; i<root->size; i++){
			entryT *curr=root[i].table;
			if (root[i].table!=NULL){
				while (curr!=NULL){
					if (strcmp(curr->word, word)==0) {
						meso_score = curr->score/curr->amount;
						return (meso_score);
					}
					curr=curr->next;
				}
			}
		}
	}
	
	return (0);
}

int exist(char *word){
	
	if (word!=NULL){
		for (int i=0; i<root->size; i++){
			entryT *curr=root[i].table;
			if (root[i].table!=NULL){
				while (curr!=NULL){
					if (strcmp(curr->word, word)==0) {
						return (1);
					}
					curr=curr->next;
				}
			}
		}
	}
	
	return(0);
}

void free_lists(){
	
	for (int i=0; i<root->size; i++){
		entryT *curr=root[i].table;
		while (curr!=NULL){
			root[i].table=root[i].table->next;
			free(curr);
			curr=root[i].table;
		}
	}
}
