#ifndef _QSORT_
#define _QSORT_

using namespace std;

#include <vector>

template <typename T , typename S>
void quicksort( vector< T > *images , vector<S> *distances , int low , int high  ){
    int p;
    if ( low < high ) {
        p = partition( images , distances , low , high );
        quicksort( images , distances , low, p - 1);  
        quicksort( images , distances , p + 1, high); 
    }
    
}

template <typename T , typename S>
int partition ( vector< T > *images , vector<S> *distances , int low, int high)  {  
    int pivot = distances->at(high); 
    int i = (low - 1);
  
    for (int j = low; j <= high - 1; j++)  
    {  
        if ( distances->at(j) < pivot)  
        {  
            i++; 
            swap( images , distances , i , j );  
        }  
    }  
    swap( images , distances , i + 1 , high ); 
    return (i + 1);  
}  
  
template <typename T , typename S>
void swap( vector< T > *images , vector<S> *distances , int i , int j ){
    T temp1;
    S temp2;

    temp1 = images->at(i);
    images->at(i) = images->at(j);
    images->at(j) = temp1;

    temp2 = distances->at(i);
    distances->at(i) = distances->at(j);
    distances->at(j) = temp2;

}

#endif