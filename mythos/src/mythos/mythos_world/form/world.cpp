#include <mythos_world\form\world.h>

MythosContainer<MythosForm>* MythosBasicOpenFormContainer::addChild(MythosForm* child) {

	const vec3f& pos = child->getPosition();
	
	if (inBounds(pos)) {

		addChildWithoutOOBCheck(child);
	} 
	else {

		child->shiftPosition(mPosition);

		MythosContainer<MythosForm>* parent = mParent->addChild(child);
		if (!parent)
			child->shiftPosition(mPosition * -1);

		return parent;
	}
}



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



MythosChunkForm::~MythosChunkForm() {

	for (int i = __MYTHOS_CHUNK_SIZE - 1; i >= 0; --i) {
		for (int j = __MYTHOS_CHUNK_SIZE - 1; j >= 0; --j)
			delete mGround[i][j];
	}
}

vec3f MythosChunkForm::getWorldPosition() {

	if (MythosAreaForm* openParent = dynamic_cast<MythosAreaForm*>(mParent))
		return openParent->getWorldPosition() + mPosition;
	else
		return mPosition;
}

void MythosChunkForm::render() {

	// render the ground
	for (int i = __MYTHOS_CHUNK_SIZE - 1; i >= 0; --i) {
		for (int j = __MYTHOS_CHUNK_SIZE - 1; j >= 0; --j)
			mGround[i][j]->render();
	}

	// render other stuff
	renderChildren();
}



typedef std::unordered_set<int> IndexSet;


vec3f MythosAreaForm::getWorldPosition() {

	return mPosition;
}

MythosContainer<MythosForm>* MythosAreaForm::addChildWithoutOOBCheck(MythosForm* child) {

	const vec3f& pos = child->getPosition();
	int I = ((int)pos.x) / divisionFactor;
	int J = ((int)pos.y) / divisionFactor;
	child->shiftPosition(vec3f(-divisionFactor * I, -divisionFactor * J, 0.0f));

	return mChunks[I][J].addChildWithoutOOBCheck(child);
}

MythosFormPtr MythosAreaForm::removeChild(MythosForm* child) {

	// TODO???
	return child->getParentForm()->removeChild(child);
}

void MythosAreaForm::renderChildren() {

	// determine corners

	vec3f zNormal = vec3f(0.0f, 0.0f, 1.0f);
	Plane groundPlane = { zNormal, 0.0f };
	Line cornerLines[4] = {
		mythosUnproject(Line(vec3f(-1.0f, 1.0f, 0.0f), zNormal), MYTHOS_WORLDVIEW),
		mythosUnproject(Line(vec3f(1.0f, 1.0f, 0.0f), zNormal), MYTHOS_WORLDVIEW),
		mythosUnproject(Line(vec3f(1.0f, -1.0f, 0.0f), zNormal), MYTHOS_WORLDVIEW),
		mythosUnproject(Line(vec3f(-1.0f, -1.0f, 0.0f), zNormal), MYTHOS_WORLDVIEW)
	};
	vec3f groundPlaneIntersects[4];
	for (int i = 3; i >= 0; --i)
		groundPlaneIntersects[i] = cornerLines[i].getPoint(groundPlane.getIntersect(cornerLines[i])) + mPosition;

	renderChildren(groundPlaneIntersects);
}

void MythosAreaForm::renderChildren(const vec3f* corners) {

	mythosPushMatrix(MYTHOS_WORLDVIEW);
	mythosTranslatef(mPosition);

	IndexSet indices;
	for (int i = 3; i >= 0; --i) {
		int I = (int)(corners[i].x - mPosition.x) / divisionFactor;
		int J = (int)(corners[i].y - mPosition.y) / divisionFactor;
		int index = (__MYTHOS_AREA_SIZE * I) + J;

		if (indices.find(index) == indices.end())
			indices.insert(index);
	}

	for (IndexSet::iterator it = indices.begin(); it != indices.end(); ++it) {
		int index = *it;
		mChunks[index / __MYTHOS_AREA_SIZE][index % __MYTHOS_AREA_SIZE].render();
	}

	mythosPopMatrix(MYTHOS_WORLDVIEW);
}



