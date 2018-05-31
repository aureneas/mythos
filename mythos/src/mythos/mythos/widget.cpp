#include <mythos\widget.h>
#include <mythos\window.h>
#include <mythos\utility\stack.h>


void MythosWidget::setParent(MythosContainer<MythosWidget>* parent) {

	if (mParent)
		mParent->removeChild(this);
	mParent = parent;
	if (mParent)
		mParent->addChild(this);
}


int MythosWidgetContainer::inBounds(const vec2f& pos) {

	for (MythosWidgetPtrContainer::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {

		if ((*it)->inBounds(pos))
			return MYTHOS_TRUE;
	}

	return MYTHOS_FALSE;
}

MYTHOS_EVENT_RETURN MythosWidgetContainer::update(MYTHOS_EVENT_KEY key, const MythosEvent& ptr) {

	MYTHOS_EVENT_RETURN res = MYTHOS_CONTINUE;

	MythosWidgetPtrContainer::iterator it = mChildren.begin();

	while (it != mChildren.end() && res == MYTHOS_CONTINUE) {

		res = (*it)->update(key, ptr);
		++it;
	}

	return res;
}

void MythosWidgetContainer::render() {

	float scaleFactor = 1.0f / mChildren.size();

	mythosPushMatrix();
	mythosScalef(1.0f, 1.0f, MYTHOS_FAR * scaleFactor);

	for (MythosWidgetPtrContainer::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {

		mythosTranslatef(0.0f, 0.0f, -1.0f);
		mythosPushMatrix();
		mythosScalef(1.0f, 1.0f, scaleFactor);

		(*it)->render();

		mythosPopMatrix();
	}

	mythosPopMatrix();
}

MythosContainer<MythosWidget>* MythosWidgetContainer::addChild(MythosWidget* widget) {

	MythosWidgetPtr widgetPtr(widget);
	mChildren.push_back(widgetPtr);
	return this;
}

MythosWidgetPtr MythosWidgetContainer::removeChild(MythosWidget* widget) {

	for (MythosWidgetPtrContainer::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {

		if (it->get() == widget) {

			MythosWidgetPtr widgetPtr(*it);
			mChildren.erase(it);
			return widgetPtr;
		}
	}

	return MythosWidgetPtr(nullptr);
}

void MythosWidgetContainer::bumpChild(MythosWidget* widget) {

	for (MythosWidgetPtrContainer::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {

		if (it->get() == widget) {

			mChildren.splice(mChildren.begin(), mChildren, it);
			break;
		}
	}
}


MythosGenericWidget::MythosGenericWidget(vec2f pos) {

	mPos = pos;
}

MYTHOS_EVENT_RETURN MythosGenericWidget::update(MYTHOS_EVENT_KEY key, const MythosEvent& ptr) {

	MythosEventFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end()) 
		return it->second(this, ptr);

	return MYTHOS_CONTINUE;
}

void MythosGenericWidget::setEventFunc(MYTHOS_EVENT_KEY key, MythosEventFunc func) {

	MythosEventFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end()) {

		mEvents.erase(it);
		mEvents.emplace_hint(it, key, func);
	}
	else {

		mEvents.emplace(key, func);
	}
}

MythosEventFunc MythosGenericWidget::getEventFunc(MYTHOS_EVENT_KEY key) {

	MythosEventFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end())
		return it->second;

	return nullptr;
}


MythosGenericContainerWidget::MythosGenericContainerWidget(vec2f pos) : MythosGenericWidget(pos) {}

int MythosGenericContainerWidget::inBounds(const vec2f& pos) {

	mythosPushMatrix();
	mythosTranslatef(mPos.x, mPos.y, 0.0f);

	int ret = MythosWidgetContainer::inBounds(pos);

	mythosPopMatrix();

	return ret;
}

MYTHOS_EVENT_RETURN MythosGenericContainerWidget::update(MYTHOS_EVENT_KEY key, const MythosEvent& e) {

	mythosPushMatrix();
	mythosTranslatef(mPos.x, mPos.y, 0.0f);

	MYTHOS_EVENT_RETURN ret = MythosWidgetContainer::update(key, e);

	mythosPopMatrix();

	return ret;
}

void MythosGenericContainerWidget::render() {

	mythosPushMatrix();
	mythosTranslatef(mPos.x, mPos.y, 0.0f);

	MythosWidgetContainer::render();

	mythosPopMatrix();
}