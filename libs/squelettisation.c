#include "squelettisation.h"

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
	if(x > 0)
	{
		return(flags[x-1][y]);
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
	if(x < imgHeight - 1)
	{
		return(flags[x+1][y]);
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
	if((x > 0) && (y < imgWidth - 1))
	{
		return(flags[x-1][y+1]);
	}
	return(0);
}

t_flag flags_SE(t_flag **flags, int x, int y)
{
	if((x < imgHeight - 1) && (y < imgWidth - 1))
	{
		return(flags[x+1][y+1]);
	}
	return(0);
}

t_flag flags_SO(t_flag **flags, int x, int y)
{
	if((x < imgHeight - 1) && (y > 0))
	{
		return(flags[x+1][y-1]);
	}
	return(0);
}

t_flag flags_NO(t_flag **flags, int x, int y)
{
	if((x > 0) && (y > 0))
	{
		return(flags[x-1][y-1]);
	}
	return(0);
}

int multiple(t_flag **flags, int x, int y)
{
	if(!(flags_N(flags, x, y) ^ flags_S(flags, x, y))&(INTERNE | FOND)
		|| !(flags_E(flags, x, y) ^ flags_O(flags, x, y))&(INTERNE | FOND))
	{
		return(1);
	}
	if((flags_NE(flags, x, y) & CONTOUR) 
		&& (flags_SE(flags, x, y) & CONTOUR) 
		&& (flags_SO(flags, x, y) & CONTOUR) 
		&& (flags_NO(flags, x, y) & CONTOUR))
	{
		if((flags_N(flags, x, y) & FOND) 
			&& (flags_E(flags, x, y) & FOND) 
			&& (flags_S(flags, x, y) & FOND) 
			&& (flags_O(flags, x, y) & FOND))
		{
			return(1);
		}
	}
	return(0);
}

void squelettisation(int **transformee, t_flag **flags)
{
	int max, dist;
	int x, y;
	int compteur;

	max = max_dist(transformee);
	
	for(dist = 1; dist <= max && compteur; dist++)
	{
		// Actualisation des contours
		compteur = 0;
		for(x = 0; x < imgHeight; x++)
		{
			for(y = 0; y < imgWidth; y++)
			{
				if(transformee[x][y] == dist)
				{
					flags[x][y] = flags[x][y] | CONTOUR;
					compteur += 1;
				}
				else if(transformee[x][y] > dist)
				{
					flags[x][y] = flags[x][y] | INTERNE;
				}
				else if(transformee[x][y] == 0)
				{
					flags[x][y] = flags[x][y] | FOND;
				}
			}
		}

		// Recherche des "multiples"
		for(x = 0; x < imgHeight; x++)
		{
			for(y = 0; y < imgWidth; y++)
			{
				if(flags[x][y] & CONTOUR)
				{
					if(multiple(flags, x, y))
					{
						flags[x][y] = flags[x][y] | MULTIPLE;
						compteur -= 1;
					}
				}
			}
		}

		// Tri des "multiples"
		for(x = 0; x < imgHeight; x++)
		{
			for(y = 0; y < imgWidth; y++)
			{
				if(flags[x][y] & CONTOUR)
				{
					if(flags[x][y] & MULTIPLE)
					{
						flags[x][y] = flags[x][y] | SQUELETTE;
					}
					else
					{
						flags[x][y] = flags[x][y] ^ (CONTOUR | FOND);
					}
				}
			}
		}
	}
	/* TODO: Amincissement */
}
