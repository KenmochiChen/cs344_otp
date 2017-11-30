#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]){

    int keygen_len=0;

    if(argc != 2){
        perror("argument wrong\n");
        exit(1);
    }
    keygen_len=atoi(argv[1]);
    //printf("%d\n",keygen_len);

    srand((unsigned)time(0));

    int i;
    int temp_num;
    char key[10000]="\0";
    char temp_key[3]="\0";
    for(i=0;i<keygen_len;i++){
        temp_num=rand()%27;
        if(temp_num==26){
            strcpy(temp_key," ");
            strcat(key,temp_key);
            continue;
        }
        sprintf(temp_key,"%c",temp_num+65);
        temp_key[1]='\0';
        strcat(key,temp_key);
    }
    key[keygen_len]='\0';
    //key[keygen_len+1]='\0';
    printf("%s\n",key);
}
