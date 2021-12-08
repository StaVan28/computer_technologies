#include "select.h"
#include "debug.h"

//-----------------------------------------------------------------------------

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
    int   num_proc;

    int*  read_fds;
    int* write_fds;
};

//!

#define PARENT_CREATE -1

//!

void connect_info_construct (struct connect_info* cnct_info, int num_proc);

void connect_info_destruct  (struct connect_info* cnct_info);

void elem_cnct_child_construct (elem_connect* elem_cnct, struct connect_info* cnct_info, int i_proc, int fd_file);

void elem_cnct_prnt_construct  (elem_connect* elem_cnct, struct connect_info* cnct_info, int i_proc);

void elem_cnct_destruct        (elem_connect* elem_cnct);

//------------------------------------------------------------

void transfer_data (int num_proc, const char* file_name)
{
    DBG_PRINT ("\ntransfer_data ()\n\n");

    struct connect_info      cnct_info = {0};
    connect_info_construct (&cnct_info, num_proc);  

    //выделим память под структурки соединения
    elem_connect* arr_cnct = NULL; 
    if          ((arr_cnct = (elem_connect*) calloc (2 * num_proc - 1, sizeof (elem_connect))) == NULL)
    {
        ERROR_INFO ("calloc (): arr_cnct");
        exit       (EXIT_FAILURE);
    }
    PRINT_STEP (arr_cnct, %p);

    //откроем файл 
    int  fd_file = -1;
    if ((fd_file = open (file_name, O_RDONLY)) < 0)
    {
        ERROR_INFO ("open()");
        exit       (EXIT_FAILURE);        
    }
    PRINT_STEP (fd_file, %d);

    for (int i_proc = 0; i_proc < num_proc; i_proc++)
    {
        pid_t pid = fork ();

        if (pid > 0)
        {
            if (i_proc == num_proc - 1)
                break;

            elem_cnct_prnt_construct (arr_cnct + i_proc, &cnct_info, i_proc);

            continue;
        }
        else if (pid == 0)
        {
            if (prctl (PR_SET_PDEATHSIG, SIGTERM) < 0)
            {
                ERROR_INFO ("prctl ()");
                exit       (EXIT_FAILURE);                
            }

            elem_cnct_child_construct (arr_cnct + i_proc, &cnct_info, i_proc, fd_file);

            while (true)
            {
                int     num_symb = -1;
                while ((num_symb = read ((arr_cnct + i_proc)->fd_in, (arr_cnct + i_proc)->buf, PIPE_BUF)) > 0)
                {
                    if (write ((arr_cnct + i_proc)->fd_in, (arr_cnct + i_proc)->buf, num_symb) != num_symb)
                    {
                        ERROR_INFO ("write ()");
                        exit       (EXIT_FAILURE);
                    }
                }

                if (num_symb < 0)
                {
                    ERROR_INFO ("read ()");
                    exit       (EXIT_FAILURE);     
                }
            }

            exit (EXIT_SUCCESS); 
        }
        else if (pid == -1)
        {
            ERROR_INFO ("fork ()");
            exit       (EXIT_FAILURE);
        }
    }

    fd_set fd_rd, fd_wr;
    FD_ZERO(&fd_rd);
    FD_ZERO(&fd_wr);

    close (fd_file);
    fd_file = -1;

    free (arr_cnct);
    arr_cnct = NULL;

    PAUSE;

    connect_info_destruct (&cnct_info);
}

//------------------------------------------------------------

void connect_info_construct (struct connect_info* cnct_info, int num_proc)
{
    DBG_PRINT ("\nconnect_info construct\n");

    cnct_info->num_proc = num_proc;
    PRINT_STEP (num_proc, %d);
    
    if ((cnct_info->read_fds  = (int*) calloc (num_proc, sizeof (int))) == NULL)
    {
        ERROR_INFO ("calloc (): read_fds");
        exit       (EXIT_FAILURE);
    }
    PRINT_STEP (cnct_info->read_fds, %p);

    if ((cnct_info->write_fds = (int*) calloc (num_proc, sizeof (int))) == NULL)
    {
        ERROR_INFO ("calloc (): write_fds");
        exit       (EXIT_FAILURE);
    }
    PRINT_STEP (cnct_info->write_fds, %p);

    int tmp_fd[2] = {};
    for (int i = 0; i < num_proc; i++)
    {
        if (pipe2 (tmp_fd, O_NONBLOCK) < 0)
        {
            ERROR_INFO ("pipe() ");
            exit       (EXIT_FAILURE);
        }

        cnct_info->read_fds[i]  = tmp_fd[0];
        cnct_info->write_fds[i] = tmp_fd[1];
    }

    DBG_PRINT ("\n");
}

