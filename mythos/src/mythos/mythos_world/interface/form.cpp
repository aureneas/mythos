#include <mythos_world\interface\form.h>


MythosFormWidget::MythosFormWidget(MythosForm* form) {

	mForm = form;
}

int MythosFormWidget::inBounds(const vec2f& pos) {

	MythosTexture* texture = mForm->getTexture();

	if (texture) {

		vec3f& fPos = mForm->getVolume()->getCenter();
		vec3f tPos = vec3f(mythosProject(vec4f(fPos), MYTHOS_WORLDVIEW));

		mythosPushMatrix(MYTHOS_MODELVIEW);
		mythosTranslatef(tPos.x, tPos.y, tPos.z, MYTHOS_MODELVIEW);

		mythosPushMatrix(MYTHOS_WORLDVIEW);
		mythosTranslatef(fPos.x, fPos.y, fPos.z, MYTHOS_WORLDVIEW);

		int ret = texture->inBounds(pos);

		mythosPopMatrix(MYTHOS_MODELVIEW);
		mythosPopMatrix(MYTHOS_WORLDVIEW);

		return ret;
	}

	return MYTHOS_FALSE;
}

void MythosFormWidget::render() {

	MythosTexture* texture = mForm->getTexture();

	if (texture) {

		vec3f& fPos = mForm->getVolume()->getCenter();
		vec3f tPos = vec3f(mythosProject(vec4f(fPos), MYTHOS_WORLDVIEW));

		mythosPushMatrix(MYTHOS_MODELVIEW);
		mythosTranslatef(tPos.x, tPos.y, tPos.z, MYTHOS_MODELVIEW);

		mythosPushMatrix(MYTHOS_WORLDVIEW);
		mythosTranslatef(fPos.x, fPos.y, fPos.z, MYTHOS_WORLDVIEW);

		texture->render();

		mythosPopMatrix(MYTHOS_MODELVIEW);
		mythosPopMatrix(MYTHOS_WORLDVIEW);
	}
}

MYTHOS_EVENT_RETURN MythosFormWidget::update(MYTHOS_EVENT_KEY key, const MythosEvent& e) {

	switch (key) {

		case MYTHOS_MOUSE_PRESS:
		case MYTHOS_MOUSE_RELEASE:

			if (inBounds(e.mouse))
				mForm->update(key, e);
			break;

		case MYTHOS_MOUSE_MOVE:
			
			if (inBounds(e.mouse) == MYTHOS_TRUE && mMouseover == MYTHOS_FALSE)
				mForm->update(MYTHOS_MOUSE_OFF, e);
			else if (inBounds(e.mouse) == MYTHOS_FALSE && mMouseover == MYTHOS_TRUE)
				mForm->update(MYTHOS_MOUSE_ON, e);
			break;

		default:

			mForm->update(key, e);
			break;
	}
}


MythosContainerFormWidget::MythosContainerFormWidget(MythosContainerForm* container) {

	mContainer = container;
}

int MythosContainerFormWidget::inBounds(const vec2f& pos) {

	return MythosWidgetContainer::inBounds(pos);
}

MYTHOS_EVENT_RETURN MythosContainerFormWidget::update(MYTHOS_EVENT_KEY key, const MythosEvent& e) {

	if (key == MYTHOS_MOUSE_RELEASE && e.mouse.drag != nullptr) {

		if (MythosFormWidget* fWidget = dynamic_cast<MythosFormWidget*>(e.mouse.drag)) {

			MythosForm* form = fWidget->getForm();
			
			vec2f scrPos = e.mouse;
			scrPos += e.mouse.dragDisplacement;
			vec3f pos = mythosUnproject(scrPos, MYTHOS_WORLDVIEW);

			vec3f& center = form->getVolume()->getCenter();
			vec3f prevPos = center;
			center = pos;

			if (form->setParent(mContainer)) 
				fWidget->setParent(this);
			else 
				center = prevPos;

			return MYTHOS_STOP;
		}
	}

	return MythosWidgetContainer::update(key, e);
}

void MythosContainerFormWidget::render() {

	MythosWidgetContainer::render();
}



MythosCameraContainerFormWidget::MythosCameraContainerFormWidget(MythosContainerForm* container) : MythosContainerFormWidget(container) {
	
	mCamera.identity();
	mCameraInverse.identity();

	mCenter = vec3f(0.0f, 0.0f, 0.0f);

}

int MythosCameraContainerFormWidget::inBounds(const vec2f& pos) {

	mythosPushMatrix(MYTHOS_WORLDVIEW);
	mythosTransformf(mCamera, mCameraInverse, MYTHOS_WORLDVIEW);

	int ret = MythosContainerFormWidget::inBounds(pos);

	mythosPopMatrix(MYTHOS_WORLDVIEW);

	return ret;
}

