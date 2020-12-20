#include "Image.hpp" 



Image::Image(int num_rows1, int num_cols1, int pixel_size1) 
    : pixels(num_cols1*num_rows1, 0), num_rows(num_rows1), num_cols(num_cols1), size(num_cols1*num_rows1), pixel_size(pixel_size1) {
        std::cout << num_rows1 << ", " << num_cols  << ", " << size  << ", " << pixel_size << std::endl;
    }



void Image::scan(int fd, int id) {

    this->id = id;

    for (int i=0; i<this->size; i++) {
        if (read(fd, &(this->pixels[i]), this->pixel_size) < ssize_t(this->pixel_size))
            errExit("Image.scan.read");
        if (this->pixel_size == sizeof(short))
            this->pixels[i] = be16toh(this->pixels[i]);
    }
}



void Image::print(std::ostream& os) const {

    if (id != -1)
        os << "(image number in dataset: " << id << ")" << std::endl;

    for (int i=0; i < this->num_rows; i++){
        for (int j=0; j < this->num_cols; j++) {
            os << std::setw(3) << int(pixels[i*(this->num_rows) + j]);
            os << ", ";
        }

        os << std::endl;
    }
}



bool operator== (const Image& img1, const Image& img2) {
    
    if (img1.size != img2.size)
        return false;

    for (int i=0; i < (img1.size); i++)
        if (img1.pixels[i] != img2.pixels[i])
            return false;

    return true;
}