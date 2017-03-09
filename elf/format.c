#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if (argc < 2){
        printf("argv[] required!\n");
        exit(0);
    }
    
    printf("You input: ");
    printf(argv[1]);
    printf("Down\n");

    return 0;
}