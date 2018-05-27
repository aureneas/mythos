#pragma once

#include "../mythos_important_stuff.h"

#define __MAT_INDEX(i, j) (((j) * W) + i)

/*
*	Numeric: some type of number (int, unsigned, float, etc.)
*	W: number of rows
*	H: number of columns
*/
template <typename Numeric, int W, int H>
struct MYTHOS_CORE_API Matrix {
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

	Matrix<Numeric, W, H>& operator=(Matrix<Numeric, W, H>& rhs) {

		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				operator()(i, j) = rhs(i, j);
		}
		return *this;
	}


	template <typename NumericRHS>
	Matrix<Numeric, W, H>& operator+=(Matrix<NumericRHS, W, H>& rhs) {
		
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				operator()(i, j) += rhs(i, j);
		}
		return *this;
	}

	template <typename NumericRHS>
	Matrix<Numeric, W, H>& operator-=(Matrix<NumericRHS, W, H>& rhs) {
		
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				operator()(i, j) -= rhs(i, j);
		}
		return *this;
	}

	template <typename NumericRHS>
	Matrix<Numeric, W, H>& operator*=(Matrix<NumericRHS, W, H>& rhs) {
		
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				operator()(i, j) *= rhs(i, j);
		}
		return *this;
	}


	template <typename NumericRHS>
	Matrix<Numeric, W, H> operator+(Matrix<NumericRHS, W, H>& rhs) {
		
		Matrix<Numeric, W, H> result = *this;
		result += rhs;
		return result;
	}

	template <typename NumericRHS>
	Matrix<Numeric, W, H> operator-(Matrix<NumericRHS, W, H>& rhs) {
		
		Matrix<Numeric, W, H> result = *this;
		result -= rhs;
		return result;
	}

	template <typename NumericRHS>
	Matrix<Numeric, W, H> operator*(Matrix<NumericRHS, W, H>& rhs) {
		
		Matrix<Numeric, W, H> result = *this;
		result *= rhs;
		return result;
	}

	template <typename NumericRHS, int H_RHS>
	Matrix<Numeric, W, H_RHS> operator%(Matrix<NumericRHS, H, H_RHS>& rhs) {
		
		Matrix<Numeric, W, H_RHS> result;
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H_RHS - 1; j >= 0; --j) {
				for (int k = H - 1; k >= 0; --k)
					result(i, j) += operator()(i, k) * rhs(k, j);
			}
		}
		return result;
	}


	Matrix<Numeric, H, W> transpose() {

		Matrix<Numeric, H, W> result;
		for (int i = H - 1; i >= 0; --i) {
			for (int j = W - 1; j >= 0; --j)
				result(i, j) = operator()(j, i);
		}
		return result;
	}
};

template <typename Numeric, int W>
struct MYTHOS_CORE_API VectorW : public Matrix<Numeric, W, 1> {

	VectorW<Numeric, W>() {}

	VectorW<Numeric, W>(Matrix<Numeric, W, 1>& v) {
		
		for (int i = W - 1; i >= 0; --i)
			m[i] = v(i, 0);
	}

	VectorW<Numeric, W>& operator=(Matrix<Numeric, W, 1>& rhs) {

		return (VectorW<Numeric, W>&)Matrix<Numeric, W, 1>::operator=(rhs);
	}

	Numeric& operator()(int i) {

		return m[i];
	}
	
	Numeric& operator[](int i) {

		return m[i];
	}

	double length() {
		
		double sum_squares = 0;
		for (int i = W - 1; i >= 0; --i)
			sum_squares += m[i] * m[i];
		return sqrt(sum_squares);
	}

	template <typename NumericRHS, int W_RHS>
	float dot(VectorW<NumericRHS, W_RHS>& rhs) {
		
		float sum = 0;
		for (int i = std::min(W, W_RHS) - 1; i >= 0; --i)
			sum += m[i] * rhs[i];
		return sum;
	}
};

template <typename Numeric>
struct MYTHOS_CORE_API Vec2 : public VectorW<Numeric, 2> {
	Numeric& x;
	Numeric& y;

	Vec2(Numeric px = 0, Numeric py = 0) : x(m[0]), y(m[1]) {
		x = px;
		y = py;
	}

