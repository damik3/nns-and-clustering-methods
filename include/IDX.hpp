#ifndef IDX_HPP
#define IDX_HPP

#include <unistd.h>

#include "ErrExit.h"

void getIdxHeaders(int fdInputFile,
    int& magicNumber, 
    int& numImages, 
    int& nomRows, 
    int& numCols);

#endif // IDX_HPP