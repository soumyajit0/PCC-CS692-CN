#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<netinet/in.h>
#include<arpa/inet.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<unistd.h>

#define MAX 100

int
main (int ac, char **av)
{
  struct sockaddr_in saddr, caddr;
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
      printf ("Cannot connect to the server...\n");
      close (sid);
      exit (1);
    }
  listen (sid, 5);
  int len = sizeof (caddr);
  int cid = accept (sid, (struct sockaddr *) &caddr, &len);
  while (1)
    {
      read (cid, (void *) &ip, MAX);
      if (strcmp (ip, "end") == 0)
	{
	  printf ("Server terminated...\n");
	  break;
	}
      printf ("Server received %s from the client\n", ip);
      int i, num = 0;
      for (i = 0; i < strlen (ip); i++)
	{
	  if (ip[i] == '.')
	    {
	      if (num >= 0 && num <= 255)
		num = 0;
	      else
		{
		  strcpy (ans, "NO");
		  write (cid, (void *) &ans, strlen (ans) + 1);
		  printf ("Not valid IPv4 address\n\n");
		  break;
		}
	    }
	  else if (ip[i] >= '0' && ip[i] <= '9')
	    {
	      num = (num * 10) + (ip[i] - 48);
	    }
	  else
	    {
	      strcpy (ans, "NO");
	      write (cid, (void *) &ans, strlen (ans) + 1);
	      printf ("Not valid IPv4 address\n\n");
	      break;
	    }
	}
      strcpy (ans, "YES");
      write (cid, (void *) &ans, strlen (ans) + 1);
      printf ("Valid IPv4 address\n\n");
    }
  close (sid);
  close (cid);
  return 0;
}
