#include "queue.h"

//-----------------------------------------------

int main (int argc, const char *argv[])
{
    int num_proc = parsing_input (argc, argv);

    int id = create_msg ();

    struct my_msg get_msg = {0};
    struct my_msg snd_msg = {0};

    int i_proc = 1;
    int signal = getpid();

    create_procs (&i_proc, num_proc);

    if (signal == getpid())
    {
        for (int i = 1; i < num_proc + 1; i++)
        {
            snd_msg.type = i;

            if (msgsnd (id , &snd_msg, 0, 0) < 0)
            {
                delete_msg (id);

                perror ("ERROR! msgsnd ()");
                exit   (EXIT_FAILURE);
            }

            if (msgrcv (id, &get_msg, 0, num_proc + 1, 0) < 0)
            {
                delete_msg (id);

                perror ("ERROR! msgrcv ()");
                exit   (EXIT_FAILURE);
            }
        }
    }
    else
    {
        if (msgrcv (id, &get_msg, 0, i_proc, 0) < 0)
        {
            delete_msg (id);

            perror ("ERROR! msgrcv ()");
            exit   (EXIT_FAILURE);
        }

        printf ("Child proc = %ld\n", i_proc);
        fflush (stdout);

        snd_msg.type = num_proc + 1;

        if (msgsnd (id, &snd_msg, 0, 0) < 0)
        {
            delete_msg (id);

            perror ("ERROR! msgsnd ()");
            exit   (EXIT_FAILURE);
        }
        
        exit (EXIT_SUCCESS);
    }

    delete_msg (id);

    return 0;
}

//------------------------------------------------