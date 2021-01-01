import sys
import numpy as np
import copy
import dist as d

from Image import Image




def usage():
   print('Usage: search.py  –d <dataset>  -dupto <optional int>  -q <query file>  -qupto <optional int>  -o <output_file> -l1 <labels of input dataset> -l2 <labels of query dataset> -EMD')




def getCommandLineArgs(argv):

   inputFile=None
   readUpto=None
   queryFile=None
   queryUpto=None
   outputFile=None
   labelDataFile=None
   labelQueryFile=None

   if (len(argv) != 12) and (len(argv) !=14) and (len(argv) != 15):
      print("Wrong number of parameters!")
      usage()
      sys.exit()

   i = 0
   while i < len(argv):
      
      if argv[i] == "-d":
         i += 1
         inputFile = argv[i]
      elif argv[i] == "-dupto":
         i += 1
         readUpto = int(argv[i])
      elif argv[i] == "-q":
         i += 1
         queryFile = argv[i]
      elif argv[i] == "-qupto":
         i += 1
         queryUpto = int(argv[i])
      elif argv[i] == "-o":
         i += 1
         outputFile = argv[i]
      elif argv[i] == "-l1":
         i += 1
         labelDataFile = argv[i]
      elif argv[i] == "-l2":
         i += 1
         labelQueryFile = argv[i]
      elif argv[i] == "-EMD":
         i += 1
      else:
         print("Invalid parameter ", argv[i])
         sys.exit()
      
      i += 1

   if inputFile is None or queryFile is None or outputFile is None or labelQueryFile is None or labelDataFile is None:
      usage()
      sys.exit()

   return (inputFile, readUpto, queryFile, queryUpto, outputFile, labelDataFile, labelQueryFile)




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




def main(argv):

    # Get command line arguements
    inputFile, readUpto, queryFile, queryUpto, outputFile, labelDataFile, labelQueryFile = getCommandLineArgs(argv)

    # Read input file

    f = open(inputFile, "rb")


    #Get IDX Headers
    magicnumber, numofimages, numrows, numcols = getIdxHeaders(f)

    if (readUpto is None) or (readUpto > numofimages):
      readUpto = numofimages

    img = Image(numrows, numcols)
    images = []
    train_images = np.zeros([readUpto, numrows, numcols])

    for i in range(readUpto):
      img.scan(f, i)
      images.append(copy.deepcopy(img))
      train_images[i] = copy.deepcopy(img.pixels)

    f.close()


    #Read query file

    f = open(queryFile, "rb")


    magicnumber, numofimages, numrows, numcols = getIdxHeaders(f)

    if (queryUpto is None) or (queryUpto > numofimages):
      queryUpto = numofimages

    img = Image(numrows, numcols)
    queries = []
    query_images = np.zeros([queryUpto, numrows, numcols])

    for i in range(queryUpto):
       img.scan(f, i)
       queries.append(copy.deepcopy(img))
       query_images[i] = copy.deepcopy(img.pixels)

    f.close()

    #Read dataset label file

    f = open(labelDataFile, "rb")

    magicnumber, numlabels = getIdxLabelHeaders(f)

    
    # Scan labels from training set
    

    train_labels = np.zeros(readUpto, dtype=np.uint8)


    for i in range(readUpto):
        b = f.read(1)
        train_labels[i] = int(int.from_bytes(b, "big"))


    f.close()

    #Read query label file

    f = open(labelQueryFile, "rb")

    magicnumber, numlabels = getIdxLabelHeaders(f)

    
    # Scan labels from query set
    

    query_labels = np.zeros(readUpto, dtype=np.uint8)


    for i in range(queryUpto):
        b = f.read(1)
        query_labels[i] = int(int.from_bytes(b, "big"))
    
    f.close()

    ret = d.dist(images[1],queries[1],"EMD",4)
    print(ret)


if __name__ == "__main__":
   main(sys.argv[1:])
