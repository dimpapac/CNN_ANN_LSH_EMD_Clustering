#ifndef _ASSIGN_H
#define _ASSIGN_H

#include "distance.hpp"

#include <list>
#include <vector>
#include <math.h>  

using namespace std;

template <typename T> 
void lloyds_assign_to_clusters( vector<T> *clusters , vector<T> images , vector<int> flags , vector<T> centroids , int d ){

    int min_distance,min_centroid,distance,centroid_no;
    for ( int image_no = 0 ; image_no<images.size() ; image_no++ ){
        if ( flags[image_no] == 0 ){
            //Flags == 0 means tha this images dint get assigned to a cluster durin rev assign
            //if method is classic flags are always 0
            min_distance = manDistance( images[image_no]->get_data() , centroids[0]->get_data() , d );
            min_centroid = 0;
            for ( centroid_no = 1 ; centroid_no < centroids.size() ; centroid_no++ ){
                distance = manDistance( images[image_no]->get_data() , centroids[centroid_no]->get_data() , d );
                if ( distance < min_distance ){
                    min_distance = distance;
                    min_centroid = centroid_no;
                }
            }
            clusters[min_centroid].push_back(images[image_no]);
        }
    }

}

template <typename T , typename H > 
vector<T> *assign( vector<T> images  , vector<T> centroids , int d , string method , H hash){
    // T is Image<int> *
    // H is type of hash LSH or Hypercube or int if lloyds :)

    vector<int> flags;
    vector<int> cluster_ass;
    vector<T> *clusters = new vector<T>[centroids.size()];
    int index;
    int curr_cluster;
    int curr_dist,cand_dist,to_remove;
    int new_found;
    int flag = 1;
    int remained_same = 0;

    int cluster_size[centroids.size()] = {0};

    for ( int i = 0 ; i < images.size() ; i++ ){
        flags.push_back(0);
        cluster_ass.push_back(-1);
    }
            

    if ( method.compare("Classic") == 0 ){
        lloyds_assign_to_clusters( clusters , images  , flags , centroids , d );
    }
    else{
        int radii = 1;
        while ( radii <= 4 || flag == 1 ){

            for ( int i = 0 ; i < images.size() ; i++ ){
                flags[i]=0;
                cluster_ass[i] = -1;
            }
            
            remained_same = 0;
            for ( int i = 0 ; i < centroids.size() ; i++ ) {
                new_found = 0;
                clusters[i] =  hash->aproximate_range( 10000 * radii  , centroids[i] , 60000  , &new_found );
                for ( int j = 0 ; j < clusters[i].size() ; j++ ){
                    //find index of image in dataset
                    auto it = std::find(images.begin(), images.end(), clusters[i].at(j) );
                    index = distance(images.begin(), it);
                    if ( flags[index] == 1 ){
                        //this means that image was already assigned to a cluster so we calc exact distance and compare
                        curr_cluster = cluster_ass[index];
                        //check if assigned to the cluster that we are checking.If yes then dont do anything
                        if ( curr_cluster != i ){
                            //calculate distance from current centroid
                            curr_dist = manDistance( clusters[i].at(j)->get_data() , centroids[curr_cluster]->get_data() , d );
                            //calulate distance from candidate centroid
                            cand_dist = manDistance( clusters[i].at(j)->get_data() , centroids[i]->get_data() , d );
                            if( cand_dist < curr_dist ){
                                //if candidate cetroid is closer update cluster_ass array
                                cluster_ass[index] = i;
                                //and erase from curr_cluster
                                auto it2 = std::find(clusters[curr_cluster].begin(), clusters[curr_cluster].end(), clusters[i].at(j) );
                                to_remove = distance(clusters[curr_cluster].begin(), it2);
                                clusters[curr_cluster].erase (clusters[curr_cluster].begin()+to_remove);
                            }
                            else{
                                //this means that its closer to the centroid already assigned
                                //so erase from the candidate cluster
                                clusters[i].erase (clusters[i].begin()+j);
                                //because we iterate based on clusters[i] size and we just erased j
                                //for loop is going to skip an image so we subtract j by one
                                j -= 1;
                                new_found--;
                            }
                        }
                    }
                    else{
                        flags[index] = 1;
                        cluster_ass[index] = i;
                    }
                }
                if ( clusters[i].size() == cluster_size[i] ){
                    remained_same++;
                }

                cluster_size[i] = clusters[i].size();
            }
            
            if ( (float)remained_same > (float)( 15 / 100 ) * (float)centroids.size() ){
                flag = 0;
            }

            radii *= 2;
        }
        //run lloyds for the images that didnt get assigned to a cluster
        lloyds_assign_to_clusters( clusters , images  , flags , centroids , d );
    }
    
    return clusters;
}

#endif