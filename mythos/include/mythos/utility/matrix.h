#ifndef MYTHOS_MATRIX_H
#define MYTHOS_MATRIX_H

#include <cmath>

template <int W, int H, typename N>
struct Matrix {
	N		elem[W][H];

	Matrix(N def = 0) {
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				elem[i][j] = def;
		}
	}


	template <typename Nrhs>
	void operator+=(const Matrix<W, H, Nrhs>& rhs) {
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				elem[i][j] += rhs.elem[i][j];
		}
	}

	template <typename Nrhs>
	void operator-=(const Matrix<W, H, Nrhs>& rhs) {
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				elem[i][j] -= rhs.elem[i][j];
		}
	}

	template <typename Nrhs>
	void operator*=(Nrhs rhs) {
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				elem[i][j] *= rhs;
		}
	}


	template <typename Nrhs, typename Nret = N>
	Matrix<W, H, Nret> operator+(const Matrix<W, H, Nrhs>& rhs) {
		Matrix<W, H, Nret> m;
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				m.elem[i][j] = elem[i][j] + rhs.elem[i][j];
		}
		return m;
	}

	template <typename Nrhs, typename Nret = N>
	Matrix<W, H, Nret> operator-(const Matrix<W, H, Nrhs>& rhs) {
		Matrix<W, H, Nret> m;
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				m.elem[i][j] = elem[i][j] - rhs.elem[i][j];
		}
		return m;
	}

	template <typename Nrhs, typename Nret = N>
	Matrix<W, H, Nret> operator*(Nrhs rhs) {
		Matrix<W, H, Nret> m;
		for (int i = W - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j)
				m.elem[i][j] = elem[i][j] * rhs;
		}
		return m;
	}

	template <int Wrhs, typename Nrhs, typename Nret = N>
	Matrix<Wrhs, H, Nret> operator*(const Matrix<Wrhs, W, Nrhs>& rhs) {
		Matrix<Wrhs, H, Nret> m;
		for (int i = Wrhs - 1; i >= 0; --i) {
			for (int j = H - 1; j >= 0; --j) {
				for (int k = W - 1; k >= 0; --k)
					m.elem[i][j] += elem[k][j] * rhs.elem[i][k];
			}
		}
		return m;
	}
};

// TODO specific round implementations


template <int N>
struct VecN: public Matrix<1, N, int> {
	VecN() : Matrix<1, N, int>() {}

	VecN<N> operator+(const VecN<N>& rhs) {
		VecN<N> m;
		for (int i = N - 1; i >= 0; --i)
			m.elem[0][i] = elem[0][i] + rhs.elem[0][i];
		return m;
	}

	VecN<N> operator-(const VecN<N>& rhs) {
		VecN<N> m;
		for (int i = N - 1; i >= 0; --i)
			m.elem[0][i] = elem[0][i] - rhs.elem[0][i];
		return m;
	}

	template <int H>
	int operator*(const VecN<H>& rhs) {
		int ret = 0;
		for (int i = std::min(H, N) - 1; i >= 0; --i)
			ret += this->elem[0][i] * rhs.elem[0][i];
		return ret;
	}

	double length() {
		double r = 0.0;
		for (int i = N - 1; i >= 0; --i)
			r += std::pow(elem[0][i], 2.0);
		return std::sqrt(r);
	}
};

struct Vec2: public VecN<2> {
	int&	x;
	int&	y;

	Vec2(int px = 0, int py = 0) : VecN<2>(), x(elem[0][0]), y(elem[0][1]) {
		x = px;
		y = py;
	}

	Vec2& operator=(const Vec2& rhs) {
		x = rhs.x;
		y = rhs.y;
		return this;
	}

	template <int H>
	Vec2& operator=(const VecN<H>& rhs) {
		x = rhs.elem[0][0];
		if (H > 1)
			y = rhs.elem[0][1];
		return this;
	}

	Vec2 operator+(const Vec2& rhs) {
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2 operator-(const Vec2& rhs) {
		return Vec2(x - rhs.x, y - rhs.y)
	}
};

typedef Vec2 Point;

struct Vec3: public VecN<3> {
	int&	x;
	int&	y;
	int&	z;

	Vec3(int px, int py, int pz) : VecN<3>(), x(elem[0][0]), y(elem[0][1]), z(elem[0][2]) {
		x = px;
		y = py;
		z = pz;
	}

	Vec3& operator=(const Vec3& rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return this;
	}

	template <int H>
	Vec3& operator=(const VecN<H>& rhs) {
		x = rhs.elem[0][0];
		if (H > 1)
			y = rhs.elem[0][1];
		if (H > 2)
			z = rhs.elem[0][2];
		return this;
	}

	Vec3 operator+(const Vec3& rhs) {
		return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vec3 operator-(const Vec3& rhs) {
		return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Vec2 reduce() {
		return Vec2(x, y);
	}
};

#endif // MYTHOS_MATRIX_H
