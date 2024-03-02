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

  while (1)
    {
      printf ("Waiting for Process 1...\n");

      struct message_struct message;
      msgrcv (msgid, (void *) &message, sizeof (message.message_body) + 1, 1,
	      0);

      char roll[32];
      strcpy (roll, message.message_body);
      if (strcmp (roll, "end") == 0)
	{
	  printf ("Process 2 is terminated...\n");
	  break;
	}

      printf ("Roll No. received : %s\n", roll);
      printf ("Reversed Roll No. : ");
      int i;
      for (i = strlen (roll) - 1; i >= 0; i--)
	printf ("%c", roll[i]);
      printf ("\n\n");
    }

  return 0;
}
