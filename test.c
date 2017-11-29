#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void enc(char plaintext[], char key[], char ciphertext_return[]){
    char ciphertext[1000];
    int i,plaintext_num,key_num,temp_num;
    char temp_num_string[3],temp_ciphertext[3];
    for(i=0;i<strlen(plaintext);i++){
        if(plaintext[i]==' '){
            plaintext_num=27;
        }
        else{
            sprintf(temp_num_string,"%d",plaintext[i]);
            plaintext_num=atoi(temp_num_string)-65;
        }

        if(key[i]==' '){
            key_num=27;
        }
        else{
            sprintf(temp_num_string,"%d",key[i]);
            key_num=atoi(temp_num_string)-65;
        }
        
        temp_num=plaintext_num+key_num;
        temp_num=temp_num%27;
        if(temp_num<0)
            temp_num=temp_num+27;
        if(temp_num==26)
            strcpy(temp_ciphertext," ");
        else sprintf(temp_ciphertext,"%c",temp_num+65);

        strcat(ciphertext,temp_ciphertext);
        
        
    }
    ciphertext[i]='\0';
    strcpy(ciphertext_return,ciphertext);
}

void dec(char plaintext[], char key[], char ciphertext_return[]){
    char ciphertext[1000];
    int i,plaintext_num,key_num,temp_num;
    char temp_num_string[3],temp_ciphertext[3];
    for(i=0;i<strlen(plaintext);i++){
        if(plaintext[i]==' '){
            plaintext_num=27;
        }
        else{
            sprintf(temp_num_string,"%d",plaintext[i]);
            plaintext_num=atoi(temp_num_string)-65;
        }

        if(key[i]==' '){
            key_num=27;
        }
        else{
            sprintf(temp_num_string,"%d",key[i]);
            key_num=atoi(temp_num_string)-65;
        }
        
        temp_num=plaintext_num-key_num;
        temp_num=temp_num%27;
        if(temp_num<0)
            temp_num=temp_num+27;
        if(temp_num==26)
            strcpy(temp_ciphertext," ");
        else sprintf(temp_ciphertext,"%c",temp_num+65);

        strcat(ciphertext,temp_ciphertext);
        
        
    }
    ciphertext[i]='\0';
    strcpy(ciphertext_return,ciphertext);
}

int main(int argc, char *argv[]){

    char ciphertext[1000];
    dec(argv[1],argv[2],ciphertext);
    printf("%s\n",ciphertext);

}
