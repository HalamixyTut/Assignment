#include <stdio.h>
#include <stdlib.h>

int maze[6][6];
int flag[6][6];
struct Path
{
  int x;
  int y;
} path[50];
int k = 0;

void DFSMaze(int maze[6][6], int x, int y){
  if (x+1 == 5 && y == 4){
    flag[x][y] = 1;
    flag[5][4] = 1;
    flag[0][1] = 1;
    printf("maze completed!\n");
    return;
  }
  if(maze[x][y+1] == 0 && flag[x][y+1] == 0){
    flag[x][y] = 1;
    flag[x][y+1] = 1;
    DFSMaze(maze, x, y+1);
  }
  if(maze[x+1][y] == 0 && flag[x+1][y] == 0){
    flag[x][y] = 1;
    flag[x+1][y] = 1;
    DFSMaze(maze, x+1, y);
  }
  if(x-1 >= 0 && maze[x-1][y] == 0 && flag[x-1][y] == 0){
    flag[x][y] = 1;
    flag[x-1][y] = 1;
    DFSMaze(maze, x-1, y);
  }
  if(y-1 >= 0 && maze[x][y-1] == 0 && flag[x][y-1] == 0){
    flag[x][y] = 1;
    flag[x][y-1] = 1;
    DFSMaze(maze, x, y-1);
  }
}

void checkFlag(int x, int y){
  if(x == 0 && y == 1){
    return;
  }
  if(x == 5 && y == 4){
    return;
  }
  int count = 0;
  if(flag[x][y+1] == 1){
    count++;
  }
  if(y-1 >= 0 && flag[x][y-1] == 1){
    count++;
  }
  if(x-1 >= 0 && flag[x-1][y] == 1){
    count++;
  }
  if(flag[x+1][y] == 1){
    count++;
  }
  if (count == 1){
    flag[x][y] = 0;
    maze[x][y] = 1;
    k++;
  }
}

void findDFSPath(){
  k=0;
  for(int i = 1; i < 5; i++){
    for (int j = 1; j < 5; j++){
      if(flag[i][j] == 1){
        checkFlag(i, j);
      }
    }
  }
}

void drawMaze(){
  printf("\n");
  for (int y = 0; y < 6; y++){
    for (int x = 0; x < 6; x++){
      if(maze[x][y] == 1){
        printf("#");
      }
      if(flag[x][y] == 1){
        printf("+");
      }
      if (x == 5){
        printf("\n");
      }
    }
  }
}

int main(void){

  maze[0][0] = 1; maze[1][0] = 1; maze[2][0] = 1; maze[3][0] = 1; maze[4][0] = 1; maze[5][0] = 1;
  maze[0][1] = 0; maze[1][1] = 0; maze[2][1] = 1; maze[3][1] = 0; maze[4][1] = 1; maze[5][1] = 1;
  maze[0][2] = 1; maze[1][2] = 0; maze[2][2] = 0; maze[3][2] = 0; maze[4][2] = 1; maze[5][2] = 1;
  maze[0][3] = 1; maze[1][3] = 0; maze[2][3] = 1; maze[3][3] = 0; maze[4][3] = 0; maze[5][3] = 1;
  maze[0][4] = 1; maze[1][4] = 0; maze[2][4] = 0; maze[3][4] = 1; maze[4][4] = 0; maze[5][4] = 0;
  maze[0][5] = 1; maze[1][5] = 1; maze[2][5] = 1; maze[3][5] = 1; maze[4][5] = 1; maze[5][5] = 1;
  
  DFSMaze(maze, 0, 1);
  findDFSPath();
  while(k!=0){
    findDFSPath();
  }
  drawMaze();

  return 0;
}