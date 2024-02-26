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

char rollnum[MAX][MAX];

void
sort (int arrSize)
{
  int i, j;
  for (i = 0; i < arrSize - 1; i++)
    {
      for (j = i + 1; j < arrSize; j++)
	{
	  if (strcmp (rollnum[j], rollnum[i]) < 0)
	    {
	      char temp[MAX];
	      strcpy (temp, rollnum[i]);
	      strcpy (rollnum[i], rollnum[j]);
	      strcpy (rollnum[j], temp);
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
      msgrcv (msgid, (void *) &message, sizeof (message.message_body), 2, 0);
      if (strcmp (message.message_body, "end") == 0)
	{
	  running = 0;
	  break;
	}
      strcpy (rollnum[i++], message.message_body);
      printf ("Roll Number received : %s\n", message.message_body);
    }
  sort (i);
  printf ("\nRoll Numbers Sorted\n");

  msgid = msgget ((key_t) 28322, 0666 | IPC_CREAT);
  if (msgid == -1)
    {
      perror ("msgget failed");
      exit (EXIT_FAILURE);
    }

  int j;
  for (j = 0; j < i; j++)
    {
      struct message_struct message;
      strcpy (message.message_body, rollnum[j]);
      message.message_id = 4;
      msgsnd (msgid, (void *) &message, sizeof (message.message_body), 0);
    }
  struct message_struct message;
  strcpy (message.message_body, "end");
  message.message_id = 4;
  msgsnd (msgid, (void *) &message, sizeof (message.message_body), 0);
  printf ("\nProcess 3 is Terminated...\n");

  return 0;
}