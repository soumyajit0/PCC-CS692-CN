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
  struct sockaddr_in saddr;
  char sip_addr[MAX], ip[MAX], ans[MAX];
  if (ac == 1)
    strcpy (sip_addr, "127.0.0.1");
  else
    strcpy (sip_addr, av[1]);
  int sid = socket (AF_INET, SOCK_STREAM, 0);
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr (sip_addr);
  saddr.sin_port = htons (1234);
  int res = connect (sid, (struct sockaddr *) &saddr, sizeof (saddr));
  if (res == -1)
    {
      printf ("Cannot connect to the server...\n");
      close (sid);
      exit (1);
    }
  printf ("Enter the IPv4 for process %d : ", getpid ());
  scanf ("%s", ip);
  getchar ();
  write (sid, (void *) &ip, strlen (ip) + 1);
  if (strcmp (ip, "end") == 0)
    {
      printf ("Client terminated...\n");
      exit (1);
    }
  printf ("Client sent %s to the server\n", ip);
  read (sid, (void *) &ans, MAX);
  printf ("Server response : %s\n\n", ans);
  close (sid);
  return 0;
}
