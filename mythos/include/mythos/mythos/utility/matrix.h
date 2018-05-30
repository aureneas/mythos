#pragma once

#include <math.h>
#include "../_important_stuff.h"

#define __MAT_INDEX(i, j) (((j) * W) + i)

/*
*	Numeric: some type of number (int, unsigned, float, etc.)
*	W: number of rows
*	H: number of columns
*/
template <typename Numeric, int W, int H>
struct Matrix {
	Numeric m[W * H];

	Matrix() {
		
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				m[__MAT_INDEX(i, j)] = 0;
		}
	}

	Numeric& operator()(int i, int j) {

		return m[__MAT_INDEX(i, j)];
	}

	Numeric& get(int i, int j) {

		return m[__MAT_INDEX(i, j)];
	}

	const Numeric getValue(int i, int j) const {

		return m[__MAT_INDEX(i, j)];
	}

	const Matrix<Numeric, W, H>& operator=(const Matrix<Numeric, W, H>& rhs) {

		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				get(i, j) = rhs.getValue(i, j);
		}
		return *this;
	}


	template <typename NumericRHS>
	const Matrix<Numeric, W, H>& operator+=(const Matrix<NumericRHS, W, H>& rhs) {
		
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				get(i, j) += rhs.getValue(i, j);
		}
		return *this;
	}

	template <typename NumericRHS>
	const Matrix<Numeric, W, H>& operator-=(const Matrix<NumericRHS, W, H>& rhs) {
		
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				get(i, j) -= rhs.getValue(i, j);
		}
		return *this;
	}

	template <typename NumericRHS>
	const Matrix<Numeric, W, H>& operator*=(const NumericRHS rhs) {

		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				get(i, j) *= rhs;
		}
		return *this;
	}

	template <typename NumericRHS>
	const Matrix<Numeric, W, H>& operator*=(const Matrix<NumericRHS, W, H>& rhs) {

		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				get(i, j) *= rhs.getValue(i, j);
		}
		return *this;
	}


	template <typename NumericRHS>
	Matrix<Numeric, W, H> operator+(const Matrix<NumericRHS, W, H>& rhs) {
		
		Matrix<Numeric, W, H> result = *this;
		result += rhs;
		return result;
	}

	template <typename NumericRHS>
	Matrix<Numeric, W, H> operator-(const Matrix<NumericRHS, W, H>& rhs) {
		
		Matrix<Numeric, W, H> result = *this;
		result -= rhs;
		return result;
	}

	template <typename NumericRHS>
	Matrix<Numeric, W, H> operator*(const NumericRHS rhs) {

		Matrix<Numeric, W, H> result = *this;
		result *= rhs;
		return result;
	}

	template <typename NumericRHS>
	Matrix<Numeric, W, H> operator*(const Matrix<NumericRHS, W, H>& rhs) {
		
		Matrix<Numeric, W, H> result = *this;
		result *= rhs;
		return result;
	}

	template <typename NumericRHS, int H_RHS>
	Matrix<Numeric, W, H_RHS> operator%(const Matrix<NumericRHS, H, H_RHS>& rhs) {
		
		Matrix<Numeric, W, H_RHS> result;
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H_RHS - 1; j >= 0; --j) {
				for (int k = H - 1; k >= 0; --k)
					result.get(i, j) += getValue(i, k) * rhs.getValue(k, j);
			}
		}
		return result;
	}


	Matrix<Numeric, H, W> transpose() {

		Matrix<Numeric, H, W> result;
		for (int i = H - 1; i >= 0; --i) {
			for (int j = W - 1; j >= 0; --j)
				result.get(i, j) = get(j, i);
		}
		return result;
	}
};


template <typename Numeric, int W>
struct SquareMatrix : public Matrix<Numeric, W, W> {

	SquareMatrix<Numeric, W>() {}

