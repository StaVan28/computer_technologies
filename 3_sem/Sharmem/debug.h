#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

//------------------------------------------------------------

struct my_error_t
{
    char* file;
    int   line;
};

#define ERROR_INFO(err_str)                       \
		perror  (        "ERROR! " err_str "\n"); \
        fprintf (stderr, "errno = %d\n", errno);  \
		                                          \
		error_info.file = __FILE__;               \
        error_info.line = __LINE__                \

//!

#define PRINT_STEP(arg, print_form)                        \
        fprintf (stderr, "Get " #arg " > "                 \
                         #arg " = {"#print_form"}\n", arg) \

//------------------------------------------------------------

#endif // DEBUG_H_INCLUDED