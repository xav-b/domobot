#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
 
int main (int argc, char* const argv[])
{
  const char* const socket_name = argv[1];
  const char* const message = argv[2];
  int socket_fd;
  struct sockaddr_un name;
 
  /* Crée le socket. */
  socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
  //socket_fd = socket (PF_LOCAL, SOCK_DGRAM, 0);
  //name.sun_family = AF_LOCAL;
  name.sun_family = AF_UNIX;
  strcpy (name.sun_path, socket_name);

  connect (socket_fd, (struct sockaddr*) &name, SUN_LEN (&name));
  int length = strlen (message) + 1;
  write (socket_fd, message, length);
  while (true) {
      char buf[3];
      int recv = 0;
      recv = read(socket_fd, buf, sizeof(buf));
      if ( recv <= 1) {
          printf("** Stopping reading socket\n");
          break;
      }
      buf[recv] = '\0';
      printf("Recieved %s (%d)\n", buf, recv);
  }
  close (socket_fd);
  return 0;
}
