import os
import sys
import math
import struct as st
import numpy as np
import keras
import sklearn
import matplotlib.pyplot as plt #Graph
from keras import backend as k
from keras import layers, optimizers, losses, metrics
from keras.models import Model #ANN architecture
from keras.layers import Input, Dense, Conv2D, MaxPooling2D, UpSampling2D, BatchNormalization, Dropout, Flatten, Reshape
from sklearn.model_selection import train_test_split


dataset = ""
queryset = ""
output_dataset_file = ""
output_query_file = ""
flag = 4

if len(sys.argv) != 9:
    print("Usage: $python reduce.py –d <dataset> -q <queryset> -od <output_dataset_file> -oq <output_query_file>")
    sys.exit()

for i in range(len(sys.argv)):
    if sys.argv[i] == "-d":
        dataset = sys.argv[i+1]
        flag-=1
    elif sys.argv[i] == "-q":
        queryset = sys.argv[i+1]
        flag-=1
    elif sys.argv[i] == "-od":
        output_dataset_file = sys.argv[i+1]
        flag-=1
    elif sys.argv[i] == "-oq":
        output_query_file = sys.argv[i+1]
        flag-=1
        

print(dataset, " ", queryset, " ", output_dataset_file, " ", output_query_file, " ",)

if flag!=0:
	print("Usage: $python reduce.py –d <dataset> -q <queryset> -od <output_dataset_file> -oq <output_query_file>")
	sys.exit()



# trainfilename = {'images' : './data/train-images.idx3-ubyte' ,'labels' : './data/train-labels.idx1-ubyte'}
# testfilename = {'images' : './data/t10k-images.idx3-ubyte' ,'labels' : './data/t10k-labels.idx1-ubyte'}

with open(dataset,'rb') as f:
# with open(dataset,'rb') as f:
    magic, size = st.unpack(">II", f.read(8))
    nrows, ncols = st.unpack(">II", f.read(8))
    train_images = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
    train_images = train_images.reshape((size, nrows, ncols))
print(train_images.shape)


# with open('./data/train-labels.idx1-ubyte','rb') as f:
#     magic, size = st.unpack(">II", f.read(8))
#     # nrows, ncols = st.unpack(">II", f.read(8))
#     train_labels = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
#     train_labels = train_labels.reshape((size))
# print(train_labels.shape)


with open(queryset,'rb') as f:
    magic, size = st.unpack(">II", f.read(8))
    nrows, ncols = st.unpack(">II", f.read(8))
    test_images = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
    test_images = test_images.reshape((size, nrows, ncols))
print(test_images.shape)

# with open('./data/t10k-labels.idx1-ubyte','rb') as f:
#     magic, size = st.unpack(">II", f.read(8))
#     # nrows, ncols = st.unpack(">II", f.read(8))
#     test_labels = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
#     test_labels = test_labels.reshape((size))
# print(test_labels.shape)

train_images = (train_images/255) #normalize
test_images = (test_images/255) #normalize

train_images = np.reshape(train_images, (len(train_images), 28, 28, 1))

print("train_images", train_images.shape) #60.000 rows

loss = []
val_loss = []
text_values = []

