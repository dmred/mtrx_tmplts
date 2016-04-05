#include "matrix.h"
#include <iostream>
using namespace std;


//Конструктор по умолчанию
template <typename T>
matrix<T>::matrix() :rows(0), columns(0), _matrix(nullptr)
{

}



//конструктор с параметрами кол-во строк, кол-во столбцов
template <typename T>
matrix<T>::matrix(int _rows, int _columns): rows(_rows), columns(_columns)
{
	create_memory();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) _matrix[i][j] = 0;
	};
}

template <typename T>
matrix<T>::matrix(T **matr, unsigned int _m_rows, unsigned int _m_columns) : _matrix(new T *[_m_rows]), rows(_m_rows), columns(_m_columns) {
	for (int i = 0; i < rows; i++) {
		_matrix[i] = new T[columns];
		for (int j = 0; j < columns; j++) {
			_matrix[i][j] = matr[i][j];
		}
	}
}


//конструктор случайной матрицы
template <typename T>
matrix<T>::matrix(T **matr, unsigned int _m_rows, unsigned int _m_columns,int time) : _matrix(new T *[_m_rows]), rows(_m_rows), columns(_m_columns) {

	srand(time);
	for (int i = 0; i < rows; i++) {
		_matrix[i] = new T[columns];
		for (int j = 0; j < columns; j++)
		{
			_matrix[i][j] = rand() % 10;
		}
	};
}



//конструктор (глубокого) копирования 
template <typename T>
matrix<T>::matrix(const matrix<T> & matrix): rows(matrix.rows), columns(matrix.columns)
{
	create_memory();
	copy_matrix(matrix);
}



// Заполение матрицы из файла 
template <typename T>
void matrix<T>::get_from_file(string name)
{
	string full_name;
	full_name = name;
	ifstream fin(full_name);
	if (fin.is_open()) {
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				fin >> _matrix[i][j];
		fin.close();
	}
	else {
		cout << "Ошибка, попробуйте еще раз";
		exit(100);
	}
}



// деструктор
template <typename T>
matrix<T>::~matrix()
{
	for (int i = 0; i < rows; i++)
		delete[] _matrix[i];

	delete[] _matrix;
}



// печать матрицы
template <typename T>
void matrix<T>::print_matrix() const
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cout << "   " << _matrix[i][j];
		};
		cout << "\n";
	};
}

template <typename T>
void matrix<T>::get_line(int k)
{
	int *row_matrix;  //указатель
	row_matrix = _matrix[k - 1];
	for (int i = 0; i < columns; i++)
		cout << " " << row_matrix[i];
}


// копирование матрицы
template <typename T>
void matrix<T>::copy_matrix(const matrix<T> & matrix)
{
	for (int i = 0; i <rows; i++)
		for (int j = 0; j < columns; j++)
			_matrix[i][j] = matrix._matrix[i][j];
}



// перегрузка оператора = 
template <typename T>
matrix<T> & matrix<T>::operator=(const matrix<T> & matrix)
{
	copy_matrix(matrix);
	return *this;
}


// передача номера строки, столбца и значения
template <typename T>
void matrix<T>::set(int row, int columns, int set)
{
	_matrix[row][columns] = set;
}


// взятие элемента 
template <typename T>
int matrix<T>::get(int row, int columns) const
{
	return _matrix[row][columns];
}



// сумма построчно
template <typename T>
void matrix<T>::get_sum_r(const matrix<T> &matrix_1, const matrix<T> &matrix_2, int i)
{
	for (int j = 0; j < get_num_cols(); j++)
		set(i, j, matrix_1.get(i, j) + matrix_2.get(i, j));
}



//перегрузка оператора + 
template <typename T>
matrix<T> operator+(const matrix<T> &matrix_1, const matrix<T> &matrix_2)
{

	matrix matrix(matrix_1.rows, matrix_1.columns);//создаем новую матрицу 
	if (matrix_1.columns != matrix_2.columns || matrix_1.rows != matrix_2.rows) {
		throw "Несоответствие размеров матриц\n";
	}
	for (int i = 0; i < matrix.rows; i++)
		matrix.get_sum_r(matrix_1, matrix_2, i);
	return matrix;
};


//перегрузка оператора * 
template <typename T>
matrix<T> operator*(const matrix<T> &matrix_1, const matrix<T> &matrix_2) {
	if (matrix_1._matrix == nullptr || matrix_1.rows == 0 || matrix_1.columns == 0) {
		throw "Первая матрица пустая\n";
	}
	if (matrix_2._matrix == nullptr || matrix_2.rows == 0 || matrix_2.columns == 0) {
		throw "Вторая матрица пустая\n";
	}
	if (matrix_1.columns != matrix_2.rows) {
		throw "Несоответствие размеров матриц\n";
	}
	matrix matrix(matrix_1.rows, matrix_1.columns);
	for (int i = 0; i < matrix.rows; i++)
		matrix.get_multi_r(matrix_1, matrix_2, i);
	return matrix;
};

// количество строк
template <typename T>
int matrix<T>::get_num_rows() const
{
	return rows;
}


// Произведение
template <typename T>
void matrix<T>::get_multi_r(const matrix<T> &matrix_1, const matrix<T> &matrix_2, int i)
{
	T value = 0;
	for (int row = 0; row < matrix_1.rows; row++) {
		for (int col = 0; col < matrix_2.columns; col++) {
			for (int inner = 0; inner < matrix_2.rows; inner++) {
				value += matrix_1._matrix[row][inner] * matrix_2._matrix[inner][col];
			}
			set(row, col, value);
			value = 0;
		}
	}
};



//Количество столбцов
template <typename T>
int matrix<T>::get_num_cols() const
{
	return columns;
}


// выделяем память
template <typename T>
void matrix<T>::create_memory()
{
	_matrix = new T*[rows];

	for (int i = 0; i < rows; i++)
		_matrix[i] = new T[columns];
}



// перегрузка оператора [] 
template <typename T>
T* matrix<T>::operator [] (int i) const
{
	if (rows == 0 || _matrix == nullptr) {
		throw "Пустая матрица\n";
	}
	int *Getline = new T[columns];// одномерный массив для хранения столбцов
	for (int j = 0; j < columns; j++)
		Getline[j] = _matrix[i - 1][j];
	return Getline;

}
template <typename T>
std::ostream & operator<<(std::ostream & os, const matrix<T> & x) {
	for (int i = 0; i < x.rows; ++i) {
		for (int j = 0; j < x.columns; ++j) {
			os.width(4);
			os << x._matrix[i][j];
		}
		os << std::endl;
	}

	return os;
}

template <typename T>
std::istream & operator>>(std::istream & input, matrix<T> &__matrix) {
	for (int i = 0; i < __matrix.rows; ++i) {
		for (int j = 0; j < __matrix.columns; ++j) {
			if (!(input >> __matrix.__matrix[i][j])) {
				throw "Ошибка в потоковм вводе\n";
			}
		}
	}

	return input;
}
