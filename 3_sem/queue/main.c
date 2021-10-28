#include "queue.h"

//-----------------------------------------------

int main (int argc, const char *argv[])
{
    int num_proc = parsing_input (argc, argv);

    int id = create_msg ();

    int i_proc = 1;
    int signal = getpid();

    for ( ;i_proc < num_proc + 1; i_proc++)
    {
        pid_t pid = fork ();

        if (pid > 0)
        {
            continue;
        }
        else if (pid == 0)
        {
            break;
        }
        else if (pid == -1)
        {
            //delete_msg (id);

            fprintf (stderr, "i_proc = %d", i_proc);
            perror ("ERROR! fork ()");
            exit   (EXIT_FAILURE);
        }
    }

    if (signal == getpid())
    {
        for (int i = 1; i < num_proc + 1; i++)
        {
            struct my_msg snd_msg = {i};

            fprintf ("");

            if (msgsnd (id , &snd_msg, sizeof(long), 0) < 0)
            {
                delete_msg (id);

                printf ("this parent\n");
                perror ("ERROR! msgsnd ()");
                exit   (EXIT_FAILURE);
            }

            struct my_msg get_msg = {};

            if (msgrcv (id, &get_msg, sizeof (long), num_proc + 1, 0) < 0)
            {
                delete_msg (id);

                printf ("this parent, errno = %d\n", errno);
                perror ("ERROR! msgrcv ()");
                exit   (EXIT_FAILURE);
            }
        }
    }
    else
    {
        struct my_msg get_msg = {};

        if (msgrcv (id, &get_msg, sizeof (long), i_proc, 0) < 0)
        {
            delete_msg (id);

            printf ("this child\n");
            perror ("ERROR! msgrcv ()");
            exit   (EXIT_FAILURE);
        }

        printf ("My proc = %ld\n", get_msg.type);
        fflush (stdout);

        struct my_msg snd_msg = {num_proc + 1};

        if (msgsnd (id , &snd_msg, sizeof(long), 0) < 0)
        {
            delete_msg (id);

            printf ("this child\n");
            perror ("ERROR! msgsnd ()");
            exit   (EXIT_FAILURE);
        }
        
        exit (EXIT_SUCCESS);
    }

    delete_msg (id);

    return 0;
}

//------------------------------------------------