#pragma once
#include <iostream>
#include <cmath>
#include <immintrin.h>
#include <malloc.h>
#include <cstdint>
#include "safe_array.h"

enum SIMDMatrixType
{
	Nothinggg, Symmetricalll, AntiSymmetricalll, Diagonalll, UpperTriangularrr, LowerTriangularrr
};

/*void REQUIRE(bool condition, const char* string) {
	if (!condition)throw string;
}*/

class SIMDMatrix
{
private:
	uint8_t* m_arr = nullptr;
	int m_rows, m_cols;

	//removes a certain row and a cetain column, and returns a new matrix identical to the previuos one, expet for the row and the col that have been removed
	SIMDMatrix removeColAndRow(int r, int c) {
		REQUIRE(r >= 0 && r < m_rows && c >= 0 && c < m_cols, "Out of Bound Access");
		REQUIRE(m_cols == m_rows, "This is not a square Matrix");
		uint8_t* array = static_cast<uint8_t*>(_aligned_malloc(sizeof(uint8_t)*(m_rows - 1)*(m_cols - 1), sizeof(uint8_t)));
		size_t x = 0, y = 0;
		for (int row = 0; row < m_rows; row++)
		{
			y = 0;
			if (row == r)continue;
			for (int col = 0; col < m_cols; col++) {
				if (col == c)continue;
				array[(m_cols - 1)*x + y] = m_arr[m_cols*row + col];
				y++;
			}
			x++;
		}
		SIMDMatrix res(array, m_rows - 1, m_cols - 1);

		_aligned_free(array);
		return res;
	}
	//returns a copy of the matrix which is reduced in echelon form
	SIMDMatrix returnEchelonForm() {
		SIMDMatrix res(*this);
		res.reduceToEchelonForm();
		return res;
	}

	//Swaps two rows within the matrix
	void swapRows(size_t row1, size_t row2) {
		REQUIRE(row1 > 0 && row1 <= m_rows && row2 > 0 && row2 <= m_rows, "Out of Bounds");
		for (register int col = 0; col < m_cols; col++) {
			uint8_t temp = m_arr[m_cols*row1 + col];
			m_arr[m_cols * row1 + col] = m_arr[m_cols * row2 + col];
			m_arr[m_cols * row2 + col] = temp;
		}
	}

	//COUNTING THE NUMBER OF ZEROS BEFORE THE FIRST NON-NULL ELEMENT FOR A SPECIFIED ROW
	size_t n_of_leading_zeros(size_t row) {
		size_t counter = 0;
		for (register int j = 0; j < m_cols; ++j) {
			if (m_arr[m_cols*row + j] == 0)counter++;
			else return counter;
		}
		return counter;
	}

	//returns true in case of a floating point calculation approximation where a 0 is left as a very small floating point number
	bool false_zero(float n) {
		//only works with standard types
		if (std::abs(n) < 0.0000001) return true;
		return false;
	}
	//it reorders the rows depending on the number of leading zeros (more zeros on the bottom)
	void orderByLeadingZeros() {
		int most_zeros = -1;
		size_t n_of_zeros = 0;
		size_t counter = 0;
		for (register int k = m_rows; k > 0; --k) {
			for (register int i = 0; i < k; ++i) {
				counter = n_of_leading_zeros(i);
				if (counter >= n_of_zeros) {
					most_zeros = i;
					n_of_zeros = counter;
				}

				counter = 0;
			}
			//added a modification (k instead of m_rows);
			if (most_zeros != k - 1 && most_zeros >= 0)swapRows(most_zeros, k - 1);
			n_of_zeros = 0;
			most_zeros = -1;
		}
	}

