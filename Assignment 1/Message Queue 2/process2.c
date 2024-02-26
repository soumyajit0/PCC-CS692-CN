#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define MAX 100

struct message_struct
{
  long int message_id;
  char message_body[MAX];
};

char name[MAX][MAX];

void
sort (int arrSize)
{
  int i, j;
  for (i = 0; i < arrSize - 1; i++)
    {
      for (j = i + 1; j < arrSize; j++)
	{
	  if (strcmp (name[j], name[i]) < 0)
	    {
	      char temp[MAX];
	      strcpy (temp, name[i]);
	      strcpy (name[i], name[j]);
	      strcpy (name[j], temp);
	    }
	}
    }
}

int
main ()
{
  int running = 1;
  int msgid = msgget ((key_t) 2832, 0666 | IPC_CREAT);
  if (msgid == -1)
    {
      perror ("msgget failed");
      exit (EXIT_FAILURE);
    }
  int i = 0;
  while (running == 1)
    {
      struct message_struct message;
      msgrcv (msgid, (void *) &message, sizeof (message.message_body), 1, 0);
      if (strcmp (message.message_body, "end") == 0)
	{
	  running = 0;
	  break;
	}
      strcpy (name[i++], message.message_body);
      printf ("Name received : %s\n", message.message_body);
    }

  sort (i);
  printf ("\nNames Sorted\n");

  msgid = msgget ((key_t) 28321, 0666 | IPC_CREAT);
  if (msgid == -1)
    {
      perror ("msgget failed");
      exit (EXIT_FAILURE);
    }

  int j;
  for (j = 0; j < i; j++)
    {
      struct message_struct message;
      strcpy (message.message_body, name[j]);
      message.message_id = 3;
      msgsnd (msgid, (void *) &message, sizeof (message.message_body), 0);
    }
  struct message_struct message;
  strcpy (message.message_body, "end");
  message.message_id = 3;
  msgsnd (msgid, (void *) &message, sizeof (message.message_body), 0);
  printf ("\nProcess 2 is Terminated...\n");

  return 0;
}