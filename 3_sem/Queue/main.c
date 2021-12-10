#include "queue.h"

//-----------------------------------------------

int main (int argc, const char *argv[])
{
    int num_proc = parsing_input (argc, argv);

    int id = create_msg ();

    int i_proc = 1;
    int signal = getpid();

    struct my_msg get_msg = {0};
    struct my_msg snd_msg = {0};
    
    create_procs (&i_proc, num_proc);

    if (signal == getpid())
    {
        for (int i = 1; i < num_proc + 1; i++)
        {
            send_msg (id, i, &snd_msg);

            recv_msg (id, num_proc + 1, &get_msg);
        }
    }
    else
    {
        recv_msg (id, i_proc, &get_msg);

        printf ("Child proc = %ld\n", i_proc);
        fflush (stdout);

        send_msg (id, num_proc + 1, &snd_msg);
        
        exit (EXIT_SUCCESS);
    }

    delete_msg (id);

    return 0;
}

//------------------------------------------------