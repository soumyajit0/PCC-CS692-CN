#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct message_struct
{
  int long message_id;
  char message_body[32];
};

int
main ()
{
  key_t key = ftok ("msgkey", 2832);
  int msgid = msgget (key, 0666 | IPC_CREAT);

  printf ("To stop enter 'end' in-place of Roll No.\n\n");

  while (1)
    {
      char roll[32];
      printf ("Enter your Roll No. : ");
      scanf ("%s", roll);
      getchar ();

      struct message_struct message;
      message.message_id = 1;

      if (strcmp (roll, "end") == 0)
	{
	  strcpy (message.message_body, roll);
	  msgsnd (msgid, (void *) &message, sizeof (message.message_body) + 1,
		  0);
	  printf ("Process 1 is terminated...\n");
	  break;
	}

      strcpy (message.message_body, roll);
      msgsnd (msgid, (void *) &message, sizeof (message.message_body), 0);
      printf ("Roll No. : %s is sent to process 2...\n\n", roll);
    }

  msgctl (msgid, IPC_RMID, 0);
  return 0;
}
