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
  char sip_addr[MAX], data[MAX], code[MAX];
  if (ac == 1)
    strcpy (sip_addr, "127.0.0.1");
  else
    strcpy (sip_addr, av[1]);
  int sid = socket (AF_INET, SOCK_DGRAM, 0);
  saddr.sin_family = AF_INET;
  inet_aton (sip_addr, &(saddr.sin_addr));
  saddr.sin_port = htons (1234);
  bind (sid, (struct sockaddr *) &saddr, sizeof (saddr));
  while (1)
    {
      int len = sizeof (caddr);
      recvfrom (sid, (void *) &data, MAX, 0, (struct sockaddr *) &caddr,
		&len);
      if (strcmp (data, "end") == 0)
	{
	  printf ("Server is terminated...\n");
	  close (sid);
	  exit (0);
	}
      printf ("Dataword %s received from the client\n", data);
      strcpy (code, data);
      sendto (sid, (void *) &code, strlen (code), 0,
	      (struct sockaddr *) &caddr, sizeof (caddr));
      printf ("Codeword %s returned to the client\n\n", code);
    }
  return 0;
}