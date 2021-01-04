import copy
import keras
import getopt
import matplotlib.pyplot as plt
import numpy as np
import unittest
import sys

from keras import models, layers, optimizers, losses, metrics
from Image import Image
from autoencoder import encoder, decoder




def usage():
    print('classification.py -d <training set> -d1 <training labels> -t <testset> -t1 <test labels> -model <autoencoder h5> -o <optional output file> -dupto <optional int> -tupto <optional int>')




def getCommandLineArgs(argv):
       
    inputFile=None
    inputLabels=None
    testFile=None
    testLabels=None
    encModel=None
    outputFile=None
    readUpto=None
    testUpto=None

    if (len(argv) != 10) and (len(argv) != 12) and (len(argv) != 14) and (len(argv) != 16):
        print("Wrong number of parameters!")
        usage()
        sys.exit()

    i = 0
    while i < len(argv):
        
        if argv[i] == "-d":
            i += 1
            inputFile = argv[i]
        elif argv[i] == "-d1":
            i += 1
            inputLabels = argv[i]
        elif argv[i] == "-t":
            i += 1
            testFile = argv[i]
        elif argv[i] == "-t1":
            i += 1
            testLabels = argv[i]
        elif argv[i] == "d1":
            i += 1
            inputLabels = argv[i]
        elif argv[i] == "-model":
            i += 1
            encModel = argv[i]
        elif argv[i] == "-o":
            i += 1
            outputFile = argv[i]
        elif argv[i] == "-dupto":
            i += 1
            readUpto = int(argv[i])
        elif argv[i] == "-tupto":
            i += 1
            testUpto = int(argv[i])
        else:
            print("Invalid parameter ", argv[i])
            sys.exit()
        
        i += 1

    return (inputFile, inputLabels, testFile, testLabels, encModel, readUpto, testUpto, outputFile)




def getExecParameters():

    numConvLay = int(input("Please, give number of convolutional layers for encoder... "))

    sizeConvFil = []

    for i in range(numConvLay):
        sizeConvFil.append(int(input("Please, give size of filter " + str(i) + " for encoder... ")))

    numConvFilPerLay = int(input("Please, give number of convolutional filters per layer for encoder... "))

    numNodes = int(input("Please, give number of nodes for fully connected layer after enoder... "))
    
    epochs = int(input("Please, give number of epochs... "))

    batchSize = int(input("Please, give number of batch size... "))

    return (numConvLay, sizeConvFil, numConvFilPerLay ,numNodes, epochs, batchSize)




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
    



def getIdxHeaders2(f):
   magicnumber = f.read(4)
   magicnumber = int.from_bytes(magicnumber, "big")

   numofimages = f.read(4)
   numofimages = int.from_bytes(numofimages, "big")

   return (magicnumber, numofimages)




def fullyConnected(encoder, numNodes):
    ret = layers.Flatten()(encoder)
    ret = layers.Dense(numNodes, activation='relu')(ret)
    ret = layers.Dropout(0.5)(ret)
    ret = layers.Dense(10, activation='softmax')(ret)
    return ret




