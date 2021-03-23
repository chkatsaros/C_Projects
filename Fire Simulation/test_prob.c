#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define low 0
#define high 100 


int main(int argc, char* argv[]) {
	int pi8anothta;
	double x;
	srand((int)time(NULL));
	pi8anothta = low + rand() % (high - low + 1);
	
	printf("%d\n", pi8anothta);
	x=pi8anothta/100.0;
	printf("%1.2lf",x);
	return (pi8anothta);
}
