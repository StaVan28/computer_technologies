
#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

//------------------------------------------------------------

#include <stdio.h> 
#include <errno.h>
#include <stdlib.h>         // exit()

//------------------------------------------------------------

#define ERROR_INFO(err_str)                                         \
        do                                                          \
        {                                                           \
                perror  (        "ERROR! " err_str);                \
                fprintf (stderr, "errno = %d\n", errno);            \
                                                                    \
                fprintf (stderr, "file = {%s}, line = {%d}\n",      \
                                  __FILE__,    __LINE__       );    \
                                                                    \
                exit (EXIT_FAILURE);                                \
        } while (0)

//--------

#ifdef DEBUG
#define DBG_PRINT(pr_str)             \
        do                            \
        {                             \
            fprintf (stderr, pr_str); \
        } while (0)

//!

#define PRINT_STEP(arg, print_form)                     \
        do                                              \
        {                                               \
            fprintf (stderr, "  Get %28s > %28s = ",    \
                                #arg, #arg);            \
            fprintf (stderr, "{"#print_form"}\n", arg); \
        } while (0)

//!

#define PAUSE                                               \
        do                                                  \
        {                                                   \
            fprintf (stderr, "PAUSE START\n");              \
            fprintf (stderr, "file = {%s}, line = {%d}",    \
                              __FILE__,    __LINE__     );  \
            getchar ();                                     \
            fprintf (stderr,   "PAUSE END\n");              \
        } while (0)
#else
#define DBG_PRINT(pr_str)               \
        do                              \
        {                               \
                                        \
        } while (0)

//!

#define PRINT_STEP(arg, print_form) \
        do                          \
        {                           \
                                    \
        } while (0)

//!

#define PAUSE       \
        do          \
        {           \
                    \
        } while (0)

#endif  // DEBUG

//------------------------------------------------------------

#endif // DEBUG_H_INCLUDED
