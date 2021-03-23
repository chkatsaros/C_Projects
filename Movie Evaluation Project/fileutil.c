#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *read_next_line(char *filename) {
	static int firstrun = 1;
	static FILE *fp;
	if (firstrun) {
		fp = fopen(filename, "r");
		if (fp == NULL) {
			fprintf(stderr, "Error opening file \"%s\" for reading.\n", filename);
			exit(1);
		}
		firstrun = 0;
	}
	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	linelen = getline(&line, &linecap, fp);	
	if (linelen == -1) {
		fclose(fp);
		firstrun = 1;
		return NULL;
	}
	char *newline = strchr(line, '\n');
	if (newline) *newline='\0';
	return line;
}
	 
		
