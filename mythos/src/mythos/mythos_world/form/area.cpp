#include <mythos_world\form\area.h>

MythosUnevenGround::MythosUnevenGround(MythosImage* image, vec3f& p0, vec3f& p1, vec3f& p2, vec3f& p3, int oddPiece) {

	mTextureX = new MythosTriangleImageTexture<1>(image);
	mTextureY = new MythosTriangleImageTexture<0>(image);

	for (int i = 2; i >= 0; --i) {

		mTransformX(i, 0) = p1(i) - p0(i);
		mTransformX(i, 1) = p2(i) - p1(i) - p0(i);
		mTransformX(i, 3) = p0(i);

		mTransformY(i, 0) = p3(i) - p0(i);
		mTransformY(i, 1) = p2(i) - p3(i) - p0(i);
		mTransformY(i, 3) = p0(i);
	}

	mat4f* mats[4] = { &mTransformX, &mInverseX, &mTransformY, &mInverseY };

	for (int i = 2; i >= 0; i -= 2) {

		mat4f& trans = *mats[i];
		mat4f& inv = *mats[i + 1];

		float a = trans(0, 3);
		float b = trans(1, 3);
		float c = trans(2, 3);
		float d = trans(0, 0);
		float e = trans(1, 0);
		float f = trans(2, 0);
		float g = trans(0, 1);
		float h = trans(1, 1);
		float j = trans(2, 1);

		float invDet = 1.0f / ((d * h) - (e * g));

		inv(0, 0) = h * invDet;
		inv(0, 1) = -g * invDet;
		inv(1, 0) = -e * invDet;
		inv(1, 1) = d * invDet;

		inv(2, 0) = ((e * j) - (f * h)) * invDet;
		inv(2, 1) = ((f * g) - (d * j)) * invDet;

		inv(0, 3) = ((b * g) - (a * h)) * invDet;
		inv(1, 3) = ((a * e) - (b * d)) * invDet;
		inv(2, 3) = ((c * e * g) - (b * f * g) - (c * d * h) + (a * f * h) + (b * d * j) - (a * e * j)) * invDet;

		trans(2, 2) = 1.0f;
		trans(3, 3) = 1.0f;
		inv(2, 2) = 1.0f;
		inv(3, 3) = 1.0f;
	}

	if (oddPiece) {

		mat4f temp;
		temp(0, 1) = -1.0f;
		temp(1, 0) = 1.0f;
		temp(0, 3) = 1.0f;
		temp(2, 2) = 1.0f;
		temp(3, 3) = 1.0f;
		mTransformX = mTransformX % temp;
		mTransformY = mTransformY % temp;

		temp(0, 1) = 1.0f;
		temp(1, 0) = -1.0f;
		temp(0, 3) = 0.0f;
		temp(1, 3) = 1.0f;
		mInverseX = temp % mInverseX;
		mInverseY = temp % mInverseY;
	}
}

MythosUnevenGround::~MythosUnevenGround() {

	delete mTextureX;
	delete mTextureY;
}

void MythosUnevenGround::render() {

	mythosPushMatrix(MYTHOS_WORLDVIEW);
	mythosTransformf(mTransformX, mInverseX, MYTHOS_WORLDVIEW);
	mTextureX->render(MYTHOS_WORLDVIEW);
	mythosPopMatrix(MYTHOS_WORLDVIEW);
	
	mythosPushMatrix(MYTHOS_WORLDVIEW);
	mythosTransformf(mTransformY, mInverseY, MYTHOS_WORLDVIEW);
	mTextureY->render(MYTHOS_WORLDVIEW);
	mythosPopMatrix(MYTHOS_WORLDVIEW);
}



void MythosChunk::render() {

	for (int i = MYTHOS_CHUNK_SIZE - 1; i >= 0; --i) {
		for (int j = MYTHOS_CHUNK_SIZE - 1; j >= 0; --j)
			mGround[i][j]->render();
	}

	renderChildren();
}


void MythosArea::render() {

	renderChildren();
}

void MythosArea::renderChildren() {


}