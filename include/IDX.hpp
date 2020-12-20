#ifndef IDX_HPP
#define IDX_HPP

#include <fcntl.h>
#include <unistd.h>

#include <vector>

#include "ErrExit.h"
#include "Image.hpp"

void getIdxHeaders(int fdInputFile,
    int& magicNumber, 
    int& numImages, 
    int& nomRows, 
    int& numCols);

std::vector<Image> getIdxData(const char *filename, 
    int pixel_size, 
    int read_upto);

#endif // IDX_HPP