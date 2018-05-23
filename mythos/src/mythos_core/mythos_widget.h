#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include "mythos_important_stuff.h"
#include "mythos_event.h"


class MythosWidget;
class MythosContainerWidget;


typedef MYTHOS_EVENT_RETURN(*MythosEventFunc)(MythosWidget*, const MythosEvent&);

typedef std::unordered_map<MYTHOS_EVENT_KEY, MythosEventFunc> MythosEventFuncMap;


class MYTHOS_API MythosWidget {

	private:

		MythosContainerWidget*		mParent = nullptr;

		MythosEventFuncMap			mEvents;

	public:

		virtual void				update(void) {}

		virtual MYTHOS_EVENT_RETURN	update(MYTHOS_EVENT_KEY, const MythosEvent&);

		virtual void				render(void) {}

		void						setParent(MythosContainerWidget*);

		MythosContainerWidget*		getParent(void) { return mParent; }

		void						setEventFunc(MYTHOS_EVENT_KEY, MythosEventFunc);

		MythosEventFunc				getEventFunc(MYTHOS_EVENT_KEY);
};


typedef std::shared_ptr<MythosWidget>		MythosWidgetPtr;
typedef std::list<MythosWidgetPtr>			MythosWidgetPtrVector;

class MYTHOS_API MythosContainerWidget : public MythosWidget {

	protected:

		MythosWidgetPtrVector		mChildren;

	public:

		virtual void update(void);

		virtual MYTHOS_EVENT_RETURN update(MYTHOS_EVENT_KEY, const MythosEvent&);

		virtual void render(void);

		void addChildWidget(MythosWidget*);

		void removeChildWidget(MythosWidget*);
};