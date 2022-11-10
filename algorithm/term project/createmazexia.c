#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
static int Rank = 0;
int checkneigbor(int maze[10][10], int x, int y);
int CreateMaze(int maze[10][10], int x, int y, int endx, int endy);
int main(void)
{
	srand((unsigned)time(NULL));
	int Maze[10][10];
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
	// for (int i = 0; i < 10; i++) {
	// 	for (int j = 0; j < 10; j++) {
	// 		if (showmaze[i][j] == 1) {
	// 			printf("[]");
	// 		}
	// 		else {
	// 			printf("  ");
	// 		}
	// 	}
	// 	printf("\n");
	// }
	CreateMaze(Maze, 1, 2, 9, 6);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Maze[j][i] == 1)
			{
				printf("[]");
			}
			else
			{
				printf("  ");
			}
		}
		printf("\n");
	}
	// for (int i = L - 3; i >= 0; i--) {
	// 	if (Maze[i][L - 3] == ROUTE) {
	// 		Maze[i][L - 2] = ROUTE;
	// 		break;
	// 	}
	// }
	// for (int i = 0; i < 10; i++) {
	// 	for (int j = 0; j < 10; j++) {
	// 		if (Maze[i+1][j+1] == ROUTE) {
	// 			a[i][j]=0;
	// 		}
	// 		else {
	// 			a[i][j]=1;
	// 		}
	// 	printf("%d",a[i][j]);
	// 	}
	// 	printf("\n");
	// }

	// for (int i = 0; i < 10; i++) free(Maze[i]);
	// free(Maze);
	//system("pause");
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
	//int direction[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };
	// for (int i = 0; i < 4; i++) {
	// 	int r = rand() % 4;
	// 	int temp = direction[0][0];
	// 	direction[0][0] = direction[r][0];
	// 	direction[r][0] = temp;

	// 	temp = direction[0][1];
	// 	direction[0][1] = direction[r][1];
	// 	direction[r][1] = temp;
	// }
	int dx = x;
	int dy = y;
	maze[x][y] = 0;
	int udcount = 0;
	int count = 0;
	int i = 0;
	// int range = 1 + (Rank == 0 ? 0 : rand() % Rank);
	while (1)
	{
		//printf("yes");
		//printf("dx:%d",dx);
		//printf("dy:%d",dy);
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
			//printf("right\n");
			continue;
		}
		while (1)
		{
			int r = rand() % 4;
			if (r == 0)
			{
				//go right
				if (dx + 1 > 7)
				{
					break;
				}
				int temp = dx + 1;
				if (checkneigbor(maze, temp, dy))
				{
					//printf("right flag\n");
					count = 0;
					udcount++;
					break;
				}
				dx++;

				if (dx > 7)
				{
					break;
				}
				// if(maze[dx][dy] == 0){
				// 	dx--;
				// 	udcount++;
				// 	break;
				// }
				maze[dx][dy] = 0;
				count++;
				udcount = 0;
				//printf("right\n");
				break;
			}
			if (r == 1)
			{
				//go left
				if (dx - 1 < 1)
				{
					break;
				}
				int temp = dx - 1;
				if (checkneigbor(maze, temp, dy))
				{
					//printf("left falg\n");
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
				// if(maze[dx][dy] == 0){
				// 	dx++;
				// 	udcount++;
				// 	break;
				// }
				maze[dx][dy] = 0;
				count++;
				udcount++;
				//printf("left\n");
				break;
			}
			if (r == 2)
			{
				//go up
				// int temp  = dy-1;
				// if(maze[dx-1][temp]==0&maze[temp][dy-1]==0){
				// 	udcount = 0;
				// 	count = 0;
				// 	break;
				// }
				if (dy < 1)
				{
					break;
				}
				int temp = dy - 1;
				if (checkneigbor(maze, dx, temp))
				{
					//printf("up flag\n");
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
				// if(maze[dx][dy] == 0){
				// 	dy++;
				// 	udcount++;
				// 	break;
				// }
				maze[dx][dy] = 0;
				count++;
				udcount++;
				//printf("up\n");
				break;
			}
			if (r == 3)
			{
				if (dy + 1 > 8)
				{
					break;
				}
				//go down
				int temp = dy + 1;
				if (checkneigbor(maze, dx, temp))
				{
					//printf("down flag\n");
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
				// if(maze[dx][dy] == 0){
				// 	dy--;
				// 	udcount++;
				// 	break;
				// }
				maze[dx][dy] = 0;
				count++;
				udcount++;
				//printf("down\n");
				break;
			}
			// int count = 0;
			// for (int j = dx - 1; j < dx + 2; j++) {
			// 	for (int k = dy - 1; k < dy + 2; k++) {
			// 		if (abs(j - dx) + abs(k - dy) == 1 && maze[j][k] == 1) {
			// 			count++;
			// 		}
			// 	}
			// }

			// if (count > 1) {
			// 	break;
			// }
		}
	}
	//printf("yes");
	//CreateMaze(maze, dx, dy, endx, endy);
}
