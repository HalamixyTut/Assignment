#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int mst[20];
int pair[40];
int num = 0;
int mstlen = 2;
int pairlen = 1;
int group1[20];
int group2[20];
int group3[20];
int len1 = 0;
int len2 = 0;
int len3 = 0;

struct edge
{
    int firstP;
    int secondP;
    int similarity;
    int flag;
} edges[40];

void firstFind(){
    int max = 0;
    int ind;
    for(int i=0; i<39; i++){
        if(edges[i].similarity > max && edges[i].flag == 1){
            max = edges[i].similarity;
            ind = i;
        }
    }
    mst[0] = edges[ind].firstP;
    mst[1] = edges[ind].secondP;
    pair[0] = ind;
    edges[ind].flag = 0;
}

int findCycle(int node1, int node2, int node3){
    for(int i=0; i<pairlen; i++){
        if(pair[i] != -1 && edges[pair[i]].firstP == node1 && edges[pair[i]].secondP != node3){
            if(edges[pair[i]].secondP == node2){
                return 1;
            }else{
                return findCycle(edges[pair[i]].secondP, node2, edges[pair[i]].firstP);
            }
        }else if(pair[i] != -1 && edges[pair[i]].secondP == node1 && edges[pair[i]].firstP != node3){
            if(edges[pair[i]].firstP == node2){
                return 1;
            }else{
                return findCycle(edges[pair[i]].firstP, node2, edges[pair[i]].secondP);
            }
        }
    }
}

void findMax(){
    int max = 0;
    int ind;
    int flag1 = 0;
    int flag2 = 0;
    for(int i=0; i<39; i++){
        if(edges[i].similarity > max && edges[i].flag == 1){
            max = edges[i].similarity;
            ind = i;
        }
    }
    for(int i=0; i<mstlen; i++){
        if(edges[ind].firstP == mst[i]){
            flag1 = 1;
        }
    }
    for(int i=0; i<mstlen; i++){
        if(edges[ind].secondP == mst[i]){
            flag2 = 1;
        }
    }
    if(flag1 == 1 && flag2 == 1){
        if(findCycle(edges[ind].firstP, edges[ind].secondP, 0) == 1){

        }else{
            pair[pairlen] = ind;
            pairlen++;
        }
    }else{
        if(flag1 == 0){
            mst[mstlen] = edges[ind].firstP;
            mstlen++;
        }
        if(flag2 == 0){
            mst[mstlen] = edges[ind].secondP;
            mstlen++;
        }
        pair[pairlen] = ind;
        pairlen++;
    }
    edges[ind].flag = 0;
}

void findMin(){
    pair[pairlen-1] = -1;
    edges[pair[pairlen-1]].firstP = 0;
    edges[pair[pairlen-1]].secondP = 0;
    edges[pair[pairlen-1]].similarity = 100;
}

void search1(int photoNumber){
    for(int i=0; i<pairlen; i++){
        if(pair[i] != -1 && photoNumber == edges[pair[i]].firstP){
            group1[len1] = edges[pair[i]].secondP;
            int temp = edges[pair[i]].secondP;
            pair[i] = -1;
            len1++;
            search1(temp);
        }else if(pair[i] != -1 && photoNumber == edges[pair[i]].secondP){
            group1[len1] = edges[pair[i]].firstP;
            int temp = edges[pair[i]].firstP;
            pair[i] = -1;
            len1++;
            search1(temp);
        }
    }
}

void search2(int photoNumber){
    for(int i=0; i<pairlen; i++){
        if(pair[i] != -1 && photoNumber == edges[pair[i]].firstP){
            group2[len2] = edges[pair[i]].secondP;
            int temp = edges[pair[i]].secondP;
            pair[i] = -1;
            len2++;
            search2(temp);
        }else if(pair[i] != -1 && photoNumber == edges[pair[i]].secondP){
            group2[len2] = edges[pair[i]].firstP;
            int temp = edges[pair[i]].firstP;
            pair[i] = -1;
            len2++;
            search2(temp);
        }
    }
}

void search3(int photoNumber){
    for(int i=0; i<pairlen; i++){
        if(pair[i] != -1 && photoNumber == edges[pair[i]].firstP){
            group3[len3] = edges[pair[i]].secondP;
            int temp = edges[pair[i]].secondP;
            pair[i] = -1;
            len3++;
            search3(temp);
        }else if(pair[i] != -1 && photoNumber == edges[pair[i]].secondP){
            group3[len3] = edges[pair[i]].firstP;
            int temp = edges[pair[i]].firstP;
            pair[i] = -1;
            len3++;
            search3(temp);
        }
    }
}

void category1(){
    int flag1 = 0;
    int flag2 = 0;
    for(int i=0; i<pairlen; i++){
        if(pair[i] != -1){
            group1[len1] = edges[pair[i]].firstP; len1++;
            flag1 = edges[pair[i]].firstP;
            group1[len1] = edges[pair[i]].secondP; len1++;
            flag2 = edges[pair[i]].secondP;
            pair[i] = -1;
            search1(flag1);
            search1(flag2);
            return;
        }
    }
}

void category2(){
    int flag1 = 0;
    int flag2 = 0;
    for(int i=0; i<pairlen; i++){
        if(pair[i] != -1){
            group2[len2] = edges[pair[i]].firstP; len2++;
            flag1 = edges[pair[i]].firstP;
            group2[len2] = edges[pair[i]].secondP; len2++;
            flag2 = edges[pair[i]].secondP;
            pair[i] = -1;
            search2(flag1);
            search2(flag2);
            return;
        }
    }
}

