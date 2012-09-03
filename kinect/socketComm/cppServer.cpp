#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
 
int main() {
  int         server_sockfd;
  int         server_len;
  sockaddr_un server_address;
 
  char buf[1024];
  int recvd = 0;
 
  unlink("/tmp/python_unix_sockets_example");
  server_sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
 
  server_address.sun_family = AF_UNIX;
  strcpy(server_address.sun_path, "/tmp/python_unix_sockets_example");
  server_len = sizeof(server_address);
  bind(server_sockfd, (sockaddr*)&server_address, server_len);
 
  listen(server_sockfd, 5);
 
  while(true) {
    recvd = read(server_sockfd, buf, sizeof(buf));
    if( recvd < 0) {
      printf("error reading socket");
      break;
    }
 
    buf[recvd] = *"\0";
    printf("%s\n", buf);
  }
  close(server_sockfd);
 
}
