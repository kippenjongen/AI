#include "Matrix.h"

#include <cassert>

Matrix::~Matrix(){
	if(m_matrix)
		delete[] m_matrix;
}

float& Matrix::operator()(const int x, const int y){
	assert("Operator(): " && x < m_width && x >= 0 && y < m_height && y >= 0);
	return m_matrix[m_index(x, y)];
}

float& Matrix::operator[](const int z){
	assert("Operator[]: " && z < m_height * m_width && z >= 0);
	return m_matrix[z];
}

int Matrix::m_index(const int x, const int y){
	assert("m_index(): " && y < m_height && y >= 0);
	assert("m_index(): " && x < m_width && x >= 0);
	return (y * m_width) + x;
}

std::ostream& operator<<(std::ostream& out, Matrix matrix){
	for(int y = 0; y < matrix.m_height; y++){
		for(int x = 0; x < matrix.m_width; x++){
			out << matrix(x, y) << '\t';
		}
		out << "\n";
	}
	out << '\n';

	return out;

}

float sigmoid(float x){
	return 1/(1+std::exp(-x));
}

float derivative(float x){
	return x*(1-x);
}

int Matrix::getHeight(){
	return m_height;
}

int Matrix::getWidth(){
	return m_width;
}

Matrix operator-(Matrix &m1, Matrix &m2){

	assert("operator-: " && m1.m_height == m2.m_height);
	assert("operator-: " && m1.m_width == m2.m_width);

//	if(m1.m_height != m2.m_height || m1.m_width != m2.m_width)
//		std::cerr << "operator-: Invalid size!\n";

	Matrix out(m1.m_height, m1.m_width);

	for(int z = 0; z < m1.m_width * m1.m_height; z++){
		out[z] = m1[z] - m2[z];
	}

	return out;
}

Matrix operator+(Matrix m1, Matrix m2){

	assert("operator+: " && m1.m_height == m2.m_height);
	assert("operator+: " && m1.m_width == m2.m_width);

//	if(m1.m_height != m2.m_height || m1.m_width != m2.m_width)
//		std::cerr << "operator+: Invalid size!\n";

	Matrix out(m1.m_height, m1.m_width);

	for(int z = 0; z < out.m_width * out.m_height; z++){
		out[z] = m1[z] + m2[z];
	}

	return out;
}

Matrix operator*(Matrix m1, Matrix m2){

	assert("operator*: " && m1.m_height == m2.m_height);
	assert("operator*: " && m1.m_width == m2.m_width);

//	if(m1.m_height != m2.m_height || m1.m_width != m2.m_width)
//		std::cerr << "operator*: Invalid size!\n";

	Matrix out(m1.m_height, m1.m_width);

	for(int z = 0; z < m1.m_height * m1.m_width; z++) {
		out[z] = m1[z] * m2[z];
	}

	return out;
}

Matrix dot(Matrix m1, Matrix m2){

	assert("dot(): " && m1.m_width == m2.m_height);

	Matrix out(m1.m_height, m2.m_width);

	int rest = m1.m_width;

	for(int y = 0; y < out.m_height; y++){
		for(int x = 0; x < out.m_width; x++){
			for(int z = 0; z < m1.m_width; z++){
				out(x, y) += m1(z, y) * m2(x, z);
			}
		}
	}

	return out;
}

Matrix Matrix::transpose(){
	Matrix out(m_width, m_height);

	for (int i = 0; i < m_height * m_width; i++) {
		int row = i / m_height;
		int col = i % m_height;
		out[i] = m_matrix[m_width * col + row];
	}

	return out;
}

Matrix::Matrix(const Matrix& source) {
	// because m_length is not a pointer, we can shallow copy it
	m_height = source.m_height;
	m_width = source.m_width;

	// m_data is a pointer, so we need to deep copy it if it is non-null
	if (source.m_matrix)
	{
		// allocate memory for our copy
		m_matrix = new float[m_width * m_height];

		for(int z = 0; z < m_height * m_width; z++){
			m_matrix[z] = source.m_matrix[z];
		}

	}
	else
		m_matrix = 0;
}

Matrix& Matrix::operator=(const Matrix& source){
	// check for self-assignment
	if (this == &source)
		return *this;

	// first we need to deallocate any value that this string is holding!
	delete[] m_matrix;

	// because m_length is not a pointer, we can shallow copy it
	m_height = source.m_height;
	m_width = source.m_width;

	// m_data is a pointer, so we need to deep copy it if it is non-null
	if (source.m_matrix)
	{
		// allocate memory for our copy
		m_matrix = new float[m_width * m_height];

		for(int z = 0; z < m_height * m_width; z++){
			m_matrix[z] = source.m_matrix[z];
		}

	}
	else
		m_matrix = 0;

	return *this;
}

void Matrix::init(const std::initializer_list<std::initializer_list<float>> &list){

	if(m_matrix)
		delete[] m_matrix;

	m_height = 0;

	for(auto &element : list){
		m_width = element.size();
		m_height += 1;
	}

	m_matrix = new float[m_height * m_width];

	int count = 0;
	for(auto &element : list){
		for(auto &el : element) {
			m_matrix[count] = el;
			count++;
		}
	}

//	std::cout << "\nheight: " << m_height << "\twidth: " << m_width << "\n\n";
}

Matrix sigmoid(Matrix x){
	Matrix out(x.m_height, x.m_width);

	for(int z = 0; z < out.m_height * out.m_width; z++) {
		out[z] = sigmoid(x[z]);
//		std::cout << z << '\n';
	}

	return out;
}

Matrix derivative(Matrix x){
	Matrix out(x.m_height, x.m_width);

	for(int z = 0; z < out.m_height * out.m_width; z++)
		out[z] = derivative(x[z]);

	return out;
}

float mean(Matrix mat){
	float count = 0;
	for(int z = 0; z < mat.getHeight() * mat.getWidth(); z++){
		count += mat[z];
	}
	return count / (mat.getHeight() * mat.getWidth());
}

Matrix abs(Matrix &mat){
	Matrix out = mat;
	for(int z = 0; z < mat.getHeight() * mat.getWidth(); z++){
		if(mat[z] < 0)
			out[z] = mat[z] * -1;
	}
	return out;
}
