#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char s[10000];
int max(int a, int b);
char temp[10000];
char res[10000];
char str[40];
int count = 0;
int c[50][50];
char b[50][50];
int *ind;
int k = 0;
int z = 0;


void check(char *x, int i, int j){
    if(i == 0 || j == 0){
        return;
    }
    if(b[i][j] == 'D'){
        //printf("reverse LCS is %c\n",x[i]);
        str[k] = x[i];
        // printf("%c\n", x[i]);
        // ind[z] = i;
        k++;
        return check(x, i-1, j-1);
    }else if(b[i][j]=='U'){
        // printf("UU\n");
        return check(x, i, j-1);
    }else if(b[i][j]=='L'){
        // printf("LL");
        return check(x, i-1, j);
    }
}

int main(int argc, char *argv[]){
    char x[] = "6541254939322816220209974565477289648317";
    char y[] = "3142522751761601737419090933147067701840";
    
    int m = strlen(x);
    int n = strlen(y);

    for(int i=0; i<m; i++){
        c[i][0] = 0;
    }
    for(int j=0; j<n; j++){
        c[0][j] = 0;
    }
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            if(x[i]==y[j]){
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = 'D';
            }else{
                if(c[i-1][j] >= c[i][j-1]){
                    c[i][j] = c[i-1][j];
                    b[i][j] = 'L';
                }else{
                    c[i][j] = c[i][j-1];
                    b[i][j] = 'U';
                }
            }
        }
    }

    check(x, m-1, n-1);
    // int len = strlen(ind);
    int n1;
    n1 = strlen(str);
    
    printf("Length of LCS is %d\n", c[m-1][n-1]);
    printf("LCS is ");
    for(int k = 0; k < n1+1; k++){
        printf("%c",str[n1-k]);
    }
    printf("\n");
    
    return 0;
}
