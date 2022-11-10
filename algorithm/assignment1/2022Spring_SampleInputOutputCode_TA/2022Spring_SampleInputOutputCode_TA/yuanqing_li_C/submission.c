// This program reads an inputfile of numbers.
// The input file has on number per line.
// The program prints out the squares of the input file numbers
// to the output file.
// The output file has on numner per line. 

#include <stdio.h>
#include <stdlib.h>

long long findNumLines(FILE *fp);

FILE *srcFP, *destFP;
long long numLines, *data;

int main(int argc, char *argv[]) {

  int i;
  char str[100];

  if(argc!=3) {
    printf("Usage: progname <input_file> <output_file>");
    exit(0);
  }
  
  if((srcFP =fopen(argv[1], "r")) == NULL) {
    perror("Error opening input file");
    exit(0);
  }
  
  numLines = findNumLines(srcFP);
  
  data = (long long *) malloc(numLines * sizeof(long long));

  for(i=0; i<numLines; i++) {
    fgets(str, 100, srcFP);
    sscanf(str, "%lld", &(data[i]));
  }

  if((destFP = fopen(argv[2], "w")) == NULL) {
    perror("Error opening output file");
    exit(0);
  }
  
  writeDestFile(destFP);

  fclose(srcFP);
  fclose(destFP);
}

int writeDestFile(FILE *fp) {
  long long i;
  long long count =0;
  for(i = 0; i < numLines; i++){
  	for(long long j = 0; j < numLines; j++){
  		if(i==j){
  		    continue;
  		}
  		for(long long k = j; k < numLines; k++){
  		    if(i==k || j==k){
  		    	continue;
  		    }
  		    if(data[i]==data[j]+data[k]){
  		    	count++;
  		    	fprintf(fp, "%d=%d+%d\n", data[i], data[j], data[k]);
  		    }
  		}
  	}
  }
  fprintf(fp, "result: %d\n", count);
}

//returns file size as number of lines in the file
long long findNumLines(FILE *fp) {

  long long numLines = 0;
  char str[100];

  while(!feof(fp)) {
    fgets(str, 100, fp);
    numLines++;
  }
  
  numLines--;//the above increments count even when EOF reached
  
  if(fseek(fp, 0L, SEEK_SET) == EOF) {
    perror("Error while seeking to begining of file");
    exit(0);
  }

  return numLines;
}
