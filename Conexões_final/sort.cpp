#include "sort.hpp"

void sort(job jobs[SIZE_X][SIZE_Y], job result[SIZE_X][SIZE_Y])
{
	int y = 0;
	int x = 0;

	begin:
	for (int i = 0; i < SIZE_X; i++)
	{
		for (int j = 0; i < SIZE_Y; j++)
		{
			if(jobs[i][j].x == x && jobs[i][j].y == y)
			{
				result[x][y].x = jobs[i][j].x;
				result[x][y].y = jobs[i][j].y;
				result[x][y].value = jobs[i][j].value;

				y++;

				if(y == SIZE_Y){
					y = 0;
					x++;
				}

				if(x == SIZE_X)
					goto end;

				goto begin;
			}
				
		
		}
	}
	
	end:
	cout << "Sorted! \n";
}