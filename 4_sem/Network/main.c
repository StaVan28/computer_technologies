#include "common.h"

struct start_pack parse_args (int args, char* argv[]);

int main (int argc, char* argv[]) {

    struct start_pack start = parse_args (argc, argv);

    if (start.server == -1)
        return 0;

    if (start.server == 1)
        run_server (start.n_threads, start.n_machines);
    else if (start.server == 0)
        run_worker();

    return 0;
}

struct start_pack parse_args (int argc, char* argv[]) {

    struct start_pack res = {0, 0, 0};

    char* serv_flag = "-s";

    if (argc > 2 && argc < 4) {
        fprintf (stderr, "Bad args");
        res.server = -1;
        return res;
    }

    if (argc == 1) {
        return res;
    }

    if (argc == 4) {

        if (strcmp (argv[1], serv_flag) != 0) {
            res.server = -1;
            return res;
        }

        res.server = 1;
        res.n_threads  = (int) strtol (argv[2], NULL, 10);
        res.n_machines = (int) strtol (argv[3], NULL, 10);
    }

    return res;
}
