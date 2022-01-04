#include "config.h"
#include "debug.h"

//----------------------------------------------

typedef struct
{
    int fd_rd;
    int fd_wr;

    char*     buf;
    char* end_buf;

    char* cur_read;
    char* cur_write;

    int full;
    int empty;
} info_connect;

//!

typedef struct 
{
    int fd__to__prnt[2];
    int fd_from_prnt[2];
} info_elem;

//!

#define BUFF_SIZE 4096

#define RD 0
#define WR 1

#define MIN(a, b) ((a) < (b)) ? (a) : (b)

//!

static const int STDIN_ARGC = 3;

void transfer_data (int num_proc, const char* file_name);

//----------------------------------------------

int main(int argc, const char *argv[])
{
    if (argc != STDIN_ARGC)
    {
        fprintf (stderr, "ERROR! Execute: ./a_out <num_proc> <out_file>\n");
        return EXIT_FAILURE;
    }

    int num_proc = get_num (argc, argv);

    transfer_data (num_proc, argv[2]);
    
    return 0;
}

//----------------------------------------------

void transfer_data (int num_proc, const char* file_name)
{
    info_elem* info_elems = (info_elem*) calloc (num_proc, sizeof (info_elem));
    if        (info_elems == NULL)
        ERROR_INFO ("calloc ()");

    info_elem cur_elem = {};
    for (int i_chld = 0; i_chld < num_proc; i_chld++)
    {
        if (pipe2 (cur_elem.fd__to__prnt, O_NONBLOCK) < 0)
            ERROR_INFO ("pipe2() ");

        if (pipe2 (cur_elem.fd_from_prnt, O_NONBLOCK) < 0)
            ERROR_INFO ("pipe2() ");

        pid_t pid_prnt = getpid ();
        pid_t pid_chld = fork   ();

        if (pid_chld == 0) // child
        {
            printf ("child = %d\n", i_chld);
            if (prctl (PR_SET_PDEATHSIG, SIGTERM) < 0)
                ERROR_INFO ("prctl () ");

            if (pid_prnt != getppid ())
                ERROR_INFO ("No parent no more... ");

            free (info_elems);
            info_elems = NULL;


            if (close (cur_elem.fd_from_prnt[WR]) < 0) 
                ERROR_INFO ("close ()");
            cur_elem.fd_from_prnt[WR] = -1;

            if (close (cur_elem.fd__to__prnt[RD]) < 0)
                ERROR_INFO ("close ()");
            cur_elem.fd__to__prnt[RD] = -1;

            
            int fd_rd = -1;
            int fd_wr = -1;

            if (i_chld == 0)
            {
                if ((fd_rd = open (file_name, O_RDONLY)) < 0)
                {
                    ERROR_INFO ("open ()");
                }
            }
            else
            {
                fd_rd = cur_elem.fd_from_prnt[RD];
            }
            

            if (i_chld == num_proc)
                fd_wr = STDOUT_FILENO;
            else
                fd_wr = cur_elem.fd__to__prnt[WR];

            if (fcntl (fd_rd, F_SETFL, O_RDONLY))
                ERROR_INFO ("fcntl ()");         

            if (fcntl (fd_wr, F_SETFL, O_WRONLY))
                ERROR_INFO ("fcntl ()");


            char  buff[BUFF_SIZE] = {};
            int   num_symb        = 0;
  
            while ((num_symb = read (fd_rd, buff, BUFF_SIZE)) > 0)
            {
                if (write (fd_wr, buff, num_symb) != num_symb)
                    ERROR_INFO ("write ()");
            }
            if (num_symb < 0)
                ERROR_INFO ("ERROR! Something wrong with read()\n");

            if (close(fd_rd) < 0)
                ERROR_INFO ("close ()");

            if (close(fd_wr) < 0)
                ERROR_INFO ("close ()");

            exit (EXIT_SUCCESS);
        }
        else  // parent
        {
            if (close(cur_elem.fd_from_prnt[RD]) < 0) 
                ERROR_INFO ("close ()");
            cur_elem.fd_from_prnt[RD] = -1;

            if (close(cur_elem.fd__to__prnt[WR]) < 0)
                ERROR_INFO ("close ()");
            cur_elem.fd__to__prnt[WR] = -1;

            info_elems[i_chld] = cur_elem;
        }

        if (pid_chld < 0)
            ERROR_INFO ("fork ()");
    }

    info_connect* inf_con = (info_connect*) calloc (num_proc, sizeof (info_connect));
    if           (inf_con == NULL)
        ERROR_INFO ("calloc ()");

    for (int i_prnt = 0; i_prnt < num_proc; i_prnt++)
    {
        inf_con[i_prnt].fd_rd = info_elems[i_prnt    ].fd__to__prnt[RD];
        inf_con[i_prnt].fd_wr = info_elems[i_prnt + 1].fd_from_prnt[WR];

        int size_buf = MIN (pow (3, num_proc - i_prnt + 4), 128000);

            inf_con[i_prnt].buf = (char*) calloc (size_buf, sizeof (char));
        if (inf_con[i_prnt].buf == NULL)
            ERROR_INFO ("calloc ()");

        inf_con[i_prnt].end_buf = inf_con[i_prnt].buf + size_buf;

        inf_con[i_prnt].cur_read  = inf_con[i_prnt].buf;
        inf_con[i_prnt].cur_write = inf_con[i_prnt].buf;

        inf_con[i_prnt].full  = 0;
        inf_con[i_prnt].empty = size_buf;
    }

    int i_chld = 0;
    while (i_chld < num_proc)
    {
        fd_set fds_rd, fds_wr;
        FD_ZERO (&fds_rd);
        FD_ZERO (&fds_wr);

        int max_fd = 0;
        for (int i = i_chld; i < num_proc - 1; i++)
        {
            if (inf_con[i].fd_rd != -1 && inf_con[i].empty > 0) 
            {
                FD_SET (inf_con[i].fd_rd, &fds_rd);

                if (inf_con[i].fd_rd > max_fd)
                    max_fd = inf_con[i].fd_rd;
            }

            if (inf_con[i].fd_wr != -1 && inf_con[i].full > 0) 
            {
                FD_SET (inf_con[i].fd_wr, &fds_wr);

                if (inf_con[i].fd_wr > max_fd)
                    max_fd = inf_con[i].fd_wr;
            }
        }

        printf ("Before select\n");

        int ret_select = select (max_fd + 1, &fds_rd, &fds_wr, NULL, NULL);
        if (ret_select < 0 && errno != EINTR)
            ERROR_INFO ("select ()");
        errno = 0;

        printf ("After select\n");

        for (int i = i_chld; i < num_proc - 1; i++)
        {
            if (FD_ISSET (inf_con[i].fd_rd, &fds_rd) && inf_con[i].empty > 0)
            {
                int ret_read = read (inf_con[i].fd_rd, inf_con[i].cur_read, inf_con[i].empty);
                if (ret_read < 0) 
                    ERROR_INFO ("read ()");

                if (ret_read == 0) 
                {
                    close (inf_con[i].fd_rd);
                    inf_con[i].fd_rd = -1;
                    
                    break;
                }

                if (inf_con[i].cur_read >= inf_con[i].cur_write)
                    inf_con[i].full     += ret_read;

                if (inf_con[i].cur_read + ret_read == inf_con[i].end_buf) 
                {
                    inf_con[i].cur_read = inf_con[i].buf;
                    inf_con[i].empty    = inf_con[i].cur_write - inf_con[i].cur_read;
                }
                else 
                {
                    inf_con[i].cur_read += ret_read;
                    inf_con[i].empty    -= ret_read;
                }
            }

            if (FD_ISSET (inf_con[i].fd_wr, &fds_wr) && inf_con[i].full > 0)
            {
                int ret_write = write (inf_con[i].fd_wr, inf_con[i].cur_write, inf_con[i].empty);
                if (ret_write < 0 && errno != EAGAIN) 
                    ERROR_INFO ("write ()");
                errno = 0;

                if (ret_write == 0) 
                {
                    close (inf_con[i].fd_wr);
                    inf_con[i].fd_wr = -1;
                    
                    break;
                }

                if (inf_con[i].cur_write >= inf_con[i].cur_read)
                    inf_con[i].empty     += ret_write;

                if (inf_con[i].cur_write + ret_write == inf_con[i].end_buf) 
                {
                    inf_con[i].cur_write = inf_con[i].buf;
                    inf_con[i].full      = inf_con[i].cur_read - inf_con[i].cur_write;
                }
                else 
                {
                    inf_con[i].cur_write += ret_write;
                    inf_con[i].full      -= ret_write;
                }
            }

            if (inf_con[i].fd_rd == -1 && inf_con[i].full == 0) 
            {
                if (i != i_chld)
                    ERROR_INFO ("Child died");

                errno = 0;
                if (close(inf_con[i].fd_wr < 0))
                    ERROR_INFO ("close ()");
                inf_con[i].fd_wr = -1;

                i_chld++;
            }
        }
    }

    for (int i_prnt = 0; i_prnt < num_proc - 1; i_prnt)
    {
        free (inf_con[i_prnt].buf);
        inf_con[i_prnt].buf = NULL;
    }

    free (inf_con);
    inf_con = NULL;

    while (waitpid (-1, NULL, 0))
        if (errno == ECHILD) 
            break;
}

//----------------------------------------------