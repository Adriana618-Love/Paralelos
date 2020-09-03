#include<iostream>
#include <stdio.h> 
#include <string.h> 

using namespace std;

const int MAX = 10;

int main(){
	int i,j;
	double A[MAX][MAX]={0}, x[MAX]={0}, y[MAX]={0};
	for (i = 0; i < MAX; i++)
		for (j = 0; j < MAX; j++)
			y[i] += A[i][j]*x[j];
	memset(y,0,MAX*sizeof(y[0]));
	for (j = 0; j < MAX; j++)
		for (i = 0; i < MAX; i++)
			y[i] += A[i][j]*x[j];
	return 0;
}
