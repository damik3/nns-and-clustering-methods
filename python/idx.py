import unittest
from Image import Image



def getIdxHeaders(f):
   magicnumber = f.read(4)
   magicnumber = int.from_bytes(magicnumber, "big")

   numofimages = f.read(4)
   numofimages = int.from_bytes(numofimages, "big")

   numrows = f.read(4)
   numrows = int.from_bytes(numrows, "big")

   numcols = f.read(4)
   numcols = int.from_bytes(numcols, "big")

   return (magicnumber, numofimages, numrows, numcols)




def getIdxLabelHeaders(f):

    magicnumber = f.read(4)
    magicnumber = int.from_bytes(magicnumber, "big")

    numlabels = f.read(4)
    numlabels = int.from_bytes(numlabels, "big")

    return (magicnumber, numlabels)




# Assumed that headers have been already read from file f
def getIdxData(f, num_elements, num_rows, num_cols, element_size):
    
    ret = [[[0] * num_cols for i in range(num_rows)] for i in range(num_elements)]

    for e in range(num_elements):
        for i in range(num_rows):
            for j in range(num_cols):
                ret[e][i][j] = f.read(element_size)
                ret[e][i][j] = int.from_bytes(ret[e][i][j], "big")

    return ret


    

# Assumed that headers have been already read from file f
def getIdxLabels(f, num_labels, label_size):

    ret = [0 for i in range(num_labels)]
    
    for i in range(num_labels):
        ret[i] = f.read(label_size)
        ret[i] = int.from_bytes(ret[i], "big")

    return ret
    



def print_upto(filename, upto):
    with open(filename, "rb") as f:
        magicnumber, numofimages, numrows, numcols = getIdxHeaders(f)
        if upto > numofimages:
            upto = numofimages
        img = Image(numrows, numcols)
        for i in range(upto):
            img.scan(f, id=i+1, normalize=False)
            img.print()




def print_img(filename, img_num):
    with open(filename, "rb") as f:
        magicnumber, numofimages, numrows, numcols = getIdxHeaders(f)
        img = Image(numrows, numcols)
        for i in range(numofimages):
            img.scan(f, id=i+1, normalize=False)
            if i == img_num-1:
                img.print() 
                return




class Test(unittest.TestCase):

    @classmethod
    def setUpClass(self):

        self.tests_folder = './test_input/'
        self.data_file_1 = self.tests_folder + 't10k-images-idx3-ubyte'
        self.label_file_1 = self.tests_folder + 't10k-labels-idx1-ubyte'


    # Just prints, does not actually test
    def test_1(self):

        read_upto = 10

        f = open(self.data_file_1, "rb")
        magicnumber, numofimages, numrows, numcols = getIdxHeaders(f)
        ret = getIdxData(f, read_upto, numrows, numcols, 1)
        f.close()
        
        images = [Image(numrows, numcols, i+1) for i in range(read_upto)]
        for i in range(read_upto):
            images[i].pixels = ret[i]
        

        f = open(self.label_file_1, "rb")
        magicnumber, numlabels = getIdxLabelHeaders(f)
        ret = getIdxLabels(f, read_upto, 1)
        f.close()

        labels = [0 for i in range(read_upto)]
        for i in range(read_upto):
            labels[i] = ret[i]

        # Print
        for i in range(read_upto):
            print("Label:", labels[i])
            images[i].print()




if __name__ == "__main__":
    unittest.main()