#include <stdio.h>
#include <stdlib.h>

char* enc(char plaintext[], char key[]){
    char ciphertext[1000];
    int i,plaintext_num,key_num,temp_num;
    char temp_num_string;
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
            ciphertext[i]=' ';
        else sprintf(ciphertext[i],"%c",temp_num+65);
        
        
    }
    ciphertext[i]='\0';
    return ciphertext;
}

int main(int argc, char *argv[]){

    char ciphertext[1000];
    strcpy(ciphertext,enc(argv[1],argv[2]));
    printf("%s\n",ciphertext);

}
