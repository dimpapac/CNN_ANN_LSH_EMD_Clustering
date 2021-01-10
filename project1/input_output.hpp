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
vector<Image <int> *> readFileAddHash( H *hash_struct , string filePath , int size ) {
    //H represents the type of hashing we use.Group of lsh or hypercube.

    int final;
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

        if (size == -1 ){
            final = n_images;
        }
        else{
            final = size;
        }
        for(int i=0;i<final;++i) {;
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
vector<Image <int> *> readFile(string filePath , int size ) {

    int final;
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

        if (size == -1 ){
            final = n_images;
        }
        else{
            final = size;
        }
        for(int i=0;i<final;++i) {;
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


//returns a vector with images of given file
vector<Image <int> *> readFile_latent(string filePath , int size ) {

    vector <Image <int> *> images;
    ifstream file (filePath,ios::binary);
    if (file.is_open()) {
        int final;
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
        if (size == -1 ){
            final = n_images;
        }
        else{
            final = size;
        }
        for(int i=0;i<final;++i) {;
            //Create Image object 
            img = new Image< int >(); 

            //Fill Image Object
            for(int r=0;r<rows;++r) {
                for(int c=0;c<cols;++c) {
                    unsigned char temp1=0;
                    unsigned char temp2=0;
                    file.read((char*)&temp1,sizeof(temp1));
                    file.read((char*)&temp2,sizeof(temp1));
                    int number = temp2 | temp1 << 8;
                    img->add_pixel( number );
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
void output(vector<Image <int> *> images, vector<Image <int> *> images_latent ,  H group, int flag, Params params){

    vector<Image <int> *> queries = readFile(params.queryFile, 100 );

    vector<Image <int> *> queries_latent = readFile_latent(params.queryFile_latent, 100 );

    ofstream myfile;
    myfile.open (params.outputFile);

    float sum1 = 0;
    float sum2 = 0;
    float sum3 = 0;
    float sum4 = 0;
    float sum5 = 0;

    vector<Image <int> *>::iterator ptrI; 
    int i=0;
    int j = 0;
    for (ptrI = queries.begin(); ptrI < queries.end(); ptrI++) {
        myfile << "Query: " << i << endl; 
        // // cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << endl; // milisecs
        vector< Image<int> *> *result;
        vector< Image<int> *> *result_latent;
        vector< int > *distances;
        vector< int > *distances_latent;
        vector< int > apr_distances;

        //Initialize result vector
        result = new vector<Image<int> *>();
        result_latent = new vector<Image<int> *>();
        distances = new vector<int>();    
        distances_latent = new vector<int>();   


        //aproximate knn with time
        std::chrono::steady_clock::time_point start0 = std::chrono::steady_clock::now();
        vector<Image<int> * > * res = group->aproximate_knn(params.N, *ptrI, &apr_distances,params.M);
        std::chrono::steady_clock::time_point end0 = std::chrono::steady_clock::now();
        float duration0 = std::chrono::duration<float>(end0 - start0).count();
        sum1 = sum1 + (float) duration0;

        //exact knn with time
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        true_knn(params.N, *ptrI, &result, &distances, images);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        float duration = std::chrono::duration<float>(end - start).count();
        sum2 = sum2 + (float) duration;

        
        std::chrono::steady_clock::time_point start1 = std::chrono::steady_clock::now();
        true_knn(params.N, queries_latent[i] , &result_latent , &distances_latent , images_latent);
        std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
        float duration1 = std::chrono::duration<float>(end1 - start1).count();
        sum3 = sum3 + (float) duration1;


        vector<Image <int> *>::iterator ptr2; 
        ptr2 = result->begin();
        vector<Image <int> *>::iterator ptr3; 
        ptr3 = result_latent->begin();

        std::vector<Image<int> *>::iterator it2 = std::find(images.begin(), images.end(), *ptr2);
        int index2 = std::distance(images.begin(), it2);

        std::vector<Image<int> *>::iterator it3 = std::find(images_latent.begin(), images_latent.end(), *ptr3);
        int index3 = std::distance(images_latent.begin(), it3);

        myfile << "Nearest neighbor Reduced: " << index3 << endl;
        if ( res->size() > 0 ){
            vector<Image <int> *>::iterator ptr; 
            ptr = res->begin();
            std::vector<Image<int> *>::iterator it = std::find(images.begin(), images.end(), *ptr);
            int index = std::distance(images.begin(), it);
            myfile << "Nearest neighbor LSH: " << index << endl;
        }
        myfile << "Nearest neighbor True: " << index2 << endl;

        int dist_red = manDistance( queries[i]->get_data() , images[index3]->get_data(), 784 );


        myfile << "distanceReduced: " << dist_red << endl;
        if ( apr_distances.size() > 0 ){
            myfile << "distanceLsh: " << apr_distances.at(0) << endl;
            sum4 += (float) apr_distances.at(0) / (float) distances->at(0);
            j++;
        }
        myfile << "distanceTrue: " << distances->at(0) << endl;
        myfile << endl;

        sum5 += (float) dist_red / (float) distances->at(0);

        i++;

        delete distances;
        delete res;
        delete result;

    }


    
    myfile << "tReduced: " << (float) sum3/ (float) i << endl;
    myfile << "tLSH: " << (float) sum1/ (float) i << endl ;
    myfile << "tTrue: " << (float) sum2/ (float) i<< endl ;
    if ( j > 0 ){
        myfile << "Approximation Factor LSH: " << (float) sum4/ (float) j<< endl ;
    }
    myfile << "Approximation Factor Reduced: " << (float) sum5/ (float) i<< endl ;
    

    for ( int i = 0 ; i < queries.size() ; i++ ){
        delete queries[i];
    }

    myfile.close();
}

vector<int> read_labels( string filePath ){
    // cout<<filePath<<endl;
    vector<int> labels;
    ifstream file (filePath,ios::binary);
    if (file.is_open()) {
        
        int magic_number=0;
        int n_items=0;

        //Read Magic Number 
        file.read((char*)&magic_number,sizeof(magic_number));
        
        //Get number of items
        file.read((char*)&n_items,sizeof(n_items));
        n_items= ReverseInt(n_items);


        for(int i=0;i<n_items;++i) {;
            unsigned char temp1=0;
            file.read((char*)&temp1,sizeof(temp1));
            labels.push_back(int(temp1));
        }

        // Close the file.
        file.close();
    }
    else {
        cout << "readFile: file does not exist" << endl;
        exit(0);
    }
    return labels;
}



void output_3(vector<Image <int> *> images, Params params , int train_array_size , int test_array_size , float emd_score ){
    vector<Image <int> *> queries = readFile(params.queryFile, test_array_size );


    vector<int> labels_input = read_labels(params.labels_input);
    vector<int> labels_query = read_labels(params.labels_query);

    ofstream myfile;
    myfile.open (params.outputFile);


    vector<Image <int> *>::iterator ptrI; 
    int i=0;
    int sum = 0;
    for (ptrI = queries.begin(); ptrI < queries.end(); ptrI++) {
        vector< Image<int> *> *result;
        vector< int > *distances;


        //Initialize result vector
        result = new vector<Image<int> *>();
        distances = new vector<int>();    

        //exact knn with time
        auto start = std::chrono::system_clock::now();
        /*vector<Image<int> * > * resTrue = */true_knn( 10 , *ptrI, &result, &distances, images);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> duration = end-start;

        int counter = 0;
        vector<Image <int> *>::iterator ptr; 
        ptr = result->begin();
        for (int j = 0; j < result->size() ; ++j){
            std::vector<Image<int> *>::iterator it = std::find(images.begin(), images.end(), *ptr);
            int index = std::distance(images.begin(), it);
            if ( labels_input[index] == labels_query[i] ){
                counter++;
            }
            ptr++;
        } 
        // cout<<counter<<endl;
        sum += counter;
        
        i++;

        delete distances;
        delete result;

    }

    float man_score = (float)sum/ (float) i;
    
    myfile << "Average Correct Search Results EMD: " << emd_score <<endl;
    myfile << "Average Correct Search Results MANHATTAN: " << man_score ;

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