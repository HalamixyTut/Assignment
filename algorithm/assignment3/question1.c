#include <stdio.h>
#include <stdlib.h>

// New York City 0
// Albany 1
// Montreal 2
// Allentown 3
// Scranton 4
// Binghamton 5
// Syracuse 6
// Kingston 7
// Rochester 8
// Buffalo 9
// Toronto 10

struct Dist
{
  int ori;
  int des;
  int dist;
} path[20];

int city[20];

int c[20][20];

void compareDist(int c[20][20], int x, int y){
  if(city[x]+c[x][y]<city[y]){
    city[y]=city[x]+c[x][y];
  }
}

int main(void){
  path[0].ori = 0; path[0].des = 3; path[0].dist = 70;
  path[1].ori = 3; path[1].des = 4; path[1].dist = 65;
  path[2].ori = 0; path[2].des = 4; path[2].dist = 100;
  path[3].ori = 4; path[3].des = 5; path[3].dist = 60;
  path[4].ori = 0; path[4].des = 5; path[4].dist = 180;
  path[5].ori = 0; path[5].des = 1; path[5].dist = 140;
  path[6].ori = 5; path[6].des = 6; path[6].dist = 70;
  path[7].ori = 1; path[7].des = 6; path[7].dist = 100;
  path[8].ori = 1; path[8].des = 2; path[8].dist = 130;
  path[9].ori = 2; path[9].des = 7; path[9].dist = 160;
  path[10].ori = 6; path[10].des = 7; path[10].dist = 65;
  path[11].ori = 6; path[11].des = 8; path[11].dist = 70;
  path[12].ori = 8; path[12].des = 9; path[12].dist = 60;
  path[13].ori = 7; path[13].des = 10; path[13].dist = 180;
  path[14].ori = 9; path[14].des = 10; path[14].dist = 100;

  city[0] = 0;
  city[1] = __INT_MAX__;
  city[2] = __INT_MAX__;
  city[3] = __INT_MAX__;
  city[4] = __INT_MAX__;
  city[5] = __INT_MAX__;
  city[6] = __INT_MAX__;
  city[7] = __INT_MAX__;
  city[8] = __INT_MAX__;
  city[9] = __INT_MAX__;
  city[10] = __INT_MAX__;

  c[0][3] = 70; c[3][4] = 65; c[0][4] = 100; c[4][5] = 60; c[0][5] = 180; c[0][1] = 140; c[5][6] = 70;
  c[1][6] = 100; c[1][2] = 130; c[2][7] = 160; c[6][7] = 65; c[6][8] = 70; c[8][9] = 60; c[7][10] = 180; c[9][10] = 100;

  int n = 11;

  for(int i=0; i< n-1; i++){
    for(int m=0; m<15; m++){
      compareDist(c, path[m].ori, path[m].des);
    }
  }
  
  printf("shortest path: %d\n", city[10]);
  return 0;
}