#include "config.h"
#include "debug.h"

//----------------------------------------------

typedef struct
{
    char* buf;
    int   size_buf;

    int   create_num;

    int   fd_in;
    int   fd_out;
} elem_connect;

//!

struct connect_info
{
    int*  read_fds;
    int* write_fds;
};

//!

#define PARENT_STRUCT -1
#define BUFF_SIZE     4096

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
    DBG_PRINT ("\ntransfer_data ()\n\n");

    struct connect_info cnct_info = {0};

    DBG_PRINT ("\nconnect_info construct\n");

    PRINT_STEP (num_proc, %d);
    
    if ((cnct_info.read_fds  = (int*) calloc (num_proc, sizeof (int))) == NULL)
    {
        ERROR_INFO ("calloc (): read_fds");
        exit       (EXIT_FAILURE);
    }
    PRINT_STEP (cnct_info.read_fds, %p);

    if ((cnct_info.write_fds = (int*) calloc (num_proc, sizeof (int))) == NULL)
    {
        ERROR_INFO ("calloc (): write_fds");
        exit       (EXIT_FAILURE);
    }
    PRINT_STEP (cnct_info.write_fds, %p);

    int tmp_fd[2] = {};
    for (int i = 0; i < num_proc; i++)
    {
        if (pipe2 (tmp_fd, O_NONBLOCK) < 0)
        {
            ERROR_INFO ("pipe() ");
            exit       (EXIT_FAILURE);
        }

        cnct_info.read_fds[i]  = tmp_fd[0];
        cnct_info.write_fds[i] = tmp_fd[1];
    }

    DBG_PRINT ("\n"); 

    // alloc memory for structs
    elem_connect* elem_cnct = NULL; 
    if          ((elem_cnct = (elem_connect*) calloc (num_proc, sizeof (elem_connect))) == NULL)
    {
        ERROR_INFO ("calloc (): arr_cnct");
        exit       (EXIT_FAILURE);
    }
    PRINT_STEP (elem_cnct, %p);

    // open file
    int  fd_file = -1;
    if ((fd_file = open (file_name, O_RDONLY)) < 0)
    {
        ERROR_INFO ("open()");
        exit       (EXIT_FAILURE);        
    }
    PRINT_STEP (fd_file, %d);
    
    // create childs
    for (int i_proc = 0; i_proc < num_proc; i_proc++)
    {
        pid_t pid = fork ();

        if (pid > 0) // parent
        {
            if (i_proc == num_proc - 1)
                break;

            int  dynam_size_buf = pow (3, 7 + i_proc);
            int static_size_buf = 12800;

            DBG_PRINT ("\n\nparent\n");

            if (dynam_size_buf > static_size_buf)
                elem_cnct->size_buf = static_size_buf;
            else
                elem_cnct->size_buf =  dynam_size_buf;

            PRINT_STEP (elem_cnct->size_buf, %d);

            if ((elem_cnct->buf = (char*) calloc (elem_cnct->size_buf, sizeof (char))) == NULL)
            {
                ERROR_INFO ("calloc ()");
                exit       (EXIT_FAILURE);
            }
            PRINT_STEP (elem_cnct->buf, %p);

            elem_cnct->create_num = i_proc;
            PRINT_STEP (elem_cnct->create_num, %d);

            elem_cnct->fd_in  = cnct_info.write_fds[i_proc];
            elem_cnct->fd_out = cnct_info.read_fds[i_proc + 1];

            PRINT_STEP (elem_cnct->fd_in,  %d);
            PRINT_STEP (elem_cnct->fd_out, %d);

            continue;
        }
        else if (pid == 0) // child
        {
            DBG_PRINT ("\n\nchild\n");

            if (prctl (PR_SET_PDEATHSIG, SIGTERM) < 0)
            {
                ERROR_INFO ("prctl ()");
                exit       (EXIT_FAILURE);                
            }
            
            elem_cnct->size_buf = -1;
            elem_cnct->buf      = NULL;
            PRINT_STEP (elem_cnct->size_buf, %d);

            elem_cnct->create_num = i_proc;
            PRINT_STEP (elem_cnct->create_num, %d);

            elem_cnct->fd_in  = cnct_info.read_fds[i_proc];
            elem_cnct->fd_out = cnct_info.write_fds[i_proc];             
            
            if (fcntl (elem_cnct->fd_in, F_SETFL, O_RDONLY))
            {
                ERROR_INFO ("fcntl ()");
                exit       (EXIT_FAILURE);        
            }
            
            if (fcntl (elem_cnct->fd_out, F_SETFL, O_WRONLY))
            {
                ERROR_INFO ("fcntl ()");
                exit       (EXIT_FAILURE);        
            }

            if (i_proc == 0)
                elem_cnct->fd_in  = fd_file;
            
            if (i_proc == num_proc - 1)
                elem_cnct->fd_out = STDOUT_FILENO;

            PRINT_STEP (elem_cnct->fd_in,  %d);
            PRINT_STEP (elem_cnct->fd_out, %d);

            char  buff[BUFF_SIZE] = {};
            int   num_symb   = 0;

            while ((num_symb = read (elem_cnct->fd_in, buff, BUFF_SIZE)) > 0)
            {
                if (write (elem_cnct->fd_out, buff, num_symb) != num_symb)
                {
                    ERROR_INFO ("ERROR! Something wrong with write()\n");
                    exit       (EXIT_FAILURE);
                }
            }

            if (num_symb < 0)
            {
                ERROR_INFO ("ERROR! Something wrong with read()\n");
                exit       (EXIT_FAILURE);
            }

            if (close (elem_cnct->fd_in))
            {
                ERROR_INFO ("close_in ()");
                exit       (EXIT_FAILURE); 
            }
            elem_cnct->fd_in = -1;         

            if (close (elem_cnct->fd_out))
            {
                ERROR_INFO ("close_out ()");
                exit       (EXIT_FAILURE); 
            }
            elem_cnct->fd_out = -1;

            DBG_PRINT ("!children died\n");

            exit (EXIT_SUCCESS); 
        }
        else if (pid == -1)
        {
            ERROR_INFO ("fork ()");
            exit       (EXIT_FAILURE);
        }
    }

    int max_fd    = -1;
    int ret_read  = -1;
    int ret_write = -1;
    int num_rd    = -1;
    int num_wr    = -1;

    for (int i = 0; i < num_proc - 1; i++)
    {
        DBG_PRINT ("\nstart cycle\n");

        fd_set fds_rd;
        FD_ZERO(&fds_rd);

        for (int i_fd = 0; i_fd < i + 1; i_fd++)
        {
            if (i_fd == num_proc - 1)
                break;

            if (elem_cnct[i_fd].fd_in > max_fd)
                max_fd = elem_cnct[i_fd].fd_in;

            if (elem_cnct[i_fd].fd_in != -1)
            {
                FD_SET (elem_cnct[i_fd].fd_in, &fds_rd);
                num_rd++;
            }
        }
        PRINT_STEP (num_rd, %d);

        if (num_rd > 0)
        {
            if (select (max_fd + 1, &fds_rd, NULL, NULL, NULL) < 0)
            {
                ERROR_INFO ("select {read} ()");
                exit       (EXIT_FAILURE);
            }

            num_rd = -1;
        }

        for (int i_fd = 0; i_fd < i + 1; i_fd++)
        {
            if (i_fd == num_proc - 1)
                break;

            if (FD_ISSET (elem_cnct[i_fd].fd_in, &fds_rd))
            {
                PRINT_STEP (elem_cnct[i_fd].fd_in, %d);

                ret_read = read (elem_cnct[i_fd].fd_in, elem_cnct[i_fd].buf, elem_cnct[i_fd].size_buf);

                if (ret_read > 0)
                {
                    num_proc++;
                    break;
                }

                if (ret_read == 0)
                {
                    if (close (elem_cnct[i_fd].fd_in))
                    {
                        ERROR_INFO ("close ()");
                        exit       (EXIT_FAILURE);                        
                    }

                    elem_cnct[i_fd].fd_in = -1;
                }

                if (ret_read < 0)
                {
                    ERROR_INFO ("read ()");
                    exit       (EXIT_FAILURE);
                }
            }
        }

        fd_set fds_wr;
        FD_ZERO(&fds_wr);

        for (int i_fd = 0; i_fd < i + 1; i_fd++)
        {
            if (i_fd == num_proc - 1)
                break;

            if (elem_cnct[i_fd].fd_out > max_fd)
                max_fd = elem_cnct[i_fd].fd_out;

            if (elem_cnct[i_fd].fd_out != -1)
            {
                FD_SET (elem_cnct[i_fd].fd_out, &fds_wr);
                num_wr++;
            }
        }

        if (num_wr > 0)
        {
            if (select (max_fd + 1, NULL, &fds_wr, NULL, NULL) < 0)
            {
                ERROR_INFO ("select {read} ()");
                exit       (EXIT_FAILURE);
            }

            num_wr = -1;
        }

        for (int i_fd = 0; i_fd < i + 1; i_fd++)
        {
            if (i_fd == num_proc - 1)
                break;

            if (FD_ISSET (elem_cnct[i_fd].fd_out, &fds_rd))
            {
                ret_write = write (elem_cnct[i_fd].fd_out, elem_cnct[i_fd].buf, elem_cnct[i_fd].size_buf);

                if (ret_write == 0)
                {
                    if (close (elem_cnct[i_fd].fd_out))
                    {
                        ERROR_INFO ("close ()");
                        exit       (EXIT_FAILURE);                        
                    }

                    elem_cnct[i_fd].fd_out = -1;
                }

                if (ret_write < 0)
                {
                    ERROR_INFO ("write ()");
                    exit       (EXIT_FAILURE);
                }
            }
        }
    }

    DBG_PRINT ("\n\nconnect_info destruct\n");

    while (wait (NULL) != -1)
    {}

    for (int i = 0; i < num_proc; i++)
    {
        free (elem_cnct[i].buf);
        elem_cnct[i].buf = NULL;
    }

    free (elem_cnct);
    elem_cnct = NULL;

    free (cnct_info.read_fds);
    cnct_info.read_fds  = NULL;

    free (cnct_info.write_fds);
    cnct_info.write_fds = NULL;

    num_proc  = -1;
}

//----------------------------------------------