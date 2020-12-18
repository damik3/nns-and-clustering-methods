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