//------------------------------------------------------------

void connect_info_destruct (struct connect_info* cnct_info)
{
    DBG_PRINT ("\n\nconnect_info destruct\n");

    free (cnct_info->read_fds);
    cnct_info->read_fds  = NULL;

    free (cnct_info->write_fds);
    cnct_info->write_fds = NULL;

    cnct_info->num_proc  = -1;

    DBG_PRINT ("\n");
}

//------------------------------------------------------------

void elem_cnct_child_construct (elem_connect* elem_cnct, struct connect_info* cnct_info, int i_proc, int fd_file)
{
    DBG_PRINT ("\n\nelem_cnct_child_construct\n");

    int  dynam_size_buf = pow (3, 7);
    int static_size_buf = 128000;
    
    elem_cnct->size_buf = (dynam_size_buf > static_size_buf) ? static_size_buf : dynam_size_buf;
    PRINT_STEP (elem_cnct->size_buf, %d);

    if ((elem_cnct->buf = (char*) calloc (elem_cnct->size_buf, sizeof (char))) == NULL)
    {
        ERROR_INFO ("calloc ()");
        exit       (EXIT_FAILURE);
    }
    PRINT_STEP (elem_cnct->buf, %p);

    elem_cnct->create_num = i_proc;
    PRINT_STEP (elem_cnct->create_num, %d);

    elem_cnct->fd_in  = cnct_info->read_fds[i_proc];
    elem_cnct->fd_out = cnct_info->write_fds[i_proc];             
    
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
    {
        elem_cnct->fd_in  = fd_file;
    }
    
    if (i_proc == cnct_info->num_proc - 1)
    {
        elem_cnct->fd_out = STDOUT_FILENO;        
    }

    PRINT_STEP (elem_cnct->fd_in,  %d);
    PRINT_STEP (elem_cnct->fd_out, %d);

    DBG_PRINT ("\n")
}

//------------------------------------------------------------

void elem_cnct_prnt_construct (elem_connect* elem_cnct, struct connect_info* cnct_info, int i_proc)
{
    DBG_PRINT ("\n\nelem_cnct_prnt_construct\n");

    int  dynam_size_buf = pow (3, 7);
    int static_size_buf = 128000;
    
    elem_cnct->size_buf = (dynam_size_buf > static_size_buf) ? static_size_buf : dynam_size_buf;
    PRINT_STEP (elem_cnct->size_buf, %d);

    if ((elem_cnct->buf = (char*) calloc (elem_cnct->size_buf, sizeof (char))) == NULL)
    {
        ERROR_INFO ("calloc ()");
        exit       (EXIT_FAILURE);
    }
    PRINT_STEP (elem_cnct->buf, %p);

    elem_cnct->create_num = PARENT_CREATE;
    PRINT_STEP (elem_cnct->create_num, %d);

    elem_cnct->fd_in  = cnct_info->write_fds[i_proc];
    elem_cnct->fd_out = cnct_info->read_fds[i_proc + 1];

    PRINT_STEP (elem_cnct->fd_in,  %d);
    PRINT_STEP (elem_cnct->fd_out, %d);

    DBG_PRINT ("\n")
}

//------------------------------------------------------------

void elem_cnct_destruct (elem_connect* elem_cnct)
{
    elem_cnct->size_buf = -1;
    free (elem_cnct->buf);
    elem_cnct->buf      = NULL;

    elem_cnct->fd_in  = -1;
    elem_cnct->fd_out = -1;

    elem_cnct->create_num = -1;
}

//------------------------------------------------------------
