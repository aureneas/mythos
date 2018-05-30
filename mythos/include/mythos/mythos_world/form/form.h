#pragma once

#include <memory>
#include <unordered_map>
#include "..\_important_stuff.h"
#include "..\geometry\volume.h"


#define MYTHOS_WORLDVIEW		1


#define MYTHOS_MOUSE_OFF		8
#define MYTHOS_MOUSE_ON			9


class MythosForm;


class MYTHOS_WORLD_API MythosForm {

	private:

		MythosContainer<MythosForm>*		mParent;

	protected:

		std::unique_ptr<MythosVolume>		mVolume;

		MythosTexture*						mTexture;

	public:

		MythosForm(MythosVolume*);

		virtual MYTHOS_EVENT_RETURN			update(MYTHOS_EVENT_KEY, const MythosEvent&) { return MYTHOS_CONTINUE; }

		virtual void						render(void);
	
		void								setParent(MythosContainer<MythosForm>*);

		MythosContainer<MythosForm>*		getParent(void) { return mParent; }

		void								setTexture(MythosTexture*);

		MythosTexture*						getTexture(void) { return mTexture; }

		MythosVolume*						getVolume(void) { return mVolume.get(); }
};


typedef std::shared_ptr<MythosForm> MythosFormPtr;

typedef std::unordered_set<MythosFormPtr> MythosFormPtrSet;

class MYTHOS_WORLD_API MythosContainerForm : public MythosForm, public MythosContainer<MythosForm> {

	public:

		virtual void renderChildren(void) = 0;
};

class MYTHOS_WORLD_API MythosDynamicContainerForm : public MythosContainerForm {

	protected:

		MythosFormPtrSet			mChildren;

	public:

		virtual int					addChild(MythosForm*);

		virtual void				removeChild(MythosForm*);

		virtual void				renderChildren(void);
};



typedef MYTHOS_EVENT_RETURN(*MythosEventFormFunc)(MythosForm*, const MythosEvent& e);

typedef std::unordered_map<MYTHOS_EVENT_KEY, MythosEventFormFunc> MythosEventFormFuncMap;

class MYTHOS_WORLD_API MythosGenericFormAttr {

	private:

		MythosEventFormFuncMap				mEvents;

	public:

		void								setEventFunc(MYTHOS_EVENT_KEY, MythosEventFormFunc);
		MythosEventFormFunc					getEventFunc(MYTHOS_EVENT_KEY);
};

class MYTHOS_WORLD_API MythosGenericForm : public MythosForm, public MythosGenericFormAttr {

	public:

		MythosGenericForm(MythosVolume*);

		MYTHOS_EVENT_RETURN					update(MYTHOS_EVENT_KEY, const MythosEvent&);
};