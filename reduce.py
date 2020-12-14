import copy
import keras
import getopt
import matplotlib.pyplot as plt
import numpy as np
import sys
import tensorflow as tf

from keras import models, layers, optimizers, losses, metrics
from Image import Image

AUTOENCODER_LATENT_VECTOR_SIZE = 10
AUTOENCODER_NORM_FACTOR = 65535
AUTOENCODER_OUT_COORD_SIZE = 2


def usage():
   print('autoencoder.py  –d <dataset>  -dupto <optional int>  -q <query file>  -qupto <optional int>  -o <output file>')




def getCommandLineArgs(argv):

   inputFile=None
   readUpto=None
   queryFile=None
   queryUpto=None
   outputFile=None

   if (len(argv) != 6) and (len(argv) !=8) and (len(argv) != 10):
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
      else:
         print("Invalid parameter ", argv[i])
         sys.exit()
      
      i += 1

   if inputFile is None or queryFile is None or outputFile is None:
      usage()
      sys.exit()

   return (inputFile, readUpto, queryFile, queryUpto, outputFile)





def getExecParams():

   numConvLay = int(input("Please, give number of convolutional layers... "))
   
   sizeConvFil = []
   for i in range(numConvLay):
      sizeConvFil.append(int(input("Please, give size of filter " + str(i) + "... ")))

   numConvFilPerLay = int(input("Please, give number of convolutional filters per layer... "))
   epochs = int(input("Please, give number of epochs... "))
   batchSize = int(input("Please, give number of batch size... "))

   return (numConvLay, sizeConvFil, numConvFilPerLay, epochs, batchSize)




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




def writeIdx(f, magicnumber, numimages, numrows, numcols, array):

   f.write((magicnumber).to_bytes(4, byteorder="big"))
   f.write((numimages).to_bytes(4, byteorder="big"))
   f.write((numrows).to_bytes(4, byteorder="big"))
   f.write((numcols).to_bytes(4, byteorder="big"))

   for row in array:
      for coor in row:
         f.write((coor).to_bytes(AUTOENCODER_OUT_COORD_SIZE, byteorder="big"))
   



def encoder(input_img, numConvLay, sizeConvFil, numConvFilPerLay):
   # input_img = 28 x 28 x 1

   ret = input_img

   for i in range(numConvLay):
      for _ in range(numConvFilPerLay):
         # ret = layers.Conv2D(sizeConvFil[i], (3, 3), activation='relu', padding='same')(ret)
         ret = layers.Conv2D(sizeConvFil[i], (3, 3), padding='same')(ret)
         ret = layers.BatchNormalization()(ret)
         ret = layers.Activation('relu')(ret)
      ret = layers.MaxPooling2D(pool_size=(2, 2))(ret)

   ret = layers.Flatten()(ret)
   ret = layers.Dense(AUTOENCODER_LATENT_VECTOR_SIZE, activation='relu')(ret)

   return ret




def decoder(latent_vector, numConvLay, sizeConvFil, numConvFilPerLay, img_numrows, img_numcols):

   ret = latent_vector

   d1 = int(img_numrows / (2 ** numConvLay))
   d2 = int(img_numcols / (2 ** numConvLay))
   d3 = int(sizeConvFil[-1])

   ret = layers.Dense(d1*d2*d3, activation='relu')(ret)
   ret = tf.reshape(ret, [-1, d1, d2, d3])

   for i in range(numConvLay):
      for _ in range(numConvFilPerLay):
         ret = layers.Conv2D(sizeConvFil[numConvLay - 1 - i], (3, 3), activation='relu', padding='same')(ret)
         ret = layers.BatchNormalization()(ret)
      ret = layers.UpSampling2D((2, 2))(ret)

   ret = layers.Conv2D(1, (3, 3), activation='sigmoid', padding='same')(ret)

   return ret




