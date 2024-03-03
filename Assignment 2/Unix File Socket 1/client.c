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
  struct sockaddr_un address;
  int sockfd = socket (AF_UNIX, SOCK_STREAM, 0);
  address.sun_family = AF_UNIX;
  strcpy (address.sun_path, "socket_server");
  int len = sizeof (address);
  int result = connect (sockfd, (struct sockaddr *) &address, len);
  if (result == -1)
    {
      printf ("Cannot connect to the Server...\n");
      exit (1);
    }
  while (1)
    {
      char bit[MAX];
      printf ("Enter a Bit-Stream : ");
      scanf ("%s", bit);
      getchar ();

      write (sockfd, bit, strlen (bit) + 1);

      if (strcmp (bit, "end") == 0)
	{
	  printf ("Client is terminated...\n\n");
	  break;
	}
      printf ("%s is sent to the server\n", bit);
      read (sockfd, bit, MAX);
      printf ("%s is received from the Server\n\n", bit);
    }
  close (sockfd);
  return 0;
}
