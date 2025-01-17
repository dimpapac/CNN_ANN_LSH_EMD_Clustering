#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <regex>
#include <vector>


#include "input_output.hpp"
#include "silhouette.hpp"
#include "params.hpp"
#include "hash_group.hpp"
#include "centroids.hpp"
#include "assign.hpp"

#include <string>


using namespace std;

void read_config( int **params , string configurationFile){

    string line;
    istringstream line_stream;
    string id;
    string value;

    //assign default values
    params[0][0] = 2 ;
    params[0][1] = 3 ;
    params[0][2] = 4 ;
    params[0][3] = 10 ;
    params[0][4] = 3 ;
    params[0][5] = 2 ;
    
    ifstream file (configurationFile);
    if (file.is_open()) {
        while(getline(file, line, '\n')){
            string myText(line);
            istringstream iss(myText);
            getline(iss, id, ':');
            getline(iss, value , ' ');
            getline(iss, value , ' ');
            if ( id.compare("number_of_clusters") == 0 ){
                params[0][0] = stoi(value);
            }   
            else if ( id.compare("number_of_vector_hash_tables") == 0 ){
                params[0][1] = stoi(value);
            }
            else if ( id.compare("number_of_vector_hash_functions") == 0 ){
                params[0][2] = stoi(value);
            }
            else if ( id.compare("max_number_M_hypercube") == 0 ){
                params[0][3] = stoi(value);
            }
            else if ( id.compare("number_of_hypercube_dimensions") == 0 ){
                params[0][4] = stoi(value);
            }
            else if ( id.compare("number_of_probes") == 0 ){
                params[0][5] = stoi(value);
            } 
        }
        file.close();
    }
    else {
        cout << "configuration file does not exist" << endl;
        exit(0);
    }

}

template <typename T , typename H > 
vector<T> *clustering( vector<T> images  , int d , string method , H hash , int total_clusters , vector<T> *new_centroids) {
    // T is Image<int> *
    // H is type of hash LSH or Hypercube or int if lloyds :)
    vector<Image<int> *> centroids = initialize_centroids( images , total_clusters , d  );
    // vector<Image<int> *>new_centroids;
    vector<Image <int> *> *clusters;

    clusters = assign( images  , centroids  , d , method , hash );
    (*new_centroids) = update_centroids( clusters , d , total_clusters , centroids );
    int total_updates = 0;

    while ( check_centroids( centroids , (*new_centroids) , d , 1000 ) ) {
        if ( total_updates == 10 ) {
            break;
        }
        
        centroids = (*new_centroids);

        clusters = assign( images  , centroids  , d , method , hash );

        int sum = 0;
        
        (*new_centroids) = update_centroids( clusters , d , total_clusters , centroids );
        total_updates++;
    }

    return clusters;
}


template <typename T > 
vector<T> *read_classification_results( string filename , vector<T> images , int total_images ) {
    vector<T> *clusters = new vector<T>[10];


    ifstream file(filename);
    
    
    string line;
    regex reg("[0-9]+");

    for (int i = 0; i < 10; i++)
    {
      smatch matches;
    
      getline(file, line);
      regex_search(line, matches, reg);
      line = matches.suffix().str();
    
      /* match again to get the size, and remove it */
      regex_search(line, matches, reg);
      int size = stoi(matches[0]);
      line = matches.suffix().str();


      while(regex_search(line, matches, reg))
      {
        if ( total_images == -1 ) {
            clusters[i].push_back( images[stoi( matches[0] ) ] );
        }
        else{
            if ( stoi( matches[0] ) < total_images ){
                clusters[i].push_back( images[stoi( matches[0] ) ] );
            }
        }
        line = matches.suffix().str();
      }
    

    }

    file.close();

    return clusters;
}

template <typename T > 
uint64_t objective_value( vector<T> images , vector<T> centroids , int d ){

    uint64_t o_val = 0;
    int min = 0;
    int dist;
    for ( int i = 0 ; i < images.size() ; i++ ){
        min = manDistance( images.at(i)->get_data() , centroids.at(0)->get_data() , d );
        for ( int j = 1 ; j < centroids.size() ; j++ ){
            dist = manDistance( images.at(i)->get_data() , centroids.at(j)->get_data() , d );
            if ( dist < min ){
                min = dist;
            }
        }
        o_val += min;
    }
    return o_val;
}

