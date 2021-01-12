import sys
import struct as st
import numpy as np
import keras
import os
import sklearn
import matplotlib.pyplot as plt #Graph
from keras import layers, optimizers, losses, metrics
from keras.models import Model #ANN architecture
from keras.layers import Input, Dense, Conv2D, MaxPooling2D, UpSampling2D, BatchNormalization, Dropout , Flatten
from keras.utils import plot_model
from sklearn.model_selection import train_test_split
from keras.utils import to_categorical
from sklearn.metrics import classification_report


trainingSet = ""
trainingLabels = ""
testSet = ""
testLabels = ""
model = ""
flag = 5


# Read arguments from command line

#python classification.py -d ./data/train-images.idx3-ubyte -dl ./data/train-labels.idx1-ubyte -t ./data/t10k-images.idx3-ubyte -tl ./data/t10k-labels.idx1-ubyte -model ./autoencoder.h5
if len(sys.argv) != 11:
	sys.exit()

for i in range(len(sys.argv)):
	if sys.argv[i] == "-d":
		trainingSet = sys.argv[i+1]
		flag-=1
	elif sys.argv[i] == "-dl":
		trainingLabels = sys.argv[i+1]
		flag-=1
	elif sys.argv[i] == "-t":
		testSet = sys.argv[i+1]
		flag-=1
	elif sys.argv[i] == "-tl":
		testLabels = sys.argv[i+1]
		flag-=1
	elif sys.argv[i] == "-model":
		model = sys.argv[i+1]
		flag-=1

if flag!=0:
	sys.exit()

# Open and read files

trainfilename = {'images' : trainingSet, 'labels' : trainingLabels}
testfilename = {'images' : testSet, 'labels' : testLabels}

with open(trainfilename['images'],'rb') as f:
# with open(dataset,'rb') as f:
    magic, size = st.unpack(">II", f.read(8))
    nrows, ncols = st.unpack(">II", f.read(8))
    train_images = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
    train_images = train_images.reshape((size, nrows, ncols))
print(train_images.shape)


with open(trainfilename['labels'],'rb') as f:
    magic, size = st.unpack(">II", f.read(8))
    # nrows, ncols = st.unpack(">II", f.read(8))
    train_labels = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
    train_labels = train_labels.reshape((size))
print(train_labels.shape)


with open(testfilename['images'],'rb') as f:
    magic, size = st.unpack(">II", f.read(8))
    nrows, ncols = st.unpack(">II", f.read(8))
    test_images = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
    test_images = test_images.reshape((size, nrows, ncols))
print(test_images.shape)

with open(testfilename['labels'],'rb') as f:
    magic, size = st.unpack(">II", f.read(8))
    # nrows, ncols = st.unpack(">II", f.read(8))
    test_labels = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
    test_labels = test_labels.reshape((size))
print(test_labels.shape)

train_set = train_images
# Split train dataset for training
train_images,val_images,train_labels,val_labels = train_test_split(train_images,train_labels,test_size=0.2,random_state=13)

train_labels = to_categorical(train_labels)
test_labels_cat = to_categorical(test_labels)
val_labels = to_categorical(val_labels)

# Initialize lists for the hyperparameters of each iteration
loss = []
accuracy = []
test_loss = []
acc = []
test_acc = []
precision = []
recall = []
f1 = []
text_values = []

