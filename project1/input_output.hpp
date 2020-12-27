#ifndef _IO_H_
#define _IO_H_


#include <iostream>
#include <fstream>
#include <chrono>

#include "image.hpp"
#include "lsh_hash.hpp"
#include "hash_group.hpp"
#include "params.hpp"

using namespace std;

int ReverseInt (int i) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1=i&255;
    ch2=(i>>8)&255;
    ch3=(i>>16)&255;
    ch4=(i>>24)&255;
    return((int)ch1<<24)+((int)ch2<<16)+((int)ch3<<8)+ch4;
}



template <typename H>
vector<Image <int> *> readFileAddHash( H *hash_struct , string filePath) {
    //H represents the type of hashing we use.Group of lsh or hypercube.


    vector <Image <int> *> images;
    ifstream file (filePath, ios::binary);
    if (file.is_open()) {
        
        int magic_number=0;
        int n_images=0;
        int rows=0;
        int cols=0;

        //Read Magic Number 
        file.read((char*)&magic_number,sizeof(magic_number));
        
        //Get number of images
        file.read((char*)&n_images,sizeof(n_images));
        n_images= ReverseInt(n_images);

        //Get number of rows
        file.read((char*)&rows,4);
        rows= ReverseInt(rows);
        
        //Get number of columns
        file.read((char*)&cols,4);
        cols= ReverseInt(cols);

        Image<int> *img;


        for(int i=0;i<1000;++i) {;
            //Create Image object 
            img = new Image< int >(); 

            //Fill Image Object
            for(int r=0;r<rows;++r) {
                for(int c=0;c<cols;++c) {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    img->add_pixel( (int) temp );
                }
            }

            //Print for Testing Purposes
            // img->print();

            images.push_back(img);
            hash_struct->add( img );

        }


        // Close the file.
        file.close();
    }
    else{
        cout << "readFileAddHash: File does not exist" << endl;
        exit(0);
    }
    return images;
}


//returns a vector with images of given file
vector<Image <int> *> readFile(string filePath) {

    vector <Image <int> *> images;
    ifstream file (filePath,ios::binary);
    if (file.is_open()) {
        
        int magic_number=0;
        int n_images=0;
        int rows=0;
        int cols=0;

        //Read Magic Number 
        file.read((char*)&magic_number,sizeof(magic_number));
        
        //Get number of images
        file.read((char*)&n_images,sizeof(n_images));
        n_images= ReverseInt(n_images);

        //Get number of rows
        file.read((char*)&rows,4);
        rows= ReverseInt(rows);
        
        //Get number of columns
        file.read((char*)&cols,4);
        cols= ReverseInt(cols);

        Image<int> *img;

        for(int i=0;i<1000;++i) {;
            //Create Image object 
            img = new Image< int >(); 

            //Fill Image Object
            for(int r=0;r<rows;++r) {
                for(int c=0;c<cols;++c) {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    img->add_pixel( (int) temp );
                }
            }

            //Print for Testing Purposes
            // img->print();

            images.push_back(img);
        }

        // Close the file.
        file.close();
    }
    else {
        cout << "readFile: file does not exist" << endl;
        exit(0);
    }
    return images;
}

// T = Image<int>
template< typename T>
void true_knn( int k , T *v , vector<T *> **result , vector <int> **distances , vector<Image <int> *> images){

    vector<int> input_vector = v->get_data();

    typename std::vector<T *>::const_iterator iterator;

    for (iterator = images.begin(); iterator != images.end(); ++iterator) {

        if ((*result)->size() < k)
        {
            (*result)->push_back((*iterator));
            int dist = manDistance(input_vector, (*iterator)->get_data(), input_vector.size());
            (*distances)->push_back(dist);
            quicksort((*result), (*distances), 0, (*result)->size()-1);
        }
        else{
            int dist = manDistance(input_vector, (*iterator)->get_data(), input_vector.size());
            if (dist < (*distances)->at(k-1))
            {
                (*distances)->at(k-1) = dist;
                (*result)->at(k-1) = (*iterator);
                quicksort((*result), (*distances), 0, k-1);
            }
        }

    }
    
}

