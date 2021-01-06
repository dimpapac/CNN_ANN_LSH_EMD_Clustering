import os
import sys
import math
import struct as st
import numpy as np
import sklearn
from pulp import *
from math import sqrt


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



# train_array = list()
# for i in range(60000):
#     train_array.append(create_array_of_image( 7 , train_images[i] ))


# for i in range(60000):
#     sum1 = 0
#     for j in range(16):
#         sum1 += train_array[i][j][1]
#     if sum1 < 100 :
#         train_array[i][0][1] += 100 - sum1
#     elif sum1 > 100 :
#         diff = sum1 - 100
#         counter = 0
#         while diff > 0:
#             if train_array[i][counter][1] > 0 :
#                 train_array[i][counter][1] -= 1
#                 diff -= 1
#             counter += 1
#             if counter == 16:
#                 counter = 0

arr1 = create_array_of_image( 7 , train_images[1] )
arr2 = create_array_of_image( 7 , train_images[2] )


sum1 = 0
for j in range(16):
    sum1 += arr1[j][1]
if sum1 < 100 :
    arr1[0][1] += 100 - sum1
elif sum1 > 100 :
    diff = sum1 - 100
    counter = 0
    while diff > 0:
        if arr1[counter][1] > 0 :
            arr1[counter][1] -= 1
            diff -= 1
        counter += 1
        if counter == 16:
            counter = 0

sum1 = 0
for j in range(16):
    sum1 += arr2[j][1]
if sum1 < 100 :
    arr2[0][1] += 100 - sum1
elif sum1 > 100 :
    diff = sum1 - 100
    counter = 0
    while diff > 0:
        if arr2[counter][1] > 0 :
            arr2[counter][1] -= 1
            diff -= 1
        counter += 1
        if counter == 16:
            counter = 0

#Add variables for flow capacity
flows_in_clusters = list()
for i in range(16):
    flows = list()
    for j in range(16):
        x = LpVariable("f"+str(i)+"_"+str(j), 0 , arr1[i][1])
        flows.append(x)
    flows_in_clusters.append(flows)

def euclidian( p1 , p2 ):
    return sqrt( pow(p1[0] - p2[0] , 2) + pow(p1[1] - p2[1] , 2) )

distances = list()
#Calculate distance for each flow
for i in range(16):
    for j in range(16):
        distances.append( euclidian( arr1[i][0] , arr2[j][0] ) )


prob = LpProblem("myProblem", LpMinimize)

# for i in range(16):
#     prob += lpSum(clusters[i]) == arr1[i][1] 

for j in range(16):
    prob += lpSum([flows_in_clusters[j][i] for i in range(16)]) == arr1[j][1], "constr"+"_"+str(j)

for j in range(16):
    prob += lpSum([flows_in_clusters[i][j] for i in range(16)]) == arr2[j][1], "constr2"+"_"+str(j)

final_sum = list()
for i in range(16):
    for j in range(16):
        final_sum.append(flows_in_clusters[i][j])

prob += lpSum([final_sum[i] * distances[i] for i in range(16*16)]) , "goal"


try:
     prob.solve()
except Exception:
     print("problem with pup")

print(prob.objective.value())