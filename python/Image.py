import numpy as np

from math import ceil

IMAGE_NORM_FACTOR = 255.0



class Image:

    def __init__(self, numrows, numcols, id = 0):
        self.numrows = numrows
        self.numcols = numcols
        self.size = numrows * numcols
        # self.pixels = [0.0] * (self.size)
        self.pixels = np.zeros([numrows, numcols])
        self.id = id

    def scan(self, file, id = 0, normalize = True):
        self.id = id
        for i in range(self.numrows):
            for j in range(self.numcols):
                p = file.read(1)
                self.pixels[i][j] = int.from_bytes(p, "big")

                if normalize is True:
                    self.pixels[i][j] = self.pixels[i][j] / IMAGE_NORM_FACTOR

    def print(self):
        print("id: ", self.id)
        for i in range(self.numrows):
            for j in range(self.numcols):
                if self.pixels[i][j] != 0:
                    print(' *', end = '')
                else:
                    print('  ', end = '')
            print('')

    def printAcc(self):
        print("id: ", self.id)
        for i in range(self.numrows):
            for j in range(self.numcols):
                if self.pixels[i][j] == 0:
                    print('  0', end = ', ')
                else:
                    print('{:>3.0f}'.format(100*self.pixels[i][j]), end = ', ')
            print('')