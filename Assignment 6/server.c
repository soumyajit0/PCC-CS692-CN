#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<netinet/in.h>
#include<arpa/inet.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<unistd.h>

#include<signal.h>

#define MAX 100

void
customHandler (int signum)
{
  printf ("Server terminated...\n");
  exit (0);
}

int
main (int ac, char **av)
{
  struct sockaddr_in saddr, caddr;
  struct in_addr ipv4;
  char sip_addr[MAX], ip[MAX], ans[MAX];
  if (ac == 1)
    strcpy (sip_addr, "127.0.0.1");
  else
    strcpy (sip_addr, av[1]);
  int sid = socket (AF_INET, SOCK_STREAM, 0);
  saddr.sin_family = AF_INET;
  inet_aton (sip_addr, &(saddr.sin_addr));
  saddr.sin_port = htons (1234);
  int res = bind (sid, (struct sockaddr *) &saddr, sizeof (saddr));
  if (res == -1)
    {
      printf ("Cannot start the server...\n");
      close (sid);
      exit (0);
    }
  listen (sid, 5);
  signal (SIGINT, customHandler);
  while (1)
    {
      int len = sizeof (caddr);
      int cid = accept (sid, (struct sockaddr *) &caddr, &len);
      if (fork () == 0)
	{
	  read (cid, (void *) &ip, MAX);
	  if (strcmp (ip, "end") == 0)
	    {
	      kill (getppid (), SIGINT);
	      close (cid);
	      break;
	    }
	  printf ("Server received %s from the client\n", ip);
	  if (inet_pton (AF_INET, ip, &ipv4) == 1)
	    {
	      printf ("Valid IPv4 address\n\n");
	      strcpy (ans, "YES");
	    }
	  else
	    {
	      printf ("Not a valid IPv4 address\n\n");
	      strcpy (ans, "NO");
	    }
	  write (cid, (void *) &ans, strlen (ans) + 1);
	  close (cid);
	}
      else
	{
	  close (cid);
	}
    }
  close (sid);
  return 0;
}