MYTHOS_EVENT_RETURN MythosCameraContainerFormWidget::update(MYTHOS_EVENT_KEY key, const MythosEvent& e) {

	mythosPushMatrix(MYTHOS_WORLDVIEW);
	mythosTransformf(mCamera, mCameraInverse, MYTHOS_WORLDVIEW);

	MYTHOS_EVENT_RETURN ret = MythosContainerFormWidget::update(key, e);

	mythosPopMatrix(MYTHOS_WORLDVIEW);

	return ret;
}

void MythosCameraContainerFormWidget::render() {

	mythosPushMatrix();
	mythosTransformf(mCamera, mCameraInverse, MYTHOS_WORLDVIEW);

	MythosContainerFormWidget::render();

	mythosPopMatrix();
}

void MythosCameraContainerFormWidget::resetCamera() {

	mCamera.identity();
	mCameraInverse.identity();

	float cosW = cosf(mWorldviewAngle);
	float sinW = sinf(mWorldviewAngle);
	mCamera(0, 0) = mResX;
	mCamera(1, 1) = mResY * (cosW + (mWorldviewShear * sinW));
	mCamera(1, 2) = mResY * (-sinW + (mWorldviewShear * cosW));
	mCamera(2, 1) = sinW;
	mCamera(2, 2) = cosW;

	// TODO make more efficient
	mCameraInverse(0, 0) = 1.0f / mResX;
	mCameraInverse(1, 1) = cosW / mResY;
	mCameraInverse(2, 1) = -sinW / mResY;
	mCameraInverse(1, 2) = sinW - (mWorldviewShear * cosW);
	mCameraInverse(2, 2) = cosW + (mWorldviewShear * sinW);

	mat4f temp;

	float cosT = cosf(mTopdownAngle);
	float sinT = sinf(mTopdownAngle);
	
	temp.identity();
	temp(0, 0) = cosT;
	temp(0, 1) = -sinT;
	temp(1, 0) = sinT;
	temp(1, 1) = cosT;
	mCamera = mCamera % temp;

	temp(1, 0) = temp(0, 1);
	temp(0, 1) = sinT;
	mCameraInverse = temp % mCameraInverse;

	temp.identity();
	for (int i = 2; i >= 0; --i)
		temp(i, 3) = -mCenter(i);
	mCamera = mCamera % temp;

	for (int i = 2; i >= 0; --i)
		temp(i, 3) = mCenter(i);
	mCameraInverse = temp % mCameraInverse;
}

void MythosCameraContainerFormWidget::setCenter(const vec3f& center) {

	vec3f disp = mCenter - center;
	vec3f transDisp = mCamera % vec4f(disp);
	for (int i = 2; i >= 0; --i) {

		mCamera(i, 3) -= transDisp(i);
		mCameraInverse(i, 3) += disp(i);
	}

	mCenter = center;
}

void MythosCameraContainerFormWidget::shiftCenter(const vec3f& disp) {

	vec3f transDisp = mCamera % vec4f(disp);
	for (int i = 2; i >= 0; --i) {

		mCamera(i, 3) -= transDisp(i);
		mCameraInverse(i, 3) += disp.m[i];
	}

	mCenter += disp;
}

void MythosCameraContainerFormWidget::setTopdownAngle(float angle) {

	float cosT = cosf(angle);
	float sinT = sinf(angle);

	mat4f temp;
	
	temp(0, 0) = cosT;
	temp(0, 1) = -sinT;
	temp(1, 0) = sinT;
	temp(1, 1) = cosT;

	temp(2, 2) = 1.0f;
	temp(3, 3) = 1.0f;

	temp(0, 3) = (mCenter(0) * (1.0f - cosT)) + (mCenter(1) * sinT);
	temp(1, 3) = (mCenter(1) * (1.0f - cosT)) - (mCenter(0) * sinT);

	mCamera = mCamera % temp;

	temp(1, 0) = temp(0, 1);
	temp(0, 1) = sinT;

	// TODO make more efficient
	temp(0, 3) = (mCenter(0) * (1.0f - cosT)) - (mCenter(1) * sinT);
	temp(1, 3) = (mCenter(1) * (1.0f - cosT)) + (mCenter(0) * sinT);

	mCameraInverse = temp % mCameraInverse;

	mTopdownAngle = angle;
}

void MythosCameraContainerFormWidget::setWorldviewAngle(float angle) {

	float cosW = cosf(angle);
	float sinW = sinf(angle);

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

	mWorldviewAngle = angle;
}

void MythosCameraContainerFormWidget::setWorldviewShear(float shear) {

	mat4f temp;
	
	temp.identity();
	temp(1, 2) = mResY * (shear - mWorldviewShear);
	mCamera = temp % mCamera;

	temp(1, 2) = -temp(1, 2);
	mCameraInverse = mCameraInverse % temp;

	mWorldviewShear = shear;
}

void MythosCameraContainerFormWidget::setResolution(float resX, float resY) {

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