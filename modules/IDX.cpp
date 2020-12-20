#include "IDX.hpp"

void getIdxHeaders(int fdInputFile, int& magicNumber, int& numImages, int& numRows, int& numColumns) {
    // Read magic number
    if (read(fdInputFile, &magicNumber, sizeof(int)) < ssize_t(sizeof(int)))
        errExit("read");
    magicNumber = be32toh(magicNumber);
    // cout << "magic number = " << magicNumber << endl;

    // Read number of images
    if (read(fdInputFile, &numImages, sizeof(int)) < ssize_t(sizeof(int)))
        errExit("read");
    numImages = be32toh(numImages);
    // cout << "number of images = " << numImages << endl;

    // Read number of rows
    if (read(fdInputFile, &numRows, sizeof(int)) < ssize_t(sizeof(int)))
        errExit("read");
    numRows = be32toh(numRows);
    // cout << "number of rows = " << numRows << endl;

    // Read number of columns
    if (read(fdInputFile, &numColumns, sizeof(int)) < ssize_t(sizeof(int)))
        errExit("read");
    numColumns = be32toh(numColumns);
    // cout << "number of columns = " << numColumns << endl;
}



std::vector<Image> getIdxData(const char *filename, int pixel_size, int read_upto, int* img_size) {
    // Open file
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1)
        errExit("IDX.open(filename)");

    // Read IDX headers
    int magicNumber;
    int numImages;
    int numRows;
    int numColumns;
    getIdxHeaders(fd, magicNumber, numImages, numRows, numColumns);

    Image img(numRows, numColumns, pixel_size);
    std::vector<Image> ret;

    // Read images
    for (int i=0; i<read_upto; i++) {
        img.scan(fd, i+1);
        ret.push_back(img);
    }

    close(fd);

    *img_size = numRows*numColumns;
    return ret;
}