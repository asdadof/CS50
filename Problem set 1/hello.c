#include <stdio.h>
#include <cs50.h>

// Includes two libraries, stdio for the printf function and cs50 for the get_string function


int main(void){
    string name = get_string("What is you name? ");
    //Ask the user for a string, here a name, then stores it in a variabel called name

    printf("hello, %s\n", name);
    //Print hello, (the name name of the user) 
}
