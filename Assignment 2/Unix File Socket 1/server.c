#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>

#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/un.h>

#define MAX 100

int
main ()
{
  struct sockaddr_un client_address;
  struct sockaddr_un server_address;

  unlink ("socket_server");

  int client_sockfd, server_sockfd;

  server_sockfd = socket (AF_UNIX, SOCK_STREAM, 0);
  server_address.sun_family = AF_UNIX;
  strcpy (server_address.sun_path, "socket_server");

  int server_len, client_len;

  server_len = sizeof (server_address);
  bind (server_sockfd, (struct sockaddr *) &server_address, server_len);

  listen (server_sockfd, 5);
  printf ("Server is running...\n");

  client_len = sizeof (client_address);
  client_sockfd =
    accept (server_sockfd, (struct sockaddr *) &client_address, &client_len);

  while (1)
    {
      printf ("\nServer is waiting...\n");
      //sleep (2);

      char bit[MAX];
      read (client_sockfd, bit, MAX);

      if (strcmp (bit, "end") == 0)
	{
	  printf ("\nServer is terminated...\n");
	  close (server_sockfd);
	  exit (1);
	}

      printf ("\nServer receieved %s from the client...\n", bit);
      int len = strlen (bit);
      int i, n = 0;
      for (i = 0; i < len; i++)
	{
	  if (bit[i] == '1')
	    n++;
	}
      if (n % 2 == 1)
	bit[i++] = '1';
      else
	bit[i++] = '0';
      bit[i] = '\0';

      printf ("Server is sending %s to Client...\n", bit);
      write (client_sockfd, bit, strlen (bit) + 1);
    }

  return 0;
}
