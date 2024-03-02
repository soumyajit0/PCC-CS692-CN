#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct message_struct
{
  long int message_id;
  char message_body[16];
};

void
process (char *stream)
{
  int i, count = 0;
  for (i = 0; i < strlen (stream); i++)
    {
      if (stream[i] == '0')
	{
	  if (count == 4)
	    {
	      stream[i] = '2';
	      count = 0;
	    }
	  else
	    count++;
	}
      else
	count = 0;
    }
}

int
main ()
{
  key_t key = ftok ("msg_key", 2832);
  int msgid = msgget (key, 0666 | IPC_CREAT);

  while (1)
    {
      printf ("Waiting for Client...\n");
      struct message_struct message;
      char stream[16];
      msgrcv (msgid, (void *) &message, sizeof (message.message_body) + 1, 1,
	      0);
      strcpy (stream, message.message_body);

      if (strcmp (stream, "end") == 0)
	{
	  printf ("Process 2 is terminated...\n");
	  break;
	}

      printf ("Process 2 received %s Bit Stream from Process 1\n", stream);

      process (stream);

      struct message_struct output;
      output.message_id = 2;
      strcpy (output.message_body, stream);
      msgsnd (msgid, (void *) &output, sizeof (output.message_body) + 1, 1);
      printf ("Processed Stream is sent back to Process 1\n\n");

    }

  return 0;
}