#Models list keeps the model of each iteration eith different hyperparameters
models = []
iterations = 0
usr_input = '1'
while (usr_input == '1'):
	iterations += 1
	# First load the encoder model from autoencoder.py
	encoder = keras.models.load_model(model)
	# Add layers for classifier
	params_flag = 1
	while params_flag :
		filters  = input("Enter filters for fc layer: ")
		#accept only digits
		if  (not(filters.isdigit()) ):
			print("please enter int")
		else:
			filtersNum = int(filters)
			params_flag = 0
	flat = Flatten()((encoder.layers[-1].output))
	den = Dense(filters, activation='relu')(flat)
	dropped = Dropout(0.2)(den)
	out = Dense(10, activation='softmax')(dropped)
	merged = Model(encoder.layers[0].input,outputs=[out])

	#Deactivate the encoder's layers so we can train only the classifiers layers 
	for layer in merged.layers[0:len(encoder.layers)]:
		layer.trainable = False

	# Read hyperparameters
	params_flag = 1
	while params_flag :
		batch_size  = input("Enter batchsize for classifier training without encoder : ")
		epochs  = input("Enter epochs for classifier training without encoder : ")
		#accept only digits
		if  (not(batch_size.isdigit()) or not(epochs.isdigit())):
			print("please enter ints")
		else:
			batch_size = int(batch_size)
			epochs = int(epochs)
			params_flag = 0

	# Compile and train the model without the encoder
	merged.compile(loss=keras.losses.categorical_crossentropy, optimizer=keras.optimizers.Adam(),metrics=['accuracy'])
	classifier_train = merged.fit(train_images, train_labels, batch_size,epochs,verbose=1,validation_data=(val_images, val_labels))

	#Activate the encoder's layers
	for layer in merged.layers[0:len(encoder.layers)]:
		layer.trainable = True

	params_flag = 1
	while params_flag :
		batch_size_full  = input("Enter batchsize for classifier training with encoder : ")
		epochs_full  = input("Enter epochs for classifier training with encoder : ")
		#accept only digits
		if  (not(batch_size_full.isdigit()) or not(epochs_full.isdigit())):
			print("please enter ints")
		else:
			batch_size_full = int(batch_size_full)
			epochs_full = int(epochs_full)
			params_flag = 0

	# Compile and train the model with the encoder
	merged.compile(loss=keras.losses.categorical_crossentropy, optimizer=keras.optimizers.Adam(),metrics=['accuracy'])

	classifier_train = merged.fit(train_images, train_labels, batch_size_full,epochs_full,verbose=1,validation_data=(val_images, val_labels))

	#Add current model to our models list
	models.append(merged)
	loss.append(classifier_train.history['loss'][-1])
	acc.append(classifier_train.history['accuracy'][-1])

	# Evaluate the model
	test_eval = merged.evaluate(test_images, test_labels_cat , verbose=0)

	# Predict with trained model
	predicted_classes = merged.predict(test_images)
	predicted_classes = np.argmax(np.round(predicted_classes),axis=1)

	#Save accuracy,loss,f1,precision,recall
	target_names = ["Class {}".format(i) for i in range(10)]
	report = classification_report(test_labels, predicted_classes, target_names=target_names,output_dict=True)
	strr = "fNum: %s\nbatch_size(only class): %d\nepochs(only class): %d\nbatchsize(full): %d\nepochs(full): %d" % ( filtersNum, batch_size, epochs, batch_size_full,epochs_full)
	text_values.append(strr)   
	test_loss.append(test_eval[0])
	test_acc.append(test_eval[1])
	precision.append([report.get('macro avg').get('precision'),report.get('weighted avg').get('precision')])
	recall.append([report.get('macro avg').get('recall'),report.get('weighted avg').get('recall')])
	f1.append([report.get('macro avg').get('f1-score'),report.get('weighted avg').get('f1-score')])


	print("Options:")
	print("Press 1 to repeat with different hyperparameters")
	print("Press 2 to plot")
	print("Press 3 to show predictions")
	print("Press 4 to predict dataset")
	usr_input = input("Input: ")

	if usr_input == '2':
		x_values = np.arange(0, len(text_values), 1)

		plt.figure(figsize=(len(text_values)*2.5, 4))
		plt.plot(acc)
		plt.plot(test_acc)
		plt.title('model accuracy')
		plt.ylabel('accuracy')
		plt.xlabel('hyperparameters')
		plt.legend(['acc', 'test_acc'], loc='upper left')
		plt.xticks(x_values, text_values)
		plt.show()
  
		plt.figure(figsize=(len(text_values)*2.5, 4))
		plt.plot(loss)
		plt.plot(test_loss)
		plt.title('model loss')
		plt.ylabel('loss')
		plt.xlabel('hyperparameters')
		plt.legend(['loss', 'test_loss'], loc='upper left')
		plt.xticks(x_values, text_values)
		plt.show()

		#Plot Tables

		data = []
		for i in range(0,iterations):
			data.append([precision[i][0],recall[i][0],f1[i][0] , precision[i][1],recall[i][1],f1[i][1]])

		columns = ('precision-macro','recall-macro','f1-score-macro','precision-weighted','recall-weighted','f1-score-weighted')
		rows = text_values
		fig, ax = plt.subplots() 
		ax.set_axis_off() 
		the_table = ax.table(cellText=data,rowLabels=rows,colLabels=columns,cellLoc ='center',loc ='upper left')
		the_table.scale(3, 7)
		plt.show()




	elif usr_input == '3':

		for i in range(0,iterations):
			print( "\nModel no ",i+1,"\n",text_values[i] )
		model_no = input("\nChoose model:")

		models[int(model_no)-1].save('classifier.h5')
		#Take first 15 images from test data
		predict_data = test_images[0:15]
		predicted_classes = models[int(model_no)-1].predict(predict_data)

		predicted_classes = np.argmax(np.round(predicted_classes),axis=1)

		for i in range(0,15):
			plt.subplot(3,5,i+1)
			plt.imshow(predict_data[i].reshape(28,28), cmap='gray', interpolation='none')
			plt.title("Predicted {}, Class {}".format(predicted_classes[i], test_labels[i]))
			plt.tight_layout()
		plt.show()
  
	elif usr_input == '4':

		for i in range(0,iterations):
			print( "\nModel no ",i+1,"\n",text_values[i] )
		model_no = input("\nChoose model:")

		models[int(model_no)-1].save('classifier.h5')

    # Predict with trained model
		predicted_classes = models[int(model_no)-1].predict(train_set)
		predicted_classes = np.argmax(np.round(predicted_classes),axis=1)

		lists = list()  
		for i in range(10):
		  listi = list()
		  lists.append(listi)

		for i in range(len(predicted_classes)):
		  lists[predicted_classes[i]].append(i)

    
		with open('./project1/data/classification_results.txt','w') as f:
		  for i in range(len(lists)):
		    if ( len(lists[i]) > 0 ):
		        f.write( "CLUSTER-" + str(i) +" { size: " + str(len(lists[i])) + " , ")
		        for j in range(len(lists[i]) - 1):
		            f.write( str(lists[i][j]) + " , ")
		        f.write( str(lists[i][len(lists[i]) - 1 ] ) )
		        f.write( " }\n")