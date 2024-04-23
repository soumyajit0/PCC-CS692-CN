#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<unistd.h>
#include<fcntl.h>
#include<sys/un.h>
#include<sys/stat.h>

#define MAX 100

int
main ()
{
  struct sockaddr_un client_address, server_address;
  unlink ("socket_server");
  int server_sockfd = socket (AF_UNIX, SOCK_STREAM, 0);
  server_address.sun_family = AF_UNIX;
  strcpy (server_address.sun_path, "socket_server");
  int server_len = sizeof (server_address);
  bind (server_sockfd, (struct sockaddr *) &server_address, server_len);
  listen (server_sockfd, 5);
  printf ("Server is running...\n\n");

  int client_len = sizeof (client_address);
  int client_sockfd =
    accept (server_sockfd, (struct sockaddr *) &client_address, &client_len);

  while (1)
    {
      char bit[MAX];
      read (client_sockfd, bit, MAX);
      if (strcmp (bit, "end") == 0)
	{
	  printf ("Server is terminated...\n");
	  break;
	}
      printf ("Server received %s from Client\n", bit);
      int i, count = 0;
      for (i = 0; i < strlen (bit); i++)
	{
	  if (bit[i] == '1')
	    count++;
	}
      if (count % 2 == 1)
	bit[i++] = '1';
      else
	bit[i++] = '0';
      bit[i] = '\0';
      write (client_sockfd, bit, strlen (bit) + 1);
      printf ("Server sent back %s to the Client\n\n", bit);
    }
  close (server_sockfd);
  return 0;
}
