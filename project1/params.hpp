#ifndef _PARAMS_H
#define _PARAMS_H

#include <string.h>

#include "distance.hpp"

typedef struct
{
	int k;
	int L;
	int N;
	int M;
	int probes;
	int R;
	int complete;
	int flagFiles;
	int total_images;
	int total_queries;
	string inputFile, queryFile,inputFile_latent, queryFile_latent , outputFile, configurationFile, method , labels_input , labels_query , clusterFile;
} Params;


Params inputValidate (int argc, char *argv[])
{
    Params params;
    if (strcmp(argv[0],"./search2") == 0)
    {
	    if(argc==1)
	    {
			cout << "Give path to the dataset" << endl;
			cin >> params.inputFile;

			params.M = 10;
	        params.k = 14;
	        params.L = 5;
	        params.N = 1;
	        params.R = 10000;
	        params.flagFiles = 0;
			params.total_images = 1000;
			params.total_queries = 100;
	        // params.inputFile = "./data/train-images.idx3-ubyte";
	        // params.queryFile = "./data/t10k-images.idx3-ubyte";
	        // params.outputFile = "outputFile";

	        return params;
	    }
	    else if ( argc == 15 )
	    { 
	    	if ( strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-i") == 0 && strcmp(argv[5], "-q") == 0 && strcmp(argv[7], "-s") == 0 && strcmp(argv[9], "-k") == 0  && strcmp(argv[11], "-L") == 0 && strcmp(argv[13], "-o") == 0 )
	    	{
	            params.inputFile = argv[2];
	            params.queryFile = argv[4];
				params.inputFile_latent = argv[6];
	            params.queryFile_latent = argv[8];
	            params.k = atoi(argv[10]);
	            params.L = atoi(argv[12]);
	            params.outputFile = argv[14];
				params.flagFiles = 1;
				params.R = 10000;
				params.N = 1;
				params.total_images = -1;
				params.total_queries = -1;
	        	return params;
	    	}
	    	else 
	    	{
	    		cout << "Error. Argument related error." << endl;
	    		exit(0);
	    	}
	    }
		else if ( argc == 17 )
	    { 
	    	if ( strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-i") == 0 && strcmp(argv[5], "-q") == 0 && strcmp(argv[7], "-s") == 0 && strcmp(argv[9], "-k") == 0  && strcmp(argv[11], "-L") == 0 && strcmp(argv[13], "-o") == 0 )
	    	{
	            params.inputFile = argv[2];
	            params.queryFile = argv[4];
				params.inputFile_latent = argv[6];
	            params.queryFile_latent = argv[8];
	            params.k = atoi(argv[10]);
	            params.L = atoi(argv[12]);
	            params.outputFile = argv[14];
				params.total_images = atoi(argv[15]);
				params.total_queries = atoi(argv[16]);
				params.flagFiles = 1;
				params.R = 10000;
				params.N = 1;
	        	return params;
	    	}
	    	else 
	    	{
	    		cout << "Error. Argument related error." << endl;
	    		exit(0);
	    	}
	    }
		else{
	    		cout <<"Usage: $./search2 –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>" << endl;
	    		exit(0);
	    }
    }
	else if (strcmp(argv[0],"./cluster") == 0 )
    {
    	if(argc==1)
	    {
	        params.inputFile = "./data/train-images.idx3-ubyte";
	        params.inputFile_latent = "../train_latent";
	        params.clusterFile = "./data/classification_results.txt";
			params.configurationFile = "./cluster.conf";
	        params.outputFile = "outputFileCluster";
		    params.total_images = 1000;
			params.complete = 1;
	        params.method = "Classic";

	        return params;
	    }

	    else if (argc == 11)
	    { 
	    	if ( strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-i") == 0 && strcmp(argv[5], "-n") == 0  \
	    		&& strcmp(argv[7], "-c") == 0 && strcmp(argv[9], "-o") == 0 )
	    	{	
	            
	            params.inputFile = argv[2];
		        params.inputFile_latent = argv[4];
	            params.clusterFile = argv[6];
				params.configurationFile = argv[8];
		        params.outputFile = argv[10];
		        params.total_images = -1; 
				params.complete = 1;
		        params.method = "Classic";
	        	return params;
	    	}
	    	else 
	    	{
	    		cout << "Error. Argument related error." << endl;
	    		cout <<"./cluster –d <input file original space> -i <input file new space>"<< \
					"-n <classes from NN as clusters file> –c <configuration file> -o <output file>" << endl;
	    		exit(0);
	    	}
	    }
	    else if (argc == 12)
	    { 
	    	if ( strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-i") == 0 && strcmp(argv[5], "-n") == 0  \
	    		&& strcmp(argv[7], "-c") == 0 && strcmp(argv[9], "-o") == 0 )
	    	{	
	            
	            params.inputFile = argv[2];
		        params.inputFile_latent = argv[4];
	            params.clusterFile = argv[6];
				params.configurationFile = argv[8];
		        params.outputFile = argv[10];
		        params.total_images = atoi(argv[11]);
				params.complete = 1;
		        params.method = "Classic";
	        	return params;
	    	}
	    	else 
	    	{
	    		cout << "Error. Argument related error." << endl;
	    		cout <<"./cluster –d <input file original space> -i <input file new space>"<< \
					"-n <classes from NN as clusters file> –c <configuration file> -o <output file>" << endl;
	    		exit(0);
	    	}
	    }
	    else{
	    	cout <<"./cluster –d <input file original space> -i <input file new space>"<< \
					"-n <classes from NN as clusters file> –c <configuration file> -o <output file>" << endl;
	    	exit(0);
	    }
    }
	else if (strcmp(argv[0],"./search3") == 0 )
    {
    	if(argc==1)
	    {
	        params.inputFile = "../data/train-images.idx3-ubyte";
	        params.queryFile = "../data/t10k-images.idx3-ubyte";
			params.labels_input = "../data/train-labels.idx1-ubyte";
	        params.labels_query = "../data/t10k-labels.idx1-ubyte";
			params.outputFile = "outputFileSearch3";
	        return params;
	    }

	    else if (argc == 12)
	    { 
	    	if ( strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-q") == 0 && strcmp(argv[5], "-l1") == 0  \
	    		&& strcmp(argv[7], "-l2") == 0 && strcmp(argv[9], "-o") == 0 && strcmp(argv[11], "-EMD") == 0)
	    	{	
	            
	            params.inputFile = argv[2];
	            params.queryFile = argv[4];
	            params.labels_input = argv[6];
	            params.labels_query = argv[8];
	            params.outputFile = argv[10];
	        	return params;
	    	}
	    	else 
	    	{
	    		cout << "Error. Argument related error." << endl;
	    		cout <<"Usage: ./search3 –d <input file original space> –q <query file original space> -l1 "<< \
					"<labels of input dataset> -l2 <labels of query dataset> -ο <output file> -EMD" << endl;
	    		exit(0);
	    	}
	    }
	    else{
	    	cout <<"Usage: ./search3 –d <input file original space> –q <query file original space> -l1 "<< \
					"<labels of input dataset> -l2 <labels of query dataset> -ο <output file> -EMD" << endl;
	    	exit(0);
	    }
    }
	return params;
}


#endif
