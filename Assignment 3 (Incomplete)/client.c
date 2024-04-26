#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/un.h>
#include<fcntl.h>

#define MAX 100

struct input_struct
{
  char dataword[MAX];
  char divisor[MAX];
  char codeword[MAX];
};

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
      struct input_struct input;
      printf ("Enter the Data Word : ");
      scanf ("%s", input.dataword);
      getchar ();

      if (strcmp (input.dataword, "end") == 0)
	{
	  strcpy (input.divisor, "end");
	  write (sockfd, (void *) &input, sizeof (input));
	  printf ("\nClient is terminated...\n");
	  close (sockfd);
	  exit (1);
	}

      printf ("Enter the Divisor : ");
      scanf ("%s", input.divisor);
      getchar ();
      printf ("Sending Data Word : %s and Divisor : %s to the Server...\n",
	      input.dataword, input.divisor);
      write (sockfd, (void *) &input, sizeof (input));

      printf ("\nWaiting for Server response...\n");

      struct input_struct output;
      read (sockfd, (void *) &output, sizeof (output));

      printf ("\nCode Word received from the Server : %s\n\n",
	      output.codeword);
    }

  return 0;
}
