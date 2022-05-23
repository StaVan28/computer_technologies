#include <assert.h>
#include "common.h"
#include "server.h"
#include "worker.h"

const double begin = 0.0;
const double end   = 300.0;

static void* send_hellos (void* arg) {

    assert (!arg);
    int error = SUCCESS;
    while (1) {

        error = send_hello_message();

        if (error != SUCCESS) {
            p_error (error);
            pthread_exit (NULL);
        }
    }
}

static void* get_tcps (void* arg) {

    struct tasks_for_workers* tasks = arg;

    int error = get_tcp_connections (tasks);

    if (error != SUCCESS) {
        p_error (error);
        fprintf (stderr, "Failed\n");
    }

    return NULL;
}

void run_server(int n_threads, int n_machines) {

    int error = SUCCESS;
    struct tasks_for_workers* tasks;

    tasks = divide_work (n_machines, n_threads, begin, end, &error);
    // dump_tasks (tasks);

    if (error != SUCCESS) {
        p_error (error);
        fprintf (stderr, "Failed\n");
        return;
    }

    pthread_t th_sender;
    pthread_t th_getter;

    if (pthread_create (&th_getter, NULL, get_tcps, tasks) != 0) {
        perror ("pthread_create getter:");
        return;
    }

    if (pthread_create (&th_sender, NULL, send_hellos, NULL) != 0) {
        perror ("pthread_create sender:");
        return;
    }

    if (pthread_join (th_getter, NULL) != 0) {
        perror ("pthread_join getter:");
        goto exit;
    }

    pthread_cancel (th_sender);

    double res = 0;
    error = get_result (tasks, &res);

    if (error != SUCCESS) {
        p_error (error);
        fprintf (stderr, "Failed\n");
        goto exit;
    }

    printf ("res: %f\n", res);

exit:
    for (int i = 0; i < tasks->size; ++i) {
        close (tasks->task[i].socket);
    }
}

void run_worker () {

    int error = SUCCESS;

    struct sockaddr_in serv_addr = get_hello_message (&error);

    if (error != SUCCESS) {
        p_error (error);
        return;
    }

    int serv_socket = connect_to_server (serv_addr, &error);

    if (error != SUCCESS) {
        p_error (error);
        close (serv_socket);
        return;
    }

    error = do_computation (serv_socket);

    if (error != SUCCESS) {
        close (serv_socket);
        p_error (error);
        return;
    }

    printf ("--- Success finished ---\n");

    close (serv_socket);
}

int set_keepalive (int socket, int keepcnt, int keepidle, int keepintvl) {

    int ret;
    int true = 1;

    ret = setsockopt (socket, SOL_SOCKET, SO_KEEPALIVE, &true, sizeof true);
    if (ret != 0) {
        perror ("setsockopt (timeout) for server socket:");
        return -1;
    }

    ret = setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int));
    if (ret != 0) {
        perror ("tcp_keepcnt");
        return -1;
    }

    ret = setsockopt (socket, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int));
    if (ret != 0) {
        perror ("tcp_keepcnt");
        return -1;
    }

    ret = setsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int));
    if (ret != 0) {
        perror ("tcp_keepcnt");
        return -1;
    }

    return 0;
}

void p_error (enum error err) {

    switch (err) {

        case SUCCESS:
            fprintf (stderr, "Success\n");
            break;
        case E_INVAL:
            fprintf (stderr, "Bad args ");
            break;
        case E_SOCK:
            fprintf (stderr, "Error socket\n");
            break;
        case E_CONN:
            fprintf (stderr, "Error connecting\n");
            break;
        case E_MEM:
            fprintf (stderr, "Error memory\n");
            break;
        case E_THREAD:
            fprintf (stderr, "Error thread\n");
            break;
        default:
            fprintf (stderr, "Unknown error\n");
    }
}
