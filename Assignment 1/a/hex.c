#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct message_struct
{
  long int message_type;
  char message_body[BUFSIZ];
};

int
main ()
{
  int running = 1;

  int msgid = msgget ((key_t) 2832, 0666 | IPC_CREAT);

  struct message_struct message;

  while (running == 1)
    {
      msgrcv (msgid, (void *) &message, BUFSIZ, 3, 0);
      if (strcmp (message.message_body, "end") == 0)
	{
	  running = 0;
	  printf ("\nProcess Terminated...\n");
	  continue;
	}
      printf ("%s\n", message.message_body);
    }

  return 0;
}
