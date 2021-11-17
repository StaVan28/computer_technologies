#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

//------------------------------------------------------------

#define DEBUG 0

#define ERROR_INFO(err_str)                                      \
        if (DEBUG)                                               \
        {                                                        \
                perror  (        "ERROR! " err_str);             \
                fprintf (stderr, "errno = %d\n", errno);         \
                                                                 \
                fprintf (stderr, "file = {%s}, line = {%d}\n",   \
                                  __FILE__,    __LINE__       ); \
        }                                                        \

//!

#define DBG_PRINT(pr_str)             \
        if (DEBUG)                    \
        {                             \
            fprintf (stderr, pr_str); \
        }                             \

//!

#define PRINT_STEP(arg, print_form)                               \
        if (DEBUG)                                                \
        {                                                         \
            fprintf   (stderr, "  Get " #arg " > "                \
                               #arg " = {"#print_form"}\n", arg); \
        }                                                         \

//!

#define PAUSE                                               \
        if (DEBUG)                                          \
        {                                                   \
            fprintf (stderr, "PAUSE START\n");              \
            fprintf (stderr, "file = {%s}, line = {%d}",    \
                              __FILE__,    __LINE__     );  \
            getchar ();                                     \
            fprintf (stderr,   "PAUSE END\n");              \
        }                                                   \

//------------------------------------------------------------

#endif // DEBUG_H_INCLUDED