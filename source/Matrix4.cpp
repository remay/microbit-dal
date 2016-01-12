#include "MicroBit.h"

/**
* Class definition for a simple matrix, optimised for n x 4 or 4 x n matrices.
*
* This class is heavily optimised for these commonly used matrices as used in 3D geometry,
* and is not intended as a general purpose matrix class. For programmers needing more flexible
* Matrix support, the mbed Matrix and MatrixMath classes from Ernsesto Palacios provide a good basis:
*
* https://developer.mbed.org/cookbook/MatrixClass
* https://developer.mbed.org/users/Yo_Robot/code/MatrixMath/
*/

/**
 * Constructor.
 * Create a matrix of the given size.
 * @param rows the number of rows in the matrix to be created.
 * @param cols the number of columns in the matrix to be created.
 *
 * Example:
 * @code
 * Matrix4(10, 4);        // Creates a Matrix with 10 rows and 4 columns.
 * @endcode
 */
Matrix4::Matrix4(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;

	int size = rows * cols;

	if (size > 0)
		data = new double[size];
	else
		data = NULL;
}

/**
* Constructor.
* Create a matrix that is an identicval copy of the given matrix.
* @param matrix The matrix to copy.
*
* Example:
* @code
*
* Matrix newMatrix(matrix);        .
* @endcode
*/
Matrix4::Matrix4(const Matrix4 &matrix)
{
	this->rows = matrix.rows;
	this->cols = matrix.cols;

	int size = rows * cols;

	if (size > 0)
	{
		data = new double[size];
		for (int i = 0; i < size; i++)
			data[i] = matrix.data[i];
	}
	else
	{
		data = NULL;
	}

}

/**
* Determines the number of columns in this matrix.
*
* @return The number of columns in the matrix.
*
* Example:
* @code
* int c = matrix.width();
* @endcode
*/
int Matrix4::width()
{
	return cols;
}

/**
* Determines the number of rows in this matrix.
*
* @return The number of rows in the matrix.
*
* Example:
* @code
* int r = matrix.height();
* @endcode
*/
int Matrix4::height()
{
	return rows;
}

/**
* Reads the matrix element at the given position.
*
* @param row The row of the element to read
* @param col The column of the element to read
* @return The value of the matrix element at the given position. NAN is returned if the given index is out of range.
*
* Example:
* @code
* double v = matrix.get(1,2);
* @endcode
*/
double Matrix4::get(int row, int col)
{
	if (row < 0 || col < 0 || row >= rows || col >= cols)
		return 0;

	return data[width() * row + col];
}

/**
* Writes the matrix element at the given position.
*
* @param row The row of the element to write
* @param col The column of the element to write
* @param v The new value of the element
*
* Example:
* @code
* matrix.set(1,2,42.0);
* @endcode
*/
void Matrix4::set(int row, int col, double v)
{
	if (row < 0 || col < 0 || row >= rows || col >= cols)
		return;

	data[width() * row + col] = v;
}

/**
* Transposes this matrix.
* @return the resultant matrix.
*
* Example:
* @code
* matrix.transpose();
* @endcode
*/
Matrix4 Matrix4::transpose()
{
	Matrix4 result = Matrix4(cols, rows);

	for (int i = 0; i < width(); i++)
		for (int j = 0; j < height(); j++)
			result.set(i, j, get(j, i));

	return result;
}

/**
* Multiplies this matrix with the given matrix (if possible).
* @return the resultant matrix. An empty matrix is returned if the operation canot be completed.
*
* Example:
* @code
* Matrix result = matrixA.multiply(matrixB);
* @endcode
*/
Matrix4 Matrix4::multiply(Matrix4 &matrix)
{
	if (width() != matrix.height())
		return Matrix4(0, 0);

	Matrix4 result(height(), matrix.width());

	for (int r = 0; r < result.height(); r++)
	{
		for (int c = 0; c < result.width(); c++)
		{
			double v = 0.0;

			for (int i = 0; i < width(); i++)
				v += get(r, i) * matrix.get(i, c);

			result.set(r, c, v);
		}
	}

	return result;
}

