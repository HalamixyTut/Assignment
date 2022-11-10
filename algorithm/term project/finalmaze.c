#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
struct node
{
    int x;
    int y;
    int previousnode;
};
int lastnode;
struct node nd[100];
void BFS(int maze[10][10]);
bool checkdeadend(int i, int j);
int Maze[10][10];
int maze1[10][10];
int maze2[10][10];
int maze3[10][10];
int flag[10][10];
int k = 0;
int checkneigbor(int maze[10][10], int x, int y);
int CreateMaze(int maze[10][10], int x, int y, int endx, int endy);
int Check(int i, int j);
void Output(int qe);
void vistednode(int i,int j);
void BFSpath();
void DFSMaze(int maze[10][10], int x, int y);
void checkFlag(int x, int y);
void findDFSPath();
void checkdead();
int main(void)
{
	clock_t bfsstart, bfsend, dfsstart, dfsend;
	srand((unsigned)time(NULL));
	int a[10][10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Maze[i][j] = 1;
		}
	}
	for (int i = 8; i > 0; i--)
	{
		Maze[8][i] = 0;
	}
	Maze[0][2] = 0;
	Maze[9][6] = 0;
	CreateMaze(Maze, 1, 2, 9, 6);
	for(int i=0; i<10; i++){
		for(int j=0; j<10; j++){
			maze1[i][j] = Maze[i][j];
			flag[i][j] = Maze[i][j];
		}
	}
	
	bfsstart = clock();
	BFS(Maze);
	Maze[0][2] = 2;
	BFSpath();

	printf("BFS:\n");
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Maze[j][i] == 1)
			{
				printf("[]");
			}
			if(Maze[j][i] == 2)
			{
				printf("->");
			}
			if(Maze[j][i] == -1)
			{
				printf("  ");
			}
			if(Maze[j][i] == 0)
			{
				printf("  ");
			}
		}
		printf("\n");
	}
	bfsend = clock();
	double BFStime = ((double)(bfsend-bfsstart));
	printf("BFS running time: %f ms\n", BFStime);

	dfsstart = clock();
	DFSMaze(maze1, 0, 2);
	flag[1][2] = 2;

	printf("\n");
	printf("DFS:\n");
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (flag[j][i] == 1)
			{
				printf("[]");
			}
			if(flag[j][i] == 0)
			{
				printf("  ");
			}
			if(flag[j][i] == -1)
			{
				printf("  ");
			}
			if(flag[j][i] == 2)
			{
				printf("->");
			}
		}
		printf("\n");
	}
	dfsend = clock();
	double DFStime = ((double)(dfsend-dfsstart));
	printf("DFS running time: %f ms\n", DFStime);
	return 0;
}
int checkneigbor(int maze[10][10], int x, int y)
{
	int count = 0;
	if (maze[x - 1][y] == 0 && x > 1)
	{
		count++;
	}
	if (maze[x + 1][y] == 0 && x < 8)
	{
		count++;
	}
	if (maze[x][y + 1] == 0 && y < 8)
	{
		count++;
	}
	if (maze[x][y - 1] == 0 && y > 1)
	{
		count++;
	}
	if (count > 1)
	{
		return 1;
	}
	return 0;
}
int CreateMaze(int maze[10][10], int x, int y, int endx, int endy)
{
	int dx = x;
	int dy = y;
	maze[x][y] = 0;
	int udcount = 0;
	int count = 0;
	int i = 0;
	while (1)
	{
		if (dx > 7)
		{
			break;
		}
		if (count >= rand() % 3 && count != 0)
		{
			count = 0;
			continue;
		}
		if (udcount >= 10)
		{

			dx++;
			if (dx > 7)
			{
				break;
			}
			maze[dx][dy] = 0;
			udcount = 0;
			count = 0;
			continue;
		}
		while (1)
		{
			int r = rand() % 4;
			if (r == 0)
			{
				if (dx + 1 > 7)
				{
					break;
				}
				int temp = dx + 1;
				if (checkneigbor(maze, temp, dy))
				{
					count = 0;
					udcount++;
					break;
				}
				dx++;

				if (dx > 7)
				{
					break;
				}
				maze[dx][dy] = 0;
				count++;
				udcount = 0;
				break;
			}
			if (r == 1)
			{
				if (dx - 1 < 1)
				{
					break;
				}
				int temp = dx - 1;
				if (checkneigbor(maze, temp, dy))
				{
					count = 0;
					udcount++;
					break;
				}
				dx--;
				if (dx < 1)
				{
					dx++;
					break;
				}
				maze[dx][dy] = 0;
				count++;
				udcount++;
				break;
			}
			if (r == 2)
			{
				if (dy < 1)
				{
					break;
				}
				int temp = dy - 1;
				if (checkneigbor(maze, dx, temp))
				{
					count = 0;
					udcount++;
					break;
				}
				dy--;
				if (dy < 1)
				{
					dy++;
					break;
				}
				maze[dx][dy] = 0;
				count++;
				udcount++;
				break;
			}
			if (r == 3)
			{
				if (dy + 1 > 8)
				{
					break;
				}
				int temp = dy + 1;
				if (checkneigbor(maze, dx, temp))
				{
					count = 0;
					udcount++;
					break;
				}
				dy++;
				if (dy > 8)
				{
					dy--;
					break;
				}
				maze[dx][dy] = 0;
				count++;
				udcount++;
				break;
			}
		}
	}
}
void vistednode(int i,int j){
    Maze[i][j] = -1;
}
void findstartpoint(int maze[10][10]){
    for(int i = 0; i<10;i++){
        if(maze[0][i] == 0){
            nd[0].x = 0;
            nd[0].y = i;
        }
    }
}
bool checkdeadend(int i, int j){
    if(Maze[i][j] == 1 || Maze[i][j] == -1 ){
        return false;
    }
    return true;
}
bool checkend(int i, int j){
    if(i == 9 && j == 6){
        return true;
    }
    return false;
}
void BFS(int maze[10][10]){
    maze[0][2] = -1;
    int i,j;
    int curfirstnode = 0;
    int curlastnode = 1;
    bool getend = true;
	nd[0].x = 0;
	nd[0].y = 2;
	nd[0].previousnode = -1;
	nd[1].x = 1;
	nd[1].y = 2;
	nd[1].previousnode = 0;
    while(getend){
        curfirstnode++;
        for(int k = 0; k<=3; k++){
            if(k==0){
                //go up
                i = nd[curfirstnode].x;
                j = nd[curfirstnode].y + 1;
                if(checkdeadend(i,j)){
                    curlastnode++;
                    nd[curlastnode].x = i;
                    nd[curlastnode].y = j;
                    nd[curlastnode].previousnode = curfirstnode;
                    maze[i][j]=-1;
					if(checkend(i,j)){
                    	getend = false;
						lastnode = curlastnode;
                    	return;
                	}
                }
            } 
            if(k==1){
                //go down
                i = nd[curfirstnode].x;
                j = nd[curfirstnode].y - 1;
                if(checkdeadend(i,j)){
                    curlastnode++;
                    nd[curlastnode].x = i;
                    nd[curlastnode].y = j;
                    nd[curlastnode].previousnode = curfirstnode;
                    maze[i][j]=-1;
					if(checkend(i,j)){
                    	getend = false;
						lastnode = curlastnode;
                    	return;
                	}
                }
            } 
            if(k==2){
                //go left
                i = nd[curfirstnode].x - 1;
                j = nd[curfirstnode].y;
                if(checkdeadend(i,j)){
                    curlastnode++;
                    nd[curlastnode].x = i;
                    nd[curlastnode].y = j;
                    nd[curlastnode].previousnode = curfirstnode;
                    maze[i][j]=-1;
					if(checkend(i,j)){
                    	getend = false;
						lastnode = curlastnode;
                    	return;
                	}
                }
            } 
            if(k==3){
                //go right
                i = nd[curfirstnode].x + 1;
                j = nd[curfirstnode].y;
                if(checkdeadend(i,j)){
                    curlastnode++;
                    nd[curlastnode].x = i;
                    nd[curlastnode].y = j;
                    nd[curlastnode].previousnode = curfirstnode;
                    maze[i][j]=-1;
					if(checkend(i,j)){
						getend = false;
						lastnode = curlastnode;
						return;
               		}
                }
            }
        }
    }
}

