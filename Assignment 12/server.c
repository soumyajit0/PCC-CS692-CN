#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#define SIZE 512
#define TIMEOUT 4
#define error(msg) {perror(msg);printf("\n");exit(1);}

int sockfd;
typedef struct
{
  unsigned int seq_num;
  char data[SIZE];
} Frame;

void *
receiver (void *arg)
{
  int seq_no = 0, ack_no = 1;
  int newfd = *(int *) arg;
  Frame frame;
  srand (time (NULL));

  while (seq_no < 5)
    {
      recv (newfd, &frame, sizeof (frame), 0);
      int seq_recv = frame.seq_num;
      if (seq_recv != seq_no)
	{
	  printf ("\nDuplicate frame %d received..\n", seq_recv);
	  int t = seq_recv + 1;
	  send (newfd, &t, sizeof (t), 0);
	}
      else
	{
	  int delay = rand () % 7 + 1;
	  printf ("\nFrame %d Received.. Will acknowledge after %d seconds\n",
		  seq_no, delay);
	  sleep (delay);
	  printf ("Sending acknowledgement for %d\n", ack_no - 1);
	  send (newfd, &ack_no, sizeof (int), 0);
	  seq_no++;
	  ack_no++;
	}


    }

}

void
main (int argc, char **argv)
{
  if (argc < 2)
    {
      printf ("Provide port!\n");
      exit (1);
    }

  struct sockaddr_in server = {
    .sin_family = AF_INET,
    .sin_port = htons (atoi (argv[1])),
    .sin_addr.s_addr = INADDR_ANY,
  };

  struct sockaddr_in client;
  int sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error ("socket creation failed");

  int reuse = 1;
  int res =
    setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (int));
  if (res < 0)
    error ("Setsockopt Failed!");

  res = bind (sockfd, (struct sockaddr *) &server, sizeof (server));
  if (res < 0)
    error ("Failed to bind!");
  listen (sockfd, 2);
  printf ("\nListening for connections..\n");
  socklen_t len = sizeof (client);
  int newfd = accept (sockfd, (struct sockaddr *) &client, &len);
  printf ("Connection accepted..\n");
  pthread_t recv_th;

  int seq_no = 0, ack_no = 0;
  pthread_create (&recv_th, NULL, receiver, &newfd);

  pthread_exit (NULL);
}
