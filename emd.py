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

def euclidian( p1 , p2 ):
    return sqrt( pow(p1[0] - p2[0] , 2) + pow(p1[1] - p2[1] , 2) )

def emd( n , arr1 , arr2 ):
    total_clusters = pow( int(28 / n) , 2) 
    #Add variables for flow capacity
    flows_in_clusters = list()
    for i in range(total_clusters):
        flows = list()
        for j in range(total_clusters):
            x = LpVariable("f"+str(i)+"_"+str(j), 0 , arr1[i][1])
            flows.append(x)
        flows_in_clusters.append(flows)

    distances = list()
    #Calculate distance for each flow
    for i in range(total_clusters):
        for j in range(total_clusters):
            distances.append( euclidian( arr1[i][0] , arr2[j][0] ) )

    prob = LpProblem("myProblem", LpMinimize)

    for j in range(total_clusters):
        prob += lpSum([flows_in_clusters[j][i] for i in range(total_clusters)]) == arr1[j][1], "constr"+"_"+str(j)

    for j in range(total_clusters):
        prob += lpSum([flows_in_clusters[i][j] for i in range(total_clusters)]) == arr2[j][1], "constr2"+"_"+str(j)

    final_sum = list()
    for i in range(total_clusters):
        for j in range(total_clusters):
            final_sum.append(flows_in_clusters[i][j])

    prob += lpSum([final_sum[i] * distances[i] for i in range(total_clusters*total_clusters)]) , "goal"

    try:
        prob.solve(PULP_CBC_CMD(msg=False))
    except Exception:
        print("problem with pulp")
    return prob.objective.value()



def find_10_closest( n , test_img , train_array ):
    total = 0
    ten_min = list()
    ten_min_index = list()
    for i in range(len(train_array)):
        x = emd( n , test_img , train_array[i])
        if total < 10:
            ten_min.append(x)
            ten_min_index.append(i)
            ten_min, ten_min_index = (list(t) for t in zip(*sorted(zip(ten_min, ten_min_index))))
            total += 1
        else:
            if x < ten_min[9]:
                ten_min[9] = x 
                ten_min_index[9] = i
                ten_min, ten_min_index = (list(t) for t in zip(*sorted(zip(ten_min, ten_min_index))))
    return ten_min_index


def test_no( o , n , test , train_array):
    x = find_10_closest( n , test , train_array)
    counter = 0
    for i in range(len(x)):
        if train_labels[x[i]] == test_labels[o]:
            counter += 1
    return counter


train_array_size = 100
test_array_size = 10
n = 4
k = pow(int(28/n),2)
train_array = list()
for i in range(train_array_size):
    train_array.append(create_array_of_image( n , train_images[i] ))

test_array = list()
for i in range(test_array_size):
    test_array.append(create_array_of_image( n , test_images[i] ))

for i in range(train_array_size):
    sum1 = 0
    for j in range(k):
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
            if counter == k:
                counter = 0

for i in range(test_array_size):
    sum1 = 0
    for j in range(k):
        sum1 += test_array[i][j][1]
    if sum1 < 100 :
        test_array[i][0][1] += 100 - sum1
    elif sum1 > 100 :
        diff = sum1 - 100
        counter = 0
        while diff > 0:
            if test_array[i][counter][1] > 0 :
                test_array[i][counter][1] -= 1
                diff -= 1
            counter += 1
            if counter == k:
                counter = 0

sum = 0
for i in range(len(test_array)):
    sum += test_no( i , n , test_array[i] , train_array )

print( sum / len(test_array) )