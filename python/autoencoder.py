import copy
import keras
import getopt
import matplotlib.pyplot as plt
import numpy as np
import sys

from keras import models, layers, optimizers, losses, metrics
from Image import Image




def usage():
   print('autoencoder.py  –d <dataset>  -dupto <optional int>')




def getCommandLineArgs(argv):

   inputFile=None
   readUpto=None

   if (len(argv) != 2) and (len(argv) != 4):
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
      else:
         print("Invalid parameter ", argv[i])
         sys.exit()
      
      i += 1

   if inputFile is None:
      usage()
      sys.exit()

   return (inputFile, readUpto)





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

   return ret




def decoder(conv, numConvLay, sizeConvFil, numConvFilPerLay):

   ret = conv

   for i in range(numConvLay):
      for _ in range(numConvFilPerLay):
         ret = layers.Conv2D(sizeConvFil[numConvLay - 1 - i], (3, 3), activation='relu', padding='same')(ret)
         ret = layers.BatchNormalization()(ret)
      ret = layers.UpSampling2D((2, 2))(ret)

   ret = layers.Conv2D(1, (3, 3), activation='sigmoid', padding='same')(ret)

   return ret




def main(argv):

   # Get command line arguements
   inputFile, readUpTo = getCommandLineArgs(argv)

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

   if (readUpTo is None) or (readUpTo > numofimages):
      readUpTo = numofimages

   img = Image(numrows, numcols)
   images = []
   train_images = np.zeros([readUpTo, numrows, numcols])

   print("Scanning images...")
   
   for i in range(readUpTo):
      img.scan(f, i)
      images.append(copy.deepcopy(img))
      train_images[i] = copy.deepcopy(img.pixels)

   print(readUpTo, " images scanned!")



   # Split training set

   from sklearn.model_selection import train_test_split 

   train_X, valid_X, train_ground, valid_ground = train_test_split(train_images, 
                                                                  train_images,
                                                                  test_size=0.2,
                                                                  random_state=13)



   # Build model

   input_img = keras.Input(shape = (numrows, numcols, 1))

   autoencoder = models.Model(input_img, decoder(encoder(input_img, numConvLay, sizeConvFil, numConvFilPerLay), numConvLay, sizeConvFil, numConvFilPerLay))

   autoencoder.compile(loss='mean_squared_error', optimizer= optimizers.RMSprop())



   # Print summary
   print(autoencoder.summary())

   # Train
   history = autoencoder.fit(train_X, train_ground, batch_size=batchSize ,epochs=epochs, verbose=1, validation_data=(valid_X, valid_ground))



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



   # Ask to save model
   
   ans = input("Save the current model? (y/n) ... ")

   if ans == 'y':
      # filename = input("Pease, give name of file for saving... ")

      name = str(numConvLay)
      for i in range(numConvLay):
         name += "-" + str(sizeConvFil[i])
      name += "-" + str(numConvFilPerLay)
      name += "-" + str(epochs)
      name += "-" + str(batchSize)

      filename = "weights-" + name
      print("filename =", filename)

      autoencoder.save_weights(filename + ".h5")
      print("Model saved!")

      #print("\n\nWeights ")
      #print(autoencoder.get_weights()[0][1])



if __name__ == "__main__":
   main(sys.argv[1:])


