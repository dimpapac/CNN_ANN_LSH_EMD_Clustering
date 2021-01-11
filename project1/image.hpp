#ifndef _COMPLEX_H
#define _COMPLEX_H

#include <vector>
using namespace std;

template <typename S>
class Image {

    vector<S> data;

    public:

    Image();
    vector<S> get_data( void );
    void add_pixel( S p );
    void print( ofstream*  );

};

template <typename S>
Image<S>::Image(){

}

template <typename S>
vector<S> Image<S>::get_data(){
    return this->data;
}

template <typename S>
void Image<S>::add_pixel( S p ){
    this->data.push_back(p);
}

template <typename S>
void Image<S>::print( ofstream *myfile ){
    // myfile<<"PRINTING IMAGE!"<<"\n";
    for (S p = 0 ; p < this->data.size() - 1 ; p++ )
        (*myfile)<<this->data[p]<<",";
    // myfile << endl;
    (*myfile)<<this->data[this->data.size() - 1];
}



#endif