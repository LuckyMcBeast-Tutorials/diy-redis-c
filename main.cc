#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <string.h>

static void do_something(int connfd) {
  char rbuf[64] = {};
  ssize_t n = read(connfd, rbuf, sizeof(rbuf)-1);
  if (n < 0) {
    fprintf(stderr, "read() error occurred in do_something(int connfd)");
    return;
  }
  printf("client says: %s\n", rbuf);

  char wbuf[] = "world";
  write(connfd, wbuf, strlen(wbuf));
}

int main(int argc, char *argv[]) {
  //Obtain socket handle with IPv4 and TCP
  int fd = socket(AF_INET, SOCK_STREAM, 0);

  //Set socket options (allows for IP:PORT reuse on restart)
  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  //Bind to an address
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(1234);
  addr.sin_addr.s_addr = htonl(0);
  int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
  // bind returns -1 if fails and 0 if it is successful (man bind.2)
  // C considers non-zero values as true
  if (rv) { perror("bind"); return 1;}

  //Listen
  rv = listen(fd, SOMAXCONN);
  if (rv) { perror("listen"); return 1; }

  //Accept Connections
  while(true) {
    struct sockaddr_in client_addr = {};
    socklen_t addrlen = sizeof(client_addr);
    int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
    if (connfd < 0) { continue; }
    do_something(connfd);
    close(connfd);
  }
  
  printf("%i\n", fd);
  return 0;
}
