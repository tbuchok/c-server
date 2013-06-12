#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {

  int message_buffer = 1, message_offset, read_length;
  char * message = malloc(message_buffer);
  fgets(message, message_buffer, stdin);
  
  while(message[strlen(message) - 1] != '\n') {
    message_buffer *= 2;
    message = realloc(message, message_buffer);

    message_offset = (message_buffer / 2) - 1;
    read_length = message_offset + 2;

    fgets(message + message_offset, read_length, stdin);
  }

  printf("\nThis is what you entered:\n%s\n", message);

  return 0;
}