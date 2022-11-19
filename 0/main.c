#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
int main(){
	int **a = (int**) malloc(sizeof(int*) * ((int)1e5));
	for(int i=0; i <(int)1e9; i++)
		a[i] = (int *) malloc(sizeof(int) * (int)1e5);
	for(int i = 0; i < 1e5; i++)
		for(int j=0; j <1e5; j++)
			printf("%d\n", a[i][j]);
}
