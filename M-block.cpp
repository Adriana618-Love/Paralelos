#include<iostream>

const int n=100;
const int s=6; //blocks

int main(){
	int a[n][n]={0},b[n][n]={0},c[n][n]={0};
	for(int jj=0;jj<=(n/s);jj += s){
            for(int kk=1;kk<=(n/s);kk += s){
                    for(int i=1;i<=(n/s);i++){
                            for(int j = jj; j<=((jj+s-1)>(n/s)?(n/s):(jj+s-1)); j++){
                                    int temp = 0;
                                    for(int k = kk; k<=((kk+s-1)>(n/s)?(n/s):(kk+s-1)); k++){
                                            temp += b[i][k]*a[k][j];
                                    }
                                    c[j][i] += temp;
                            }
                    }
            }
    } 
	return 0;
}
