#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<netinet/in.h>
#include<arpa/inet.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<unistd.h>

#define MAX 100

char *
hammingCode (char *data)
{
  int m = strlen (data);
  int i, j, r = 1, k = 0, count = 0, shift = 0;
  for (i = 0; i < m / 2; i++)
    {
      char temp = data[i];
      data[i] = data[m - 1 - i];
      data[m - 1 - i] = temp;
    }

  while ((1 << r < (m + r + 1)))
    {
      r++;
    }

  char *code = (char *) malloc (sizeof (char) * (m + r + 1));
  code[m + r] = '\0';

  for (i = 0; i < m + r; i++)
    {
      if ((i + 1) & i)
	code[i] = data[k++];
      else
	code[i] = 'r';
    }

  for (i = 0; i < r; i++)
    {
      count = 0;
      shift = 1 << i;
      for (j = 1; j <= (m + r); j++)
	{
	  if (j & shift)
	    {
	      if (code[j - 1] == '1')
		count++;
	    }
	}
      code[shift - 1] = '0' + (count & 1);
    }
  int Hm = strlen (code);
  for (i = 0; i < Hm / 2; i++)
    {
      char temp = code[i];
      code[i] = code[Hm - 1 - i];
      code[Hm - 1 - i] = temp;
    }

  return code;
}

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
      strcpy (code, hammingCode (code));
      sendto (sid, (void *) &code, strlen (code), 0,
	      (struct sockaddr *) &caddr, sizeof (caddr));
      printf ("Codeword %s returned to the client\n\n", code);
    }
  return 0;
}
