#ifndef WRITE_H_INCLUDED
#define WRITE_H_INCLUDED

//------------------------------------------------------------

#include "sharmem_config.h"

//------------------------------------------------------------

void  writer   (const char* file_path);

int   my_open  (const char* file_path);

void  my_close (      int   fd_file  );

//------------------------------------------------------------

#endif // WRITE_H_INCLUDED