def main(argv):

   # Get command line arguements
   inputFile, readUpto, queryFile, queryUpto, outputFile = getCommandLineArgs(argv)

   # Get hyperparameters 
   numConvLay, sizeConvFil, numConvFilPerLay, epochs, batchSize = getExecParams()

   if numConvLay == 0:
      print("0 Convolutioinal layers? Lol bye!")
      sys.exit()


   #
   # Read input file
   #

   f = open(inputFile, "rb")

   #Get IDX Headers
   magicnumber, numofimages, numrows, numcols = getIdxHeaders(f)

   if (readUpto is None) or (readUpto > numofimages):
      readUpto = numofimages

   img = Image(numrows, numcols)
   images = []
   train_images = np.zeros([readUpto, numrows, numcols])

   print("Scanning input images...")
   
   for i in range(readUpto):
      img.scan(f, i)
      images.append(copy.deepcopy(img))
      train_images[i] = copy.deepcopy(img.pixels)

   f.close()

   print(readUpto, "images scanned!")



   #
   # Read query file
   #

   f = open(queryFile, "rb")

   #Get IDX Headers
   qmagicnumber, qnumofimages, qnumrows, qnumcols = getIdxHeaders(f)

   if (queryUpto is None) or (queryUpto > qnumofimages):
      queryUpto = qnumofimages

   img = Image(qnumrows, qnumcols)
   queries = []
   query_images = np.zeros([queryUpto, qnumrows, qnumcols])

   print("Scanning query images...")

   for i in range(queryUpto):
      img.scan(f, i)
      queries.append(copy.deepcopy(img))
      query_images[i] = copy.deepcopy(img.pixels)

   f.close()

   print(queryUpto, "images scanned!")
   

   # Split training set

   from sklearn.model_selection import train_test_split 

   train_X, valid_X, train_ground, valid_ground = train_test_split(train_images, 
                                                                  train_images,
                                                                  test_size=0.2,
                                                                  random_state=13)



   # Build model

   input_img = keras.Input(shape = (numrows, numcols, 1))

   autoencoder = models.Model(input_img, decoder(encoder(input_img, numConvLay, sizeConvFil, numConvFilPerLay), numConvLay, sizeConvFil, numConvFilPerLay, img.numrows, img.numcols))

   autoencoder.compile(loss='mean_squared_error', optimizer= optimizers.RMSprop())


   
   # Print summary
   print(autoencoder.summary())

   # Train
   history = autoencoder.fit(train_X, train_ground, batch_size=batchSize ,epochs=epochs, verbose=1, validation_data=(valid_X, valid_ground))



   # Build encoder 
   encoder_model = models.Model(input_img, encoder(input_img, numConvLay, sizeConvFil, numConvFilPerLay))
   
   # Load weights from autoencoder
   # trained_layers_num = numConvLay*(3*numConvFilPerLay + 1) + 1 + 2
   trained_layers_num = len(encoder_model.layers)
   for l1, l2 in zip(encoder_model.layers[0:trained_layers_num], autoencoder.layers[0:trained_layers_num]):
        l1.set_weights(l2.get_weights())
   
   # Compile encoder_model
   encoder_model.compile(loss=losses.categorical_crossentropy, optimizer=optimizers.Adam(),metrics=['accuracy'])


   
   # Predict
   predictions = encoder_model.predict(query_images)
   


   #
   # Normalize predictions
   #

   # predictions = [[0.1, 500.6, 1000.0], [100.8, 250.1, 750.6], [150.0, 500.2, 100.1]]

   # Find min and max
   norm_max = max([max(p) for p in predictions])
   
   # Edge case, not supposed to happen
   if norm_max == 0:
      norm_max = 1

   norm_min = min([min(p) for p in predictions])
   
   print("\n\nmax:", norm_max, "min:", norm_min, end="\n\n")

   # Normalize
   normalized_predictions = [[] for p in predictions]
   for i in range(len(predictions)):
      normalized_predictions[i] = [int(round(((coord-norm_min)/(norm_max-norm_min)) * AUTOENCODER_NORM_FACTOR)) for coord in predictions[i]]

   
   # for i in range(len(predictions)):
      # print("predictions[i]:", predictions[i], "\n normalized_predictions[i]:", normalized_predictions[i], end='\n\n')



   #
   # Write predictions to output file
   #

   f = open(outputFile, "wb")
   writeIdx(f, 666, queryUpto, 1, AUTOENCODER_LATENT_VECTOR_SIZE, normalized_predictions)
   f.close()



   #
   # Check first n vectors
   #
   n = 10

   f = open(outputFile, "rb")
   omagicnumber, onumofimages, onumrows, onumcols = getIdxHeaders(f)

   for i in range(n):
      for j in range(onumrows*onumcols):
         val = f.read(AUTOENCODER_OUT_COORD_SIZE)
         val = int.from_bytes(val, "big")
         print(val, end=' ')
      print()

   f.close()

   # Print some predictions

   print("Printing 10 first images with their latent vectors and normalized predictions...")

   for i in range(10):
      images[i].print()
      print(predictions[i])   
      print(normalized_predictions[i])



   # Graph loss on train and loss on validation set
   
   ans = input("Show graph loss on training and validation set? (y/n) ... ")
   
   if ans == 'y':
      plt.plot(history.history['loss'])
      plt.plot(history.history['val_loss'])
      plt.title('model loss')
      plt.ylabel('loss')
      plt.xlabel('epoch')
      plt.legend(['train', 'val'], loc='upper left')
      plt.show()
            


   # Print original and autoencoded images, for the first n imges

   n = 10

   ans = input("Show original and autoencoded images, for the first " + str(n) + " images of validation set? (y/n) ... ")

   if ans == 'y':
      decoded_imgs = autoencoder.predict(valid_X)

      plt.figure(figsize=(20, 4))
      for i in range(1, n + 1):
         # Display original
         ax = plt.subplot(2, n, i)
         plt.imshow(valid_X[i].reshape(28, 28))
         plt.gray()
         ax.get_xaxis().set_visible(False)
         ax.get_yaxis().set_visible(False)

         # Display reconstruction
         ax = plt.subplot(2, n, i + n)
         plt.imshow(decoded_imgs[i].reshape(28, 28))
         plt.gray()
         ax.get_xaxis().set_visible(False)
         ax.get_yaxis().set_visible(False)
      plt.show()




if __name__ == "__main__":
   main(sys.argv[1:])


