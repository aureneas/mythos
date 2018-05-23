#include "mythos_widget.h"


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


void MythosContainerWidget::update() {

	for (MythosWidgetPtrVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		(*it)->update();
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

	double zIncrement = 100.0 / mChildren.size();
	double zScale = 1.0 / mChildren.size();

	glPushMatrix();
	for (MythosWidgetPtrVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {
		glTranslated(0.0, 0.0, -zIncrement);
		glPushMatrix();
		glScaled(1.0, 1.0, zScale);
		(*it)->render();
		glPopMatrix();
	}
	glPopMatrix();
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