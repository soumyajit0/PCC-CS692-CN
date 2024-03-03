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
main ()
{
  struct sockaddr_un address;
  int sockfd = socket (AF_UNIX, SOCK_STREAM, 0);
  address.sun_family = AF_UNIX;
  strcpy (address.sun_path, "socket_server");
  int len = sizeof (address);
  int result = connect (sockfd, (struct sockaddr *) &address, len);
  if (result == -1)
    {
      printf ("Cannot connect to Server...\n");
      exit (1);
    }
  printf ("Connected to Server...\n\n");
  while (1)
    {
      char ip[MAX];
      printf ("Enter an IPv4 Address : ");
      scanf ("%s", ip);
      getchar ();
      write (sockfd, ip, strlen (ip) + 1);
      if (strcmp (ip, "end") == 0)
	{
	  printf ("Client is terminated...\n");
	  break;
	}
      printf ("Client sent IPv4 : %s to the Server\n", ip);
      char valid[MAX];
      read (sockfd, valid, MAX);
      if (strcmp (valid, "1") == 0)
	printf ("IPv4 : %s is VALID\n\n", ip);
      else
	printf ("IPv4 : %s is INVALID\n\n", ip);
    }
  close (sockfd);
  return 0;
}