MythosWorldForm::~MythosWorldForm() {

	for (int i = __MYTHOS_WORLD_SIZE - 1; i >= 0; --i) {
		for (int j = __MYTHOS_WORLD_SIZE - 1; j >= 0; --j)
			delete mArea[i][j];
	}
}

MythosContainer<MythosForm>* MythosWorldForm::addChildWithoutOOBCheck(MythosForm* child) {

	static const int divisionFactor = __MYTHOS_AREA_SIZE * __MYTHOS_CHUNK_SIZE * __MYTHOS_GROUND_SIZE;

	const vec3f& pos = child->getPosition();
	int I = ((int)pos.x) / divisionFactor;
	int J = ((int)pos.y) / divisionFactor;
	child->shiftPosition(vec3f(-divisionFactor * I, -divisionFactor * J, 0.0f));

	return mArea[I][J]->addChildWithoutOOBCheck(child);
}

MythosFormPtr MythosWorldForm::removeChild(MythosForm* child) {

	// TODO????
	return child->getParentForm()->removeChild(child);
}

int MythosWorldForm::inBounds(const vec2f& pos) {

	// TODO
}

MYTHOS_EVENT_RETURN MythosWorldForm::update(MYTHOS_EVENT_KEY key, const MythosEvent& e) {

	if (key == MYTHOS_TIMER_EVENT) {

		for (int i = __MYTHOS_WORLD_SIZE - 1; i >= 0; --i) {
			for (int j = __MYTHOS_WORLD_SIZE - 1; j >= 0; --j)
				mArea[i][j]->update(key, e);
		}
		return MYTHOS_CONTINUE;
	}
	else {

		// TODO
	}
}

void MythosWorldForm::renderChildren() {
	
	static const int divisionFactor = __MYTHOS_AREA_SIZE * __MYTHOS_CHUNK_SIZE * __MYTHOS_GROUND_SIZE;

	// do initial transformations

	mythosCloneStack(MYTHOS_WORLDVIEW, MYTHOS_MODELVIEW);

	mythosPushMatrix(MYTHOS_WORLDVIEW);
	mythosTransformf(mCamera, mCameraInverse, MYTHOS_WORLDVIEW);

	// determine which areas are visible, draw them

	vec3f zNormal = vec3f(0.0f, 0.0f, 1.0f);
	Plane groundPlane = { zNormal, 0.0f };
	Line cornerLines[4] = {
		mythosUnproject(Line(vec3f(-1.0f, 1.0f, 0.0f), zNormal), MYTHOS_WORLDVIEW),
		mythosUnproject(Line(vec3f(1.0f, 1.0f, 0.0f), zNormal), MYTHOS_WORLDVIEW),
		mythosUnproject(Line(vec3f(1.0f, -1.0f, 0.0f), zNormal), MYTHOS_WORLDVIEW),
		mythosUnproject(Line(vec3f(-1.0f, -1.0f, 0.0f), zNormal), MYTHOS_WORLDVIEW)
	};
	vec3f groundPlaneIntersects[4];
	for (int i = 3; i >= 0; --i) 
		groundPlaneIntersects[i] = cornerLines[i].getPoint(groundPlane.getIntersect(cornerLines[i]));

	IndexSet indices;
	for (int i = 3; i >= 0; --i) {
		int I = (int)groundPlaneIntersects[i].x / divisionFactor;
		int J = (int)groundPlaneIntersects[i].y / divisionFactor;
		int index = (__MYTHOS_WORLD_SIZE * I) + J;

		if (indices.find(index) == indices.end())
			indices.insert(index);

		groundPlaneIntersects[i] -= mPosition;
	}

	for (IndexSet::iterator it = indices.begin(); it != indices.end(); ++it) {
		int index = *it;
		mArea[index / __MYTHOS_WORLD_SIZE][index % __MYTHOS_WORLD_SIZE]->renderChildren(groundPlaneIntersects);
	}

	mythosPopMatrix(MYTHOS_WORLDVIEW);
}

vec3f MythosWorldForm::getWorldPosition() {

	return mPosition;
}

