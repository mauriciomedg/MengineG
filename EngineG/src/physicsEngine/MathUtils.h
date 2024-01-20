#pragma once

#include <vector>

namespace MG
{
	void multiply(const std::vector<float>& a, int nra, int nca, bool atrans, const std::vector<float>& b, int nrb, int ncb, bool btrans, std::vector<float>& result);

	void choldc(std::vector<float>& a, int n, std::vector<float>& p);

	void invA(std::vector<float>& a, int n, const std::vector<float>& p, std::vector<float>& Ainv);

	void addBlockDiagonal(std::vector<float>& a, int nra, int nca, std::vector<float>& b, int nrb, int ncb);

	void addBlockColumn(std::vector<float>& a, int nra, int nca, std::vector<float>& b, int ncb);
	
	void addBlockRow(std::vector<float>& a, int nra, int nca, std::vector<float>& b, int nrb);
}