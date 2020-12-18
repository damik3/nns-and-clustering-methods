#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <unistd.h>
#include <sys/types.h>

#include <iostream>
#include <iomanip>      // std::setw
#include <vector>

#include "ErrExit.h"

// Each image is 28x28 = 784 pixels
#define IMGROWS 28
#define IMGCOLS 28
#define IMGSIZ IMGROWS * IMGCOLS

// The actual type of a pixel
#define PIXEL_T unsigned char

struct Image {

    std::vector<PIXEL_T> pixels;
    int id;

    Image();
    void scan(int fd, int id = -1);
    void print(std::ostream& os = std::cout) const;

};

bool operator== (const Image& img1, const Image& img2);

#endif // IMAGE_HPP