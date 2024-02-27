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
  struct sockaddr_un address;
  int sockfd = socket (AF_UNIX, SOCK_STREAM, 0);
  address.sun_family = AF_UNIX;
  strcpy (address.sun_path, "socket_server");
  int len = sizeof (address);

  printf ("Client is running...\n");

  int result = connect (sockfd, (struct sockaddr *) &address, len);
  if (result == -1)
    {
      printf ("\nCannot connect to Server\n");
      exit (1);
    }

  while (1)
    {
      char bit[MAX];
      printf ("Enter a Bit-Stream : ");
      scanf ("%s", bit);
      getchar ();

      if (strcmp (bit, "end") == 0)
	{
	  printf ("\nClient is terminated...\n");
	  write (sockfd, "end", strlen (bit));
	  close (sockfd);
	  exit (1);
	}

      printf ("\nClient is sending %s to the Server\n", bit);
      write (sockfd, bit, strlen (bit) + 1);

      printf ("\nWaiting for Server response\n");

      read (sockfd, bit, MAX);

      printf ("\nServer sent %s\n", bit);
    }

  return 0;
}
