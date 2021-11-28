#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define N 3000
//===============================================

char *randstring(int length) {
    static char charset[] = "abcdefghijklmnopqrstuvwxyz";        
    char *randomString = NULL;
    randomString = malloc(sizeof(char) * (length +1));
           
    for (int n = 0; n < length; n++) {            
        int key = rand() % (int)(sizeof(charset) -1);
        randomString[n] = charset[key];
    }
    randomString[length] = '\0';
    return randomString;
}
typedef struct random_word{
    char a[10];
    char b[10];
}list;

int main(){
    list sample[N];
    int l = 5;

    FILE *fptr;
    fptr = fopen("list.txt","w");
    for (int i = 0; i < N; i++){
        strcpy(sample[i].a, randstring(l));
        strcpy(sample[i].b, randstring(l));
        fprintf(fptr, "%s %s\n", sample[i].a, sample[i].b);
    }
    fclose(fptr);
}