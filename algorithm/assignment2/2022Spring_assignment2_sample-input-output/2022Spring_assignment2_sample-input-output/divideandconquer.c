#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

long long readFile(FILE *fp);

FILE *srcFP, *destFP;
struct POINT{
	long long x;
	long long y;
}point[10000];

struct minobj{
	long long a;
	long long b;
	float distance;
};

// calculate the distance between two points
float dist(struct POINT p1, struct POINT p2){
	float d = (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
	return sqrt(d);
}

//find the smaller number of two numbers
float min(float a, float b){
	return (a<b)?a:b;
}

//find two closest points
struct minobj minDist(struct POINT p[], int n){
	float min = FLT_MAX;
	long long a1;
	long long b1;
	for(int i=0;i<n;i++){
		for(int j=i+1;j<n;j++){
			if(dist(p[i],p[j])<min){
				min = dist(p[i],p[j]);
				a1 = i; b1 = j;
			}	
		}
	}
	struct minobj z;
	z.a = a1;
	z.b = b1;
	z.distance = min;
	return z;
}

struct minobj findClosest(struct POINT p[], int n){
	struct minobj obj;
	if(n<=3){
		obj = minDist(p, n);
		return obj;
	}
	int mid=n/2;
	struct POINT lp[mid];
	struct POINT rp[n-mid];
	/*for(int m=0;m<mid;m++){
		lp[m]=p[m];
	}
	for(int q=mid;q<n-mid;q++){
		rp[q]=p[q];
	}*/
	struct minobj lmin = minDist(p, mid);
	struct minobj rmin = minDist(p+mid, n-mid);
	rmin.a = rmin.a + mid;
	rmin.b = rmin.b + mid;
	float d = min(lmin.distance, rmin.distance);
	struct minobj lrmin = (lmin.distance<rmin.distance)?lmin:rmin;
	
	struct POINT ins[10000];
	int i=0;
	for(int j=0;j<n;j++){
		if(abs(p[j].x-p[mid].x)<d){
			ins[i]=p[j];
			i++;
		}
	}
	struct minobj insmin = minDist(ins, i-1);
	float abmin = min(d, insmin.distance);
	return (d<insmin.distance)?lrmin:insmin;
}

int main(int argc, char *argv[]) {

    int i=0;
    char str[100];

    if((srcFP =fopen(argv[1], "r")) == NULL) {
    	perror("Error opening input file");
    	exit(0);
  	}
  	
  	if((destFP =fopen(argv[2], "w+")) == NULL) {
    	perror("Error opening input file");
    	exit(0);
  	}
  
  	struct POINT p;
  	while(!feof(srcFP)) {
	    fgets(str, 100, srcFP);
	    sscanf(str, "%lld %lld", &p.x, &p.y);
	    point[i].x=p.x;
	    point[i].y=p.y;
	    i++;
	}
	
	int len = sizeof(point)/sizeof(point[0]);
	struct minobj obj = findClosest(point, i-1);
	fprintf(destFP, "closest pair points: p[%lld] and p[%lld], the smallest distance: %f\n", obj.a, obj.b, obj.distance);

}
