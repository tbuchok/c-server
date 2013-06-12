#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <fcntl.h>

#define DEFAULT_PORT 3000  // the port users will be connecting to
#define QUEUE 10 // how many pending connections queue will hold

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
    int i;
    for (i = 0; i < hangman->word_size; i++) {
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

  printf("Booting up server ...\n");
  struct sockaddr_in server_address, client_address;
  struct timeval select_timeout;
  fd_set readable_fds, writable_fds, error_fds;
  int server_address_size = sizeof server_address;
  int client_address_size = sizeof client_address;
  int server_fd, client_fd, max_fd;

  // Hangman stuff
  struct word hangman = { .letters = "Hello", .tries = 0 };
  hangman.word_size = sizeof(hangman.letters) / sizeof(hangman.letters[0]);
  hangman.total_tries = hangman.word_size * 2;
  // End Hangman stuff

  memset(&client_address, 0, client_address_size);
  memset(&server_address, 0, server_address_size);

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(DEFAULT_PORT);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  server_fd = socket(PF_INET, SOCK_STREAM, 0);
  
  max_fd = server_fd + 1;

  printf("| Socket connected FD %i ... \n", server_fd);
  int bound = bind(server_fd, (struct sockaddr*)&server_address, sizeof (struct sockaddr));
  if (bound < 0) {
    printf("Not bound!\n");
    return 1;
  }
    
  printf("| Bound ... %i\n", bound); // Log
  int listening = listen(server_fd, 20);
  printf("| Listening ... %i\n", listening); // Log

  FD_ZERO(&readable_fds);
  FD_SET(server_fd, &readable_fds);  // solved our problem? set server_fds `readable_fds`

    int a = 0, b = 0;
  while (1) {
    // FD_ZERO(&writable_fds);
    // FD_ZERO(&error_fds);

    select_timeout.tv_sec = 1; // seconds
    select_timeout.tv_usec = 0; // microseconds
    select(max_fd, &readable_fds, &writable_fds, (fd_set *) 0, NULL);
    int i;
    for (i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &readable_fds)) {
        if (i == server_fd) {
          client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_size);
          if (client_fd < 0) {
            printf("!! Bad connection on %i with errono: %i!! \n", client_fd, errno);
            continue;
          }
          FD_SET(client_fd, &readable_fds);
        } else {
          char msg[1024];
          recv(i, &msg, 1024, 0);
          checkWord(msg[0], &hangman);
          int l = 0;
          while (msg[l] > 0)
            l++;
          send(i, &msg, l, 0);
          printf("msg: %s\n", msg);
          FD_CLR(i, &readable_fds);
          close(i);
        }
      }
    }
    printf("select is released, re-entering loop ...\n");
  }
  close(server_fd);
  printf("Closing down the server ... \n");
  return 0; // end!
}