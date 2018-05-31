#pragma once

#include "..\_important_stuff.h"
#include "form.h"


#define MYTHOS_VOLUME			200
#define MYTHOS_TEXTURE			201





class MythosForm;


typedef MYTHOS_EVENT_RETURN(*MythosEventFormFunc)(MythosForm*, const MythosEvent&);
typedef std::unordered_map<MYTHOS_EVENT_KEY, MythosEventFormFunc> MythosEventFormFuncMap;

typedef int MYTHOS_FORM_KEY;
typedef std::shared_ptr<MythosGenericFormAttribute> MythosGenericFormAttributePtr;
typedef std::unordered_map<MYTHOS_FORM_KEY, MythosGenericFormAttributePtr> MythosGenericFormAttributeMap;


class MYTHOS_WORLD_API MythosGenericFormData : public MythosFormData {

	private:
		struct MythosGenericFormAttribute {};

		template <typename T>
		struct MythosGenericFormTypedAttribute {

			T value;

			MythosGenericFormTypedAttribute(T val) { value = val; }
		};

		MythosEventFormFuncMap					mEvents;
		MythosGenericFormAttributeMap			mAttributes;

	public:
		virtual int								inBounds(const vec2f&);
		virtual MYTHOS_EVENT_RETURN				update(MYTHOS_EVENT_KEY, MythosForm*, const MythosEvent&);
		virtual void							render(void);

		virtual void							setEvent(MYTHOS_EVENT_KEY, MythosEventFormFunc);
		virtual MythosEventFormFunc				getEvent(MYTHOS_EVENT_KEY);

		template <typename T>
		virtual void setAttribute(MYTHOS_FORM_KEY key, T value) {

			MythosGenericFormAttributeMap::iterator it = mAttributes.find(key);

			if (it != mAttributes.end()) {
				mAttributes.erase(it);
				mAttributes.emplace_hint(it, key, new MythosGenericFormTypedAttribute<T>(value));
			}
			else {
				mAttributes.emplace(key, new MythosGenericFormTypedAttribute<T>(value));
			}
		}

		template <typename T>
		virtual T& getAttribute(MYTHOS_FORM_KEY key) {

			MythosGenericFormAttributeMap::iterator it = mAttributes.find(key);

			if (it == mAttributes.end())
				throw MythosError("Incorrect Key", "Attempted to access a key that does not exist.");

			MythosGenericFormTypedAttribute<T>* ptr = static_cast<MythosGenericFormTypedAttribute<T>*>(it->second.get());
			if (!ptr)
				throw MythosError("Incorrect Type", "Attempted an incorrect type casting.");

			return ptr->value;
		}
};