//
//  tool.c
//  Project1
//
//  Created by Xiang on 2020/1/28.
//  Copyright © 2020年 Xiang. All rights reserved.
//

#include "tool.h"



char *replaceWord(const char *s, const char *oldW,
                  const char *newW)
{
    char *result;
    int i, cnt = 0;
    unsigned long newWlen = strlen(newW);
    unsigned long oldWlen = strlen(oldW);
    
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;
            
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
    
    // Making new string of enough length
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);
    
    i = 0;
    while (*s)
    {
        // compare the substring with the result
        if (strstr(s, oldW) == s)
        {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
    
    result[i] = '\0';
    return result;
}


void load_memory_map(int *memory, char *path){
    FILE *fp;
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("cannot open file: %s", path);
        exit(1);
    }
    char c;
    c = fgetc(fp);
    char num_str[10]={0};
    int str_idx = 0;
    int memory_idx = 0;
    while (c != EOF)
    {
        if (c == ' ') {
            num_str[str_idx] = '\0';
            memory[memory_idx++] = atoi(num_str);
            str_idx = 0;
        }else{
            num_str[str_idx++] = c;
        }
//        printf ("%c", c);
        c = fgetc(fp);
    }
    fclose(fp);
    if (memory_idx != 16*1024) {
        printf("memory size(%d) failed!\n",memory_idx);
        exit(1);
    }
    
    
}
