#ifndef _MEDIAN_H
#define _MEDIAN_H

#include "distance.hpp"

#include <list>
#include <vector>
#include <math.h>  

using namespace std;

template<typename T>
T find_median( vector<T> cluster , int d ){
    
    T median = new Image<int> ();
    int d_median;
    int k;
    //for each dimension find median 
    for ( int i = 0 ; i < d ; i++ ){
        //first sort all objects of cluster based on the value of ith dimension

        if ( cluster.size() % 2 == 0 ){
            k = cluster.size()/2;
        }
        else{
            k = (cluster.size()/2) + 1;
        }

        d_median = find_kth( cluster , cluster.size() - 1 , k , i );
        median->add_pixel(d_median);
    }
    return median;

}


template <class T>
int find_kth ( vector<T> a, unsigned n, unsigned k , int dim  ){
    for (int l=1, r=n; ; ) {
        if (r <= l+1){
            if (r == l+1 && a[r]->get_data().at(dim) < a[l]->get_data().at(dim) )
                median_swap (&a , l , r );
            return a[k]->get_data().at(dim);
        }

        int mid = (l + r) >> 1;
        median_swap (&a , mid , l+1 );
        if (a[l]->get_data().at(dim)  > a[r]->get_data().at(dim) )
            median_swap (&a , l , r );
        if (a[l+1]->get_data().at(dim)  > a[r]->get_data().at(dim) )
            median_swap (&a , l+1 , r );
        if (a[l]->get_data().at(dim)  > a[l+1]->get_data().at(dim) )
            median_swap (&a , l , l+1 );

        int i = l+1,j = r;
        int cur = a[l+1]->get_data().at(dim) ;
        for (;;){
            while (a[++i]->get_data().at(dim)  < cur) ;
            while (a[--j]->get_data().at(dim)  > cur) ;
            if (i > j){
                break;
            }
            median_swap (&a , i , j );
        }

        a[l+1]->get_data().at(dim)  = a[j]->get_data().at(dim) ;
        a[j]->get_data().at(dim)  = cur;

        if (j >= k){
            r = j-1;
        }

        if (j <= k){
            l = i;
        }
    }
}

template <typename T >
void median_swap( vector< T > *cluster , int i , int j ){
    T temp1;

    temp1 = cluster->at(i);
    cluster->at(i) = cluster->at(j);
    cluster->at(j) = temp1;
    
}
  

#endif