def main(argv):

    #Get command line arguments
    inputFile, inputLabels, testFile, testLabels, encModel, readUpto, testUpto, outputFile = getCommandLineArgs(argv)

    #Get hyperparameters
    numConvLay, sizeConvFil, numConvFilPerLay, numNodes, epochs, batchSize = getExecParameters()

    # Open files
    inputFile = open(inputFile, "rb")
    inputLabels = open(inputLabels, "rb")
    testFile = open(testFile, "rb")
    testLabels = open(testLabels, "rb")

    # Get headers
    magicnumber1, numImages, numrows, numcols = getIdxHeaders(inputFile)
    magicnumber2, numImages2 = getIdxHeaders2(inputLabels)
    magicnumber3, numImagesTest, numrowsTrain, numcolsTrain = getIdxHeaders(testFile)
    magicnumber4, numImagesTest2 = getIdxHeaders2(testLabels)
    
    # Small check
    if numImages != numImages2:
        print("Size of train file and label file do not match!")
        sys.exit()

    # Small check 
    if numImagesTest != numImagesTest2:
        print("Size of train file and label file do not match!")
        sys.exit()

    # Small check 
    if (numrows != numrowsTrain) or (numcols != numcolsTrain):
        print("Image size in training file is not equal to image size in test file!")
        sys.exit()

    '''
    print(magicnumber1, numImages, numrows, numcols)
    print(magicnumber2, numImages2)
    print(magicnumber3, numImagesTest, numrowsTrain, numcolsTrain)
    print(magicnumber4, numImagesTest2)
    '''
    
    # Small check 
    if (readUpto is None) or (readUpto > numImages):
      readUpto = numImages

    # Small check 
    if (testUpto is None) or (testUpto > numImagesTest):
      testUpto = numImagesTest


    #
    # Scan images from training set
    #

    img = Image(numrows, numcols)
    images = []
    train_images = np.zeros([readUpto, numrows, numcols])

    print("Scanning training images...")
    
    for i in range(readUpto):
        img.scan(inputFile, i)
        images.append(copy.deepcopy(img))
        train_images[i] = copy.deepcopy(img.pixels)

    train_images_original = train_images
    print(readUpto, "training images scanned!")



    #
    # Scan labels from training set
    #

    train_labels = np.zeros(readUpto, dtype=np.uint8)

    print("Scanning training labels...")

    for i in range(readUpto):
        b = inputLabels.read(1)
        train_labels[i] = int(int.from_bytes(b, "big"))

    print(readUpto, "training labels scanned!")

    '''
    # Print
    for i in range(readUpto):
        print("Label =", train_labels[i])
        images[i].print()
        print('')
    '''

    # Convert to one-hot encoding
    one_hot_train_labels = keras.utils.to_categorical(train_labels)



    #
    # Scan images from test set
    #

    img = Image(numrows, numcols)
    images2 = []
    test_images = np.zeros([testUpto, numrows, numcols])

    print("Scanning testing images...")
    
    for i in range(testUpto):
        img.scan(testFile, i)
        images2.append(copy.deepcopy(img))
        test_images[i] = copy.deepcopy(img.pixels)

    print(readUpto, "testing images scanned!")



    #
    # Scan labels from test set
    #

    test_labels = np.zeros(testUpto, dtype=np.uint8)

    print("Scanning testing labels...")

    for i in range(testUpto):
        b = testLabels.read(1)
        test_labels[i] = int(int.from_bytes(b, "big"))

    print(readUpto, "testing labels scanned!")

    '''
    # Print
    for i in range(testUpto):
        print("Label =", test_labels[i])
        images2[i].print()
        print('')
    '''

    # Convert to one-hot encoding
    one_hot_test_labels = keras.utils.to_categorical(test_labels)

    

    #
    # Set up classificator model
    #

    # Define input shape
    input_img = keras.Input(shape = (numrows, numcols, 1))

    # Load autoencoder weights
    tempModel = models.Model(input_img, decoder(encoder(input_img, numConvLay, sizeConvFil, numConvFilPerLay), numConvLay, sizeConvFil, numConvFilPerLay))
    tempModel.compile(loss='mean_squared_error', optimizer= optimizers.RMSprop())
    tempModel.load_weights(encModel)

    # Define classificatior model
    classifier = models.Model(input_img, fullyConnected(encoder(input_img, numConvLay, sizeConvFil, numConvFilPerLay), numNodes))

    # Set weights in new model
    trainedLayers = numConvLay*(2*numConvFilPerLay + 1) + 1
    for l1, l2 in zip(classifier.layers[:trainedLayers], tempModel.layers[0:trainedLayers]):
        l1.set_weights(l2.get_weights())

    '''
    # Check validity of weights
    print("\n\nWeights #1")
    print(tempModel.get_weights()[0][1])
    print("\n\nWeights #2")
    print(classifier.get_weights()[0][1])
    '''

    # Set already trained weights to non trainable
    for layer in classifier.layers[0:trainedLayers]:
        layer.trainable = False

    # Compile classifier
    classifier.compile(loss=losses.categorical_crossentropy, optimizer=optimizers.Adam(),metrics=['accuracy'])
    
    # Print summary
    print(classifier.summary())



    # Split train data
    from sklearn.model_selection import train_test_split
    train_images, validation_images, train_labels, validation_labels = train_test_split(train_images, one_hot_train_labels, test_size=0.2, random_state=13)

    # Train the model
    history = classifier.fit(train_images, train_labels, batch_size=batchSize, epochs=epochs, verbose=1, validation_data=(validation_images, validation_labels))
    '''
    # Plot
    plt.plot(history.history['loss'])
    plt.plot(history.history['val_loss'])
    plt.title('model loss')
    plt.ylabel('loss')
    plt.xlabel('epoch')
    plt.legend(['train', 'val'], loc='upper left')
    plt.show()
    '''


    
    # Re train the whole model

    for layer in classifier.layers[0:trainedLayers]:
        layer.trainable = True
    
    classifier.compile(loss=losses.categorical_crossentropy, optimizer=optimizers.Adam(),metrics=['accuracy'])

    history2 = classifier.fit(train_images, train_labels, batch_size=batchSize, epochs=epochs, verbose=1, validation_data=(validation_images, validation_labels))

    # Plot
    plt.plot(history2.history['loss'])
    plt.plot(history2.history['val_loss'])
    plt.title('model loss')
    plt.ylabel('loss')
    plt.xlabel('epoch')
    plt.legend(['train', 'val'], loc='upper left')
    plt.show()
    



    # Predict some stuff
    predictions = classifier.predict(test_images)
    predictions = np.argmax(np.round(predictions), axis=1)

    # Print stats
    print('')
    from sklearn.metrics import classification_report
    target_names = ["Class {}".format(i) for i in range(10)]
    print(classification_report(test_labels, predictions, target_names=target_names))



    ans = input("Print some extra stats? (y/n) ...")

    if ans == "y":

        # Calculate correct labels
        correct = np.where(predictions == test_labels, predictions, -1)
        asdf = np.where(correct != -1)[0]
        print("\nFound", len(asdf), " correct labels")
        
        # Print first 10 correct
        j = 0
        print("Printing first 10 correct...")
        for i, c in enumerate(correct):
            if c != -1:
                print("correct[i] =", correct[i], ", test_labels[i] =", test_labels[i])
                j += 1
            if j == 10:
                break

        # Calculate incorrect labels
        incorrect = np.where(predictions != test_labels, predictions, -1)
        asdf2 = np.where(incorrect != -1)[0]
        print ("\nFound",  len(asdf2), " incorrect labels")

        # Print first 10 icorrect
        j = 0
        print("Printing first 10 incorrect...")
        for i, c in enumerate(incorrect):
            if c != -1:
                print("incorrect[i] =", incorrect[i], ", test_labels[i] =", test_labels[i])
                j += 1
            if j == 10:
                break




    predictions = classifier.predict(train_images_original)
    predictions = np.argmax(np.round(predictions), axis=1)
    write_predictions(predictions, outputFile)


    

def write_predictions(predictions, outputFile):
    if outputFile is None:
        return
    
    prediction_classes = {}
    for i in range(10):
        prediction_classes[i] = []
    
    for i in range(len(predictions)):
        prediction_classes[predictions[i]].append(i+1)

    with open(outputFile, "w") as f:
        
        for p in prediction_classes:
            f.write("CLUSTER-" + str(p) + " { size: " + str(len(prediction_classes[p])) + ", ")
        
            for img_num in prediction_classes[p]:
                f.write(str(img_num) + ", ")
        
            f.write("}\n")




class Test(unittest.TestCase):
    
    def test_write_predictions(self):
        predictions = [1, 4, 5, 6, 1, 4, 5, 6, 8, 8, 8]
        write_predictions(predictions, "test.out")
    


if __name__ == "__main__":
    if len(sys.argv) > 1 and (sys.argv[1] == "testit"):
        unittest.main(argv=['first-arg-is-ignored'], exit=False)
    else:
        main(sys.argv[1:])
