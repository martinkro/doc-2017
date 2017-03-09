/* gcc -S -m32 -o hello.s hello.c */

#include <stdio.h>

int add(int a, int b){
    return a + b;
}

int main(int argc, char* argv[]){
    int a = 3;
    int b = 4;
    int c = add(a,b);
    printf("a+b=%d\n", c);
    printf("Hello World!\n");
    return 0;
    
}