#ifndef _CENTROIDS_H
#define _CENTROIDS_H

#include "distance.hpp"
#include "median.hpp"

#include <list>
#include <vector>
#include <math.h>  

using namespace std;

typedef struct Image_Dist {
    int image_no;
    int distance;
} Image_Dist;


int pick_centroid( vector<Image_Dist> distances ){
    int centroid;
    //first find max D(i)
    int max = 0;
    for ( vector<Image_Dist>::iterator it = distances.begin() ; it != distances.end(); ++it){
        if( it->distance > max ){
            max = it->distance;
        }
    }
    vector<float> sums;
    float current_sum = 0;
    //normalize all D(i)s by dividing them with max D(i)
    for ( vector<Image_Dist>::iterator it = distances.begin() ; it != distances.end(); ++it){
        float normalized = (float)it->distance / (float)max ;
        current_sum = current_sum + pow(normalized,2.0);
        sums.push_back(current_sum);
    }

    //Choose with uniformal distribution from all the images
    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_real_distribution<float>  distr(0.0,current_sum - 0.1);
    float x = distr(generator);

    //Find r
    int size = distances.size();
    int i = 0;
    int r;
    for ( vector<float>::iterator it = sums.begin() ; it != sums.end(); ++it){
        if (i!=0) {
            if ( x > sums[i-1] && x <= sums[i+1] ){
                r = i;
            }
        }
        i++;
        if( i == size ){
            break;
        }
    }


    return distances[r].image_no;
}


template <typename T>
vector<Image_Dist> calculate_distances( vector<T> images , vector<T> centroids , int d ){
    vector<Image_Dist> distances;
    int distance;
    int min_distance;
    int image_no = 0;
    int centroid_no;
    for ( typename vector<T>::iterator img_it = images.begin() ; img_it != images.end(); ++img_it){
        if( find( centroids.begin(), centroids.end(), (*img_it) ) == centroids.end() ) {
            //check that is not one of the centroids already chosen
            centroid_no = 0;
            for( typename vector<T>::iterator cent_it = centroids.begin() ; cent_it != centroids.end(); ++cent_it){
                distance = manDistance( (*img_it)->get_data() , (*cent_it)->get_data() , d ); 

                if ( centroid_no == 0 ){
                    min_distance = distance;
                }
                else if ( distance < min_distance ){
                    //we just want to store min distance from one of all the centroids .Dont care which centroid (for now)
                    min_distance = distance;
                }
                centroid_no++;
            }
            Image_Dist imDist;
            imDist.image_no = image_no;
            imDist.distance = min_distance;
            distances.push_back(imDist);
        } 
        image_no++;
    }
    return distances;
}



template <typename T> 
vector<T> initialize_centroids( vector<T> images , int k , int d  ){
    //T is Image<int> *
    //k is total centroids
    vector<T> centroids;
    vector<Image_Dist> distances;
    //Find first centroid
    //Choose with uniformal distribution from all the images
    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_int_distribution<int>  distr(0,images.size() - 1);
    centroids.push_back(images[distr(generator)]);
    for ( int t = 1; /*centroids chosen*/ t < k ; t++ ){    
        distances = calculate_distances(images,centroids,d);
        //CHOOSE NEW CENTROID AND ADD IT TO CENTROIDS
        int centroid = pick_centroid(distances);
        centroids.push_back(images[centroid]);
    }

    return centroids;
}

template <typename T> 
vector<T> update_centroids( vector<T> *clusters , int d , int total_clusters , vector<T> old_centroids ){
    //T is Image<int> *
    //k is total centroids
    vector<T> new_centroids;

    T centr;
    for( int i = 0; i < total_clusters; i++ ){
        //For each cluster find median
        if ( clusters[i].size() > 1 ){
            centr = find_median( clusters[i] , d );
            new_centroids.push_back(centr);
        } 
        else if ( clusters[i].size() == 1 ){
            new_centroids.push_back(clusters[i][0]);
        }
        else if ( clusters[i].size() == 0 ){
            new_centroids.push_back(old_centroids[i]);
        }
    }



    return new_centroids;
}

template <typename T> 
int check_centroids( vector<T> prev_centroids , vector<T> new_centroids , int d , int e ){

    int flag = 0;
    for ( int i = 0 ; i < prev_centroids.size() ; i++ ){
        if ( manDistance( prev_centroids[i]->get_data() , new_centroids[i]->get_data() , d ) <= e ) {
            flag++;
        }
    }

    if ( flag == prev_centroids.size() ){
        return 0;
    }

    return 1;
}


#endif
