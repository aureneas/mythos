#include "mythos_widget.h"
#include "mythos_window.h"
#include "utility/mythos_stack.h"


MythosWidget::MythosWidget(vec2f pos) {

	mPos = pos;
}

MYTHOS_EVENT_RETURN MythosWidget::update(MYTHOS_EVENT_KEY key, const MythosEvent& ptr) {

	MythosEventFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end()) 
		return it->second(this, ptr);

	return MYTHOS_CONTINUE;
}

void MythosWidget::setParent(MythosContainerWidget* parent) {

	mParent = parent;
}

void MythosWidget::setEventFunc(MYTHOS_EVENT_KEY key, MythosEventFunc func) {

	MythosEventFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end()) {

		mEvents.erase(it);
		mEvents.emplace_hint(it, key, func);
	}
	else {

		mEvents.emplace(key, func);
	}
}

MythosEventFunc MythosWidget::getEventFunc(MYTHOS_EVENT_KEY key) {

	MythosEventFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end())
		return it->second;

	return nullptr;
}


MythosContainerWidget::MythosContainerWidget(vec2f pos) : MythosWidget(pos) {}

int MythosContainerWidget::inBounds(vec2f& pos) {

	vec2f nPos = pos - mPos;

	for (MythosWidgetPtrVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {
		
		if ((*it)->inBounds(nPos))
			return true;
	}
	
	return false;
}

MYTHOS_EVENT_RETURN MythosContainerWidget::update(MYTHOS_EVENT_KEY key, const MythosEvent& ptr) {

	MYTHOS_EVENT_RETURN res = MythosWidget::update(key, ptr);

	MythosWidgetPtrVector::iterator it = mChildren.begin();

	while (it != mChildren.end() && res == MYTHOS_CONTINUE) {

		res = (*it)->update(key, ptr);

		++it;
	}

	return res;
}

void MythosContainerWidget::render() {

	float scaleFactor = 1.0f / mChildren.size();

#ifdef MYTHOS_STACK
	mythosPushMatrix();
	mythosTranslatef(mPos.x, mPos.y, 0.0f);
	mythosScalef(1.0f, 1.0f, MYTHOS_FAR * scaleFactor);
#else
	glPushMatrix();
	glTranslated(mPos.x, mPos.y, 0.0);
	glScaled(1.0, 1.0, MYTHOS_FAR / mChildren.size());
#endif
	
	for (MythosWidgetPtrVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {

#ifdef MYTHOS_STACK
		mythosTranslatef(0.0f, 0.0f, -1.0f);
		mythosPushMatrix();
		mythosScalef(1.0f, 1.0f, scaleFactor);
#else
		glTranslated(0.0, 0.0, -1.0);
		glPushMatrix();
		glScaled(1.0, 1.0, 1.0 / mChildren.size());
#endif

		(*it)->render();

#ifdef MYTHOS_STACK
		mythosPopMatrix();
#else
		glPopMatrix();
#endif
	}

#ifdef MYTHOS_STACK
	mythosPopMatrix();
#else
	glPopMatrix();
#endif
}

void MythosContainerWidget::addChildWidget(MythosWidget* widget) {

	mChildren.emplace_back(widget);
	widget->setParent(this);
}

void MythosContainerWidget::removeChildWidget(MythosWidget* widget) {

	for (MythosWidgetPtrVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {
		
		if (it->get() == widget) {

			it->swap(MythosWidgetPtr(nullptr));
			mChildren.erase(it);
			break;
		}
	}
}

void MythosContainerWidget::bumpChildWidget(MythosWidget* widget) {

	for (MythosWidgetPtrVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {

		if (it->get() == widget) {

			mChildren.splice(mChildren.begin(), mChildren, it);
			break;
		}
	}
}