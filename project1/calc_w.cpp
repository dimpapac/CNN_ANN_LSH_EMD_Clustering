#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <limits>

#include "image.hpp"
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


int main (int argc, char** argv){

	

    vector<Image<int>*> img_list;

	ifstream file ("./data/train-images.idx3-ubyte",ios::binary);
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

	    // cout << n_images << endl;
	    for(int i=0;i<n_images;++i) {

	        //Create Image object 
	        Image<int>* img = new Image<int>;

	        //Fill Image Object
	        for(int r=0;r<rows;++r) {
	            for(int c=0;c<cols;++c) {
	                unsigned char temp=0;
	                file.read((char*)&temp,sizeof(temp));
	                img->add_pixel( (int) temp );
	            }
	        }

	        // img->print();
	        //add Image to List of Images
	        img_list.push_back(img);

	    }


	    // Close the file.
	    file.close();
	}


	int step = 0;
	cout << "Give step (total elements:"<< img_list.size() << ")" << endl;
	cin >> step;

	if (!(step>0))
	{
		cout << "step must be a positive integer (1-"<< img_list.size() << ")" << endl;
		return -1;
	}
	
	unsigned long long dist_sum = 0;

	for (int i=0; i < img_list.size(); i=i+step)
	{
		cout << "i: " << i << endl;
		int min = INT32_MAX;
		for (int j=0; j < img_list.size(); j++)
		{
			if (i!=j)
			{
				int dist = manDistance(img_list[i]->get_data(), img_list[j]->get_data(), img_list[i]->get_data().size());
				// cout << "dist: " << dist << endl;
				if (dist < min)
				{
					min = dist;
				}
			}
		}

		// cout << "min: " << min << endl;
		dist_sum += min;
	}

	int checked = img_list.size()/step;
	cout << "average: " << dist_sum / checked << endl;

	return 0;
}
