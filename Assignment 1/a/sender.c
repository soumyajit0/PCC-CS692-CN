#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define MAXSIZE 50

struct message_struct
{
  long int message_type;
  char message_body[MAXSIZE];
};

void
toBin (int n, char *bin)
{
  int i = 0;
  while (n > 0)
    {
      bin[i++] = (char) (48 + n % 2);
      n /= 2;
    }
  int left = 0, right = i - 1;
  while (left < right)
    {
      char temp = bin[left];
      bin[left] = bin[right];
      bin[right] = temp;
      left++;
      right--;
    }
}

void
toOct (int n, char *oct)
{
  //sprintf(oct,"%o",n);
  int i = 0;
  while (n > 0)
    {
      oct[i++] = (char) (48 + n % 8);
      n /= 8;
    }
  int left = 0, right = i - 1;
  while (left < right)
    {
      char temp = oct[left];
      oct[left] = oct[right];
      oct[right] = temp;
      left++;
      right--;
    }
}

void
toHex (int n, char *hex)
{
  //sprintf(hex,"%x",n);
  int i = 0;
  while (n > 0)
    {
      if (n % 16 < 10)
	hex[i++] = (char) (48 + n % 16);
      else
	hex[i++] = (char) (55 + n % 16);
      n /= 16;
    }
  int left = 0, right = i - 1;
  while (left < right)
    {
      char temp = hex[left];
      hex[left] = hex[right];
      hex[right] = temp;
      left++;
      right--;
    }
}

int
main ()
{
  int running = 1;

  int msgid = msgget ((key_t) 2832, 0666 | IPC_CREAT);

  char num[MAXSIZE], bin[MAXSIZE], oct[MAXSIZE], hex[MAXSIZE];
  int n;
  while (running == 1)
    {
      printf ("Enter a Decimal Number : ");
      scanf ("%d", &n);
      if (n == -1)
	{
	  running = 0;

	  struct message_struct message1;
	  message1.message_type = 1;
	  strcpy (message1.message_body, "end");
	  msgsnd (msgid, (void *) &message1, sizeof (message1.message_body),
		  0);

	  struct message_struct message2;
	  message2.message_type = 2;
	  strcpy (message2.message_body, "end");
	  msgsnd (msgid, (void *) &message2, sizeof (message2.message_body),
		  0);

	  struct message_struct message3;
	  message3.message_type = 3;
	  strcpy (message3.message_body, "end");
	  msgsnd (msgid, (void *) &message3, sizeof (message3.message_body),
		  0);

	  printf ("\nSender Process Terminated\n");
	}
      else
	{
	  toBin (n, bin);
	  struct message_struct message1;
	  message1.message_type = 1;
	  strcpy (message1.message_body, "Binary : ");
	  strcat (message1.message_body, bin);
	  msgsnd (msgid, (void *) &message1, sizeof (message1.message_body),
		  0);

	  toOct (n, oct);
	  struct message_struct message2;
	  message2.message_type = 2;
	  strcpy (message2.message_body, "Octal : ");
	  strcat (message2.message_body, oct);
	  msgsnd (msgid, (void *) &message2, sizeof (message2.message_body),
		  0);

	  toHex (n, hex);
	  struct message_struct message3;
	  message3.message_type = 3;
	  strcpy (message3.message_body, "Hexadecimal : ");
	  strcat (message3.message_body, hex);
	  msgsnd (msgid, (void *) &message3, sizeof (message3.message_body),
		  0);
	}
    }

  msgctl (msgid, IPC_RMID, 0);

  return 0;
}
