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
  char sip_addr[MAX], data[MAX], code[MAX];
  if (ac == 1)
    strcpy (sip_addr, "127.0.0.1");
  else
    strcpy (sip_addr, av[1]);
  int sid = socket (AF_INET, SOCK_DGRAM, 0);
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr (sip_addr);
  saddr.sin_port = htons (1234);
  while (1)
    {
      printf ("Enter the Dataword :");
      scanf ("%s", data);
      getchar ();
      sendto (sid, (void *) &data, strlen (data) + 1, 0,
	      (struct sockaddr *) &saddr, sizeof (saddr));
      if (strcmp (data, "end") == 0)
	{
	  printf ("Client terminated...\n");
	  close (sid);
	  exit (0);
	}
      printf ("Client sent Dataword %s to the server\n", data);
      int len = sizeof (saddr);
      recvfrom (sid, (void *) &code, MAX, 0, (struct sockaddr *) &saddr,
		&len);
      printf ("Client received Codeword %s from the server\n\n", code);
    }
  return 0;
}
