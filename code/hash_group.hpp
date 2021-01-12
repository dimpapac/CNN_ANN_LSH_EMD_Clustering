#ifndef _GROUP_H
#define _GROUP_H

#include "lsh_hash.hpp"

template<typename T , typename S >
class Hash_Group{

    T **hash_tables;
    int tables;
    int **h_params;
    int n;

    public:

    Hash_Group( int , int , int , int , int , int , int  );
    ~Hash_Group();
    void test_print( void );
    void add( S * );
    void bucket_statistics(void);
    vector< S * > *aproximate_knn( int , S *, vector<int>* , int  );
    vector< S * > aproximate_range( int , S * , int , int * );

};

template<typename T , typename S >
Hash_Group<T,S>::Hash_Group( int M , int total , int n_buckets , int n , int k , int w , int m  ){

    timeval t;
    gettimeofday(&t, NULL);
    srand(t.tv_usec * t.tv_sec );

    this->h_params = new int*[n];
    for( int i = 0 ; i < n ; i++ ){
        this->h_params[i] = new int[n];
        for ( int j = 0 ; j < n ; j++ ){
            h_params[i][j] = rand()%w;
        }
    }

    this->n = n;
    this->tables = total;
    this->hash_tables = new T*[total];
    for ( int i = 0 ; i<total; i++ ){
        this->hash_tables[i] = new T( M , n_buckets , n , k , w , m , h_params); 
    }
}


template<typename T , typename S >
Hash_Group<T,S>::~Hash_Group(){ 

    for ( int i = 0 ; i < this->tables ; i++ ){
        delete hash_tables[i];
    }
    delete[] hash_tables;

    for ( int i = 0 ; i < this->n ; i++ ){
        delete[] h_params[i];
    }
    delete[] h_params; 
}

template<typename T , typename S>
void Hash_Group<T,S>::add( S *v ){
    uint64_t g;
    for ( int i = 0 ; i < this->tables ; i++ ){
        g = this->hash_tables[i]->hash_function(v);
        this->hash_tables[i]->add_to_bucket(v,g);
    }
}


template<typename T , typename S>
void Hash_Group<T,S>::test_print(void){
    for ( int i = 0 ; i < this->tables ; i++ ) {
        this->hash_tables[i]->test_print();
    }
}

template<typename T , typename S>
void Hash_Group<T,S>::bucket_statistics(void){
    for ( int i = 0 ; i < this->tables ; i++ ) {
        this->hash_tables[i]->bucket_statistics();
    }
}

template<typename T , typename S>
vector< S * > *Hash_Group<T,S>::aproximate_knn( int k , S *v , vector <int> *distances , int max_checks ){
    // S represents vector type.For our project v is Image<int> *
    vector< S *> *result ;
    int g;
    int checked = 0;
    //Initialize result vector
    result = new vector<S *>;

    for ( int i = 0 ; i < this->tables ; i++ ){
        g = this->hash_tables[i]->hash_function( v );
        this->hash_tables[i]->aproximate_knn( k , v , &result , distances , g , &checked );
    }
    
    return result;
}

template<typename T , typename S>
vector< S * > Hash_Group<T,S>::aproximate_range( int range , S *v , int max_checks , int *new_found ){
    // S represents vector type.For our project v is Image<int> *
    // vector< S *> *result ;
    int g;
    int checked = 0;
    vector<S *> result;
    // //Initialize result vector
    // result = new vector<S *>();

    for ( int i = 0 ; i < this->tables ; i++ ){
        g = this->hash_tables[i]->hash_function( v );
        this->hash_tables[i]->aproximate_range( range , v , &result , g , &checked , new_found );
    }

    return result;
}









#endif