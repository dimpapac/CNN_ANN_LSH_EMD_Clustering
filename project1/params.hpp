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
	string inputFile, queryFile, outputFile, configurationFile, method;
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

	    	if ( strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-q") == 0 && strcmp(argv[5], "-k") == 0  && strcmp(argv[7], "-L") == 0 && strcmp(argv[9], "-o") == 0 \
	    			&& strcmp(argv[11], "-N") == 0 && strcmp(argv[13], "-R") == 0 )
	    	{
	            params.inputFile = argv[2];
	            params.queryFile = argv[4];
	            params.k = atoi(argv[6]);
	            params.L = atoi(argv[8]);
	            params.outputFile = argv[10];
	            params.N = atoi(argv[12]);
	            params.R = atof(argv[14]);
				params.M = 10;
				params.flagFiles = 1;
	        	return params;
	    	}
	    	else 
	    	{
	    		cout << "Error. Argument related error." << endl;
	    		exit(0);
	    	}
	    }
    }
    else if (strcmp(argv[0],"./cube") == 0 || strcmp(argv[0],"./hypercube") == 0)
    {
    	if(argc==1)
	    {
	    	cout << "Give path to the dataset" << endl;
			cin >> params.inputFile;

	        params.k = 4;
	        params.M = 100;
	        params.probes = 2;
	        params.N = 1;
	        params.R = 10000;
	        // params.inputFile = "./data/train-images.idx3-ubyte";
	        // params.queryFile = "./data/t10k-images.idx3-ubyte";
	        // params.outputFile = "outputFile";
			params.flagFiles = 0;
	        return params;
	    }

	    else
	    { 
	    	if ( argc != 17 )
	    	{
	    		cout <<"Usage: ./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο "<< \
						"<output file> -Ν <number of nearest> -R <radius>" << endl;
	    		exit(0);
	    	}

	    	if ( (strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-q") == 0 && strcmp(argv[5], "-k") == 0  && strcmp(argv[7], "-M") == 0 && strcmp(argv[9], "-probes") == 0 && strcmp(argv[11], "-o") == 0 \
	    			&& strcmp(argv[13], "-N") == 0 && strcmp(argv[15], "-R") == 0) )
	    	{
	            params.inputFile = argv[2];
	            params.queryFile = argv[4];
	            params.k = atoi(argv[6]);
	            params.M = atoi(argv[8]);
	            params.probes = atoi(argv[10]);
	            params.outputFile = argv[12];
	            params.N = atoi(argv[14]);
	            params.R = atof(argv[16]);
				params.flagFiles = 1;
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
