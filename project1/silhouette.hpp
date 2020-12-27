#ifndef _SILHOUETTE_H
#define _SILHOUETTE_H

#include <string>  
#include <iostream> 
#include <sstream> 

template <typename S>
void print(S my_string){
	cout << my_string << endl;
}


// average distance of img and imgs of cluster
int average_distance(Image<int> * img, vector<Image <int> *> cluster){
	if (cluster.size() == 0)
	{
		return -1;
	}
	unsigned int sum = 0;
	for (int i = 0; i < cluster.size(); ++i)
	{
		if (img != cluster[i]) // not me 
		{
			int dist = manDistance( img->get_data() , cluster[i]->get_data() , 784 ); 
			sum += dist;
		}
	}
	return sum/(cluster.size());
}

//return index of my cluster
int my_cluster(Image<int> * img, vector<Image <int> *> *clusters){
	std::vector<Image <int> *>::iterator it;
	for (int i = 0; i < clusters->size(); ++i)
	{
		it = find (clusters[i].begin(), clusters[i].end(), img);
		if (it != clusters[i].end()){
			// std::cout << "Element found in myvector: " << i << '\n';
			return i;
		}
		else{
			// std::cout << "Element not found in myvector\n";
		}
	}

	return -1;
}

//find index of next best (neighbor) cluster
int b_index(Image<int> * img, vector<Image <int> *> *centroids, int total_clusters, int index ){
	// int index = my_cluster(img, clusters);
	int min = INT32_MAX; 
	int retVal = 0;
	for (int i = 0; i < total_clusters; ++i)
	{
		if (i!= index) //not my cluster
		{
			int avg = manDistance( img->get_data() , centroids->at(i)->get_data() , 784 );
			if (avg<min)
			{
				min = avg;
				retVal = i;
			}
		}
	}

	return retVal;
}



// T = vector<Image <int> *>
template <typename T>
string silhouette (T *clusters, int total_clusters , T *centroids)
{
	//for every cluster
	vector<float> average_s;
	vector<float> si;
	for (int i = 0; i < total_clusters; ++i)
	{
		//for every point in the cluster
		float avg_s = 0;
		for (int j = 0; j < clusters[i].size(); ++j)
		{
			int a = average_distance(clusters[i].at(j), clusters[i]);
			int bin = b_index(clusters[i].at(j), centroids , total_clusters, i);
			int b = average_distance(clusters[i].at(j), clusters[bin]);
			float s= 0;
			if (a<b)
			{
				s = 1-((float)a / (float)b);
				// cout << " s=  " << s << endl;
			}else if(a==b){
				// cout << " s=  " << 0 << endl;
			}else if(a>b){
				// cout << a << " " << b << endl;
				s = ((float)b / (float)a)-1;
				// cout << " s=  " << s << endl;
			}
			avg_s += s; 
			si.push_back(s);
		}
		if (clusters[i].size() == 0 )
		{
			average_s.push_back(-1);
		}
		else{
			avg_s = avg_s / clusters[i].size();
			// cout << "AVERAGE_S: " << avg_s << endl;
			average_s.push_back(avg_s);

		}
	}


	std::stringstream buffer;
	buffer << "Silhouette: [";
	for (int i = 0; i < average_s.size(); ++i)
	{
		buffer << average_s[i] <<", ";
	}

	float avg_stotal = 0;
	for (int i = 0; i < si.size(); ++i)
	{
		avg_stotal += si[i]; 
	}
	avg_stotal = avg_stotal / si.size();
	buffer << avg_stotal <<"]" << endl;

	return buffer.str();
}

#endif