	uint8_t getValIfPresentElse0(int x, int y) const {
		if (x < 0 || x >= m_cols || y < 0 || y >= m_rows)return 0;
		else return m_arr[m_cols * y + x];
	}
	uint8_t getVal(int x, int y) const {
		REQUIRE(x >= 0 && x < m_cols && y >= 0 && y < m_rows, "Out of Bounds");
		return m_arr[m_cols * y + x];
	}

public:
	void print_array() {
		for (int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_cols; j++) {
				std::cout << (int)m_arr[m_cols*i + j] << ',';
			}
			std::cout << '\n';
		}
		std::cout << std::endl;
	}

	//SEARCHES FOR PIVOT IN A SPECIFIED ROW
	uint8_t findPivot(size_t row) {
		uint8_t val;
		for (register int j = 0; j < m_cols; j++) {
			val = m_arr[m_cols*row + j];
			if (val != 0)return val;
		}
		return 0;
	}

	//searches for very small elements that can be aproximated to 0 and chanes their value to 0
	void check_and_clear_almost_zeros() {
		for (int i = 0; i < m_rows*m_cols; i++)
		{
			if (false_zero(m_arr[i]))m_arr[i] = abs(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////CONSTRUCTORS AND DESTRUCTORS////////////////////////////////////////7///////////////////
	SIMDMatrix() {
		m_arr = nullptr;
		m_cols = 0;
		m_rows = 0;
	}
	SIMDMatrix(int m, int n)
		:m_rows(m), m_cols(n)
	{
		m_arr = static_cast<uint8_t*>(_aligned_malloc(sizeof(uint8_t)*m*n, sizeof(uint8_t)));
		memset(m_arr, 0, sizeof(uint8_t)*(m * n));
	}
	SIMDMatrix(uint8_t** arr, int m, int n)
		:m_rows(m), m_cols(n)
	{
		m_arr = static_cast<uint8_t*>(_aligned_malloc(sizeof(uint8_t)*m*n, sizeof(uint8_t)));

		//@TODO: profile the cache miss/hit for this setup
		for (int row = 0; row < m_rows; row++)
		{
			for (int col = 0; col < m_cols; col++) {
				m_arr[m_cols*row + col] = arr[row][col];
			}
		}
	}
	SIMDMatrix(uint8_t* arr, int m, int n)
		:m_rows(m), m_cols(n)
	{
		m_arr = static_cast<uint8_t*>(_aligned_malloc(sizeof(uint8_t)*m*n, sizeof(uint8_t)));
		memcpy(m_arr, arr, sizeof(uint8_t)*(m*n));
	}
	SIMDMatrix(const SIMDMatrix& mat)
		:m_rows(mat.getRows()), m_cols(mat.getCols())
	{
		if (m_arr)_aligned_free(m_arr);
		m_arr = static_cast<uint8_t*>(_aligned_malloc(sizeof(uint8_t)*m_rows*m_cols, sizeof(uint8_t)));

		memcpy(m_arr, mat.getMatrixPointer(), sizeof(uint8_t)*(m_rows*m_cols));
	}
	SIMDMatrix(SIMDMatrix&& mat)
		:m_rows(mat.getRows()), m_cols(mat.getCols())
	{
		m_arr = mat.getMatrixPointer();
		mat.nullify();
	}
	~SIMDMatrix() {
		if (m_arr) {
			_aligned_free(m_arr);
		}
	}

	//////////////////////////////////////////////////////////////////////////CLASSES//////////////////////////////////////////////////////////////////////////

	class MITM {
	public:
		MITM(uint8_t* arr_, int n_) : arr(arr_), n(n_) { }

		uint8_t& operator[](int index) {
			REQUIRE(index >= 0 && index < n, "Out Of Bounds");
			return arr[index];
		}

	private:
		uint8_t* arr = nullptr;
		int n = 0;
	};

	//////////////////////////////////////////////////////////////////////////UTILITY METHODS//////////////////////////////////////////////////////////////////////////

	//returns number of rows
	inline int getRows() const { return m_rows; }

	//returns number of columns
	inline int getCols() const { return m_cols; }

	//returns the pointer to the first element of the array, so the array itself
	inline uint8_t* getMatrixPointer() const { return m_arr; }

	//sets the pointer to the array at null value
	inline void nullify() { m_arr = nullptr; }

	//gets the selected cell and puts the object in it
	void setElement(size_t i, size_t j, uint8_t el) {
		REQUIRE(i >= 0 && i < m_rows && j >= 0 && j < m_cols, "OutOfBoundAccess");
		m_arr[m_cols*i + j] = el;
	}
	//////////////////////////////////////////////////////////////////////////OPERATION FUNCTIONS//////////////////////////////////////////////////////////////////////////
	//adds a matrix to the current object
	void add(const SIMDMatrix& mat) {
		if (mat.getRows() != m_rows || mat.getCols() != m_cols)return;

		int remaining = (m_cols * m_rows) % 8;
		int steps = (m_cols*m_rows) / 8;

		for (register int i = 0; i < steps; i++) {
			__m256i first = _mm256_load_si256((const __m256i *)(m_arr + i * 8));
			__m256i second = _mm256_load_si256((const __m256i *)(mat.getMatrixPointer() + i * 8));
			first = _mm256_adds_epi8(first, second);
			memcpy(m_arr + i * 8, (uint8_t*)&first, sizeof(uint8_t) * 8);
		}
		for (register int i = m_cols * m_rows - remaining; i < m_cols*m_rows; i++) {
			m_arr[i] += *(mat.getMatrixPointer() + i);
		}
	}
	//@TODO optimize this
	void add(const uint8_t f) {
		for (register int i = 0; i < m_cols*m_rows; i++) {
			m_arr[i] += f;
		}
	}
	//subtracts a matrix from the current object
	void subtract(const SIMDMatrix& mat) {
		if (mat.getRows() != m_rows || mat.getCols() != m_cols)return;
		int remaining = (m_cols * m_rows) % 8;
		int steps = (m_cols*m_rows) / 8;

		for (register int i = 0; i < steps; i++) {
			__m256i first = _mm256_load_si256((const __m256i *)(m_arr + i * 8));
			__m256i second = _mm256_load_si256((const __m256i *)(mat.getMatrixPointer() + i * 8));
			first = _mm256_subs_epi8(first, second);
			memcpy(m_arr + i * 8, (uint8_t*)&first, sizeof(uint8_t) * 8);
		}
		for (register int i = m_cols * m_rows - remaining; i < m_cols*m_rows; i++) {
			m_arr[i] += *(mat.getMatrixPointer() + i);
		}
	}
	void subtract(const uint8_t f) {
		for (register int i = 0; i < m_cols*m_rows; i++) {
			m_arr[i] -= f;
		}
	}

	void convolve(safe_array<int>& kernel, size_t height, size_t width) {
		if (height != width || !(kernel.size % 2))return;
		int result = 0;
		uint8_t* buffer = static_cast<uint8_t*>(_aligned_malloc(sizeof(uint8_t)*m_rows*m_cols, sizeof(uint8_t)));

		int stride = int(height / 2);
		for (int y = 0; y < m_rows; y++) {//y of the matrix
			for (int x = 0; x < m_cols; x++) {//x of the matrix
				result = 0;

				for (int l = 0; l < height; l++) { //y of the filter
					for (int k = 0; k < width; k++) { // x of the filter
						result += this->getValIfPresentElse0(x + k - stride, y + l - stride) * kernel[l*width + k];
					}
				}
				if (result < 0)result = 0;
				else if (result > 255) result = 255;
				buffer[m_cols*y + x] = result;
			}
		}
		_aligned_free(m_arr);
		m_arr = buffer;
	}

	// multiplies the elements of a specified row for a scalar
	void multiplyRowByScalar(size_t row, uint8_t scalar) {
		for (register int i = 0; i < m_cols; ++i) {
			m_arr[m_cols*row + i] *= scalar;
		}
	}
	//multiplies the whole matrix by a scalar
	void multiplyByScalar(uint8_t scalar) {
		for (int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_cols; j++)
			{
				m_arr[m_cols* i + j] *= scalar;
			}
		}
	}
	//checks whether the two matrices are equal
	bool equal(SIMDMatrix& mat) {
		if (mat.getRows() != m_rows || mat.getCols() != m_cols)return false;
		for (int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_cols; j++)
			{
				if (m_arr[m_cols* i + j] != mat[i][j])return false;
			}
		}
		return true;
	}

	//selected a row, it sums to it another selected row
	void addRowToRow(size_t destination, size_t source, uint8_t multiplication_factor = 1) {
		if (destination < 0 || destination >= m_rows || source < 0 || source >= m_rows)return;
		for (int j = 0; j < m_cols; j++) {
			m_arr[m_cols*destination + j] = (m_arr[m_cols*destination + j] + m_arr[m_cols*source + j] * multiplication_factor);
		}
	}

	uint8_t trace() {
		REQUIRE(m_rows == m_cols, "Cannot calculate trace of a non-square matrix");
		uint8_t res = 0;
		for (int i = 0; i < m_rows; i++)
		{
			res += m_arr[m_cols* i + i];
		}

		return res;
	}

	//creates a new matrix that is the transposed of the original one
	SIMDMatrix transposedMatrix() {
		SIMDMatrix res(m_cols, m_rows);

		for (int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_cols; j++)
			{
				res[j][i] = m_arr[m_cols*i + j];
			}
		}
		return res;
	}
	//it returns the matrix result of the Row by Column product between this and another matrix
	SIMDMatrix rowByColProduct(SIMDMatrix& mat) {
		REQUIRE(m_cols == mat.getRows(), "Matrices' sizes don't match the requirements");
		SIMDMatrix result(m_rows, mat.getCols());
		int counter = 0;
		uint8_t temp = 0;
		for (int i = 0; i < m_rows; i++)
		{
			counter = 0;
			while (counter < mat.getCols()) {
				for (int j = 0; j < m_cols; j++)
				{
					temp += m_arr[m_cols*i + j] * mat.getMatrixPointer()[mat.getCols()*j + counter];
				}
				result[i][counter] = temp;
				temp = 0;
				counter++;
			}
		}
		return result;
	}
	//returns the result of the scalar product between this and another matrix
	uint8_t dotProduct(SIMDMatrix& mat) {
		SIMDMatrix temp = mat.transposedMatrix();
		SIMDMatrix res = rowByColProduct(temp);
		return res.trace();
	}
	//it returns the DETERMINANT of the current matrix
	//@TODO: check which cols/rows have more zeros and calculate it with respect to it
	double determinant() {
		REQUIRE(m_rows == m_cols, "Cannot calculate determinant of non-square matrix");
		if (m_rows == 1 && m_cols == 1) return m_arr[0];
		if (m_rows == 2 && m_cols == 2) {
			return m_arr[0] * m_arr[3] - m_arr[1] * m_arr[2];
		}

		double res = 0;
		SIMDMatrix temp(m_rows - 1, m_cols - 1);
		for (int i = 0; i < m_cols; i++) {
			temp = removeColAndRow(0, i);
			res += (std::pow(-1, i)*m_arr[i] * temp.determinant());
		}
		return res;
	}

	double det_2() {
		REQUIRE(m_rows == m_cols, "Cannot calculate determinant of non-square matrix");
		if (m_rows == 1 && m_cols == 1) return m_arr[0];
		if (m_rows == 2 && m_cols == 2) {
			return m_arr[0] * m_arr[3] - m_arr[1] * m_arr[2];
		}
		SIMDMatrix mat = this->returnEchelonForm();
		double res = 1.0f;
		for (register int i = 0; i < mat.getCols(); i++) {
			res *= mat[i][i];
		}
		return res;
	}

	//it returns the RANK of the current matrix
	int rank() {
		SIMDMatrix tmp = returnEchelonForm();
		int counter = 0;
		for (register int i = 0; i < m_rows; ++i) {
			if (tmp.findPivot(i) != 0)counter++;
			else return counter;
		}
		return counter;
	}
	//it returns the inverse matrix of the current one
	SIMDMatrix inverseMatrix() {
		uint8_t det = determinant();
		SIMDMatrix res(m_rows, m_cols);
		if (det != 0) {
			SIMDMatrix temp(m_rows - 1, m_cols - 1);
			for (int i = 0; i < m_rows; i++) {
				for (int j = 0; j < m_cols; j++) {
					temp = removeColAndRow(j, i);
					res[i][j] = std::pow(-1, i + j)*temp.determinant() / det;
				}
			}
		}
		res.check_and_clear_almost_zeros();
		return res;
	}
	//returns true if the current matrix is orthogonal (i.e if its inverse and its transposed are equal)
	bool isOrthogonal() {
		SIMDMatrix inverse = inverseMatrix();
		SIMDMatrix transpose = transposedMatrix();
		return inverse == transpose;
	}
	//not the most efficient....
	//complexity : O(n^4)
	//reduces the current matrix to echelon form
	void reduceToEchelonForm() {
		uint8_t pivot;
		orderByLeadingZeros();
		for (int i = 1; i < m_rows; i++) {
			if (n_of_leading_zeros(i - 1) < n_of_leading_zeros(i))continue;
			pivot = findPivot(i - 1);
			if (pivot == 0)continue; //OR BREAK, I DON'T KNOW WHAT IS ADVISABLE
			for (int j = i; j < m_rows; j++) {
				if (n_of_leading_zeros(i - 1) < n_of_leading_zeros(j))continue;
				addRowToRow(j, i - 1, -(findPivot(j) / pivot));
				//print_array();
			}
			orderByLeadingZeros();
		}
		check_and_clear_almost_zeros();
	}
	//returns true if the current matrix is symmetrical
	bool isSymmetrical() {
		if (m_cols != m_rows)return false;
		SIMDMatrix temp = transposedMatrix();
		return temp == *this;
	}
	//returns true if the current Matrix is AntiSymmetrical
	bool isAntisymmetrical() {
		if (m_cols != m_rows)return false;
		SIMDMatrix temp = transposedMatrix();
		temp *= (-1.0);
		return temp == *this;
	}

	bool isSquared() const {
		return (m_rows == m_cols);
	}

	bool isOdd() const {
		return (this->isSquared() && (m_rows % 2));
	}
	bool isEven() const {
		return (this->isSquared() && !(m_rows % 2));
	}
	//checks if the matrix is upper triangular
	bool isUpperTriangular() {
		if (m_cols != m_rows)return false;
		for (int i = 0; i < m_rows; i++) {
			for (int j = 0; j < m_cols; j++) {
				if (i <= j)continue;
				else {
					if (m_arr[m_cols*i + j] != 0)return false;
				}
			}
		}
		return true;
	}
	//returns true if the current matrix is LowerTriangular
	bool isLowerTriangular() {
		if (m_cols != m_rows)return false;
		for (int i = 0; i < m_rows; i++) {
			for (int j = 0; j < m_cols; j++) {
				if (i >= j)continue;
				else {
					if (m_arr[i*m_cols + j] != 0)return false;
				}
			}
		}
		return true;
	}
	//returns true if the current matrix is Diagonal
	bool isDiagonal() {
		if (m_cols != m_rows)return false;
		for (int i = 0; i < m_rows; i++) {
			for (int j = 0; j < m_cols; j++) {
				if (i == j)continue;
				else {
					if (m_arr[i*m_cols + j] != 0)return false;
				}
			}
		}
		return true;
	}

	SIMDMatrixType categorizeMatrix() {
		if (isDiagonal())return SIMDMatrixType::Diagonalll;
		else if (isLowerTriangular())return SIMDMatrixType::LowerTriangularrr;
		else if (isUpperTriangular())return SIMDMatrixType::UpperTriangularrr;
		else if (isSymmetrical())return SIMDMatrixType::Symmetricalll;
		else if (isAntisymmetrical())return SIMDMatrixType::AntiSymmetricalll;
		else return SIMDMatrixType::Nothinggg;
	}

	void pow(int n) {
		while (--n > 0) {
			(*this) *= (*this);
		}
	}

	uint8_t sum() {
		REQUIRE(m_cols == 1, "Sum only available with 1 col");
		uint8_t res = 0.0f;
		for (register int i = 0; i < m_rows; i++) {
			res += m_arr[i*m_cols];
		}
		return res;
	}

	//////////////////////////////////////////////////////////////////////////OPERATORSSSSSSS///////////////////////////////////////////////////////////////////

	inline MITM operator[](int index) const {
		REQUIRE(index >= 0 && index < m_rows, "Out of Bounds");
		return MITM(m_arr + (m_cols*index), m_cols);
	}

	inline void operator=(const SIMDMatrix& mat) {
		m_rows = mat.getRows();
		m_cols = mat.getCols();
		if (m_arr)
			_aligned_free(m_arr);
		m_arr = static_cast<uint8_t*>(_aligned_malloc(sizeof(uint8_t)*m_rows*m_cols, sizeof(uint8_t)));
		memcpy(m_arr, mat.getMatrixPointer(), sizeof(uint8_t)*(m_rows * m_cols));
	}
	inline void operator=(SIMDMatrix&& mat) {
		m_rows = mat.getRows();
		m_cols = mat.getCols();
		if (m_arr)
			_aligned_free(m_arr);
		m_arr = mat.getMatrixPointer();
		mat.nullify();
	}
	inline bool operator==(SIMDMatrix& rhs) {
		return equal(rhs);
	}
	inline bool operator!=(SIMDMatrix& rhs) {
		return !equal(rhs);
	}
	inline SIMDMatrix operator+(const SIMDMatrix& rhs) {
		SIMDMatrix res(*this);
		res.add(rhs);
		return res;
	}
	inline SIMDMatrix operator+(const uint8_t rhs) {
		SIMDMatrix res(*this);
		res.add(rhs);
		return res;
	}
	inline SIMDMatrix operator-(const SIMDMatrix& rhs) {
		SIMDMatrix res(*this);
		res.subtract(rhs);
		return res;
	}
	inline SIMDMatrix operator-(const uint8_t& rhs) {
		SIMDMatrix res(*this);
		res.subtract(rhs);
		return res;
	}

	inline SIMDMatrix& operator+=(const SIMDMatrix& rhs) {
		add(rhs);
		return *this;
	}
	inline SIMDMatrix& operator-=(const SIMDMatrix& rhs) {
		subtract(rhs);
		return *this;
	}
	inline SIMDMatrix operator*(const uint8_t scalar) {
		SIMDMatrix res(*this);
		res.multiplyByScalar(scalar);
		return res;
	}
	inline SIMDMatrix operator/(const uint8_t scalar) {
		SIMDMatrix res(*this);
		res.multiplyByScalar(1.0 / scalar);
		return res;
	}
	inline SIMDMatrix& operator*=(const uint8_t scalar) {
		multiplyByScalar(scalar);
		return *this;
	}
	inline SIMDMatrix& operator*=(SIMDMatrix& rhs) {
		*this = rowByColProduct(rhs);
		return *this;
	}
	inline SIMDMatrix& operator/=(const uint8_t scalar) {
		multiplyByScalar(1 / scalar);
		return *this;
	}
};
