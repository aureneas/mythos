#include <mythos_world\data\generic.h>
#include <mythos_world\form\form.h>


typedef std::unique_ptr<MythosTexture> MythosTexturePtr;


int MythosGenericFormData::inBounds(const vec2f& pos) {

	MythosTexture* valuePtr = getAttribute<MythosTexturePtr>(MYTHOS_TEXTURE).get();

	if (valuePtr) 
		return valuePtr->inBounds(pos);

	return MYTHOS_FALSE;
}

MYTHOS_EVENT_RETURN MythosGenericFormData::update(MYTHOS_EVENT_KEY key, MythosForm* form, const MythosEvent& e) {

	MythosEventFormFunc func = getEvent(key);

	if (func) {
		func(form, e);

	return MYTHOS_CONTINUE;
}

void MythosGenericFormData::render() {

	MythosTexture* valuePtr = getAttribute<MythosTexturePtr>(MYTHOS_TEXTURE).get();

	if (valuePtr) 
		valuePtr->render();
}

void MythosGenericFormData::setEvent(MYTHOS_EVENT_KEY key, MythosEventFormFunc func) {

	MythosEventFormFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end()) {
		mEvents.erase(it);
		mEvents.emplace_hint(it, key, func);
	}
	else {
		mEvents.emplace(key, func);
	}
}

MythosEventFormFunc MythosGenericFormData::getEvent(MYTHOS_EVENT_KEY key) {

	MythosEventFormFuncMap::iterator it = mEvents.find(key);

	if (it != mEvents.end())
		return it->second;

	return nullptr;
}