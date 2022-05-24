#include "common.h"
#include "server.h"

static int create_server_socket (int* server_socket);

void* work_handler (void* arg) {

    double* res = (double*) malloc (sizeof (double));

    if (res == NULL) {
        goto exit_without_free;
    }

    struct handler_info* h_info = arg;

    if (send (h_info->socket, &h_info->w_info, sizeof h_info->w_info, 0) != sizeof h_info->w_info)
        goto exit_free_res;

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(h_info->socket, &fdset);

    if (select (h_info->socket + 1, &fdset, NULL, NULL, NULL) != 1)
        goto exit_free_res;

    if (recv (h_info->socket, res, sizeof (double), 0) != sizeof (double))
        goto exit_free_res;

    goto exit_without_free;

exit_free_res:

    free (res);
    res = NULL;

exit_without_free:

    return res;
}

struct tasks_for_workers* 
divide_work (unsigned n_machines, unsigned n_threads,
             double begin, double end, int* error) {

    int err = SUCCESS;
    if (n_machines == 0 || n_threads == 0) {
        err = E_INVAL;
        goto exit_without_free;
    }

    const unsigned n_cpus = get_nprocs ();
    const double interval_per_thread = (end - begin) / n_threads;

    unsigned tmp = n_threads / n_cpus;
    unsigned n_workers = n_threads % n_cpus == 0 ? tmp : tmp + 1;
    n_workers = n_machines < n_threads ? n_machines : n_threads;

    struct tasks_for_workers* res = (struct tasks_for_workers*) malloc (sizeof (struct tasks_for_workers));

    if (!res) {
        err = E_MEM;
        goto exit_without_free;
    }

    res->size = n_workers;
    res->task = (struct handler_info*) calloc (n_workers, sizeof (struct handler_info));

    if (!res->task) {
        err = E_MEM;
        goto exit_free_res;
    }

    for (unsigned i = 0; i < n_workers - 1; i++) {

        res->task[i].w_info.n_threads = n_threads / n_workers;
        res->task[i].w_info.begin = begin;

        begin += interval_per_thread*  (double) res->task[i].w_info.n_threads;

        res->task[i].w_info.end = begin;
    }

    res->task[n_workers - 1].w_info.n_threads = n_threads - (n_workers - 1)*  res->task[0].w_info.n_threads;
    res->task[n_workers - 1].w_info.begin = begin;
    res->task[n_workers - 1].w_info.end = end;

    goto exit_without_free;

exit_free_res:

    free (res);
    res = NULL;

exit_without_free:

    if (error)
       * error = err;

    if (err != SUCCESS)
        res = NULL;

    return res;
}

void dump_tasks (struct tasks_for_workers* tasks) {

    if (tasks == NULL) {
        fprintf (stderr, "tasks is NULL\n");
        return;
    }

    for (int i = 0; i < tasks->size; i++) {
        printf ("[%d]:\nn_nhreads: %d\nbegin: %f\nend: %f\n",
                i, tasks->task[i].w_info.n_threads,
                tasks->task[i].w_info.begin, tasks->task[i].w_info.end);
    }
}

int send_hello_message () {

    struct sockaddr_in bc_addr;

    int sk = socket (AF_INET, SOCK_DGRAM, 0);

    if (sk == -1) {
        return E_SOCK;
    }

    int true = 1;
    if (setsockopt (sk, SOL_SOCKET, SO_BROADCAST, &true, sizeof true) < 0) {
        close (sk);
        return E_SOCK;
    }

    memset (&bc_addr, 0, sizeof bc_addr);

    bc_addr.sin_family = AF_INET;
    bc_addr.sin_port = htons (UDP_PORT);
    bc_addr.sin_addr.s_addr = htonl (INADDR_BROADCAST);

    int msg = TCP_PORT;
    if (sendto (sk, &msg, sizeof msg, 0, (struct sockaddr*) &bc_addr, sizeof bc_addr) < 0) {
        close (sk);
        return E_SOCK;
    }

    close (sk);
    return SUCCESS;
}

