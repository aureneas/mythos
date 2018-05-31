#pragma once

#include "form.h"


#define __MYTHOS_GROUND_SIZE	64
#define __MYTHOS_CHUNK_SIZE		4
#define __MYTHOS_AREA_SIZE		64
#define __MYTHOS_WORLD_SIZE		7



class MYTHOS_WORLD_API MythosBasicOpenFormContainer : public MythosForm {

	public:
		virtual MythosContainer<MythosForm>*	addChild(MythosForm*);
		virtual MythosContainer<MythosForm>*	addChildWithoutOOBCheck(MythosForm*) { return nullptr; }

		virtual void							render(void) { renderChildren(); }
		virtual void							renderChildren(void) = 0;
};


class MYTHOS_WORLD_API MythosGround {

	public:
		virtual void							render(void) = 0;
};

class MYTHOS_WORLD_API MythosUnevenGround {

	private:
		mat4f									mTransformX;	// transform of (0,0)|(1,0)|(1,1) triangle
		mat4f									mTransformY;	// transform of (0,0)|(0,1)|(1,1) triangle

		mat4f									mInverseX;	// inverse transform of mTransformX
		mat4f									mInverseY;	// inverse transform of mTransformY

		MythosImageTexture*						mTextureX;
		MythosImageTexture*						mTextureY;

	public:
		MythosUnevenGround(MythosImage*, vec3f&, vec3f&, vec3f&, vec3f&, int);
		~MythosUnevenGround();

		void									render(void);
};


class MYTHOS_WORLD_API MythosChunkForm : public MythosBasicOpenFormContainer, public MythosDynamicFormContainer {

	private:
		MythosGround*							mGround[__MYTHOS_CHUNK_SIZE][__MYTHOS_CHUNK_SIZE];

	public:
		~MythosChunkForm();

		vec3f									getWorldPosition(void);

		MythosContainer<MythosForm>*			addChild(MythosForm* child) { return MythosBasicOpenFormContainer::addChild(child); }
		MythosContainer<MythosForm>*			addChildWithoutOOBCheck(MythosForm* child) { return MythosDynamicFormContainer::addChild(child); }

		void									render(void);
		void									renderChildren(void) { MythosDynamicFormContainer::renderChildren(); }
};


class MYTHOS_WORLD_API MythosAreaForm : public MythosBasicOpenFormContainer, public MythosBasicFormContainer {

	private:
		static const int						divisionFactor = __MYTHOS_CHUNK_SIZE * __MYTHOS_GROUND_SIZE;

		MythosChunkForm							mChunks[__MYTHOS_AREA_SIZE][__MYTHOS_AREA_SIZE];

	public:
		vec3f									getWorldPosition(void);

		MythosContainer<MythosForm>*			addChild(MythosForm* child) { return MythosBasicOpenFormContainer::addChild(child); }
		MythosContainer<MythosForm>*			addChildWithoutOOBCheck(MythosForm*);
		MythosFormPtr							removeChild(MythosForm*);

		void									renderChildren(void);

		/*
		Renders child forms.
		Input is four vec3f objects, with (0,0) being the origin of worldspace.
		*/
		void									renderChildren(const vec3f*);
};



class MYTHOS_WORLD_API MythosWorldForm : public MythosBasicOpenFormContainer, public MythosBasicFormContainer, public MythosWidget {

	private:
		MythosAreaForm*							mArea[__MYTHOS_WORLD_SIZE][__MYTHOS_WORLD_SIZE];

		mat4f									mCamera;
		mat4f									mCameraInverse;

		float									mResX;
		float									mResY;

		float									mWorldviewShear;
		float									mWorldviewAngle;
		float									mTopdownAngle;

	public:
		~MythosWorldForm(void);

		MythosContainer<MythosForm>*			addChild(MythosForm* child) { return addChildWithoutOOBCheck(child); }
		MythosContainer<MythosForm>*			addChildWithoutOOBCheck(MythosForm*);
		MythosFormPtr							removeChild(MythosForm*);

		int										inBounds(const vec2f&);
		MYTHOS_EVENT_RETURN						update(MYTHOS_EVENT_KEY, const MythosEvent&);
		void									render(void) { MythosBasicOpenFormContainer::render(); }
		void									renderChildren(void);

		vec3f									getWorldPosition(void);
		void									setPosition(const vec3f&);
		void									shiftPosition(const vec3f&);

		void									setTopdownAngle(float);
		void									shiftTopdownAngle(float);
		float									getTopdownAngle(void) { return mTopdownAngle; }

		void									setWorldviewAngle(float);
		void									shiftWorldviewAngle(float);
		float									getWorldviewAngle(void) { return mWorldviewAngle; }

		void									setWorldviewShear(float);
		void									shiftWorldviewShear(float);
		float									getWorldviewShear(void) { return mWorldviewShear; }

		void									setResolution(float, float);
		void									shiftResolution(float, float);
		float									getXResolution(void) { return mResX; }
		float									getYResolution(void) { return mResY; }
};