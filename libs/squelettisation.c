#include "squelettisation.h"
#include <stdio.h>

extern int imgHeight;
extern int imgWidth;

int max_dist(int **matrice)
{
	int i;
	int j;
	int max;

	max = matrice[0][0];

	for(i = 0; i < imgHeight; i++)
	{
		for(j = 0; j < imgWidth; j++)
		{
			if(matrice[i][j] > max)
			{
				max = matrice[i][j];
			}
		}
	}
	return(max);
}


t_flag flags_N(t_flag **flags, int x, int y)
{
	if(x < imgHeight - 1)
	{
		return(flags[x+1][y]);
	}
	return(0);
}

t_flag flags_E(t_flag **flags, int x, int y)
{
	if(y < imgWidth - 1)
	{
		return(flags[x][y+1]);
	}
	return(0);
}

t_flag flags_S(t_flag **flags, int x, int y)
{
	if(x > 0)
	{
		return(flags[x-1][y]);
	}
	return(0);
}

t_flag flags_O(t_flag **flags, int x, int y)
{
	if(y > 0)
	{
		return(flags[x][y-1]);
	}
	return(0);
}

t_flag flags_NE(t_flag **flags, int x, int y)
{
	//En cours
}

t_flag flags_SE(t_flag **flags, int x, int y)
{
	//En cours
}

t_flag flags_SO(t_flag **flags, int x, int y)
{
	//En cours
}

t_flag flags_NO(t_flag **flags, int x, int y)
{
	//En cours
}


void squelettisation(int **transformee, t_flag **flags)
{
	int max;
	int dist = 1;
	int x;
	int y;

	max = max_dist(transformee);
	
	//for(dist = 1; dist <= max; dist++)
	//{
		// Actualisation des contours
		for(x = 0; x < imgHeight; x++)
		{
			for(y = 0; y < imgWidth; y++)
			{
				if(transformee[x][y] == 0)
				{
					flags[x][y] = flags[x][y] | FOND;
				}
				else if(transformee[x][y] == dist)
				{
					flags[x][y] = flags[x][y] | CONTOUR;
				}
				else if(transformee[x][y] > dist)
				{
					flags[x][y] = flags[x][y] | INTERNE;
				}
			}
		}

		// Recherche des "multiples"
		for(x = 0; x < imgHeight; x++)
		{
			for(y = 0; y < imgWidth; y++)
			{
				if((flags[x][y] & CONTOUR)
					&& !((flags_N(flags, x, y) ^ flags_S(flags, x ,y)) & (CONTOUR | INTERNE))
					&& !((flags_E(flags, x, y) ^ flags_O(flags, x ,y)) & (CONTOUR | INTERNE)))
				{
					flags[x][y] = flags[x][y] | MULTIPLE;
				}
			}
		}
	//}
}