/**
* Performs an optimised inversion of a 4x4 matrix.
* Only 4x4 matrices are supported by this operation.
*
* @return the resultant matrix. An empty matrix is returned if the operation canot be completed.
*
* Example:
* @code
* Matrix result = matrixA.invert();
* @endcode
*/
Matrix4 Matrix4::invert()
{
	// We only support square matrices of size 4...
	if (width() != height() || width() != 4)
		return Matrix4(0, 0);

	Matrix4 result(width(), height());

	result.data[0] = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] - data[9] * data[6] * data[15] + data[9] * data[7] * data[14] + data[13] * data[6] * data[11] - data[13] * data[7] * data[10];
	result.data[1] = -data[1] * data[10] * data[15] + data[1] * data[11] * data[14] + data[9] * data[2] * data[15] - data[9] * data[3] * data[14] - data[13] * data[2] * data[11] + data[13] * data[3] * data[10];
	result.data[2] = data[1] * data[6] * data[15] - data[1] * data[7] * data[14] - data[5] * data[2] * data[15] + data[5] * data[3] * data[14] + data[13] * data[2] * data[7] - data[13] * data[3] * data[6];
	result.data[3] = -data[1] * data[6] * data[11] + data[1] * data[7] * data[10] + data[5] * data[2] * data[11] - data[5] * data[3] * data[10] - data[9] * data[2] * data[7] + data[9] * data[3] * data[6];
	result.data[4] = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] + data[8] * data[6] * data[15] - data[8] * data[7] * data[14] - data[12] * data[6] * data[11] + data[12] * data[7] * data[10];
	result.data[5] = data[0] * data[10] * data[15] - data[0] * data[11] * data[14] - data[8] * data[2] * data[15] + data[8] * data[3] * data[14] + data[12] * data[2] * data[11] - data[12] * data[3] * data[10];
	result.data[6] = -data[0] * data[6] * data[15] + data[0] * data[7] * data[14] + data[4] * data[2] * data[15] - data[4] * data[3] * data[14] - data[12] * data[2] * data[7] + data[12] * data[3] * data[6];
	result.data[7] = data[0] * data[6] * data[11] - data[0] * data[7] * data[10] - data[4] * data[2] * data[11] + data[4] * data[3] * data[10] + data[8] * data[2] * data[7] - data[8] * data[3] * data[6];
	result.data[8] = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] - data[8] * data[5] * data[15] + data[8] * data[7] * data[13] + data[12] * data[5] * data[11] - data[12] * data[7] * data[9];
	result.data[9] = -data[0] * data[9] * data[15] + data[0] * data[11] * data[13] + data[8] * data[1] * data[15] - data[8] * data[3] * data[13] - data[12] * data[1] * data[11] + data[12] * data[3] * data[9];
	result.data[10] = data[0] * data[5] * data[15] - data[0] * data[7] * data[13] - data[4] * data[1] * data[15] + data[4] * data[3] * data[13] + data[12] * data[1] * data[7] - data[12] * data[3] * data[5];
	result.data[11] = -data[0] * data[5] * data[11] + data[0] * data[7] * data[9] + data[4] * data[1] * data[11] - data[4] * data[3] * data[9] - data[8] * data[1] * data[7] + data[8] * data[3] * data[5];
	result.data[12] = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] + data[8] * data[5] * data[14] - data[8] * data[6] * data[13] - data[12] * data[5] * data[10] + data[12] * data[6] * data[9];
	result.data[13] = data[0] * data[9] * data[14] - data[0] * data[10] * data[13] - data[8] * data[1] * data[14] + data[8] * data[2] * data[13] + data[12] * data[1] * data[10] - data[12] * data[2] * data[9];
	result.data[14] = -data[0] * data[5] * data[14] + data[0] * data[6] * data[13] + data[4] * data[1] * data[14] - data[4] * data[2] * data[13] - data[12] * data[1] * data[6] + data[12] * data[2] * data[5];
	result.data[15] = data[0] * data[5] * data[10] - data[0] * data[6] * data[9] - data[4] * data[1] * data[10] + data[4] * data[2] * data[9] + data[8] * data[1] * data[6] - data[8] * data[2] * data[5];

	double det = data[0] * result.data[0] + data[1] * result.data[4] + data[2] * result.data[8] + data[3] * result.data[12];

	if (det == 0)
		return Matrix4(0, 0);

	det = 1.0f / det;

	for (int i = 0; i < 16; i++)
		result.data[i] *= det;

	return result;
}

/**
* Destructor.
*/
Matrix4::~Matrix4()
{
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}
}

