#pragma once

#include <mythos.h>
#include "..\form\form.h"


class MYTHOS_WORLD_API MythosFormWidget : public MythosWidget {

	private:

		MythosForm*			mForm;

		int					mMouseover = MYTHOS_FALSE;

	public:

		MythosFormWidget(MythosForm*);

		virtual int inBounds(const vec2f&);
		virtual MYTHOS_EVENT_RETURN update(MYTHOS_EVENT_KEY, const MythosEvent&);
		virtual void render(void);

		MythosForm* getForm(void) { return mForm; }
};


class MYTHOS_WORLD_API MythosContainerFormWidget : public MythosWidgetContainer, public MythosWidget {

	protected:

		MythosContainerForm*			mContainer;

	public:

		MythosContainerFormWidget(MythosContainerForm*);

		virtual int						inBounds(const vec2f&);
		virtual MYTHOS_EVENT_RETURN		update(MYTHOS_EVENT_KEY, const MythosEvent&);
		virtual void					render(void);
};


class MYTHOS_WORLD_API MythosCameraContainerFormWidget : public MythosContainerFormWidget {

	protected:

		mat4f							mCamera;
		mat4f							mCameraInverse;

		vec3f							mCenter;

		float							mTopdownAngle;
		float							mWorldviewAngle;
		float							mWorldviewShear;

		float							mResX;
		float							mResY;

	public:

		MythosCameraContainerFormWidget(MythosContainerForm*);

		virtual int						inBounds(const vec2f&);
		virtual MYTHOS_EVENT_RETURN		update(MYTHOS_EVENT_KEY, const MythosEvent&);
		virtual void					render(void);

		void							resetCamera(void);

		virtual void					setCenter(const vec3f&);
		virtual void					shiftCenter(const vec3f&);
		virtual void					setTopdownAngle(float);
		virtual void					setWorldviewAngle(float);
		virtual void					setWorldviewShear(float);
		virtual void					setResolution(float, float);
};