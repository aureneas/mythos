#ifndef MYTHOS_MATRIX_H
#define MYTHOS_MATRIX_H

template <typename N>
struct Matrix {
	int		width;
	int		height;
	N**		elem;

	Matrix(int w, int h, N def = 0) {
		width = w;
		height = h;
		elem = new N[width];
		for (int i = width - 1; i >= 0; --i) {
			elem[i] = new N[height];
			for (int j = height - 1; j >= 0; --j)
				elem[i][j] = def;
		}
	}
	~Matrix() {
		for (int i = width - 1; i >= 0; --i)
			delete elem[i];
		delete elem;
	}

	// TODO [] operator

	Matrix<int> round() {
		Matrix<int> mres(width, height);
		for (int i = width - 1; i >= 0; --i) {
			for (int j = height - 1; j >= 0; --j)
				mres.elem[i][j] = (int)elem[i][j];
		}
		return mres;
	}
};

Matrix<int> Matrix<float>::round() {
	Matrix<int> mres(width, height);
	for (int i = width - 1; i >= 0; --i) {
		for (int j = height - 1; j >= 0; --j)
			mres.elem[i][j] = (int)roundf(elem[i][j]);
	}
	return mres;
}

Matrix<int> Matrix<double>::round() {
	Matrix<int> mres(width, height);
	for (int i = width - 1; i >= 0; --i) {
		for (int j = height - 1; j >= 0; --j)
			mres.elem[i][j] = (int)round(elem[i][j]);
	}
	return mres;
}



template <typename Nlhs, typename Nrhs, typename Nres>
Matrix<Nres> matrix_add(Matrix<Nlhs>& lhs, Matrix<Nrhs>& rhs) {
	if (lhs.width != rhs.width || lhs.height != rhs.height)
		throw "Matrix dimensions do not align properly for matrix addition.";

	Matrix<Nres> mres(lhs.width, lhs.height);
	for (int i = lhs.width - 1; i >= 0; --i) {
		for (int j = lhs.height - 1; j >= 0; --j)
			mres.elem[i][j] = lhs.elem[i][j] + rhs.elem[i][j];
	}
	return mres;
}

template <typename Nlhs, typename Nrhs, typename Nres>
Matrix<Nres> matrix_subt(Matrix<Nlhs>& lhs, Matrix<Nrhs>& rhs) {
	if (lhs.width != rhs.width || lhs.height != rhs.height)
		throw "Matrix dimensions do not align properly for matrix addition.";

	Matrix<Nres> mres(lhs.width, lhs.height);
	for (int i = lhs.width - 1; i >= 0; --i) {
		for (int j = lhs.height - 1; j >= 0; --j)
			mres.elem[i][j] = lhs.elem[i][j] - rhs.elem[i][j];
	}
	return mres;
}

template <typename Nlhs, typename Nrhs, typename Nres>
Matrix<Nres> matrix_mult(Matrix<Nlhs>& lhs, Matrix<Nrhs>& rhs) {
	if (lhs.width != rhs.height)
		throw "Matrix dimensions do not align properly for matrix multiplication.";

	Matrix<Nres> mres(rhs.width, lhs.height);
	for (int i = rhs.width - 1; i >= 0; --i) {
		for (int j = lhs.height - 1; j >= 0; --j) {
			for (int k = lhs.width - 1; k >= 0; --k)
				mres.elem[i][j] += lhs.elem[k][j] * rhs.elem[i][k];
		}
	}
	return mres;
}

template <typename Nlhs, typename Nrhs, typename Nres>
Matrix<Nres> matrix_elem(Matrix<Nlhs>& lhs, Matrix<Nrhs>& rhs) {
	if (lhs.width != rhs.width || lhs.height != rhs.height)
		throw "Matrix dimensions do not align properly for element-wise multiplication.";

	Matrix<Nres> mres(lhs.width, lhs.height);
	for (int i = lhs.width - 1; i >= 0; --i) {
		for (int j = lhs.height - 1; j >= 0; --j)
			mres.elem[i][j] = lhs.elem[i][j] * rhs.elem[i][j];
	}
	return mres;
}

#endif // MYTHOS_MATRIX_H
