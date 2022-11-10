#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int studied[20];
int semester = 0;

struct courRelation
{
    int cournum;
    int prereq1;
    int prereq2;
} courRelation[20];

int checkReq(int req1, int req2){
    if(req1 == 0 && req2 == 0){
        return 1;
    }
    int flag;
    int flag1;
    int flag2;
    int len = sizeof(studied)/sizeof(studied[0]);
    for(int i=0;i<len;i++){
        if(req1 == studied[i] || req1 == 0){
            flag1 = 1;
        }else{
            flag1 = 0;
        }
    }
    for(int i=0;i<len;i++){
        if(req2 == studied[i] || req2 == 0){
            flag2 = 1;
        }else{
            flag2 = 0;
        }
    }
    if(flag1 == 1 && flag2 == 1){
        flag = 1;
    }else{
        flag = 0;
    }
    return flag;
}

void study(){
    int len = sizeof(courRelation)/sizeof(courRelation[0]);
    int req;
    int flag;
    for(int i=0;i<len;i++){
        int cour = courRelation[i].cournum;
        if(checkReq(cour, 0) == 1){
            continue;
        }
        int req1 = courRelation[i].prereq1;
        int req2 = courRelation[i].prereq2;
        flag = checkReq(req1, req2);
        if(flag == 1){
            studied[semester] == courRelation[i].cournum;
            semester++;
        }
    }
    if(semester != 15){
        study();
    }
}
int main(int argc, char *argv[]){
    courRelation[0].cournum = 1; courRelation[0].prereq1 = 0; courRelation[0].prereq2 = 0;
    courRelation[1].cournum = 2; courRelation[1].prereq1 = 0; courRelation[1].prereq2 = 0;
    courRelation[2].cournum = 3; courRelation[2].prereq1 = 0; courRelation[2].prereq2 = 0;
    courRelation[3].cournum = 4; courRelation[3].prereq1 = 2; courRelation[3].prereq2 = 0;
    courRelation[4].cournum = 5; courRelation[4].prereq1 = 4; courRelation[4].prereq2 = 0;
    courRelation[5].cournum = 6; courRelation[5].prereq1 = 1; courRelation[5].prereq2 = 3;
    courRelation[6].cournum = 7; courRelation[6].prereq1 = 4; courRelation[6].prereq2 = 0;
    courRelation[7].cournum = 8; courRelation[7].prereq1 = 5; courRelation[7].prereq2 = 6;
    courRelation[8].cournum = 9; courRelation[8].prereq1 = 7; courRelation[8].prereq2 = 0;
    courRelation[9].cournum = 10; courRelation[9].prereq1 = 9; courRelation[9].prereq2 = 0;
    courRelation[10].cournum = 11; courRelation[10].prereq1 = 8; courRelation[10].prereq2 = 0;
    courRelation[11].cournum = 12; courRelation[11].prereq1 = 3; courRelation[11].prereq2 = 0;
    courRelation[12].cournum = 13; courRelation[12].prereq1 = 6; courRelation[12].prereq2 = 0;
    courRelation[13].cournum = 14; courRelation[13].prereq1 = 4; courRelation[13].prereq2 = 6;
    courRelation[14].cournum = 15; courRelation[14].prereq1 = 14; courRelation[14].prereq2 = 0;

    study();

    printf("minimum number of semester: %d\n", semester);

    return 0;
}