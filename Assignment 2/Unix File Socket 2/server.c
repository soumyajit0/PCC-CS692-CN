#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/un.h>
#include<fcntl.h>

#define MAX 100

struct input_struct
{
  char dataword[MAX];
  char divisor[MAX];
  char codeword[MAX];
};

// Function to convert integer to binary string
char *
toBin (long long int num, int bits)
{
  char *bin_str = (char *) malloc ((bits + 1) * sizeof (char));	// Allocate memory for the binary string
  if (bin_str == NULL)
    {
      fprintf (stderr, "Memory allocation failed\n");
      exit (EXIT_FAILURE);
    }

  // Convert each bit of the integer to a binary character
  for (int i = bits - 1; i >= 0; i--)
    {
      bin_str[i] = (num & 1) + '0';	// Extract the least significant bit and convert it to a character
      num >>= 1;		// Right shift the number to get the next bit
    }
  bin_str[bits] = '\0';		// Null-terminate the string
  return bin_str;
}

// Function to convert binary string to decimal
long long int
toDec (char *bin)
{
  long long int num = 0;
  int length = strlen (bin);
  for (int i = 0; i < length; i++)
    {
      if (bin[i] == '1')
	num += 1LL << (length - i - 1);
    }
  return num;
}

// Function to compute CRC and codeword
char *
CRC (char *dataword, char *generator)
{
  int l_gen = strlen (generator);
  long long int gen = toDec (generator);

  long long int dword = toDec (dataword);

  // Initialize the remainder with the dataword
  long long int remainder = dword;

  // Perform CRC division
  for (int i = 0; i < strlen (dataword); i++)
    {
      if (remainder & (1LL << (strlen (dataword) - i - 1)))
	{
	  remainder ^= gen << (strlen (dataword) - l_gen - i);
	}
    }

  // Construct the codeword by appending the remainder to the dataword
  long long int codeword = (dword << (l_gen - 1)) | remainder;

  // Return the codeword as a binary string
  return toBin (codeword, strlen (dataword) + l_gen - 1);
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

      char *codeword = CRC (input.dataword, input.divisor);
      printf ("\nCodeword : %s is sent to the Client...\n", codeword);
      struct input_struct output;
      strcpy (output.codeword, codeword);
      write (client_sockfd, (void *) &output, sizeof (output));
    }
  return 0;
}
