#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<pthread.h>

#include<sys/socket.h>

#include<arpa/inet.h>
#include<netinet/in.h>

#include<unistd.h>

#define MAX 100

struct sockaddr_in caddr;
int sid;

void *
snd (void *args)
{
  char message[MAX];
  while (1)
    {
      scanf ("%s", message);
      getchar ();
      sendto (sid, (void *) &message, strlen (message) + 1, 0,
	      (struct sockaddr *) &caddr, sizeof (caddr));
      if (strcmp (message, "end") == 0)
	{
	  close (sid);
	  pthread_exit (NULL);
	}
    }
  pthread_exit (NULL);
}

void *
rcv (void *args)
{
  char message[MAX];
  int len = sizeof (caddr);
  while (1)
    {
      recvfrom (sid, (void *) &message, MAX, 0, (struct sockaddr *) &caddr,
		&len);
      if (strcmp (message, "end") == 0)
	{
	  close (sid);
	  pthread_exit (NULL);
	}
      printf ("Sender 1 : %s\n", message);
    }
  pthread_exit (NULL);
}

int
main ()
{
  pthread_t t1, t2;
  struct sockaddr_in saddr, daddr;
  sid = socket (AF_INET, SOCK_DGRAM, 0);
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr ("127.0.0.1");
  saddr.sin_port = htons (1234);

  daddr.sin_family = AF_INET;
  daddr.sin_addr.s_addr = inet_addr ("127.0.0.1");
  daddr.sin_port = htons (1234);

  bind (sid, (struct sockaddr *) &daddr, sizeof (daddr));

  caddr = daddr;

  pthread_create (&t1, NULL, snd, NULL);
  pthread_create (&t2, NULL, rcv, NULL);

  pthread_join (t1, NULL);
  pthread_join (t2, NULL);
  printf ("| Chat ends here |\n");
  return 0;
}
