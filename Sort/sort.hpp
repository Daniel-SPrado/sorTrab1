#include <iostream>

#define SIZE_X 4						//Put the size of X
#define SIZE_Y 4						//Put the size of Y			

using namespace std;

class job
{
	public:
		int x;
		int y;
		int value;		
};

void sort(job jobs[SIZE_X][SIZE_Y], job result[SIZE_X][SIZE_Y]);