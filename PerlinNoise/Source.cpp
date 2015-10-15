#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

double getRand(double min, double max)
{
	double num = (double)rand();
	num /= RAND_MAX;
	num *= (max - min);
	num += min;
	return num;
}

double lerp(double s, double e, double t)
{
	return s + (e - s) * t;
}

double blerp(double c11, double c21, double c22, double c12, double tx, double ty)
{
	return lerp(lerp(c11, c21, tx), lerp(c12, c22, tx), ty);
}

//Made with help from https://www.youtube.com/watch?v=Or19ilef4wE
void perlinNoise(int xn, int yn, int sps)
{
	int m = yn * sps;
	int n = xn * sps;
	double **X = new double*[m];
	double **Y = new double*[m];
	double **Z = new double*[m];
	double **U = new double*[yn + 1];
	double **V = new double*[yn + 1];
	double max = 0;

	for (int i = 0; i < m; i++)
	{
		X[i] = new double[n];
		Y[i] = new double[n];
		Z[i] = new double[n];
	}

	for (int i = 0; i < yn + 1; i++)
	{
		U[i] = new double[xn + 1];
		V[i] = new double[xn + 1];
	}

	for (int i = 0; i < yn + 1; i++)
	{
		for (int j = 0; j < xn + 1; j++)
		{
			U[i][j] = getRand(-1, 1);
			V[i][j] = getRand(-1, 1);
		}
	}

	double hx = 1.0 * xn / (n - 1);
	double hy = 1.0 * yn / (m - 1);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			X[i][j] = hx * j;
			Y[i][j] = hy * i;
		}
	}

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int xc = (int)X[i][j];
			int yc = (int)Y[i][j];
			if (!fmod(X[i][j], 1) && X[i][j])
			{
				xc -= 1;
			}
			if (!fmod(Y[i][j], 1) && Y[i][j])
			{
				yc -= 1;
			}

			double xr = X[i][j] - xc;
			double yr = Y[i][j] - yc;
			double S11[] = {    -xr,    -yr };
			double S21[] = {    -xr, 1 - yr };
			double S22[] = { 1 - xr, 1 - yr };
			double S12[] = { 1 - xr,    -yr };

			double Q11 = S11[0] * U[yc][xc] + S11[1] * V[yc][xc];
			double Q21 = S21[0] * U[yc+1][xc] + S21[1] * V[yc+1][xc];
			double Q22 = S22[0] * U[yc+1][xc+1] + S22[1] * V[yc+1][xc+1];
			double Q12 = S12[0] * U[yc][xc+1] + S12[1] * V[yc][xc+1];

			Z[i][j] = blerp(Q11, Q21, Q22, Q12, X[i][j], Y[i][j]);
			if (max < abs(Z[i][j]))
			{
				max = abs(Z[i][j]);
			}
		}
	}

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			Z[i][j] /= max;
			printf("%7.4f ", Z[i][j]);
		}
		printf("\n");
	}

	delete[] X;
	delete[] Y;
	delete[] Z;
	delete[] U;
	delete[] V;
}

int main()
{
	srand(time(NULL));
	perlinNoise(10, 10, 10);
	return 0;
}