int main (int argc, char** argv){

    
    int *config_params = new int[6];
    Params params = inputValidate(argc, argv);
    read_config(&config_params, params.configurationFile);

    ofstream myfile;
    myfile.open (params.outputFile);

    vector<Image <int> *> images1;
    vector<Image <int> *> images2;
    vector<Image <int> *> images3;
    vector<Image <int> *> *clusters1;
    vector<Image <int> *> *clusters2;
    vector<Image <int> *> clusters2_1[10];
    vector<Image <int> *> *clusters3;
    vector<Image <int> *> centroids1;
    vector<Image <int> *> centroids2;
    vector<Image <int> *> centroids3;
    vector<Image <int> *> centroids4;
    std::chrono::duration<double> duration;

    int total_images = params.total_images;
    
    images3 = readFile( params.inputFile , total_images );
    clusters3 = read_classification_results( params.clusterFile , images3 , total_images );
    centroids3 = update_centroids( clusters3 , 784 , 10 , centroids1 );

    Hash_Group<Lsh_Hash < Image<int> , int > , Image<int> > *hash;
    images1 = readFile( params.inputFile , total_images );
    std::chrono::steady_clock::time_point start0 = std::chrono::steady_clock::now();
    clusters1 = clustering( images1  , 784 , params.method , hash , config_params[0] , &centroids1);
    std::chrono::steady_clock::time_point end0 = std::chrono::steady_clock::now();
    float duration0 = std::chrono::duration<float>(end0 - start0).count();

    images2 = readFile_latent( params.inputFile_latent , total_images );
    std::chrono::steady_clock::time_point start1 = std::chrono::steady_clock::now();
    clusters2 = clustering( images2  , 10 , params.method , hash , config_params[0] , &centroids2);
    std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
    float duration1 = std::chrono::duration<float>(end1 - start1).count();



    vector<Image <int> *>::iterator ptr; 
    for ( int i = 0 ; i < 10 ; i++ ){
        for (ptr = clusters2[i].begin(); ptr < clusters2[i].end(); ptr++) {    
            std::vector<Image<int> *>::iterator it = std::find(images2.begin(), images2.end(), *ptr);
            int index = std::distance(images2.begin(), it);
            clusters2_1[i].push_back(images1[index]);
        }
    }
    centroids4 = update_centroids( clusters2_1 , 784 , 10 , centroids1 );



    // silhouette function
    string silh_string1 = silhouette(clusters1, config_params[0] , &centroids1 , 784 );
    string silh_string2 = silhouette(clusters2_1, config_params[0] , &centroids2 , 784 );
    string silh_string3 = silhouette(clusters3, config_params[0] , &centroids3 , 784 );

    uint64_t o1 = objective_value( images1 , centroids1 , 784 );
    uint64_t o2 = objective_value( images1 , centroids4 , 784 );
    uint64_t o3 = objective_value( images1 , centroids3 , 784 );

    myfile<< "NEW SPACE" << endl;
    for ( int i = 0 ; i < 10 ; i++ ){
        myfile<< "CLUSTER-"<<i<<" {size: "<<clusters2_1[i].size()<<", centroid: [";
        centroids4[i]->print(&myfile);
        myfile<< " ] }"<<endl;
    }
    myfile<< "clustering_time: " << duration1 << endl ;
    myfile<< silh_string1 ;
    myfile<< "Value of Objective Function: " << o2 << endl << endl ;

    myfile<< "ORIGINAL SPACE" << endl;
    for ( int i = 0 ; i < 10 ; i++ ){
        myfile<< "CLUSTER-"<<i<<" {size: "<<clusters1[i].size()<<", centroid: [";
        centroids1[i]->print(&myfile);
        myfile<< " ] }"<<endl;
    }
    myfile<< "clustering_time: " << duration0 << endl ;
    myfile<< silh_string2;
    myfile<< "Value of Objective Function: " << o1 << endl << endl ;

    myfile<< "CLASSES AS CLUSTERS" << endl;
    myfile<<silh_string3;
    myfile<< "Value of Objective Function: " << o3 << endl << endl ;
    // cluster_output(params, duration, clusters, &centroids, silh_string , config_params[0], images1);

    for ( int i = 0 ; i < images1.size() ; i++ ){
        delete images1[i];
    }
    for ( int i = 0 ; i < images2.size() ; i++ ){
        delete images2[i];
    }
    delete[] config_params;



    return 1;
    
}