	template <int W_rhs>
	SquareMatrix<Numeric, W>(const Matrix<Numeric, W_rhs, W_rhs>& v) {

		int M = (W < W_rhs ? W : W_rhs);

		for (int i = M - 1; i >= 0; --i) {
			for (int j = M - 1; j >= 0; --j)
				get(i, j) = v.getValue(i, j);
		}
	}

	void identity(void) {

		for (int i = W - 1; i >= 0; --i) {
			for (int j = W - 1; j >= 0; --j)
				get(i, j) = ((i == j) ? 1 : 0);
		}
	}

	const SquareMatrix<Numeric, W>& operator=(const Matrix<Numeric, W, W>& rhs) {
		
		return (SquareMatrix<Numeric, W>&)Matrix<Numeric, W, W>::operator=(rhs);
	}

	SquareMatrix<Numeric, W - 1> submatrix(int row, int col) {

		SquareMatrix<Numeric, W - 1> sMat;

		for (int iN = W - 1; iN >= 0; --iN) {

			if (iN != row) {

				int i = ((iN > row) ? (iN - 1) : iN);

				for (int jN = W - 1; jN >= 0; --jN) {

					if (jN != col) {

						int j = ((jN > col) ? (jN - 1) : jN);

						sMat(i, j) = operator()(iN, jN);
					}
				}
			}
		}

		return sMat;
	}

	Numeric determinant() {

		// TODO make more efficient

		Numeric det = 0;

		for (int i = W - 1; i >= 0; --i) 
			det += ((i % 2 == 0) ? 1 : -1) * submatrix(i, 0).determinant();

		return det;
	}

	SquareMatrix<float, W> inverse() {

		// TODO make more efficient

		SquareMatrix<float, W> inv;

		float det = determinant();
		
		if (det != 0.0f) {

			det = 1.0f / det;

			for (int i = W - 1; i >= 0; --i) {
				for (int j = W - 1; j >= 0; --j)
					inv(i, j) = (((i + j) % 2 == 0) ? 1 : -1) * submatrix(j, i).determinant() * det;
			}
		}

		return inv;
	}
};


template <typename Numeric, int W>
struct VectorW : public Matrix<Numeric, W, 1> {

	VectorW<Numeric, W>() {}

	template <int W_rhs>
	VectorW<Numeric, W>(const Matrix<Numeric, W_rhs, 1>& v) {
		
		for (int i = (W < W_rhs ? W : W_rhs) - 1; i >= 0; --i)
			get(i) = v.getValue(i, 0);
	}

	const VectorW<Numeric, W>& operator=(const Matrix<Numeric, W, 1>& rhs) {

		return (VectorW<Numeric, W>&)Matrix<Numeric, W, 1>::operator=(rhs);
	}

	Numeric& operator()(int i) {

		return m[i];
	}

	Numeric& get(int i) {

		return m[i];
	}

	const Numeric getValue(int i) const {

		return m[i];
	}

	float length() {
		
		float sum_squares = 0.0f;
		for (int i = W - 1; i >= 0; --i)
			sum_squares += getValue(i) * getValue(i);
		return sqrtf(sum_squares);
	}

	template <typename NumericRHS, int W_RHS>
	float dot(const VectorW<NumericRHS, W_RHS>& rhs) {
		
		float sum = 0;
		for (int i = ((W <= W_RHS) ? W : W_RHS) - 1; i >= 0; --i)
			sum += get(i) * rhs.getValue(i);
		return sum;
	}
};

template <typename Numeric>
struct Vec2 : public VectorW<Numeric, 2> {
	Numeric& x;
	Numeric& y;

	Vec2(Numeric px = 0, Numeric py = 0) : x(m[0]), y(m[1]) {
		x = px;
		y = py;
	}

	template <int W_rhs>
	Vec2(const Matrix<Numeric, W_rhs, 1>& v) : VectorW<Numeric, 2>(v), x(m[0]), y(m[1]) {}

