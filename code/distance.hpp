#ifndef _DISTANCE_H
#define _DISTANCE_H

#include <vector>
#include <cmath>

using namespace std;

//manhattan distance given two vectors of size d
int manDistance(const vector<int> &A, const vector<int> &B, int d){
	int sum = 0;
	for (int i = 0; i < d; i++)
	{
		sum += abs(A[i] - B[i]);
	}
	return sum;
}





#endif