#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#define SIZE 512
#define TIMEOUT 4
#define error(msg) {perror(msg);printf("\n");exit(1);}

typedef struct
{
  unsigned int seq_num;
  char data[SIZE];
} Frame;

void
main (int argc, char **argv)
{
  if (argc < 3)
    {
      printf ("Provide IP and port!\n");
      exit (1);
    }

  struct sockaddr_in server = {
    .sin_family = AF_INET,
    .sin_port = htons (atoi (argv[2])),
    .sin_addr.s_addr = inet_addr (argv[1]),
  };

  struct timeval tv = {
    .tv_sec = TIMEOUT,
    .tv_usec = 0,
  };

  int sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error ("socket creation failed");

  int res = setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv));
  if (res < 0)
    error ("Setsockopt Failed!");

  res = connect (sockfd, (struct sockaddr *) &server, sizeof (server));
  if (res < 0)
    error ("Failed to accept connection!");

  int seq_no, ack_no;

  Frame frame;

  char *data = "Hello this is some exaple frame data";
  seq_no = 0;
  int ack_recv = 0, bytesRead;
  while (seq_no < 5)
    {
      frame.seq_num = seq_no;
      strcpy (frame.data, data);
      send (sockfd, &frame, sizeof (frame), 0);
      printf ("\nTimer for frame no %d started...\n", seq_no);
      bytesRead = recv (sockfd, &ack_recv, sizeof (int), 0);
      if (bytesRead <= 0 || ack_recv != seq_no + 1)
	{
	  printf ("Failed to get acknowledgement for frame %d...\n", seq_no);
	  printf ("Initiating retransmission...");
	}
      else
	{
	  printf ("Acknowledgement for frame %d received..\n", seq_no++);
	}
    }

}
