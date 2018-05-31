#include <mythos_world\form\generic.h>

typedef std::unique_ptr<MythosTexture> MythosTexturePtr;

void MythosGenericForm::transform() {

	mythosPushMatrix(MYTHOS_MODELVIEW);
	mythosPushMatrix(MYTHOS_WORLDVIEW);

	// TODO get volume, transform
}

void MythosGenericForm::detransform() {

	mythosPopMatrix(MYTHOS_MODELVIEW);
	mythosPopMatrix(MYTHOS_WORLDVIEW);
}

int MythosGenericForm::inBounds(const vec2f& pos) {

	MythosGenericFormTypedAttribute<MythosTexturePtr>* valuePtr = getAttribute<MythosTexturePtr>(MYTHOS_TEXTURE);
	
	if (valuePtr) {
		transform();
		int ret = valuePtr->value->inBounds(pos);
		detransform();
		return ret;
	}

	return MYTHOS_FALSE;
}

MYTHOS_EVENT_RETURN MythosGenericForm::update(MYTHOS_EVENT_KEY key, const MythosEvent& e) {

	MythosEventFormFunc func = getEvent(key);

	if (func) {
		transform();
		MYTHOS_EVENT_RETURN ret = func(this, e);
		detransform();
		return ret;
	}

	return MYTHOS_CONTINUE;
}

void MythosGenericForm::render() {

	MythosGenericFormTypedAttribute<MythosTexturePtr>* valuePtr = getAttribute<MythosTexturePtr>(MYTHOS_TEXTURE);

	if (valuePtr) {
		transform();
		valuePtr->value->render();
		detransform();
	}
}

void MythosGenericForm::setEvent(MYTHOS_EVENT_KEY key, MythosEventFormFunc func) {

	MythosEventFormFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end()) {
		mEvents.erase(it);
		mEvents.emplace_hint(it, key, func);
	}
	else {
		mEvents.emplace(key, func);
	}
}

MythosEventFormFunc MythosGenericForm::getEvent(MYTHOS_EVENT_KEY key) {

	MythosEventFormFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end())
		return it->second;

	return nullptr;
}