import sys
import numpy as np
import copy
import dist as d
from itertools import chain 

from idx import *
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




def main(argv):

   # Get command line arguements
   inputFile, readUpto, queryFile, queryUpto, outputFile, labelDataFile, labelQueryFile = getCommandLineArgs(argv)



   # Read input file
   f = open(inputFile, "rb")
   magicnumber, numofimages, numrows, numcols = getIdxHeaders(f)

   if (readUpto is None) or (readUpto > numofimages):
      readUpto = numofimages

   ret = getIdxData(f, readUpto, numrows, numcols, 1)

   images = [Image(numrows, numcols, i+1) for i in range(readUpto)]
   for i in range(readUpto):
      images[i].pixels = ret[i]

   f.close()



   #Read query file
   f = open(queryFile, "rb")
   magicnumber, numofimages, numrows, numcols = getIdxHeaders(f)

   if (queryUpto is None) or (queryUpto > numofimages):
      queryUpto = numofimages

   ret = getIdxData(f, queryUpto, numrows, numcols, 1)

   queries = [Image(numrows, numcols, i+1) for i in range(queryUpto)]
   for i in range(queryUpto):
      queries[i].pixels = ret[i]

   f.close()



   #Read input label file
   f = open(labelDataFile, "rb")
   magicnumber, numlabels = getIdxLabelHeaders(f)

   # Scan labels from training set
   ret = getIdxLabels(f, readUpto, 1)

   labels = [0 for i in range(readUpto)]
   for i in range(readUpto):
      labels[i] = ret[i]

   f.close()



   #Read query label file
   f = open(labelQueryFile, "rb")
   magicnumber, numlabels = getIdxLabelHeaders(f)

   # Scan labels from training set
   ret = getIdxLabels(f, queryUpto, 1)

   query_labels = [0 for i in range (queryUpto)]
   for i in range(queryUpto):
      query_labels[i] = ret[i]

   f.close()

   

   # For each image in queries, find the top 10 nearest emd-distance neighbors
   k = 5
   n = 4*4

   avg_correct_manhattan = 0
   avg_correct_emd = 0

   for q in range(len(queries)):
      q_flat = list(chain.from_iterable(queries[q].pixels))
      
      print("\n\n***************************************************************")
      print("Query")
      queries[q].print()

      manhattan_distance_to_q = []
      emd_distance_to_q = []

      for i in images:
         i_flat = list(chain.from_iterable(i.pixels))

         manhattan_distance_to_q.append(d.dist(q_flat, i_flat, method='Manhattan'))
         emd_distance_to_q.append(d.dist(q_flat, i_flat, method='EMD', n=n))

      closest_manhattan_neighbors = sorted(range(len(manhattan_distance_to_q)), key=lambda i: manhattan_distance_to_q[i])[:k]
      closest_emd_neighbors = sorted(range(len(emd_distance_to_q)), key=lambda i: emd_distance_to_q[i])[:k]

      correct_manhattan = 0
      correct_emd = 0

      print("\n\nManhattan Closest Neibhors:")
      for i in closest_manhattan_neighbors:
         if labels[i] == query_labels[q]:
            correct_manhattan += 1
            #images[i].print()
      correct_manhattan /= 10
      print("correct_manhattan =", correct_manhattan)
      avg_correct_manhattan += correct_manhattan

      print("\n\nEmd Closest Neibhors:")
      for i in closest_emd_neighbors:
         if labels[i] == query_labels[q]:
            correct_emd += 1
            #images[i].print()
      correct_emd /= 10
      print("correct_emd =", correct_emd)
      avg_correct_emd += correct_emd
   
   avg_correct_manhattan /= queryUpto
   avg_correct_emd /= queryUpto



   f = open(outputFile, "w")
   f.write("Average Correct Search Results EMD: " + str(avg_correct_emd) + '\n')
   f.write("Average Correct Search Results Manhattan: " + str(avg_correct_manhattan))
   f.close()



if __name__ == "__main__":
   main(sys.argv[1:])
