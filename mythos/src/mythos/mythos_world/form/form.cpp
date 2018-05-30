#include <mythos_world\form\form.h>


MythosForm::MythosForm(MythosVolume* volume) : mVolume(volume) {}

void MythosForm::render() {

	if (mTexture) {

		vec3f& fPos = mVolume->getCenter();
		vec3f tPos = vec3f(mythosProject(vec4f(fPos), MYTHOS_WORLDVIEW));

		mythosPushMatrix(MYTHOS_MODELVIEW);
		mythosTranslatef(tPos.x, tPos.y, tPos.z, MYTHOS_MODELVIEW);

		mythosPushMatrix(MYTHOS_WORLDVIEW);
		mythosTranslatef(fPos.x, fPos.y, fPos.z, MYTHOS_WORLDVIEW);

		mTexture->render();

		mythosPopMatrix(MYTHOS_MODELVIEW);
		mythosPopMatrix(MYTHOS_WORLDVIEW);
	}
}

void MythosForm::setParent(MythosContainer<MythosForm>* parent) {

	if (mParent)
		mParent->removeChild(this);
	mParent = parent;
}

void MythosForm::setTexture(MythosTexture* texture) {

	mTexture = texture;
}



int MythosDynamicContainerForm::addChild(MythosForm* form) {

	mChildren.emplace(form);
	form->setParent(this);
	return MYTHOS_TRUE;
}

void MythosDynamicContainerForm::removeChild(MythosForm* form) {

	MythosFormPtrSet::iterator it = mChildren.find(MythosFormPtr(form));

	if (it != mChildren.end())
		mChildren.erase(it);
}

void MythosDynamicContainerForm::renderChildren() {

	mythosPushMatrix(MYTHOS_WORLDVIEW);
	mythosTranslatef(getVolume()->getCenter(), MYTHOS_WORLDVIEW);

	for (MythosFormPtrSet::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		(*it)->render();

	mythosPopMatrix(MYTHOS_WORLDVIEW);
}



void MythosGenericFormAttr::setEventFunc(MYTHOS_EVENT_KEY key, MythosEventFormFunc func) {

	MythosEventFormFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end()) {

		mEvents.erase(it);
		mEvents.emplace_hint(it, key, func);
	}
	else {

		mEvents.emplace(key, func);
	}
}

MythosEventFormFunc MythosGenericFormAttr::getEventFunc(MYTHOS_EVENT_KEY key) {

	MythosEventFormFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end())
		return it->second;

	return nullptr;
}


MythosGenericForm::MythosGenericForm(MythosVolume* volume) : MythosForm(volume) {}

MYTHOS_EVENT_RETURN MythosGenericForm::update(MYTHOS_EVENT_KEY key, const MythosEvent& e) {

	MythosEventFormFunc func = getEventFunc(key);

	if (func)
		return func(this, e);

	return MYTHOS_STOP;
}