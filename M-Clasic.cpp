#include<iostream>

using namespace std;

const int MAX=100;

int main(){
	
	int A[MAX][MAX]={0},B[MAX][MAX]={0},C[MAX][MAX]={0};
	int k,n,m;
	k=n=m=MAX;
	for(int i=0; i<k; ++i)
        for(int j=0; j<n; ++j)
            for(int z=0; z<m; ++z)
                C[i][j] += A[i][z] * B[z][j];
	return 0;
}
