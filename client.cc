#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) { perror("socket"); return 1; };

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
  int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv) { perror("connect"); return 1; }

  char msg[] = "Hello";
  write(fd, msg, strlen(msg));

  char rbuf[64] = {};
  ssize_t n = read(fd, rbuf, sizeof(rbuf)-1);
  if(n < 0) { perror("read"); return 1; }
  printf("server says: %s\n", rbuf);
  close(fd);
}
