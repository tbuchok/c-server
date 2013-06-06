#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DEFAULT_PORT 3000  // the port users will be connecting to
#define QUEUE 10 // how many pending connections queue will hold

int main(void) {

  printf("Booting up server ...\n");
  struct sockaddr_in server_address;
  int server_address_size = sizeof server_address;
  int server_fd, client_fd;

  memset(&server_address, 0, sizeof server_address);
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(DEFAULT_PORT);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  server_fd = socket(PF_INET, SOCK_STREAM, 0);
  printf("| Socket connected FD %i ... \n", server_fd);
  int bound = bind(server_fd, (struct sockaddr*)&server_address, sizeof (struct sockaddr));
  printf("| Bound ... %i\n", bound); // Log
  int listening = listen(server_fd, 10);
  printf("| Listening ... %i\n", listening); // Log
  while (1) {
    printf("Accepting a connections ...\n");
    client_fd = accept(server_fd, (struct sockaddr*)&server_address, &server_address_size);
    printf("> Received connection %i ... \n", client_fd);
    char msg[1024];
    recv(client_fd, &msg, 1024, 0);
    printf("< Sending :%s", msg); // Log
    send(client_fd, &msg, sizeof msg, 0);
    close(client_fd);
  }
  close(server_fd);
  printf("Closing down the server ... \n");
  return 0; // end!
}