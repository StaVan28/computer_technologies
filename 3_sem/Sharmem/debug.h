#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

//------------------------------------------------------------

#define ERROR_INFO(err_str)                             \
		perror  (        "ERROR! " err_str);            \
        fprintf (stderr, "errno = %d\n", errno);        \
		                                                \
		fprintf (stderr, "file = {%s}, line = {%d}\n",  \
                          __FILE__,    __LINE__       ) \

//!

#define DBG_PRINT(pr_str)         \
        fprintf (stderr, pr_str)  \

//!

#define PRINT_STEP(arg, print_form)                          \
        fprintf   (stderr, "  Get " #arg " > "               \
                           #arg " = {"#print_form"}\n", arg) \

//!

#define PAUSE                                          \
        fprintf (stderr, "PAUSE START\n");             \
        fprintf (stderr, "file = {%s}, line = {%d}",   \
                          __FILE__,    __LINE__     ); \
        getchar ();                                    \
        fprintf (stderr,   "PAUSE END\n")              \

//------------------------------------------------------------

#endif // DEBUG_H_INCLUDED