#include "MathUtils.h"
#include <iostream>

namespace MG
{
	void swap(int& a, int& b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

	void multiply(const std::vector<float>& a, int nra, int nca, bool atrans, const std::vector<float>& b, int nrb, int ncb, bool btrans, std::vector<float>& result)
	{
		if (atrans) swap(nra, nca);
		if (btrans) swap(nrb, ncb);

		result.resize(nra * ncb, 0.0f);

		for (int i = 0; i < nra; i++) {
			for (int j = 0; j < ncb; j++) {

				result[j + i * ncb] = 0;

				for (int k = 0; k < nrb; k++) {
					result[j + i * ncb] += (atrans ? a[i + k * nra] : a[k + i * nca]) * (btrans ? b[k + j * nrb] : b[j + k * ncb]);
				}
			}
		}
	}

	void choldc(std::vector<float>& a, int n, std::vector<float>& p)
		//Given a positive - definite symmetric matrix a[1..n][1..n], this routine constructs its Cholesky
		//decomposition, A = L � LT.On input, only the upper triangle of a need be given; it is not
		//modified.The Cholesky factor L is returned in the lower triangle of a, except for its diagonal
		//elements which are returned in p[1..n].
	{
		//void nrerror(char error_text[]);
		p.resize(n, 0.0f);

		int i, j, k;
		float sum;
		for (i = 0; i < n; i++) {
			for (j = i; j < n; j++) {
				for (sum = a[j + i * n], k = i - 1; k > 0; k--) sum -= a[k + i * n] * a[k + j * n];
				if (i == j) {
					if (sum <= 0.0) //a, with rounding errors, is not positive definite.
						std::cout << "choldc failed" << std::endl;
					p[i] = sqrt(sum);
				}
				else a[i + j * n] = sum / p[i];
			}
		}
	}

	void invL(std::vector<float>& a, int n, const std::vector<float>& p)
	{
		for (int i = 0; i < n; i++)
		{
			a[i + i * n] = 1.0 / p[i];
			for (int j = i + 1; j < n; j++) {
				float sum = 0.0;
				for (int k = i; k < j; k++) sum -= a[k + j * n] * a[i + k * n];
				a[i + j * n] = sum / p[j];
			}
		}
	}

	void invA(std::vector<float>& a, int n, const std::vector<float>& p, std::vector<float>& Ainv)
	{
		invL(a, n, p);

		for (int i = 0; i < n; i++)
		{
			for (int j = i + 1; j < n; j++)
			{
				a[j + i * n] = 0.0f;
			}
		}

		multiply(a, n, n, true, a, n, n, false, Ainv);
	}

	void addBlockDiagonal(std::vector<float>& a, int nra, int nca, std::vector<float>& b, int nrb, int ncb)
	{
		int row = nra + nrb;
		int col = nca + ncb;

		std::vector<float> temp(row * col, 0.0f);

		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				//int i1 = i % nra;
				//int j1 = j % nca;
				if (i < nra && j < nca)
					temp[j + i * col] = a[j + i * nca];
					//temp[j1 + i1 * col] = a[j1 + i1 * nca];

				if (i >= nra && j >= nca)
				{
					temp[j + i * col] = b[j - nca + (i - nra) * ncb];
				}
			}
		}
		a = temp;
	}

	void addBlockColumn(std::vector<float>& a, int nra, int nca, std::vector<float>& b, int ncb)
	{
		int row = nra;
		int col = nca + ncb;

		std::vector<float> temp(row * col, 0.0f);

		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				if (j < nca)
					temp[j + i * col] = a[j + i * nca];

				if (j >= nca)
					temp[j + i * col] = b[j - nca + i * ncb];
			}
		}

		a = temp;
	}

	void addBlockRow(std::vector<float>& a, int nra, int nca, std::vector<float>& b, int nrb)
	{
		int row = nra + nrb;
		int col = nca;

		std::vector<float> temp(row * col, 0.0f);

		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				if (i < nrb)
					temp[j + i * col] = a[j + i * nca];

				if (i >= nrb)
					temp[j + i * col] = b[j + (i - nrb) * nca];
			}
		}

		a = temp;
	}
}