int get_tcp_connections (struct tasks_for_workers* tasks) {

    if (tasks == NULL) {
        return E_INVAL;
    }

    int error = SUCCESS;

    int serv_sock;
    int new_sock;

    struct sockaddr_in new_addr;
    socklen_t new_addr_len = sizeof new_addr;

    error = create_server_socket (&serv_sock);

    if (error != SUCCESS) {
        goto exit_without_closing;
    }

    if (listen (serv_sock, 10) < 0) {
        error = E_SOCK;
        goto exit_without_closing;
    }

    int n_connected_sockets = 0;
    for (; n_connected_sockets < tasks->size; ++n_connected_sockets) {


        new_sock = accept (serv_sock, (struct sockaddr*) &new_addr, &new_addr_len);
        if (new_sock < 0) {
            error = E_CONN;
            goto exit_close_sockets;
        }

        tasks->task[n_connected_sockets].socket = new_sock;
        printf("-   CONNECT TO CLIENT\n");
    }


    goto exit_without_closing;

exit_close_sockets:

    for (int j = 0; j < n_connected_sockets; ++j)
        close (tasks->task[j].socket);


exit_without_closing:

    close (serv_sock);
    return error;
}

int get_result (struct tasks_for_workers* tasks, double* res) {

    if (!tasks || !res)
        return E_INVAL;

    int error = SUCCESS;

    pthread_t* threads = (pthread_t*) calloc (tasks->size, sizeof (pthread_t));

    if (!threads) {
        error = E_MEM;
        goto exit_without_free;
    }

    int check = 0;

    int i = 0;
    for (; i < tasks->size; i++) {

        check = pthread_create (&threads[i], NULL, work_handler, (void* ) &tasks->task[i]);

        if (check != 0) {
            perror ("pthread_create work_handler:");
            error = E_THREAD;
            goto exit_free_all;
        }
    }

    double result = 0;
    double* part = NULL;

    for (int j = 0; j < tasks->size; ++j) {

        check = pthread_join (threads[j], (void* *) &part);

        if (check != 0 || !part) {

            if (check != 0) {
                fprintf (stderr, "check = %d\n", check);
            }

            error = E_THREAD;
            goto exit_free_all;
        }

        result +=* part;
        free (part);
    }

   * res = result;

    goto exit_free_threads;

exit_free_all:

    for (int j = 0; j < i; ++j) {
        pthread_cancel (threads[j]);
    }

exit_free_threads:

    free (threads);

exit_without_free:

    return error;
}

static int create_server_socket (int* server_socket) {

    int true = 1;
    int ret = 0;
    int serv_sock;

    struct sockaddr_in serv_addr;

    serv_sock = socket (AF_INET, SOCK_STREAM, 0);

    if (serv_sock == -1) {
        perror ("creating server socket:");
        return E_SOCK;
    }

    ret = setsockopt (serv_sock, SOL_SOCKET, SO_REUSEADDR, &true, sizeof true);
    if (ret != 0) {
        perror ("setsockopt for server socket:");
        return E_SOCK;
    }

    struct timeval timeout = {
            .tv_sec  = 10,
            .tv_usec = 0
    };

    ret = setsockopt (serv_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
    if (ret != 0) {
        perror ("setsockopt (timeout) for server socket:");
        return E_SOCK;
    }

    ret = set_keepalive (serv_sock, 4, 3, 1);
    if (ret != 0) {
        return E_SOCK;
    }


    memset (&serv_addr, 0, sizeof serv_addr);

    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
    serv_addr.sin_port        = htons (TCP_PORT);


    if (bind (serv_sock, (struct sockaddr*) &serv_addr, sizeof serv_addr) < 0) {
        perror ("binding server socket");
        return E_SOCK;
    }

   * server_socket = serv_sock;
    return SUCCESS;
}
