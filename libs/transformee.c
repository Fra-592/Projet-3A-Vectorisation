#include "transformee.h"
#include <stdio.h>


extern int imgHeight;
extern int imgWidth;


int min(int x, int y)
{
	if(x >= y)
	{
		return(y);
	}
	return(x);
}

int max(int x, int y)
{
	if(x <= y)
	{
		return(y);
	}
	return(x);
}


int dist_S(int **image, int **transformee, int x, int y)
{
	if(x < imgHeight - 1)
	{
		return(transformee[x+1][y]);
	}
	return(0);
}

int dist_E(int **image, int **transformee, int x, int y)
{
	if(y < imgWidth - 1)
	{
		return(transformee[x][y+1]);
	}
	return(0);
}

int dist_N(int **image, int **transformee, int x, int y)
{
	if(x > 0)
	{
		return(transformee[x-1][y]);
	}
	return(0);
}

int dist_O(int **image, int **transformee, int x, int y)
{
	if(y > 0)
	{
		return(transformee[x][y-1]);
	}
	return(0);
}


void transformee_distance(int **image, int **transformee)
{
	int x;
	int y;

	//1ère passe
	for(x = 0; x < imgHeight; x++)
	{
		for(y = 0; y < imgWidth; y++)
		{
			if(image[x][y] == 0)
			{
				transformee[x][y] = 0;
			}
			else
			{
				transformee[x][y] = min(dist_N(image, transformee, x, y), dist_O(image, transformee, x, y)) + 1;
			}
		}
	}

	//2nde passe
	for(x = imgHeight - 1; x >= 0; x--)
	{
		for(y = imgWidth - 1; y >= 0; y--)
		{
			transformee[x][y] = min(transformee[x][y], min(dist_E(image, transformee, x, y), dist_S(image, transformee, x, y))+1);
		}
	}
}