template <typename H>
void output(vector<Image <int> *> images, H group, int flag, Params params){

    vector<Image <int> *> queries = readFile(params.queryFile);

    ofstream myfile;
    myfile.open (params.outputFile);

    vector<Image <int> *>::iterator ptrI; 
    int i=0;
    for (ptrI = queries.begin(); ptrI < queries.end(); ptrI++) {
        myfile << "Query: " << i << endl; 
        // // cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << endl; // milisecs
        vector< Image<int> *> *result;
        vector< int > *distances;
        vector< int > apr_distances;

        //Initialize result vector
        result = new vector<Image<int> *>();
        distances = new vector<int>();    


        //aproximate knn with time
        auto start0 = std::chrono::system_clock::now();
        vector<Image<int> * > * res = group->aproximate_knn(params.N, *ptrI, &apr_distances,params.M);
        auto end0 = std::chrono::system_clock::now();
        std::chrono::duration<double> duration0 = end0-start0;

        //exact knn with time
        auto start = std::chrono::system_clock::now();
        /*vector<Image<int> * > * resTrue = */true_knn(params.N, *ptrI, &result, &distances, images);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> duration = end-start;

        vector<Image <int> *>::iterator ptr; 
        ptr = res->begin();
        // vector<Image <int> *>::iterator ptrTrue;
        // ptrTrue = resTrue->begin();


        

        //print distances for k nearest Neighbors
        for (int j = 0; j < res->size() ; ++j){
            myfile << endl;
            std::vector<Image<int> *>::iterator it = std::find(images.begin(), images.end(), *ptr);
            int index = std::distance(images.begin(), it);
            myfile << "Nearest neighbor-" << j+1 << ": " << index << endl;
            if (flag){
                myfile << "distanceHypercube: " << apr_distances.at(j) << endl;
            }else{
                myfile << "distanceLsh: " << apr_distances.at(j) << endl;
            }
            myfile << "distanceTrue: " << distances->at(j) << endl;
            ptr++;
        } 
        if (flag)
        {
            myfile << "tHypercube: " << duration0.count() << endl;
        }else
        {
            myfile << "tLSH: " << duration0.count() << endl;
        }
        myfile << "tTrue: " << duration.count() << endl << endl;
        myfile << "R-near neighbors:" << endl;

        int nope = 0;
        vector<Image <int> *> resultRange = group->aproximate_range( params.R , *ptrI , params.M , &nope );
        for(int i = 0 ; i<resultRange.size() ; i++ ){
            auto it = std::find(images.begin(), images.end(), resultRange.at(i) );
            myfile << distance(images.begin(), it)<<endl;
        }
        myfile << endl;
        i++;

        delete distances;
        delete res;
        delete result;

    }

    for ( int i = 0 ; i < queries.size() ; i++ ){
        delete queries[i];
    }

    myfile.close();
}

void cluster_output(Params params, std::chrono::duration<double> duration,  vector<Image <int> *> *clusters,  \
                    vector<Image <int> *> *centroids, string silh, int total_centroids, vector<Image <int> *> images){
    ofstream myfile;
    myfile.open (params.outputFile);
    myfile << "Algorithm: ";
    if ( params.method.compare("Classic") == 0 ){
        myfile << "Lloyds" << endl << endl;
    }
    else if ( params.method.compare("LSH") == 0 ){
        myfile << " Range Search LSH" << endl << endl;
    }
    else {
        myfile << " Range Search Hypercube" << endl << endl;
    }

    for (int i = 0; i < total_centroids; ++i)
    {
        myfile << "CLUSTER-" << i+1 << " {size: " << clusters[i].size() << ", ";
        centroids->at(i)->print(&myfile); 
        myfile << "}" << endl << endl; 
    }
    myfile << "clustering_time: " << duration.count() << endl << endl;
    myfile << silh << endl << endl;

    if (params.complete)
    {
        for (int i = 0; i < total_centroids; ++i)
        {
            myfile << "CLUSTER-" << i+1 << " {";
            centroids->at(i)->print(&myfile);
            for (int j = 0; j < clusters[i].size() ; ++j)
            {
                std::vector<Image<int> *>::iterator it = std::find(images.begin(), images.end(), clusters[i].at(j));
                int index = std::distance(images.begin(), it);
                myfile << index;
                myfile << ", ";
            } 

            myfile << "}" << endl << endl;
        }
    }
    myfile.close();
}


#endif