	const Vec2<Numeric>& operator=(const Vec2<Numeric>& rhs) {
		return (Vec2<Numeric>&)Matrix<Numeric, 2, 1>::operator=(rhs);
	}


	template <typename NumericRHS>
	const Vec2<Numeric>& operator+=(const Vec2<NumericRHS>& rhs) {
		return (Vec2<Numeric>&)Matrix<Numeric, 2, 1>::operator+=(rhs);
	}

	template <typename NumericRHS>
	const Vec2<Numeric>& operator-=(const Vec2<NumericRHS>& rhs) {
		return (Vec2<Numeric>&)Matrix<Numeric, 2, 1>::operator-=(rhs);
	}

	template <typename NumericRHS>
	const Vec2<Numeric>& operator*=(const NumericRHS rhs) {
		return (Vec2<Numeric>&)Matrix<Numeric, 2, 1>::operator*=(rhs);
	}

	template <typename NumericRHS>
	const Vec2<Numeric>& operator*=(const Vec2<NumericRHS>& rhs) {
		return (Vec2<Numeric>&)Matrix<Numeric, 2, 1>::operator*=(rhs);
	}


	template <typename NumericRHS>
	Vec2<Numeric> operator+(const Vec2<NumericRHS>& rhs) {
		return Vec2<Numeric>(Matrix<Numeric, 2, 1>::operator+(rhs));
	}

	template <typename NumericRHS>
	Vec2<Numeric> operator-(const Vec2<NumericRHS>& rhs) {
		return Vec2<Numeric>(Matrix<Numeric, 2, 1>::operator-(rhs));
	}

	template <typename NumericRHS>
	Vec2<Numeric> operator*(const NumericRHS rhs) {
		return Vec2<Numeric>(Matrix<Numeric, 2, 1>::operator*(rhs));
	}

	template <typename NumericRHS>
	Vec2<Numeric> operator*(const Vec2<NumericRHS>& rhs) {
		return Vec2<Numeric>(Matrix<Numeric, 2, 1>::operator*(rhs));
	}
};

template <typename Numeric>
struct Vec3 : public VectorW<Numeric, 3> {
	Numeric& x;
	Numeric& y;
	Numeric& z;

	Vec3(Numeric px = 0, Numeric py = 0, Numeric pz = 0) : x(m[0]), y(m[1]), z(m[2]) {
		x = px;
		y = py;
		z = pz;
	}

	template <int W_rhs>
	Vec3(const Matrix<Numeric, W_rhs, 1>& v) : VectorW<Numeric, 3>(v), x(m[0]), y(m[1]), z(m[2]) {}

	const Vec3<Numeric>& operator=(const Vec3<Numeric>& rhs) {
		return (Vec3<Numeric>&)Matrix<Numeric, 3, 1>::operator=(rhs);
	}


	template <typename NumericRHS>
	const Vec3<Numeric>& operator+=(const Vec3<NumericRHS>& rhs) {
		return (Vec3<Numeric>&)Matrix<Numeric, 3, 1>::operator+=(rhs);
	}

	template <typename NumericRHS>
	const Vec3<Numeric>& operator-=(const Vec3<NumericRHS>& rhs) {
		return (Vec3<Numeric>&)Matrix<Numeric, 3, 1>::operator-=(rhs);
	}

	template <typename NumericRHS>
	const Vec3<Numeric>& operator*=(const NumericRHS rhs) {
		return (Vec3<Numeric>&)Matrix<Numeric, 3, 1>::operator*=(rhs);
	}

	template <typename NumericRHS>
	const Vec3<Numeric>& operator*=(const Vec3<NumericRHS>& rhs) {
		return (Vec3<Numeric>&)Matrix<Numeric, 3, 1>::operator*=(rhs);
	}


	template <typename NumericRHS>
	Vec3<Numeric> operator+(const Vec3<NumericRHS>& rhs) {
		return Vec3<Numeric>(Matrix<Numeric, 3, 1>::operator+(rhs));
	}

