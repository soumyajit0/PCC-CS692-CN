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
      char name[MAX];
      printf ("Enter a name : ");
      scanf ("%s", name);

      struct message_struct messageName;
      messageName.message_id = 1;
      strcpy (messageName.message_body, name);

      msgsnd (msgid, (void *) &messageName, sizeof (messageName.message_body),
	      0);

      char roll[MAX];
      printf ("Enter a Roll Number : ");
      scanf ("%s", roll);

      struct message_struct messageRoll;
      messageRoll.message_id = 2;
      strcpy (messageRoll.message_body, roll);

      msgsnd (msgid, (void *) &messageRoll, sizeof (messageRoll.message_body),
	      0);

      if (strcmp (name, "end") == 0 && strcmp (roll, "end") == 0)
	{
	  running = 0;
	  break;
	}
      i++;
    }

  msgctl (msgid, IPC_RMID, 0);

  char nameSorted[MAX][MAX];
  char rollSorted[MAX][MAX];

  running = 1;
  msgid = msgget ((key_t) 28321, 0666 | IPC_CREAT);
  if (msgid == -1)
    {
      perror ("msgget failed");
      exit (EXIT_FAILURE);
    }
  i = 0;
  while (running == 1)
    {
      struct message_struct messageName;
      msgrcv (msgid, (void *) &messageName, sizeof (messageName.message_body),
	      3, 0);
      if (strcmp (messageName.message_body, "end") == 0)
	{
	  running = 0;
	  break;
	}
      strcpy (nameSorted[i++], messageName.message_body);
    }

  msgctl (msgid, IPC_RMID, 0);

  running = 1;
  char roll[MAX][MAX];
  msgid = msgget ((key_t) 28322, 0666 | IPC_CREAT);
  if (msgid == -1)
    {
      perror ("msgget failed");
      exit (EXIT_FAILURE);
    }
  i = 0;
  while (running == 1)
    {
      struct message_struct messageRoll;
      msgrcv (msgid, (void *) &messageRoll, sizeof (messageRoll.message_body),
	      4, 0);
      if (strcmp (messageRoll.message_body, "end") == 0)
	{
	  running = 0;
	  break;
	}
      strcpy (rollSorted[i++], messageRoll.message_body);
    }

  msgctl (msgid, IPC_RMID, 0);


  printf ("\nSorted Names\tSorted Roll\n");
  int j;
  for (j = 0; j < i; j++)
    printf ("%s\t%s\n", nameSorted[j], rollSorted[j]);
  printf ("\nProcess 1 is terminated...\n");

  return 0;
}
