#include <iostream>
#include "input_output.hpp"
#include "hash_group.hpp"


using namespace std;

int main (int argc, char** argv){
	Params params = inputValidate(argc, argv);

  	 // readfile_get_dimensions
    Hash_Group<Lsh_Hash < Image<int> , int > , Image<int> > *group = new Hash_Group<Lsh_Hash< Image<int> , int > , Image<int> >( params.M , params.L , 15 , 784 , params.k , 40000 , 536870912 );   
    vector<Image <int> *> images = readFileAddHash( group, params.inputFile , params.total_images );

	vector<Image <int> *> images_latent = readFile_latent(params.inputFile_latent , params.total_images );
   
	string quit;
	if (params.flagFiles == 0)
	{
		cout << "Give path of query File" << endl;
		cin >> params.queryFile;
		cout << "Give path of output File" << endl;
		cin >> params.outputFile;
	}
    output(images, images_latent , group, 0, params);

    while(1){
    	cout << "output at -> " << params.outputFile << endl;
    	cout << "terminate? (y/n)" << endl;
    	cin >> quit;
    	if (quit.compare("y") == 0)
    	{
    		break;
    	}
    	cout << "Give path of new query file" << endl;
    	cin >> params.queryFile;
    	output(images, images_latent , group, 0, params);
    }

    for ( int i = 0 ; i < images.size() ; i++ ){
        delete images[i];
    }

    delete group;

}