void category3(){
    int flag1 = 0;
    int flag2 = 0;
    for(int i=0; i<pairlen; i++){
        if(pair[i] != -1){
            group3[len3] = edges[pair[i]].firstP; len3++;
            flag1 = edges[pair[i]].firstP;
            group3[len3] = edges[pair[i]].secondP; len3++;
            flag2 = edges[pair[i]].secondP;
            pair[i] = -1;
            search3(flag1);
            search3(flag2);
            return;
        }
    }
}

int main(int argc, char *argv[]){

    edges[0].firstP = 1; edges[0].secondP = 2; edges[0].similarity = 80; edges[0].flag = 1;
    edges[1].firstP = 5; edges[1].secondP = 9; edges[1].similarity = 73; edges[1].flag = 1;
    edges[2].firstP = 9; edges[2].secondP = 13; edges[2].similarity = 69; edges[2].flag = 1;
    edges[3].firstP = 2; edges[3].secondP = 3; edges[3].similarity = 10; edges[3].flag = 1;
    edges[4].firstP = 5; edges[4].secondP = 7; edges[4].similarity = 20; edges[4].flag = 1;
    edges[5].firstP = 5; edges[5].secondP = 2; edges[5].similarity = 74; edges[5].flag = 1;
    edges[6].firstP = 9; edges[6].secondP = 18; edges[6].similarity = 15; edges[6].flag = 1;
    edges[7].firstP = 13; edges[7].secondP = 14; edges[7].similarity = 21; edges[7].flag = 1;
    edges[8].firstP = 9; edges[8].secondP = 1; edges[8].similarity = 92; edges[8].flag = 1;
    edges[9].firstP = 13; edges[9].secondP = 2; edges[9].similarity = 59; edges[9].flag = 1;
    edges[10].firstP = 20; edges[10].secondP = 4; edges[10].similarity = 6; edges[10].flag = 1;
    edges[11].firstP = 18; edges[11].secondP = 17; edges[11].similarity = 17; edges[11].flag = 1;
    edges[12].firstP = 11; edges[12].secondP = 3; edges[12].similarity = 74; edges[12].flag = 1;
    edges[13].firstP = 7; edges[13].secondP = 18; edges[13].similarity = 83; edges[13].flag = 1;
    edges[14].firstP = 7; edges[14].secondP = 12; edges[14].similarity = 27; edges[14].flag = 1;
    edges[15].firstP = 1; edges[15].secondP = 10; edges[15].similarity = 16; edges[15].flag = 1;
    edges[16].firstP = 5; edges[16].secondP = 15; edges[16].similarity = 47; edges[16].flag = 1;
    edges[17].firstP = 20; edges[17].secondP = 14; edges[17].similarity = 94; edges[17].flag = 1;
    edges[18].firstP = 14; edges[18].secondP = 18; edges[18].similarity = 77; edges[18].flag = 1;
    edges[19].firstP = 2; edges[19].secondP = 4; edges[19].similarity = 30; edges[19].flag = 1;
    edges[20].firstP = 11; edges[20].secondP = 14; edges[20].similarity = 62; edges[20].flag = 1;
    edges[21].firstP = 3; edges[21].secondP = 20; edges[21].similarity = 85; edges[21].flag = 1;
    edges[22].firstP = 11; edges[22].secondP = 7; edges[22].similarity = 62; edges[22].flag = 1;
    edges[23].firstP = 6; edges[23].secondP = 10; edges[23].similarity = 87; edges[23].flag = 1;
    edges[24].firstP = 12; edges[24].secondP = 15; edges[24].similarity = 81; edges[24].flag = 1;
    edges[25].firstP = 3; edges[25].secondP = 19; edges[25].similarity = 39; edges[25].flag = 1;
    edges[26].firstP = 9; edges[26].secondP = 8; edges[26].similarity = 45; edges[26].flag = 1;
    edges[27].firstP = 19; edges[27].secondP = 4; edges[27].similarity = 65; edges[27].flag = 1;
    edges[28].firstP = 8; edges[28].secondP = 16; edges[28].similarity = 70; edges[28].flag = 1;
    edges[29].firstP = 15; edges[29].secondP = 17; edges[29].similarity = 64; edges[29].flag = 1;
    edges[30].firstP = 1; edges[30].secondP = 11; edges[30].similarity = 32; edges[30].flag = 1;
    edges[31].firstP = 11; edges[31].secondP = 6; edges[31].similarity = 43; edges[31].flag = 1;
    edges[32].firstP = 12; edges[32].secondP = 6; edges[32].similarity = 87; edges[32].flag = 1;
    edges[33].firstP = 12; edges[33].secondP = 19; edges[33].similarity = 70; edges[33].flag = 1;
    edges[34].firstP = 7; edges[34].secondP = 16; edges[34].similarity = 39; edges[34].flag = 1;
    edges[35].firstP = 13; edges[35].secondP = 6; edges[35].similarity = 22; edges[35].flag = 1;
    edges[36].firstP = 4; edges[36].secondP = 16; edges[36].similarity = 68; edges[36].flag = 1;
    edges[37].firstP = 17; edges[37].secondP = 10; edges[37].similarity = 56; edges[37].flag = 1;
    edges[38].firstP = 19; edges[38].secondP = 15; edges[38].similarity = 71; edges[38].flag = 1;

    firstFind();
    while(mstlen < 20){
        findMax();
    }
    findMin();
    category1();
    category2();
    category3();
    printf("group1:");
    for(int i=0; i<len1; i++){
        printf(" %d", group1[i]);
    }
    printf("\n");
    printf("group2:");
    for(int i=0; i<len2; i++){
        printf(" %d", group2[i]);
    }
    printf("\n");
    printf("group3:");
    for(int i=0; i<len3; i++){
        printf(" %d", group3[i]);
    }
    printf("\n");
    return 0;
}