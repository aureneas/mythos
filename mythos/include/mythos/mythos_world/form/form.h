#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "..\data\form.h"


#define MYTHOS_WORLDVIEW			2
#define MYTHOS_WORLDVIEW_RENDER		3


class MYTHOS_WORLD_API MythosForm {

	protected:
		MythosFormData*							mData;
		MythosContainer<MythosForm>*			mParent = nullptr;

		vec3f									mPosition;

	public:
		/*
		Gets the position of the form relative to the parent.
		*/
		virtual const vec3f&					getPosition(void) { return mPosition; }

		/*
		Gets the position of the form relative to the worldview.
		*/
		virtual vec3f							getWorldPosition(void);

		/*
		Sets the position of the form relative to the parent.
		*/
		virtual void							setPosition(const vec3f& pos) { mPosition = pos; }

		/*
		Shifts the position of the form by the passed-in vec3f.
		*/
		virtual void							shiftPosition(const vec3f& dif) { mPosition += dif; }

		/*
		Checks whether the vec2f is contained within the projection of the form onto the screen.
		When inBounds() is called, the current tranformation should have (0,0) as the position of the parent.
		*/
		virtual int								inBounds(const vec2f&) { return MYTHOS_FALSE; }

		/*
		Checks whether the vec3f is contained within the worldview volume of the form.
		The vec3f should be defined in the space where (0,0) is the position of the parent.
		*/
		virtual int								inBounds(const vec3f&);

		/*
		Updates the form with an event (keyboard, mouse, timer).
		When update() is called, the current transformation should have (0,0) as the position of the parent.
		*/
		virtual MYTHOS_EVENT_RETURN				update(MYTHOS_EVENT_KEY, const MythosEvent&);

		/*
		Renders the form.
		When render() is called, the current transformation should have (0,0) as the position of the parent.
		*/
		virtual void							render(void);

		MythosFormPtr							setParentForm(MythosContainer<MythosForm>*);
		MythosContainer<MythosForm>*			getParentForm(void) { return mParent; }
};



typedef std::shared_ptr<MythosForm>				MythosFormPtr;
typedef std::unordered_set<MythosFormPtr>		MythosFormPtrContainer;


class MYTHOS_WORLD_API MythosBasicFormContainer : public MythosContainer<MythosForm> {

	public:
		virtual void							renderChildren(void) = 0;
};

class MYTHOS_WORLD_API MythosDynamicFormContainer : public MythosBasicFormContainer {

	protected:

		MythosFormPtrContainer					mChildren;

	public:

		virtual MythosContainer<MythosForm>*	addChild(MythosForm*);
		virtual MythosFormPtr					removeChild(MythosForm*);

		virtual int								inBounds(const vec2f&);
		virtual MYTHOS_EVENT_RETURN				update(MYTHOS_EVENT_KEY, const MythosEvent&);
		virtual void							renderChildren(void);
};