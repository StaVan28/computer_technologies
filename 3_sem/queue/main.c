#include "queue.h"

//-----------------------------------------------

int main (int argc, const char *argv[])
{
    int num_proc = parsing_input (argc, argv);

    int id = create_msg ();

    struct my_msg* buf_pid = (struct my_msg*) calloc (num_proc, sizeof (struct my_msg));

    for (int i_proc = 0; i_proc < num_proc; i_proc++)
    {
		pid_t pid = fork ();

		if (pid > 0)
		{
            buf_pid[i_proc].type = i_proc;
			buf_pid[i_proc].pid  = pid;
		}
		else if (pid == 0)
		{
			// подключть к очереди
			fprintf (stderr, "I'm child! "
				             "My indx = %d, my pid = %d\n", i_proc + 1, getpid());

			exit (EXIT_SUCCESS);
		}
		else if (pid < 0)
		{
	        perror ("ERROR! fork ()");
	        exit   (EXIT_FAILURE);
		}
    }

    struct msg
    {
        long type;
        long number;
    };

    struct msg test = {1, 28};

    if (buf_pid[0].pid > 0)
    {
        if (msgsnd (id , &test, sizeof (long), 0))
        {
            perror ("ERROR! msgsnd ()");
            exit   (EXIT_FAILURE);
        }
    }
    else
    {
        if (msgrcv (id, &test, sizeof (long), 0, 0))
        {
            perror ("ERROR! msgsnd ()");
            exit   (EXIT_FAILURE);
        }

        printf ("I CCCHILD!\n");
    }


    for (int i = 0; i < num_proc; i++)
    {
    	printf ("i = %d, pid = %d\n", i, buf_pid[i]);
    }

    free  (buf_pid);
    delete_msg (id);

    return 0;
}

//------------------------------------------------