#include "IDX.hpp"

using namespace std;

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




void getIdxLabelHeaders(int fdInputFile,
    int& magicNumber, 
    int& numLabels) {

    // Read magic number
    if (read(fdInputFile, &magicNumber, sizeof(int)) < ssize_t(sizeof(int)))
        errExit("read");
    magicNumber = be32toh(magicNumber);
    cout << "magic number = " << magicNumber << endl;

    // Read number of images
    if (read(fdInputFile, &numLabels, sizeof(int)) < ssize_t(sizeof(int)))
        errExit("read");
    numLabels = be32toh(numLabels);
    cout << "number of labels = " << numLabels << endl;

}




std::vector<Image> getIdxData(const char *filename, int pixel_size, int read_upto, int* img_size) {
    // Open file
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1)
        errExit("IDX.getIdxData.open(filename)");

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



std::vector<int> getIdxLabelData(const char *filename,  
    int read_upto) {

    // Open file
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1)
        errExit("IDX.getIdxLabelData.open(filename)");

    // Read IDX headers
    int magicNumber;
    int numLabels;
    getIdxLabelHeaders(fd, magicNumber, numLabels);

    std::vector<int> ret(numLabels, 0);

    // Read labels
    for (int i=0; i<read_upto; i++) 
        if (read(fd, &ret[i], sizeof(char)) < ssize_t(sizeof(char)))
            errExit("IDX.getIdxLabelData.read:");

    close(fd);
    return ret;
}