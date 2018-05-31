#include <mythos_world\form\form.h>
#include <mythos_world\form\world.h>


vec3f MythosForm::getWorldPosition(void) {

	if (MythosChunkForm* openParent = dynamic_cast<MythosChunkForm*>(mParent))
		return openParent->getWorldPosition() + mPosition;
	else
		return mPosition;
}

MythosFormPtr MythosForm::setParentForm(MythosContainer<MythosForm>* parent) {

	MythosFormPtr ptr;
	if (mParent)
		ptr = mParent->removeChild(this);
	else
		ptr.reset(this);
	mParent = parent;
}

int MythosForm::inBounds(const vec3f& pos) {

	mythosPushMatrix(MYTHOS_WORLDVIEW);
	mythosTranslatef(mPosition * -1, MYTHOS_WORLDVIEW);

	int ret = mData->inBounds(pos);

	mythosPopMatrix(MYTHOS_WORLDVIEW);
	return ret;
}

MYTHOS_EVENT_RETURN MythosForm::update(MYTHOS_EVENT_KEY key, const MythosEvent& e) {

	mythosPushMatrix(MYTHOS_WORLDVIEW);
	mythosTranslatef(mPosition * -1, MYTHOS_WORLDVIEW);

	MYTHOS_EVENT_RETURN ret = mData->update(key, this, e);

	mythosPopMatrix(MYTHOS_WORLDVIEW);
	return ret;
}

void MythosForm::render(void) {

	mythosPushMatrix(MYTHOS_WORLDVIEW_RENDER);
	mythosTranslatef(mPosition * -1, MYTHOS_WORLDVIEW_RENDER);

	mData->render();

	mythosPopMatrix(MYTHOS_WORLDVIEW_RENDER);
}



MythosContainer<MythosForm>* MythosDynamicFormContainer::addChild(MythosForm* child) {

	MythosFormPtr ptr(child->setParentForm(this));
	mChildren.insert(ptr);
	return this;
}

MythosFormPtr MythosDynamicFormContainer::removeChild(MythosForm* child) {

	MythosFormPtrContainer::iterator it = mChildren.find(MythosFormPtr(child));

	if (it != mChildren.end()) {
		MythosFormPtr childPtr(*it);
		mChildren.erase(it);
		return childPtr;
	}

	return MythosFormPtr(nullptr);
}

int MythosDynamicFormContainer::inBounds(const vec2f& pos) {

	for (MythosFormPtrContainer::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {

		if ((*it)->inBounds(pos))
			return MYTHOS_TRUE;
	}

	return MYTHOS_FALSE;
}

MYTHOS_EVENT_RETURN MythosDynamicFormContainer::update(MYTHOS_EVENT_KEY key, const MythosEvent& e) {

	for (MythosFormPtrContainer::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {

		MYTHOS_EVENT_RETURN ret = (*it)->update(key, e);
		if (ret != MYTHOS_CONTINUE)
			return ret;
	}

	return MYTHOS_CONTINUE;
}

void MythosDynamicFormContainer::renderChildren() {

	for (MythosFormPtrContainer::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		(*it)->render();
}