#include <stdio.h>
#include <stdlib.h>

struct conn
{
  char *a;
  char *b;
} conn[20];

int main(void){
  conn[0].a = "x1"; conn[0].b = "y1";
  conn[1].a = "x1"; conn[1].b = "y2";
  conn[2].a = "y1"; conn[2].b = "x2";
  conn[3].a = "y2"; conn[3].b = "x2";
  conn[4].a = "y1"; conn[4].b = "x3";
  conn[5].a = "x3"; conn[5].b = "y3";

  conn[6].a = "a"; conn[6].b = "b";
  conn[7].a = "a"; conn[7].b = "c";
  conn[8].a = "a"; conn[8].b = "d";
  conn[9].a = "b"; conn[9].b = "c";
  conn[10].a = "b"; conn[10].b = "d";
  conn[11].a = "c"; conn[11].b = "d";

  printf("connected components in (i):\n");
  for(int i=0;i<6;i++){
    printf("%s and %s\n", conn[i].a, conn[i].b);
  }

  printf("connected components in (ii):\n");
  for(int i=6;i<12;i++){
    printf("%s and %s\n", conn[i].a, conn[i].b);
  }
}