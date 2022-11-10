#include<stdio.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>
#define L 12
static int Rank = 0;

void CreateMaze(int **maze, int x, int y) {
	maze[x][y] = 1;
	int direction[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };
	for (int i = 0; i < 4; i++) {
		int r = rand() % 4;
		int temp = direction[0][0];
		direction[0][0] = direction[r][0];
		direction[r][0] = temp;

		temp = direction[0][1];
		direction[0][1] = direction[r][1];
		direction[r][1] = temp;
	}
	for (int i = 0; i < 4; i++) {
		int dx = x;
		int dy = y;
		int range = 1 + (Rank == 0 ? 0 : rand() % Rank);
		while (range>0) {
			dx += direction[i][0];
			dy += direction[i][1];

			if (maze[dx][dy] == 1) {
				break;
			}
			int count = 0;
			for (int j = dx - 1; j < dx + 2; j++) {
				for (int k = dy - 1; k < dy + 2; k++) {
					if (abs(j - dx) + abs(k - dy) == 1 && maze[j][k] == 1) {
						count++;
					}
				}
			}

			if (count > 1) {
				break;
			}
			--range;
			maze[dx][dy] = 1;
		}
		if (range <= 0) {
			CreateMaze(maze, dx, dy);
		}
	}
}

void generateMaze(int **Maze){
	for (int i = 0; i < L; i++){
		Maze[i][0] = 1;
		Maze[0][i] = 1;
		Maze[i][L - 1] = 1;
		Maze[L - 1][i] = 1;
	}
	CreateMaze(Maze, 2, 2);
	Maze[2][1] = 1;
	for (int i = L - 3; i >= 0; i--) {
		if (Maze[i][L - 3] == 1) {
			Maze[i][L - 2] = 1;
			break;
		}
	}
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (Maze[i][j] == 1) {
				printf("  ");
			}
			else {
				printf("[]");
			}
		}
		printf("\n");
	}
}

int main(void) {
	srand(time(NULL));
	int **Maze = (int**)malloc(L * sizeof(int *));
	for (int i = 0; i < L; i++) {
		Maze[i] = (int*)calloc(L, sizeof(int));
	}
	generateMaze(Maze);

	// 111

	for (int i = 0; i < L; i++) free(Maze[i]);
	free(Maze);
	return 0;
}