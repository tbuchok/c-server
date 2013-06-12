#include <stdio.h>
#include <stdbool.h>

struct word {
  int amount_found;
  int tries;
  int total_tries;
  int word_size;
  char letters[5];
  bool found[5];
};

bool checkWord(char entry, struct word *hangman) {
    
  bool done = false;

  if (entry != '\n') {
    char word[5];
    for (int i = 0; i < hangman->word_size; i++) {
      if (hangman->letters[i] == entry || hangman->found[i]) {
        hangman->found[i] = true;
        done = true;
        word[i] = hangman->letters[i];
      } else {
        done = false;
        word[i] = '_';
      }
    }
    printf("%s\n", word);
    hangman->tries++;
  }  
  return done;
}

int main(void) {

  char entry;
  bool done = false;
  struct word hangman = { .letters = "Hello", .tries = 0 };
  hangman.word_size = sizeof(hangman.letters) / sizeof(hangman.letters[0]);
  hangman.total_tries = hangman.word_size * 2;

  while(!done && hangman.tries < hangman.total_tries) {
    printf("Enter a letter:\n");
    scanf("%1s", &entry);
    done = checkWord(entry, &hangman);
  }

  if (done) {
    printf("You've defeated the hangman!\n");
  } else {
    printf("Please try again!\n");
  }

  return 0;
}