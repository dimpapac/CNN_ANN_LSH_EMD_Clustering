#include <iostream>
#include "input_output.hpp"
#include "hash_group.hpp"


using namespace std;

int main (int argc, char** argv){

	std::fstream fs;
	int train_array_size,test_array_size;
	float emd_score;
	fs.open ("./data/emd_results.txt", std::fstream::in | std::fstream::out | std::fstream::app);

	fs >> train_array_size >> test_array_size >> emd_score;
	fs.close();

	Params params = inputValidate(argc, argv);

	vector<Image <int> *> images = readFile(params.inputFile, train_array_size );
   
	output_3( images, params , train_array_size , test_array_size , emd_score);

    for ( int i = 0 ; i < images.size() ; i++ ){
        delete images[i];
    }


}
