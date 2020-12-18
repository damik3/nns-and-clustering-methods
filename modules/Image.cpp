#include "Image.hpp" 



Image::Image() : pixels(IMGSIZ, 255) {}



void Image::scan(int fd, int id) {

    this->id = id;

    for (int i=0; i<IMGSIZ; i++) {
        
        if (read(fd, &(this->pixels[i]), sizeof(PIXEL_T)) < ssize_t(sizeof(PIXEL_T)))
            errExit("read");
    }
}



void Image::print(std::ostream& os) const {

    if (id != -1)
        os << "(image number in dataset: " << id << ")" << std::endl;

    for (int i=0; i < IMGROWS; i++){
        for (int j=0; j < IMGCOLS; j++) {
            os << std::setw(3) << int(pixels[i*IMGROWS + j]);
            os << ", ";
        }

        os << std::endl;
    }
}



bool operator== (const Image& img1, const Image& img2) {
    for (int i=0; i<IMGSIZ; i++)
        if (img1.pixels[i] != img2.pixels[i])
            return false;

    return true;
}