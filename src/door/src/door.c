#include <stdio.h>

int doorLock = 2763;
int guess;

int main(){
    while (1)
    {
        puts("There lies a door infront of you.\n");
        puts("You notice a keypad on the door. It seems to be a 4 digit lock. \n");

        puts("Enter code: ");
        scanf("%d", &guess);

        if (guess == doorLock){
            puts("The door's hydraulics hiss... and inside... is a perfectly smooth mars bar.");
            return 0;
        } else{
            puts("The door beeps. Youve guessed incorrectly.");
            return 0;
        }
    }
    
}