void BFSpath(){
	int i = 0;
	while(nd[lastnode].previousnode!=-1){
		Maze[nd[lastnode].x][nd[lastnode].y] = 2;
		lastnode = nd[lastnode].previousnode;
		i++;
	}	
}

void DFSMaze(int maze[10][10], int x, int y){
	if (x == 8 && y == 6){
    flag[x][y] = 2;
    flag[9][6] = 2;
    flag[0][2] = 2;
    return;
  }
  if(maze[x+1][y] == 0 && flag[x+1][y] == 0){
    flag[x][y] = 2;
    flag[x+1][y] = 2;
	checkdead(x+1, y);
    DFSMaze(maze, x+1, y);
	return;
  }
  if(maze[x][y+1] == 0 && flag[x][y+1] == 0){
    flag[x][y] = 2;
    flag[x][y+1] = 2;
	checkdead(x, y+1);
    DFSMaze(maze, x, y+1);
	return;
  }
  if(y-1 >= 0 && maze[x][y-1] == 0 && flag[x][y-1] == 0){
    flag[x][y] = 2;
    flag[x][y-1] = 2;
	checkdead(x, y-1);
    DFSMaze(maze, x, y-1);
	return;
  }
  if(x-1 >= 0 && maze[x-1][y] == 0 && flag[x-1][y] == 0){
    flag[x][y] = 2;
    flag[x-1][y] = 2;
	checkdead(x-1, y);
    DFSMaze(maze, x-1, y);
	return;
  }
}