void MythosWorldForm::setPosition(const vec3f& pos) {

	shiftPosition(pos - mPosition);
}

void MythosWorldForm::shiftPosition(const vec3f& disp) {

	vec3f transDisp = mCamera % vec4f(disp);
	for (int i = 2; i >= 0; --i) {

		mCamera(i, 3) -= transDisp.getValue(i);
		mCameraInverse(i, 3) += disp.getValue(i);
	}

	mPosition += disp;
}

void MythosWorldForm::setTopdownAngle(float angle) {

	shiftTopdownAngle(angle - mTopdownAngle);
}

void MythosWorldForm::shiftTopdownAngle(float diff) {

	float cosT = cosf(diff);
	float sinT = sinf(diff);

	float xAdd = mPosition.getValue(1) * sinT;
	float yAdd = mPosition.getValue(0) * sinT;

	mat4f temp;

	temp(0, 0) = cosT;
	temp(0, 1) = -sinT;
	temp(1, 0) = sinT;
	temp(1, 1) = cosT;

	temp(2, 2) = 1.0f;
	temp(3, 3) = 1.0f;

	temp(0, 3) = (mPosition.getValue(0) * (1.0f - cosT)) + xAdd;
	temp(1, 3) = (mPosition.getValue(1) * (1.0f - cosT)) - yAdd;

	mCamera = mCamera % temp;

	temp(1, 0) = temp(0, 1);
	temp(0, 1) = sinT;

	temp(0, 3) -= 2 * xAdd;
	temp(1, 3) += 2 * yAdd;

	mCameraInverse = temp % mCameraInverse;

	mTopdownAngle += diff;
}

void MythosWorldForm::setWorldviewAngle(float angle) {

	shiftWorldviewAngle(angle - mWorldviewAngle);
}

void MythosWorldForm::shiftWorldviewAngle(float diff) {

	float cosW = cosf(diff);
	float sinW = sinf(diff);

	mat4f temp;

	temp(0, 0) = 1.0f;
	temp(3, 3) = 1.0f;

	temp(1, 1) = cosW + (mWorldviewShear * sinW);
	temp(2, 1) = sinW / mResY;
	temp(1, 2) = -mResY * sinW * (1.0f + (mWorldviewShear * mWorldviewShear));
	temp(2, 2) = cosW - (mWorldviewShear * sinW);

	mCamera = temp % mCamera;

	float hold = temp(1, 1);
	temp(1, 1) = temp(2, 2);
	temp(2, 1) = -temp(2, 1);
	temp(1, 2) = -temp(1, 2);
	temp(2, 2) = hold;

	mCameraInverse = mCameraInverse % temp;

	mWorldviewAngle += diff;
}

void MythosWorldForm::setWorldviewShear(float shear) {

	shiftWorldviewShear(shear - mWorldviewShear);
}

void MythosWorldForm::shiftWorldviewShear(float diff) {

	mat4f temp;

	temp.identity();
	temp(1, 2) = mResY * diff;
	mCamera = temp % mCamera;

	temp(1, 2) = -temp(1, 2);
	mCameraInverse = mCameraInverse % temp;

	mWorldviewShear += diff;
}

void MythosWorldForm::setResolution(float resX, float resY) {

	mat4f temp;

	temp(0, 0) = resX / mResX;
	temp(1, 1) = resY / mResY;
	temp(2, 2) = 1.0f;
	temp(3, 3) = 1.0f;
	mCamera = temp % mCamera;

	temp(0, 0) = mResX / resX;
	temp(1, 1) = mResY / resY;
	mCameraInverse = mCameraInverse % temp;

	mResX = resX;
	mResY = resY;
}

void MythosWorldForm::shiftResolution(float diffX, float diffY) {

	mat4f temp;

	temp(0, 0) = diffX;
	temp(1, 1) = diffY;
	temp(2, 2) = 1.0f;
	temp(3, 3) = 1.0f;
	mCamera = temp % mCamera;

	temp(0, 0) = 1.0f / diffX;
	temp(1, 1) = 1.0f / diffY;
	mCameraInverse = mCameraInverse % temp;

	mResX *= diffX;
	mResY *= diffY;
}