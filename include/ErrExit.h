#ifndef ERREXIT_H
#define ERREXIT_H

#include <stdio.h>
#include <stdlib.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                               } while (0)
                                   
#endif  // ERREXIT_H