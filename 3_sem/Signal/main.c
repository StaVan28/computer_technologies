#include "config.h"
#include "debug.h"

//-----------------------------------------------

static const int NUM_ARGC = 2;

int BIT           = 0;
int IS_CHILD_DIED = 0;

void hnd_sigchld (int);

void hnd_sigusr1 (int);

void hnd_sigusr2 (int);

//-----------------------------------------------

int main (int argc, const char *argv[])
{
    if (argc != NUM_ARGC)
    {
        fprintf (stderr, "error input\n");
        exit    (EXIT_FAILURE);
    }

    DBG_PRINT ("Start prog\n");

    struct sigaction act;
    act.sa_flags = 0;

    if (sigemptyset (&act.sa_mask) < 0)
        ERROR_INFO  ("sigemptyset () ");

    act.sa_handler = hnd_sigchld;
    if (sigaction  (SIGCHLD, &act, NULL) < 0)
        ERROR_INFO ("sigaction () ");

    act.sa_handler = hnd_sigusr1;
    if (sigaction  (SIGUSR1, &act, NULL) < 0)
        ERROR_INFO ("sigaction () ");

    act.sa_handler = hnd_sigusr2;
    if (sigaction  (SIGUSR2, &act, NULL) < 0)
        ERROR_INFO ("sigaction () ");

    sigset_t set, old;

    if (sigfillset (&set) < 0)
        ERROR_INFO ("sigfillset () ");

    if (sigprocmask (SIG_BLOCK, &set, &old) < 0)
        ERROR_INFO  ("sigprocmask () ");

    char cur_letter = 0;
    char mask       = 0x01;
    char bit        = 0;

    pid_t prnt_pid = getpid ();
    pid_t chld_pid = fork   ();

    if (chld_pid > 0)       // parent
    {
        int ret_write = -1;

        while (true)
        {
            for (int i = 0; i < 8; i++)
            {
                mask = 0x01 << i;

                errno = 0;
                sigsuspend (&old);
                if (errno != EINTR)
                    ERROR_INFO ("sigsuspend () ");

                if (IS_CHILD_DIED)
                    exit (EXIT_SUCCESS);

                if (BIT == 1)
                    cur_letter = cur_letter |   mask;
                else
                    cur_letter = cur_letter & (~mask);

                if (kill (chld_pid, SIGUSR2) < 0)
                    ERROR_INFO ("kill () ");
            }

            if (write (STDOUT_FILENO, &cur_letter, 1) < 0)
                ERROR_INFO ("write ()");

            mask       = 0x01;
            cur_letter = 0;
        }

        exit (EXIT_SUCCESS);
    }
    else if (chld_pid == 0) // child
    {
        if (prctl (PR_SET_PDEATHSIG, SIGTERM) < 0)
            ERROR_INFO ("prctl () ");

        if (prnt_pid != getppid ())
            ERROR_INFO ("No parent no more... ");

        int  fd_file = -1;
        if ((fd_file = open (argv[1], O_RDONLY)) < 0)
            ERROR_INFO ("open () ");

        int ret_read = -1;

        while (true)
        {
            ret_read = read (fd_file, &cur_letter, 1);

            if (ret_read == 0)
                break;

            if (ret_read < 0)
                ERROR_INFO ("read () ");

            for (int i = 0; i < 8; i++)
            {
                mask = 0x01 << i;
                bit  = mask & cur_letter;

                if (bit == 0)
                {
                    if (kill (prnt_pid, SIGUSR1) < 0)
                        ERROR_INFO ("kill () ");
                }
                else
                {
                    if (kill (prnt_pid, SIGUSR2) < 0)
                        ERROR_INFO ("kill () ");
                }
                    
                errno = 0;
                sigsuspend (&old);
                if (errno != EINTR)
                    ERROR_INFO ("sigsuspend () ");
            }

            mask       = 0x01;
            bit        = 0;
            cur_letter = 0;
        }

        if (close (fd_file) < 0)
            ERROR_INFO ("close () ");        
        fd_file = -1;

        exit (EXIT_SUCCESS);
    }
    else if (chld_pid == -1)
        ERROR_INFO ("fork () ");

    return 0;
}

//------------------------------------------------

void hnd_sigchld (int)
{
    errno = 0;
    int status = 0;

    if (wait3 (&status, WNOHANG, NULL) < 0)
        ERROR_INFO ("wait3 () ");

    if (WIFEXITED (status))
    {
        IS_CHILD_DIED = 1;
        return;
    }
    else
        ERROR_INFO ("Error child ");
}

//!

void hnd_sigusr1 (int)
{
    BIT = 0;
}

//!

void hnd_sigusr2 (int)
{
    BIT = 1;
}

