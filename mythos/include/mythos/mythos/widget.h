#pragma once

#include <memory>
#include <unordered_map>
#include <forward_list>
#include "_important_stuff.h"
#include "event.h"
#include "utility\container.h"


class MYTHOS_CORE_API MythosWidget {

	private:

		MythosContainer<MythosWidget>*	mParent = nullptr;

	public:

		MythosWidget() {}

		virtual int						inBounds(const vec2f&) { return false; }

		virtual MYTHOS_EVENT_RETURN		update(MYTHOS_EVENT_KEY, const MythosEvent&) { return MYTHOS_CONTINUE; }

		virtual void					render(void) {}

		void							setParent(MythosContainer<MythosWidget>*);

		MythosContainer<MythosWidget>*	getParent(void) { return mParent; }
};


typedef std::shared_ptr<MythosWidget>		MythosWidgetPtr;
typedef std::list<MythosWidgetPtr>			MythosWidgetPtrVector;

class MYTHOS_CORE_API MythosWidgetContainer : public MythosContainer<MythosWidget> {

	protected:

		MythosWidgetPtrVector			mChildren;

	public:

		virtual int						inBounds(const vec2f&);

		virtual MYTHOS_EVENT_RETURN		update(MYTHOS_EVENT_KEY, const MythosEvent&);

		virtual void					render(void);

		MythosContainer<MythosWidget>*	addChild(MythosWidget*);
		
		void							removeChild(MythosWidget*);

		void							bumpChild(MythosWidget*);
};



typedef MYTHOS_EVENT_RETURN(*MythosEventFunc)(MythosWidget*, const MythosEvent&);

typedef std::unordered_map<MYTHOS_EVENT_KEY, MythosEventFunc> MythosEventFuncMap;

class MYTHOS_CORE_API MythosGenericWidget : public MythosWidget {

	private:

		MythosEventFuncMap				mEvents;

	protected:

		vec2f							mPos;

	public:

		MythosGenericWidget(vec2f);

		virtual MYTHOS_EVENT_RETURN		update(MYTHOS_EVENT_KEY, const MythosEvent&);

		void							setEventFunc(MYTHOS_EVENT_KEY, MythosEventFunc);

		MythosEventFunc					getEventFunc(MYTHOS_EVENT_KEY);
};



class MYTHOS_CORE_API MythosGenericContainerWidget : public MythosGenericWidget, public MythosWidgetContainer {

	public:

		MythosGenericContainerWidget(vec2f);
		
		virtual int						inBounds(const vec2f&);

		virtual MYTHOS_EVENT_RETURN		update(MYTHOS_EVENT_KEY, const MythosEvent&);

		virtual void					render(void);
};