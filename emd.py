import os
import sys
import math
import struct as st
import numpy as np
import sklearn
from pulp import *

with open('./data/train-images.idx3-ubyte','rb') as f:
# with open(dataset,'rb') as f:
    magic, size = st.unpack(">II", f.read(8))
    nrows, ncols = st.unpack(">II", f.read(8))
    train_images = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
    train_images = train_images.reshape((size, nrows, ncols))
print(train_images.shape)


with open('./data/train-labels.idx1-ubyte','rb') as f:
    magic, size = st.unpack(">II", f.read(8))
    # nrows, ncols = st.unpack(">II", f.read(8))
    train_labels = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
    train_labels = train_labels.reshape((size))
print(train_labels.shape)


with open('./data/t10k-images.idx3-ubyte','rb') as f:
    magic, size = st.unpack(">II", f.read(8))
    nrows, ncols = st.unpack(">II", f.read(8))
    test_images = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
    test_images = test_images.reshape((size, nrows, ncols))
print(test_images.shape)

with open('./data/t10k-labels.idx1-ubyte','rb') as f:
    magic, size = st.unpack(">II", f.read(8))
    # nrows, ncols = st.unpack(">II", f.read(8))
    test_labels = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
    test_labels = test_labels.reshape((size))
print(test_labels.shape)


def create_array_of_image( n , img ):
    #nxn is the size of cluster
    img_array = list()
    dist_top = int(n/2)
    dist_top_sum = 0
    rows = int(28/n)
    columns = int(28/n)

    total_sum = 0 
    for i in range(28):
        for j in range(28):
            total_sum += img[i,j]

    
    for i in range(rows):
        dist_left = int(n/2)
        dist_left_sum = 0
        for j in range(columns):
            
            w = 0
            for k in range(dist_top_sum ,dist_top_sum + n ):
                for l in range(dist_left_sum , dist_left_sum + n ):
                    w += img[k,l]
            img_array.append([[dist_top,dist_left],w/total_sum])
            dist_left += n
            dist_left_sum += n
        dist_top += n
        dist_top_sum += n
    return img_array

train_array = list()
for i in range(60000):
    train_array.append(create_array_of_image( 7 , train_images[i] ))

print(train_array[0])
print(train_array[5])

sum1 = 0
for i in range(16):
    sum1 += train_array[8][i][1]

print(sum1)


arr1 = create_array_of_image( 7 , train_images[1] )
arr2 = create_array_of_image( 7 , train_images[2] )
    
clusters = list()
for i in range(16):
    cluster = list()
    for j in range(16):
        x = LpVariable("f"+str(i)+str(j), 0 , arr1[i][1])
        cluster.append(x)
    clusters.append(cluster)

prob = LpProblem("myProblem", LpMinimize)

# for i in range(16):
#     prob += lpSum(clusters[i]) == arr1[i][1] 

prob += lpSum([clusters[0][i] for i in range(16)]) , "elareiii"


# prob += lpSum(clusters[i])

prob.solve()

print(value(clusters[0][0]))

