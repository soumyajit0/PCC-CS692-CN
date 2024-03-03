#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/un.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>

#define MAX 100

int
isValid (char *ip)
{
  int sum = 0, count = 0, i;
  for (i = 0; i < strlen (ip); i++)
    {
      if (ip[i] == '.')
	{
	  if (sum > 255 || count >= 3 || (i != 0 && ip[i - 1] == '.'))
	    return 0;
	  count++;
	  sum = 0;
	}
      else
	{
	  sum = sum * 10 + ip[i] - 48;
	}
    }
  if (count != 3)
    return 0;
  return 1;
}

int
main ()
{
  struct sockaddr_un client_address, server_address;
  int server_sockfd = socket (AF_UNIX, SOCK_STREAM, 0);
  server_address.sun_family = AF_UNIX;
  strcpy (server_address.sun_path, "socket_server");
  int server_len = sizeof (server_address);
  bind (server_sockfd, (struct sockaddr *) &server_address, server_len);
  listen (server_sockfd, 5);
  printf ("Server is waiting...\n\n");
  int client_len = sizeof (client_address);
  int client_sockfd =
    accept (server_sockfd, (struct sockaddr *) &client_address, &client_len);
  while (1)
    {
      char ip[MAX];
      read (client_sockfd, ip, MAX);
      if (strcmp (ip, "end") == 0)
	{
	  printf ("Server is terminated...\n");
	  break;
	}
      printf ("Server received IPv4 : %s from the Client\n", ip);
      char valid[MAX];
      if (isValid (ip) == 1)
	strcpy (valid, "1");
      else
	strcpy (valid, "0");
      write (client_sockfd, valid, strlen (valid) + 1);
      printf
	("Server analyzed the IP and sent its judgement to the Client\n\n");
    }
  close (server_sockfd);
  return 0;
}
