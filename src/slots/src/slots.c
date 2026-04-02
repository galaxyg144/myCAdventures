#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

  // time to add persistency!
  FILE *fptr;

  fptr = fopen("points.txt", "r");
  srand(time(NULL));

  if (fptr == NULL) {
    puts("File not found bozo!");
    return 0;
  }

  char buffer[100];
  while (fgets(buffer, 100, fptr)) {
    printf("%s\n", buffer);
  }

  int points = 10;
  int bet = 0;
  char response[4];

  printf("fuck you pq. makin me do this.\n");

  while (true) {
    if (points <= 0) {
      printf("you're broke. get out.\n");
      break;
    }

    printf("you have %d points!\n", points);
    printf("Insert bet here: ");
    scanf("%d", &bet);

    if (bet < 0 || bet > points) {
      printf("not enough points!\n");
      continue;
    } else if (bet == 0) {
      printf("no bet? get out!\n");
      break;
    }

    // hehe jjk modulo operator
    // re-roll each spin (was outside loop before whoops)
    int slot1 = (rand() % 7) + 1;
    int slot2 = (rand() % 7) + 1;
    int slot3 = (rand() % 7) + 1;

    points -= bet;

    printf("[ %d | %d | %d ]\n", slot1, slot2, slot3);

    // match 3 sevens 20x ze score!
    if (slot1 == 7 && slot2 == 7 && slot3 == 7) {
      int earnings = bet * 20;
      printf("MEGGGAAAA JACKPOOOOOOOOOOT!");
      printf("You won: %d points!\n", earnings);
      points += earnings;

    }
    // match 3, 10x it!
    else if (slot1 == slot2 && slot2 == slot3) {
      int earnings = bet * 10;
      printf("JACKPOOOOOOOOOOT!");
      printf("You won: %d points!\n", earnings);
      points += earnings;
    }
    // match 2 is boring.
    else if (slot1 == slot2 || slot2 == slot3 || slot1 == slot3) {
      int earnings = bet * 2;
      printf("You won: %d points!\n", earnings);
      points += earnings;
    } else {
      printf("you lost.\n");
    }

    printf("Spin again? (y/n): ");
    scanf("%s", response);
    if (response[0] == 'n')
      break;
  }

  printf("final score: %d points\n", points);
  return 0;
}