void checkFlag(int x, int y){
  if(x == 0 && y == 2){
    return;
  }
  if(x == 9 && y == 6){
    return;
  }
  int count = 0;
  if(flag[x][y+1] == 1 || flag[x][y+1] == -1){
    count++;
  }
  if(y-1 >= 0){
	  if (flag[x][y-1] == 1 || flag[x][y-1] == -1){
		  count++;
	  }
  }
  if(x-1 >= 0){
	  if (flag[x-1][y] == 1 || flag[x-1][y] == -1){
		  count++;
	  }
  }
  if(flag[x+1][y] == 1 || flag[x+1][y] == -1){
    count++;
  }
  if (count == 1){
    flag[x][y] = -1;
    k++;
  }
}

void findDFSPath(){
  k=0;
  for(int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      if(flag[i][j] == 2){
        checkFlag(i, j);
      }
    }
  }
}

void checkdead(int x, int y){
	int count = 0;
	int k = 0;
	if(x+1 < 10 && maze1[x+1][y] == 0){
		count++;
		k = 1;
	}
	if(x-1 > 0 && maze1[x-1][y] == 0){
		count++;
		k = 2;
	}
	if(y+1 < 10 && maze1[x][y+1] == 0){
		count++;
		k = 3;
	}
	if(y-1 > 0 && maze1[x][y-1] == 0){
		count++;
		k = 4;
	}
	if (count == 1){
		flag[x][y] = -1;
		if (k == 1){
			DFSMaze(maze1, x+1, y);
		}
		if (k == 2){
			DFSMaze(maze1, x-1, y);
		}
		if (k == 3){
			DFSMaze(maze1, x, y+1);
		}
		if (k == 4){
			DFSMaze(maze1, x, y-1);
		}
	}
}