	Vec2(Matrix<Numeric, 2, 1>& v) : VectorW<Numeric, 2>(v), x(m[0]), y(m[1]) {}

	const Vec2<Numeric>& operator=(Vec2<Numeric>& rhs) {
		return (Vec2<Numeric>&)Matrix<Numeric, 2, 1>::operator=(rhs);
	}


	template <typename NumericRHS>
	Vec2<Numeric>& operator+=(Vec2<NumericRHS>& rhs) {
		return (Vec2<Numeric>&)Matrix<Numeric, 2, 1>::operator+=(rhs);
	}

	template <typename NumericRHS>
	Vec2<Numeric>& operator-=(Vec2<NumericRHS>& rhs) {
		return (Vec2<Numeric>&)Matrix<Numeric, 2, 1>::operator-=(rhs);
	}

	template <typename NumericRHS>
	Vec2<Numeric>& operator*=(Vec2<NumericRHS>& rhs) {
		return (Vec2<Numeric>&)Matrix<Numeric, 2, 1>::operator*=(rhs);
	}


	template <typename NumericRHS>
	Vec2<Numeric> operator+(Vec2<NumericRHS>& rhs) {
		return Vec2<Numeric>(Matrix<Numeric, 2, 1>::operator+(rhs));
	}

	template <typename NumericRHS>
	Vec2<Numeric> operator-(Vec2<NumericRHS>& rhs) {
		return Vec2<Numeric>(Matrix<Numeric, 2, 1>::operator-(rhs));
	}

	template <typename NumericRHS>
	Vec2<Numeric> operator*(Vec2<NumericRHS>& rhs) {
		return Vec2<Numeric>(Matrix<Numeric, 2, 1>::operator*(rhs));
	}
};

template <typename Numeric>
struct MYTHOS_CORE_API Vec3 : public VectorW<Numeric, 3> {
	Numeric& x;
	Numeric& y;
	Numeric& z;

	Vec3(Numeric px = 0, Numeric py = 0, Numeric pz = 0) : x(m[0]), y(m[1]), z(m[2]) {
		x = px;
		y = py;
		z = pz;
	}

	Vec3(Matrix<Numeric, 3, 1>& v) : VectorW<Numeric, 3>(v), x(m[0]), y(m[1]), z(m[2]) {}

	const Vec3<Numeric>& operator=(Vec3<Numeric>& rhs) {
		return (Vec3<Numeric>&)Matrix<Numeric, 3, 1>::operator=(rhs);
	}


	template <typename NumericRHS>
	Vec3<Numeric>& operator+=(Vec3<NumericRHS>& rhs) {
		return (Vec3<Numeric>&)Matrix<Numeric, 3, 1>::operator+=(rhs);
	}

	template <typename NumericRHS>
	Vec3<Numeric>& operator-=(Vec3<NumericRHS>& rhs) {
		return (Vec3<Numeric>&)Matrix<Numeric, 3, 1>::operator-=(rhs);
	}

	template <typename NumericRHS>
	Vec3<Numeric>& operator*=(Vec3<NumericRHS>& rhs) {
		return (Vec3<Numeric>&)Matrix<Numeric, 3, 1>::operator*=(rhs);
	}


	template <typename NumericRHS>
	Vec3<Numeric> operator+(Vec3<NumericRHS>& rhs) {
		return Vec3<Numeric>(Matrix<Numeric, 3, 1>::operator+(rhs));
	}

	template <typename NumericRHS>
	Vec3<Numeric> operator-(Vec3<NumericRHS>& rhs) {
		return Vec3<Numeric>(Matrix<Numeric, 3, 1>::operator-(rhs));
	}

	template <typename NumericRHS>
	Vec3<Numeric> operator*(Vec3<NumericRHS>& rhs) {
		return Vec3<Numeric>(Matrix<Numeric, 3, 1>::operator*(rhs));
	}
};


typedef Vec2<double>			vec2d;
typedef Vec2<float>				vec2f;
typedef Vec2<int>				vec2i;

typedef Vec3<double>			vec3d;
typedef Vec3<float>				vec3f;
typedef Vec3<int>				vec3i;

typedef Matrix<float, 4, 4>		mat4f;