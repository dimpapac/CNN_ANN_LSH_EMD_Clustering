#ifndef _HASH_H
#define _HASH_H

#include <list>
#include <algorithm>

#include "quicksort.hpp"
#include "distance.hpp"


using namespace std;

template< typename S,typename T>
S modulo2(T x,S N){
    return (x % N + N) %N;
}


template <typename T>
class Hash {
    //Array of pointers to buckets
    vector<T *> *buckets;
    vector<int > *gs;
    int n_buckets;
    int max;

    public:

    Hash( int , int );
    ~Hash( void );
    //essentials
    void add_to_bucket( T * , int );
    //Getters
    int get_n_buckets(void);
    //Setters
    //test
    void test_print(void);
    void bucket_statistics(void);
    void aproximate_knn(int , T *, vector<T *> **,vector<int> *,int , int *);
    void aproximate_range(int , T *, vector<T *> * ,int , int * , int * );
    
};



template <typename T>
Hash<T>::Hash( int n_buckets , int max ){
    this->buckets = new vector<T *> [n_buckets];
    this->gs = new vector<int> [n_buckets];
    this->n_buckets = n_buckets;
    this->max = max;
}

template <typename T>
Hash<T>::~Hash(){
    delete[] this->buckets;
    delete[] this->gs;
}

template <typename T>
void Hash<T>::add_to_bucket( T *v , int g){
    int bucket = g%this->n_buckets;
    this->buckets[bucket].push_back(v);
    this->gs[bucket].push_back(g);
}

template<typename T>
int Hash<T>::get_n_buckets(void){
    return this->n_buckets;
}

template <typename T>
void Hash<T>::test_print( void ){

    typename std::vector<T *>::const_iterator iterator;

    for (iterator = this->buckets[0].begin(); iterator != this->buckets[0].end(); ++iterator) {
        cout << "image" << endl;
    }
}

template< typename T>
void Hash<T>::bucket_statistics(void){
    int sum = 0;
    int total = 0; 
    for ( int i = 0 ; i < n_buckets ; i++ ){
        if ( buckets[i].size() != 0 ) {
            sum++;
            cout<<"bucket no "<<i<<" has "<<buckets[i].size()<<"images"<<endl;
            total += buckets[i].size();
        }
    }
    cout<<"Total are "<<total<<endl;
    cout<<sum<<" buckets have images inside them"<<endl;
}

// T = Image<int>
template< typename T>
void Hash<T>::aproximate_knn( int k , T *v , vector<T *> **result , vector <int> *distances , int g , int *npne ){
    
    int checked = 0;
    
    vector<int> input_vector = v->get_data();
    int bucket = modulo2(g,this->n_buckets);



    for ( int i = 0 ; i < this->buckets[bucket].size() ; i++ ) {

        // if ( checked == this->max ){
        //     break;
        // }

        if ( g == this->gs[bucket].at(i) ){
            if( find( (*result)->begin(), (*result)->end(), this->buckets[bucket].at(i) ) == (*result)->end() /*result doesnt contain this image */ ) {
                if ( (*result)->size() < k)
                {
                    (*result)->push_back( this->buckets[bucket].at(i)  );
                    int dist = manDistance(input_vector, this->buckets[bucket].at(i)->get_data(), input_vector.size());
                    distances->push_back(dist);
                    quicksort((*result), distances, 0, (*result)->size()-1);
                }
                else{
                    int dist = manDistance(input_vector, this->buckets[bucket].at(i)->get_data(), input_vector.size());
                    if (dist < distances->at(k-1))
                    {
                        distances->at(k-1) = dist;
                        (*result)->at(k-1) = this->buckets[bucket].at(i);
                        quicksort((*result), distances, 0, k-1);
                    }
                }
            } 

            checked++;
        }
        
    }
    
}

template< typename T>
void Hash<T>::aproximate_range(  int range , T *v , vector<T *> *result , int g , int *nope , int *new_found ){

    vector<int> input_vector = v->get_data();
    int bucket = modulo2(g,this->n_buckets);
    int checked = 0;


    for ( int i = 0 ; i < this->buckets[bucket].size() ; i++ ) {

        if ( checked == this->max ){
            break;
        }

        if ( g == this->gs[bucket].at(i) ){
            if( find( (result)->begin(), (result)->end(), this->buckets[bucket].at(i) ) == (result)->end() /*result doesnt contain this image */ 
            && this->buckets[bucket].at(i) != v   /*not the query image*/) {
                int dist = manDistance(input_vector, (this->buckets[bucket].at(i))->get_data(), input_vector.size());
                if (dist < range )
                {
                    (result)->push_back(this->buckets[bucket].at(i));
                    *new_found = *new_found + 1;
                }
            }
        }

        checked++;
    }

    
}

#endif

