#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>
#include<sys/socket.h>
#include<unistd.h>

#define MAX 100

struct sock
{
  int sockf;
  struct sockaddr_in so;
};

int flag = 1;

void *
snd (void *arg)
{
  struct sock t = *(struct sock *) arg;
  int sid = t.sockf;
  struct sockaddr_in saddr = t.so;
  char message[MAX];
  while (1)
    {
      scanf ("%s", message);
      getchar ();
      flag = 0;
      sendto (sid, (void *) &message, strlen (message) + 1, 0,
	      (struct sockaddr *) &saddr, sizeof (saddr));
    }
  pthread_exit (NULL);
}

void *
rcv (void *arg)
{
  struct sock t = *(struct sock *) arg;
  int sid = t.sockf;
  struct sockaddr_in saddr = t.so;
  char message[MAX];
  int len = sizeof (saddr);
  while (1)
    {
      recvfrom (sid, (void *) &message, MAX, 0, (struct sockaddr *) &saddr,
		&len);
      if (flag == 1)
	printf ("Received : %s\n", message);
      flag = 1;
    }
  pthread_exit (NULL);
}

int
main (int ac, char **av)
{
  struct sockaddr_in multi;
  char sip_addr[MAX];
  if (ac == 1)
    strcpy (sip_addr, "224.0.0.1");
  else
    strcpy (sip_addr, av[1]);
  int sid = socket (AF_INET, SOCK_DGRAM, 0);
  multi.sin_family = AF_INET;
  multi.sin_addr.s_addr = inet_addr (sip_addr);
  multi.sin_port = htons (1234);
  int reuse = 1;
  if (setsockopt (sid, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse)))
    {
      perror ("Error setting up socket\n");
      close (sid);
      exit (1);
    }
  int res = bind (sid, (struct sockaddr *) &multi, sizeof (multi));
  if (res == -1)
    {
      perror ("Error binding socket\n");
      close (sid);
      exit (1);
    }
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr (sip_addr);
  mreq.imr_interface.s_addr = INADDR_ANY;
  res = setsockopt (sid, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof (mreq));
  if (res == -1)
    {
      perror ("Cannot socket add to group\n");
      close (sid);
      exit (1);
    }
  struct sock t;
  t.sockf = sid;
  t.so = multi;
  pthread_t t1, t2;
  pthread_create (&t1, NULL, snd, &t);
  pthread_create (&t2, NULL, rcv, &t);
  pthread_join (t1, NULL);
  pthread_join (t2, NULL);
  close (sid);
  return 0;
}
