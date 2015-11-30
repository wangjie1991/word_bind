#include <stdio.h>
#include <string.h>

#include "Bind.h"

int main(int argc,char* argv[])
{
  if (argc == 3)
  {
    Bind bd;
    return bd.BindProc(argv[1], argv[2]);
  }
  else
  {
    printf("usage:%s lm_dict bind_dict.\n", argv[0]);
    return -1;
  }
}