usr_input = '1'
while (usr_input == '1'):
    
    #input from user
    layers, batch_size, epochs = input("#layers batch_size epochs: ").split()

    #accept only digits
    if  (not(layers.isdigit()) or not(batch_size.isdigit()) or not(epochs.isdigit())):
        print("wrong input")
        sys.exit()
    print()

    layersNum = int(layers)
    batchSize = int(batch_size)
    epochsNum = int(epochs)
    input_img = Input(shape=(28, 28, 1)) 

    #input = 28 x 28 x 1 (wide and thin)
    x = input_img
    pooling_counter = 0
    latent = 0

    #keep every layer's info to use it in the decoder (mirror)
    layer_info = []

    for i in range(0, layersNum):

        # Ask for number of filters ,filtersize and if you want pooling
        filter_flag = 1
        while filter_flag :
            filters, filter_size  = input("#total_filters #filter_size : ").split()
            #accept only digits
            if  (not(filters.isdigit()) or not(filter_size.isdigit())):
                print("please enter ints")
            else:
                filtersNum = int(filters)
                filterSize = int(filter_size)
                filter_flag = 0

        pooling_flag = 1
        pool = 'n'
        while pooling_flag :
            pool = input("Do you want to add pooling to this layer? [Y/n]")
            # pool = 'Y'
            # Accept only Y or n
            if ( pool != 'Y' and pool != 'n' ) :
                print("please enter Y (yes) or n (no)")
            else :
                if pooling_counter == 2 and pool == 'Y' :
                    print("already added max amount of pooling")
                    pool = 'n'
                else :
                    if( pool == 'Y' ) :
                        pooling_counter += 1
                pooling_flag = 0

        # For each layer of the encoder keep the parameters used so we can use them on the right layers on the decoder
        info = [filtersNum,filterSize,pool]
        layer_info.insert(0,info)

        #encoder
        x = Conv2D(filtersNum, (filterSize, filterSize), activation='relu', padding='same' )(x)
        x = BatchNormalization()(x)
        if pool == 'Y' : #only in 2 first layers
            x = MaxPooling2D(pool_size=(2, 2))(x) 

    latent = int(input("Please give latent dimension : "))

    shape_before_flattening = k.int_shape(x)[1:]
    flat = Flatten()(x)
    lat = Dense(latent)(flat)
    

    # encoder = Model(input_img, lat)

    #decoder
    x = Dense(np.prod(shape_before_flattening))(lat)
    x = Reshape(shape_before_flattening)(x)

    for i in range(0,layersNum):

        info = layer_info[i]
        filtersNum = info[0]
        filterSize = info[1]
        pool = info[2]

        #decoder
        x = Conv2D(filtersNum, (filterSize, filterSize), activation='relu', padding='same')(x)
        x = BatchNormalization()(x)
        if pool == 'Y' :
            x = UpSampling2D((2,2))(x) 


    decoded = Conv2D(1, (filterSize, filterSize), activation='sigmoid', padding='same')(x) # 28 x 28 x 1

    autoencoder = Model(input_img, decoded)
    autoencoder.compile(loss = 'mean_squared_error', optimizer = optimizers.RMSprop())

    autoencoder.summary()

    train_X,valid_X,train_ground,valid_ground = train_test_split(
        train_images,
        train_images,
        test_size=0.2,
        random_state=13
        )

    #fit model with given params
    autoencoder_train = autoencoder.fit(train_X, train_ground, 
        batch_size=batchSize,
        epochs=epochsNum,
        verbose=1,
        validation_data=(valid_X, valid_X)
        )


    encoder = Model(input_img, lat)
 

    # print(autoencoder_train.history.keys())
    loss.append(autoencoder_train.history['loss'][-1])
    val_loss.append(autoencoder_train.history['val_loss'][-1])
    
    str_text = ""
    str_num = ""
    str_size = ""
    for i in range(layersNum-1,-1,-1):
        info = layer_info[i]
        filtersNum = info[0]
        filterSize = info[1]
        str_num = str_num + " " + str(filtersNum)
        str_size = str_size + " " + str(filterSize)
    strr = "layers: %d\n fNum: %s\n fSize: %s\n batch_size: %d\nepochs: %d\n latent: %d\n" % (layersNum, str_num, str_size, batchSize, epochsNum, latent)
    text_values.append(strr)    
    # print(autoencoder_train.history['loss'])
    # print(autoencoder_train.history['val_loss'][-1])

    print("Options:")
    print("Press 1 to repeat with different hyperparameters")
    print("Press 2 to plot")
    print("Press 3 to save output files")
    usr_input = input("Input: ")

    if usr_input == '2':
        print("plot")

        decoded_imgs = autoencoder.predict(test_images)
        n = 10

        plt.figure(figsize=(20, 4))
        for i in range(n):
            # display original
            ax = plt.subplot(2, n, i + 1)
            plt.imshow(test_images[i].reshape(28, 28))
            plt.gray()
            ax.get_xaxis().set_visible(False)
            ax.get_yaxis().set_visible(False)

            # display reconstruction
            ax = plt.subplot(2, n, i+1+n)
            plt.imshow(decoded_imgs[i].reshape(28, 28))
            plt.gray()
            ax.get_xaxis().set_visible(False)
            ax.get_yaxis().set_visible(False)
        plt.show()

        plt.plot(autoencoder_train.history['loss'])
        plt.plot(autoencoder_train.history['val_loss'])
        plt.title('model loss')
        plt.ylabel('loss')
        plt.xlabel('epoch')
        plt.legend(['train', 'val'], loc='upper left')
        plt.show()


        x_values = np.arange(0, len(text_values), 1)

        plt.figure(figsize=(len(text_values)*1.5, 4))
        plt.plot(loss)
        plt.plot(val_loss)
        plt.title('model loss')
        plt.ylabel('loss')
        plt.xlabel('hyperparameters')
        plt.legend(['train', 'val'], loc='upper left')
        plt.xticks(x_values, text_values)
        plt.show()

    elif usr_input == '3':
        # print("save model")

        # autoencoder.save('autoencoder.h5')

        # # save encoder part for classifier.py
        # path = input("Give the path in which you want to save the model: ")
        # print(path)
        # if not os.path.exists(path):
        #     print('save directories...', flush=True)
        #     os.makedirs(path)
        # encoder.save(path + '/latent.h5')

        #train_images->new dimension
        compressed = encoder.predict(train_images)

        compressed = compressed.astype(float)
        flat_list = [item for sublist in compressed for item in sublist]
        max1 = max(flat_list)
        min1 = min(flat_list)

        for i in range(len(compressed)):
            for j in range(latent):
                compressed[i][j] = (compressed[i][j] - min1 ) * (25500 // (max1 - min1))
            # print(compressed[i])

        compressed = compressed.astype(int)

        output_file = open(output_dataset_file, 'wb')
        output_file.write((2051).to_bytes(4, 'big'))
        output_file.write((len(train_images)).to_bytes(4, 'big'))
        output_file.write((1).to_bytes(4, 'big'))
        output_file.write((latent).to_bytes(4, 'big'))
        for i in compressed:
            for j in i:
                output_file.write(j.item().to_bytes(2, 'big'))
        output_file.close()

        with open(output_dataset_file, 'rb') as file:
            print(int.from_bytes(file.read(4), byteorder='big'))
            print(int.from_bytes(file.read(4), byteorder='big'))
            print(int.from_bytes(file.read(4), byteorder='big'))
            print(int.from_bytes(file.read(4), byteorder='big'))
            for i in range(10):
                print(int.from_bytes(file.read(2), byteorder='big'))

        #test_images->new dimension
        compressed_test = encoder.predict(test_images)

        compressed_test = compressed_test.astype(float)
        flat_list = [item for sublist in compressed_test for item in sublist]
        max_test = max(flat_list)
        min1_test = min(flat_list)

        for i in range(len(compressed_test)):
            for j in range(latent):
                compressed_test[i][j] = (compressed_test[i][j] - min1_test ) * (25500 // (max_test - min1_test))
            # print(compressed_test[i])

        compressed_test = compressed_test.astype(int)

        output_file_test = open(output_query_file, 'wb')
        output_file_test.write((2051).to_bytes(4, 'big'))
        output_file_test.write((len(test_images)).to_bytes(4, 'big'))
        output_file_test.write((1).to_bytes(4, 'big'))
        output_file_test.write((latent).to_bytes(4, 'big'))
        for i in compressed_test:
            for j in i:
                output_file_test.write(j.item().to_bytes(2, 'big'))
        output_file_test.close()

        with open(output_query_file, 'rb') as file:
            print(int.from_bytes(file.read(4), byteorder='big'))
            print(int.from_bytes(file.read(4), byteorder='big'))
            print(int.from_bytes(file.read(4), byteorder='big'))
            print(int.from_bytes(file.read(4), byteorder='big'))
            for i in range(10):
                print(int.from_bytes(file.read(2), byteorder='big'))



