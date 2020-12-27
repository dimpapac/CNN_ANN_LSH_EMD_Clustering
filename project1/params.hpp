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
	string inputFile, queryFile,inputFile_latent, queryFile_latent , outputFile, configurationFile, method;
} Params;


Params inputValidate (int argc, char *argv[])
{
    Params params;
    if (strcmp(argv[0],"./lsh") == 0)
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
	        // params.inputFile = "./data/train-images.idx3-ubyte";
	        // params.queryFile = "./data/t10k-images.idx3-ubyte";
	        // params.outputFile = "outputFile";

	        return params;
	    }

	    else
	    { 
	    	if ( argc != 15 )
	    	{
	    		cout <<"Usage: $./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>" << endl;
	    		exit(0);
	    	}

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
	        	return params;
	    	}
	    	else 
	    	{
	    		cout << "Error. Argument related error." << endl;
	    		exit(0);
	    	}
	    }
    }
	else if (strcmp(argv[0],"./cluster") == 0 )
    {
    	if(argc==1)
	    {
	        params.inputFile = "./data/train-images.idx3-ubyte";
	        params.queryFile = "./data/t10k-images.idx3-ubyte";
	        params.outputFile = "outputFileCluster";
			params.configurationFile = "./cluster.conf";
			params.complete = 1;
	        params.method = "Classic";

	        return params;
	    }

	    else if (argc == 9)
	    { 
	    	if ( strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-c") == 0 && strcmp(argv[5], "-o") == 0  && strcmp(argv[7], "-m") == 0)
	    	{	
	            if (strcmp(argv[8], "Classic") != 0 && strcmp(argv[8], "LSH") != 0 && strcmp(argv[8], "Hypercube") != 0)
	            {
	            	cout << "-m <method: Classic OR LSH or Hypercube>" << endl;
	    			exit(0);
	            }
	            params.inputFile = argv[2];
	            params.configurationFile = argv[4];
	            params.outputFile = argv[6];
	            params.complete = 0;
	            params.method = argv[8];
	        	return params;
	    	}
	    	else 
	    	{
	    		cout << "Error. Argument related error." << endl;
	    		exit(0);
	    	}
	    }
	    else if (argc == 10)
	    { 
	    	if ( strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-c") == 0 && strcmp(argv[5], "-o") == 0  && strcmp(argv[7], "-complete") == 0 && strcmp(argv[8], "-m") == 0 )
	    	{
	            if (strcmp(argv[9], "Classic") != 0 && strcmp(argv[9], "LSH") != 0 && strcmp(argv[9], "Hypercube") != 0)
	            {
	            	cout << "-m <method: Classic OR LSH or Hypercube>" << endl;
	    			exit(0);
	            }
	            params.inputFile = argv[2];
	            params.configurationFile = argv[4];
	            params.outputFile = argv[6];
	            params.complete = 1;
	            params.method = argv[9];
	        	return params;
	    	}
	    	else 
	    	{
	    		cout << "Error. Argument related error." << endl;
	    		exit(0);
	    	}
	    }
	    else{
	    	cout <<"Usage: ./cluster –i <input file> –c <configuration file> -o <output file> -complete "<< \
						"<optional> -m <method: Classic OR LSH or Hypercube>" << endl;
	    		exit(0);
	    }
    }
	return params;
}


#endif
