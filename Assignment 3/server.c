#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/un.h>
#include<fcntl.h>
#include<math.h>

#define MAX 100

struct input_struct
{
  char dataword[MAX];
  char divisor[MAX];
  char codeword[MAX];
  char remainder[MAX];
};

char code[MAX], rem[MAX];

void
toBin (long long int dec, char *bin)
{
  int i = 0;
  while (dec > 0)
    {
      if (dec % 2 == 1)
	{
	  bin[i++] = '1';
	}
      else
	{
	  bin[i++] = '0';
	}
      dec /= 2;
    }
  i = 0;
  int j = strlen (bin)-1;
  while (i < j)
    {
      char temp = bin[i];
      bin[i] = bin[j];
      bin[j] = temp;
      i++;
      j--;
    }
  bin[strlen (bin)] = '\0';
}

long long int
toDec (char *bin)
{
  long long int dec = 0;
  int len = strlen (bin);
  int i;
  for (i = 0; i < len; i++)
    {
      dec += ((bin[i] - '0') * pow (2, len - (i + 1)));
    }
  return dec;
}

void
CRC (char *dataword, char *divisor)
{
  int divLen = strlen (divisor);
  long long int divisorDec = toDec (divisor);
  long long int datawordDec = toDec (dataword);
  long long int dividendDec = (datawordDec << (divLen - 1));
  int shft = (int) (ceil (log2 (dividendDec + 1)) - divLen);
  long long int remDec;
  while ((dividendDec >= divisorDec) || (shft >= 0))
    {
      remDec = (dividendDec >> shft) ^ divisorDec;
      dividendDec = (dividendDec & ((1ll << shft) - 1)) | (remDec << shft);
      shft = (int) (ceil (log2 (dividendDec + 1)) - divLen);
    }
  long long int codewordDec = (datawordDec << (divLen - 1)) | dividendDec;
  char codewordBin[MAX];
  toBin (codewordDec, codewordBin);
  strcpy (code, codewordBin);
  strcpy (rem, codewordBin + strlen (dataword));
  printf ("Codeword : %s\n", code);
  printf ("Remainder : %s\n", rem);
}

int
main ()
{
  unlink ("socket_server");

  struct sockaddr_un client_address, server_address;
  int client_sockfd, server_sockfd;
  server_sockfd = socket (AF_UNIX, SOCK_STREAM, 0);
  server_address.sun_family = AF_UNIX;
  strcpy (server_address.sun_path, "socket_server");
  int client_len, server_len;
  server_len = sizeof (server_address);
  bind (server_sockfd, (struct sockaddr *) &server_address, server_len);
  listen (server_sockfd, 5);
  printf ("Server is running...\n");

  client_len = sizeof (client_address);
  client_sockfd =
    accept (server_sockfd, (struct sockaddr *) &client_address, &client_len);

  while (1)
    {
      printf ("\nServer is waiting\n");
      struct input_struct input;
      read (client_sockfd, (void *) &input, sizeof (input));
      if (strcmp (input.dataword, "end") == 0)
	{
	  printf ("\nServer is terminated...\n");
	  close (server_sockfd);
	  exit (1);
	}
      printf ("Server received Data Word : %s\n", input.dataword);
      printf ("Server received Divisor : %s\n", input.divisor);

      CRC (input.dataword, input.divisor);
      struct input_struct output;
      strcpy (output.codeword, code);
      strcpy (output.remainder, rem);
      printf
	("\nCodeword : %s and Remainder : %s are sent back to the Client\n",
	 output.codeword, output.remainder);
      write (client_sockfd, (void *) &output, sizeof (output));
    }
  return 0;
}
