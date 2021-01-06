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
            perc = "{0:.0f}".format(w/total_sum * 100 )
            img_array.append([[dist_top,dist_left],int(perc)])
            dist_left += n
            dist_left_sum += n
        dist_top += n
        dist_top_sum += n
    return img_array

train_array = list()
for i in range(60000):
    train_array.append(create_array_of_image( 7 , train_images[i] ))


for i in range(60000):
    sum1 = 0
    for j in range(16):
        sum1 += train_array[i][j][1]
    if sum1 < 100 :
        train_array[i][0][1] += 100 - sum1
    elif sum1 > 100 :
        diff = sum1 - 100
        counter = 0
        while diff > 0:
            if train_array[i][counter][1] > 0 :
                train_array[i][counter][1] -= 1
                diff -= 1
            counter += 1
            if counter == 16:
                counter = 0


# arr1 = create_array_of_image( 7 , train_images[1] )
# arr2 = create_array_of_image( 7 , train_images[2] )

# print(arr1)
# print(arr2)
    
# flows_in_clusters = list()
# for i in range(16):
#     flows = list()
#     for j in range(16):
#         x = LpVariable("f"+str(i)+str(j), 0 , arr1[i][1])
#         flows.append(x)
#     flows_in_clusters.append(flows)

# prob = LpProblem("myProblem", LpMinimize)

# # for i in range(16):
# #     prob += lpSum(clusters[i]) == arr1[i][1] 

# prob += lpSum([flows_in_clusters[0][i] for i in range(16)]) == arr1[0][1], "elareiii"

# prob += lpSum([flows_in_clusters[0][i] for i in range(16)]) , "elareiii2"


# # prob += lpSum(clusters[i])

# prob.solve()

# print(value(flows_in_clusters[0][0]))