	template <typename NumericRHS>
	Vec3<Numeric> operator-(const Vec3<NumericRHS>& rhs) {
		return Vec3<Numeric>(Matrix<Numeric, 3, 1>::operator-(rhs));
	}

	template <typename NumericRHS>
	Vec3<Numeric> operator*(const NumericRHS rhs) {
		return Vec3<Numeric>(Matrix<Numeric, 3, 1>::operator*(rhs));
	}

	template <typename NumericRHS>
	Vec3<Numeric> operator*(const Vec3<NumericRHS>& rhs) {
		return Vec3<Numeric>(Matrix<Numeric, 3, 1>::operator*(rhs));
	}
};

template <typename Numeric>
struct Vec4 : public VectorW<Numeric, 4> {
	Numeric& x;
	Numeric& y;
	Numeric& z;
	Numeric& w;

	Vec4(Numeric px = 0, Numeric py = 0, Numeric pz = 0, Numeric pw = 1) : x(m[0]), y(m[1]), z(m[2]), w(m[3]) {
		x = px;
		y = py;
		z = pz;
		w = pw;
	}

	template <int W_rhs>
	Vec4(const Matrix<Numeric, W_rhs, 1>& v) : VectorW<Numeric, 4>(v), x(m[0]), y(m[1]), z(m[2]), w(m[3]) {
		
		if (W_rhs < 4)
			w = 1;
	}

	const Vec4<Numeric>& operator=(const Vec4<Numeric>& rhs) {
		return (Vec4<Numeric>&)Matrix<Numeric, 4, 1>::operator=(rhs);
	}


	template <typename NumericRHS>
	const Vec4<Numeric>& operator+=(const Vec4<NumericRHS>& rhs) {
		return (Vec4<Numeric>&)Matrix<Numeric, 4, 1>::operator+=(rhs);
	}

	template <typename NumericRHS>
	const Vec4<Numeric>& operator-=(const Vec4<NumericRHS>& rhs) {
		return (Vec4<Numeric>&)Matrix<Numeric, 4, 1>::operator-=(rhs);
	}

	template <typename NumericRHS>
	const Vec4<Numeric>& operator*=(const NumericRHS rhs) {
		return (Vec4<Numeric>&)Matrix<Numeric, 4, 1>::operator*=(rhs);
	}

	template <typename NumericRHS>
	const Vec4<Numeric>& operator*=(const Vec4<NumericRHS>& rhs) {
		return (Vec4<Numeric>&)Matrix<Numeric, 4, 1>::operator*=(rhs);
	}


	template <typename NumericRHS>
	Vec4<Numeric> operator+(const Vec4<NumericRHS>& rhs) {
		return Vec4<Numeric>(Matrix<Numeric, 4, 1>::operator+(rhs));
	}

	template <typename NumericRHS>
	Vec4<Numeric> operator-(const Vec4<NumericRHS>& rhs) {
		return Vec4<Numeric>(Matrix<Numeric, 4, 1>::operator-(rhs));
	}

	template <typename NumericRHS>
	Vec4<Numeric> operator*(const NumericRHS rhs) {
		return Vec4<Numeric>(Matrix<Numeric, 4, 1>::operator*(rhs));
	}

	template <typename NumericRHS>
	Vec4<Numeric> operator*(const Vec4<NumericRHS>& rhs) {
		return Vec4<Numeric>(Matrix<Numeric, 4, 1>::operator*(rhs));
	}
};




typedef Vec2<double>				vec2d;
typedef Vec2<float>					vec2f;
typedef Vec2<int>					vec2i;

typedef Vec3<double>				vec3d;
typedef Vec3<float>					vec3f;
typedef Vec3<int>					vec3i;

typedef Vec4<float>					vec4f;

typedef SquareMatrix<float, 4>		mat4f;