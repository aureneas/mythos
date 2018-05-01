#ifndef MYTHOS_MATRIX_H
#define MYTHOS_MATRIX_H

/*
 *	Numeric: some type of number (int, unsigned, float, etc.)
 *	W: number of rows
 *	H: number of columns
 */
template <typename Numeric, int W, int H>
struct Matrix {
	Numeric m[W][H];

	Matrix() {
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				m[i][j] = 0;
		}
	}

	Matrix<Numeric, W, H>& operator=(Matrix<Numeric, W, H>& rhs) {
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				m[i][j] = rhs.m[i][j];
		}
		return *this;
	}


	template <typename NumericRHS>
	Matrix<Numeric, W, H>& operator+=(Matrix<NumericRHS, W, H>& rhs) {
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				m[i][j] += rhs.m[i][j];
		}
		return *this;
	}

	template <typename NumericRHS>
	Matrix<Numeric, W, H>& operator-=(Matrix<NumericRHS, W, H>& rhs) {
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				m[i][j] -= rhs.m[i][j];
		}
		return *this;
	}

	template <typename NumericRHS>
	Matrix<Numeric, W, H>& operator*=(Matrix<NumericRHS, W, H>& rhs) {
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				m[i][j] *= rhs.m[i][j];
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
					result.m[i][j] += m[i][k] * rhs.m[k][j];
			}
		}
		return result;
	}


	Matrix<Numeric, H, W> transpose() {
		Matrix<Numeric, H, W> result;
		for (int i = H - 1; i >= 0; --i) {
			for (int j = W - 1; j >= 0; --j)
				result.m[i][j] = m[j][i];
		}
		return result;
	}
};

template <int W>
struct VectorW : public Matrix<int, W, 1> {
	
	VectorW<W>() {}
	
	VectorW<W>(Matrix<int, W, 1>& v) {
		for (int i = W - 1; i >= 0; --i)
			m[i][0] = v.m[i][0];
	}
	
	VectorW<W>& operator=(Matrix<int, W, 1>& rhs) {
		return (VectorW<W>&)Matrix<int, W, 1>::operator=(rhs);
	}

	double length() {
		int sum_squares = 0;
		for (int i = W - 1; i >= 0; --i)
			sum_squares += m[i][0] * m[i][0];
		return sqrt(sum_squares);
	}

	template <int W_RHS>
	int dot(VectorW<W_RHS>& rhs) {
		int sum = 0;
		for (int i = std::min(W, W_RHS) - 1; i >= 0; --i)
			sum += m[i][0] * rhs.m[i][0];
		return sum;
	}
};

struct Vec2 : public VectorW<2> {
	int& x;
	int& y;

	Vec2(int px = 0, int py = 0) : x(m[0][0]), y(m[1][0]) {
		x = px;
		y = py;
	}

	Vec2(Matrix<int, 2, 1>& v) : VectorW<2>(v), x(m[0][0]), y(m[1][0]) {}


	Vec2& operator=(Vec2& rhs) {
		return (Vec2&)Matrix<int, 2, 1>::operator=(rhs);
	}


	Vec2& operator+=(Vec2& rhs) {
		return (Vec2&)Matrix<int, 2, 1>::operator+=(rhs);
	}

	Vec2& operator-=(Vec2& rhs) {
		return (Vec2&)Matrix<int, 2, 1>::operator-=(rhs);
	}

	Vec2& operator*=(Vec2& rhs) {
		return (Vec2&)Matrix<int, 2, 1>::operator*=(rhs);
	}


	Vec2 operator+(Vec2& rhs) {
		return Vec2(Matrix<int, 2, 1>::operator+(rhs));
	}

	Vec2 operator-(Vec2& rhs) {
		return Vec2(Matrix<int, 2, 1>::operator-(rhs));
	}

	Vec2 operator*(Vec2& rhs) {
		return Vec2(Matrix<int, 2, 1>::operator*(rhs));
	}
};

struct Vec3 : public VectorW<3> {
	int& x;
	int& y;
	int& z;

	Vec3(int px = 0, int py = 0, int pz = 0) : x(m[0][0]), y(m[1][0]), z(m[2][0]) {
		x = px;
		y = py;
		z = pz;
	}

	Vec3(Matrix<int, 3, 1>& v) : VectorW<3>(v), x(m[0][0]), y(m[1][0]), z(m[2][0]) {}

	const Vec3& operator=(Vec3& rhs) {
		return (Vec3&)Matrix<int, 3, 1>::operator=(rhs);
	}


	Vec3& operator+=(Vec3& rhs) {
		return (Vec3&)Matrix<int, 3, 1>::operator+=(rhs);
	}

	Vec3& operator-=(Vec3& rhs) {
		return (Vec3&)Matrix<int, 3, 1>::operator-=(rhs);
	}

	Vec3& operator*=(Vec3& rhs) {
		return (Vec3&)Matrix<int, 3, 1>::operator*=(rhs);
	}


	Vec3 operator+(Vec3& rhs) {
		return Vec3(Matrix<int, 3, 1>::operator+(rhs));
	}

	Vec3 operator-(Vec3& rhs) {
		return Vec3(Matrix<int, 3, 1>::operator-(rhs));
	}

	Vec3 operator*(Vec3& rhs) {
		return Vec3(Matrix<int, 3, 1>::operator*(rhs));
	}
};

#endif