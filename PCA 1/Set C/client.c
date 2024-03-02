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
print (char *stream)
{
  printf ("The processed Bit Stream received from Process 2 is : ");
  int i;
  for (i = 0; i < strlen (stream) + 1; i++)
    {
      if (stream[i] == '2')
	printf ("%s", "01");
      else
	printf ("%c", stream[i]);
    }
  printf ("\n\n");
}

int
main ()
{
  key_t key = ftok ("msg_key", 2832);
  int msgid = msgget (key, 0666 | IPC_CREAT);
  printf
    ("To stop enter 'end' in-place of Bit Stream input. Also, 2 Bytes Bit Stream means 16 bits input.\n\n");

  while (1)
    {
      struct message_struct message;
      char stream[16];
      printf ("Enter a 2 Bytes Bit Stream : ");
      scanf ("%s", stream);
      getchar ();

      message.message_id = 1;
      strcpy (message.message_body, stream);
      if (strcmp (stream, "end") == 0)
	{
	  msgsnd (msgid, (void *) &message, sizeof (message.message_body) + 1,
		  0);
	  printf ("Process 1 is terminated...\n");
	  break;
	}

      msgsnd (msgid, (void *) &message, sizeof (message.message_body), 0);
      printf ("Process 1 sent %s Bit Stream to Process 2\n", stream);

      struct message_struct output;
      msgrcv (msgid, (void *) &output, sizeof (output.message_body) + 1, 2,
	      0);
      strcpy (stream, output.message_body);
      print (stream);
    }

  msgctl (msgid, IPC_RMID, 0);
  return 0;
}
