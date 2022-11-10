#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char str[1000];
	char temp[1000];
	char res[1000];
	FILE * inFile;
	FILE * outFile;
	int flag = atoi(argv[4]);
	inFile = fopen(argv[1], "r");
	if(inFile != NULL){
		fgets(str, 1000, inFile);
	}
	outFile = fopen(argv[2], "w+");
	int plainlen = strlen(str);
	int keylen = strlen(argv[3])/sizeof(argv[3][0]);

	if(flag == 1){
		int index = 0;
		for(int i = 0; i < keylen; i++){
			temp[i] = argv[3][i];
		}
		for(int i = keylen; i < plainlen; i++){
			temp[i] = str[index++];
		}
		for(int i = 0; i < plainlen; i++){
			if(str[i] + temp[i] - 97 >= 'a' && str[i] + temp[i] - 97 <= 'z'){
				res[i] = str[i] + temp[i] - 97;
			}else if(res[i] = str[i] + temp[i] - 97 > 'z'){
				res[i] = str[i] + temp[i] - 97 - 26;
			}
		}
		for(int i = plainlen; i < 1000; i++){
			res[i] = '\0';
		}
	}else if(flag == 0){
		for(int i = 0; i < plainlen; i++){
			if(str[i] - argv[3][i] + 97 >= 'a' && str[i] - argv[3][i] + 97 <= 'z'){
				res[i] = str[i] - argv[3][i] + 97;
			}else if(str[i] - argv[3][i] + 97 < 'a'){
				res[i] = str[i] - argv[3][i] + 97 + 26;
			}
			argv[3][keylen++] = res[i];
		}
		for(int i = plainlen; i < 1000; i++){
			res[i] = '\0';
		}
	}
	fprintf(outFile, "%s", res);
	fclose(inFile);
	fclose(outFile);
	return 0;
}