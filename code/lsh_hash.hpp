#ifndef _LSH_H
#define _LSH_H


#define _BSD_SOURCE

#include <sys/time.h>


#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "hash.hpp"
#include "list"
#include "map"

#include "lsh_functions.hpp"

using namespace std;

template <typename T,typename S>
class Lsh_Hash : public Hash<T>{

    int n;
    int w;
    int k;
    int m;
    int M;
    //map with key from 1 to n containing randomly picked s for each dimension
    map<S,S *> h_map; 

    int **h_params;

    public:

    Lsh_Hash( int , int , int , int ,int , int , int** );
    ~Lsh_Hash();
    int hash_function(T *);
    void test_print( void );


};

template <typename T , typename S >
Lsh_Hash<T,S>::Lsh_Hash( int maximum_checks , int n_buckets , int n , int k , int w , int m , int **H ) : Hash<T> (n_buckets,maximum_checks) {

    timeval t;
    gettimeofday(&t, NULL);
    srand(t.tv_usec * t.tv_sec );
    
    int rand_h;

    this->h_params = new int*[k];
    for( int i = 0 ; i < k ; i++ ){
        this->h_params[i] = new int[n];

        rand_h = rand()%n;

        for ( int j = 0 ; j < n ; j++ ){
            h_params[i][j] = H[rand_h][j];
        }
    }

    this->n = n;
    this->k = k;
    this->w = w;
    this->m = m;
    this->M = pow(2,32/this->k) - 1 ;

}

template <typename T , typename S >
Lsh_Hash<T,S>::~Lsh_Hash(){

    for ( int i = 0 ; i < this->k ; i++ ){
        delete[] h_params[i];
    }
    delete[] h_params; 
}


template <typename T , typename S >
int Lsh_Hash<T,S>::hash_function(T *v){
    //v for the projects purpose represents a pointer to an image.
    //T object must have a vector field named data containing vectors info
    //S is type of hashing value ( exmp : S->int or S->float etc.)

    timeval t;
    gettimeofday(&t, NULL);
    srand(t.tv_usec * t.tv_sec );

    int concat;
    int h_val;
    int rand_h;
    int shift = 32/this->k - 1;

    //calculate k h functions for given vector
    for ( int i = 0 ; i < this->k ; i++ ){
        
        h_val = calculate_h( h_params[i] , v->get_data() , this->n  , this->w , this->m , this->M );
    
        if ( i == 0 ){
            concat = h_val;
        }
        else{
            concat <<= shift; //left shift 32/k bytes 
            concat |= h_val;
        }

    }

    return concat;
}




template <typename T , typename S >
void Lsh_Hash<T,S>::test_print(void){
    Hash<T>::test_print();
}



#endif